#include "BossBehavTree.h"
#include "GameScreen.h"

using std::vector;
using std::shared_ptr;

Node::Node(bool interruptible)
    : _interruptible{ interruptible },
    _gameScreen(std::dynamic_pointer_cast<GameScreen>(GameManager::GetInstance()->GetCurrentScreen())),
    _runningChild(nullptr)
{
}

std::shared_ptr<Node> Node::GetRunningChild()
{
    return _runningChild;
}

const vector<shared_ptr<Node>>& CompositeNode::GetChildren() const
{
	return children;
}
void CompositeNode::AddChild(shared_ptr<Node> child)
{
	children.push_back(child);
}

Node::Result Selector::Run(double elapsedSecs)
{
    _runningChild = nullptr;
	for (auto child : GetChildren())
	{
        Result res = child->Run(elapsedSecs);
		if (res == Result::Success)
		{
			return Result::Success;
		}
        else if (res == Result::Running)
        {
            // Propagate the running child up the tree
            _runningChild = child->GetRunningChild();
            if (!_runningChild) _runningChild = child;
            return Result::Running;
        }
	}
	return Result::Failure;
}


Node::Result Sequence::Run(double elapsedSecs)
{
	for (auto child : GetChildren())
	{
		if (!child->Run(elapsedSecs))
		{
			return Result::Failure;
		}
	}
	return Result::Success;
}

BossBehavTree::BossBehavTree(shared_ptr<Node> root, double updatePeriod)
    : _root(root), _updatePeriod(updatePeriod), _current(nullptr), _timeSinceUpdate(0)
{
}

void BossBehavTree::Update(double deltaTime)
{
    _timeSinceUpdate += deltaTime;
    if (_timeSinceUpdate > _updatePeriod)
    {
        _timeSinceUpdate = 0;

        if (!_current || _current->IsInterruptible())
        {
            _root->Run(deltaTime);
            _current = _root->GetRunningChild();
        }
        else
        {
            Node::Result res = _current->Run(deltaTime);
            if (res != Node::Result::Running)
            {
                _root->Run(deltaTime);
                _current = _root->GetRunningChild();
            }
        }
    }
}
