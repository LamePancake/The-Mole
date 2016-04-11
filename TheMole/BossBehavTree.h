#pragma once
#include <list>
#include <iostream>
#include <memory>
#include <functional>
#include <vector>
#include <stack>
#include "Vector2.h"

class GameScreen;
class BossActor;

class Node
{
public:
    enum Result
    {
        Success,
        Failure,
        Running
    };

    Node(bool interruptible);

	virtual Result Run(double elapsedSecs) = 0;
    virtual Result Resume(Result blockedResult, double elapsedSecs) = 0;
    virtual void Reset();

    virtual bool IsInterruptible() const { return _interruptible; };

    virtual std::shared_ptr<Node> Node::GetBlockingChild();

	std::shared_ptr<GameScreen> _gameScreen;

protected:
    std::shared_ptr<Node> _blockingChild;

private:
    bool _interruptible;
};

class CompositeNode : public Node
{
protected:
    virtual void SaveState(const std::shared_ptr<Node> & blocked);
    
    std::vector<std::shared_ptr<Node>> _children;
    int _blockedIndex;
public:
    CompositeNode(bool interruptible = true) : Node(interruptible) {}

	const std::vector<std::shared_ptr<Node>>& GetChildren() const;
	void AddChild(std::shared_ptr<Node> child);
    virtual void Reset();
};

class Selector : public CompositeNode
{
public:
    Selector(bool interruptible = true) :
        CompositeNode{ interruptible } {};
	virtual Result Run(double elapsedSecs) override;
    virtual Result Resume(Result blockedResult, double elapsedSecs) override;
};

class Sequence : public CompositeNode
{
public:
    Sequence(bool interruptible = true) :
        CompositeNode{ interruptible } {};
	virtual Result Run(double elapsedSecs) override;
    virtual Result Resume(Result blockedResult, double elapsedSecs) override;
};

/**
 * @brief Generic task that takes a function pointer for use in the behaviour tree.
 */
class Task : public Node
{
public:

    Task(bool interruptible, std::function<Result(double)> &taskFunction)
        : Node{ interruptible }, _taskFunc(taskFunction) {}

    virtual Result Run(double elapsedSeconds)
    {
        return _taskFunc(elapsedSeconds);
    }
    virtual Result Resume(Result blockedResult, double elapsedSecs) override;
    virtual void Reset();

private:
    std::function<Result(double)> _taskFunc;
};

class UntilSuccess : public Node
{
public:
    UntilSuccess(std::shared_ptr<Node> child)
        : Node{ false }, _child(child) {}

    virtual Result Run(double elapsedSeconds);

    // The running child is always this node, so return nullptr
    virtual std::shared_ptr<Node> GetRunningChild()
    {
        return nullptr;
    }

    virtual Result Resume(Result blockedResult, double elapsedSecs) override;
    virtual void Reset();
private:
    std::shared_ptr<Node> _child;
};

class RunNTimes : public Node
{
public:
    RunNTimes(std::shared_ptr<Node> child, int iterations, bool interruptible)
        : Node{ interruptible }, _child(child), _limit(iterations) {}

    virtual Result Run(double elapsedSeconds);

    // The running child is always this node, so return nullptr
    virtual std::shared_ptr<Node> GetRunningChild()
    {
        return nullptr;
    }
    virtual Result Resume(Result blockedResult, double elapsedSecs) override;
    virtual void Reset();
private:
    std::shared_ptr<Node> _child;
    int _limit;
    int _current;
};

class BossBehavTree
{
public:
    BossBehavTree() {}
	BossBehavTree(std::shared_ptr<Node> root, double updatePeriod);
    void Update(double detlaTime);
    void Reset();
private:
    double _updatePeriod;
    double _timeSinceUpdate;

    // Runs a blocked child holding up a given branch
    void ContinueBlocked(double deltaTime);
    void PushBlocked(std::shared_ptr<Node> blockedNode);

    std::shared_ptr<Node> _root;
    std::stack<std::shared_ptr<Node>> _blocked; // Effectively stores a call stack
};