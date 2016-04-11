#include "BossBehavTree.h"
#include "GameScreen.h"

using std::vector;
using std::shared_ptr;

Node::Node(bool interruptible)
    : _interruptible{ interruptible },
    _gameScreen(std::dynamic_pointer_cast<GameScreen>(GameManager::GetInstance()->GetCurrentScreen())),
    _blockingChild(nullptr)
{
}

void Node::Reset()
{
    _blockingChild = nullptr;
}


std::shared_ptr<Node> Node::GetBlockingChild()
{
    return _blockingChild;
}

void CompositeNode::SaveState(const std::shared_ptr<Node> & blocked)
{
    _blockedIndex = &blocked - &_children[0];
}

const vector<shared_ptr<Node>>& CompositeNode::GetChildren() const
{
	return _children;
}
void CompositeNode::AddChild(shared_ptr<Node> child)
{
    _children.push_back(child);
}

void CompositeNode::Reset()
{
    _blockedIndex = -1;
    for (auto & child : _children)
        child->Reset();
}

Node::Result Selector::Run(double elapsedSecs)
{
    bool hadRunningResult = false;
    _blockingChild = nullptr;

	for (auto & child : _children)
	{
        Result res = child->Run(elapsedSecs);
		if (res == Result::Success)
		{
			return Result::Success;
		}
        else if (res == Result::Running)
        {
            hadRunningResult = true;
            if (!child->IsInterruptible() || child->GetBlockingChild())
            {
                SaveState(child);
                _blockingChild = child;
                return Result::Running;
            }
        }
	}
	return hadRunningResult ? Result::Running : Result::Failure;
}

Node::Result Selector::Resume(Result blockedResult, double elapsedSecs)
{
    if (blockedResult == Result::Success)
        return Result::Success;

    bool hadRunningResult = blockedResult == Result::Running;
    _blockingChild = nullptr;

    // Start at the next node that was to be processed
    for (auto it = _children.begin() + _blockedIndex + 1; it != _children.end(); it++)
    {
        Result res = (*it)->Run(elapsedSecs);
        if (res == Result::Success)
        {
            return Result::Success;
        }
        else if (res == Result::Running)
        {
            hadRunningResult = true;
            if (!(*it)->IsInterruptible() || (*it)->GetBlockingChild())
            {
                SaveState(*it);
                _blockingChild = (*it);
                return Result::Running;
            }
        }
    }
    return hadRunningResult ? Result::Running : Result::Failure;
}


Node::Result Sequence::Run(double elapsedSecs)
{
    bool hadRunningResult = false;
    _blockingChild = nullptr;

	for (auto & child : _children)
	{
        Result res = child->Run(elapsedSecs);
		if (res == Node::Result::Failure)
		{
			return Result::Failure;
		}
        else if (res == Result::Running)
        {
            hadRunningResult = true;
            if (!child->IsInterruptible() || child->GetBlockingChild())
            {
                SaveState(child);
                _blockingChild = child;
                return Result::Running;
            }
        }
	}
	return hadRunningResult ? Result::Running : Result::Success;
}

Node::Result Sequence::Resume(Node::Result blockedResult, double elapsedSecs)
{
    if (blockedResult == Result::Failure)
        return Result::Failure;

    bool hadRunningResult = blockedResult == Result::Running;
    _blockingChild = nullptr;

    for (auto it = _children.begin() + _blockedIndex + 1; it != _children.end(); it++)
    {
        Result res = (*it)->Run(elapsedSecs);
        if (res == Result::Failure)
        {
            return Result::Failure;
        }
        else if (res == Result::Running)
        {
            hadRunningResult = true;
            if (!(*it)->IsInterruptible() || (*it)->GetBlockingChild())
            {
                SaveState(*it);
                _blockingChild = (*it);
                return Result::Running;
            }
        }
    }
    return hadRunningResult ? Result::Running : Result::Success;
}

BossBehavTree::BossBehavTree(shared_ptr<Node> root, double updatePeriod)
    : _root(root), _updatePeriod(updatePeriod), _blocked(), _timeSinceUpdate(0)
{
}

void BossBehavTree::Update(double deltaTime)
{
    _timeSinceUpdate += deltaTime;
    if (_timeSinceUpdate > _updatePeriod)
    {
        if (!_blocked.empty())
        {
            ContinueBlocked(_timeSinceUpdate);
        }
        else
        {
            Node::Result res = _root->Run(_timeSinceUpdate);
            if (res == Node::Result::Running)
            {
                if (_root->GetBlockingChild())
                {
                    PushBlocked(_root);
                }
            }
        }
        _timeSinceUpdate = 0;
    }
}

void BossBehavTree::Reset()
{
    _timeSinceUpdate = 0;
    _root->Reset();
    for (auto i = 0; i < _blocked.size(); ++i)
        _blocked.pop();
}

void BossBehavTree::PushBlocked(shared_ptr<Node> blocked)
{
    _blocked.push(blocked);
    while (blocked = blocked->GetBlockingChild())
    {
        _blocked.push(blocked);
    }
}

void BossBehavTree::ContinueBlocked(double deltaTime)
{
    auto current = _blocked.top();
    Node::Result res = current->Run(deltaTime);

    // Unless there are more blocked children that are blocked, we don't need to do anything else
    // since the top of the stack must be an uninterruptible node
    if (res == Node::Result::Running)
    {
        auto newChild = current->GetBlockingChild();
        if (newChild)
            PushBlocked(newChild);
    }
    else
    {
        // Get to the next node
        _blocked.pop();
        while (!_blocked.empty())
        {
            current = _blocked.top();
            res = current->Resume(res, deltaTime);
            if (res == Node::Result::Running)
            {
                if (current->GetBlockingChild())
                {
                    PushBlocked(current->GetBlockingChild());
                    return;
                }
                else if (!current->IsInterruptible())
                {
                    return; // We haven't popped the current node yet, so we can just return from here
                }
                // There's an implied else here: the current node is running but non-blocking, so we can continue
                // traversing the tree
            }
            _blocked.pop();
        }
    }
}

Node::Result UntilSuccess::Run(double elapsedSeconds)
{
    Result res = _child->Run(elapsedSeconds);
    if (res == Result::Success) return Result::Success;
    else
    {
        if (res == Result::Running)
        {
            if (!_child->IsInterruptible())
            {
                _blockingChild = _child;
                return Result::Running;
            }
            _blockingChild = _child->GetBlockingChild();
        }
        return Result::Running;
    }
}

Node::Result UntilSuccess::Resume(Result blockedResult, double elapsedSecs)
{
    return blockedResult == Result::Success ? Result::Success : Run(elapsedSecs);
}

void UntilSuccess::Reset()
{
    _child->Reset();
}

Node::Result RunNTimes::Run(double elapsedSeconds)
{
    Result res = _child->Run(elapsedSeconds);
    if (res == Result::Running)
    {
        if (!_child->IsInterruptible() || _child->GetBlockingChild())
            _blockingChild = _child;

        return Result::Running;
    }

    _current++;
    if (_current == _limit)
    {
        _current = 0;
        return Result::Success;
    }
    return Result::Running;
}

Node::Result RunNTimes::Resume(Result blockedResult, double elapsedSecs)
{
    if (blockedResult == Result::Running)
    {
        return Result::Running;
    }
    
    _current++;
    if (_current == _limit)
    {
        _current = 0;
        return Result::Success;
    }
    return Result::Running;
}

void RunNTimes::Reset()
{
    _current = 0;
    _child->Reset();
}

Node::Result Task::Resume(Result blockedResult, double elapsedSecs)
{
    // Task nodes should always be leaves, so we can't return anything meaningful
    return Result();
}

void Task::Reset()
{
    // Does nothing
}
