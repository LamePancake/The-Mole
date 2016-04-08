#pragma once
#include <list>
#include <iostream>
#include <memory>
#include <functional>
#include <vector>
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
    virtual bool IsInterruptible() const { return _interruptible; };

    std::shared_ptr<Node> Node::GetRunningChild();

	std::shared_ptr<GameScreen> _gameScreen;

protected:
    std::shared_ptr<Node>_runningChild;

private:
    bool _interruptible;
};

class CompositeNode : public Node
{
private:
    std::vector<std::shared_ptr<Node>> children;
public:
    CompositeNode() : Node(true) {}

	const std::vector<std::shared_ptr<Node>>& GetChildren() const;
	void AddChild(std::shared_ptr<Node> child);
};

class Selector : public CompositeNode
{
public:
	virtual Result Run(double elapsedSecs) override;
};

class Sequence : public CompositeNode
{
public:
	virtual Result Run(double elapsedSecs) override;
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

private:
    std::function<Result(double)> _taskFunc;
};


class BossBehavTree
{
public:
    BossBehavTree() {}
	BossBehavTree(std::shared_ptr<Node> root, double updatePeriod);
    void Update(double detlaTime);
private:
    double _updatePeriod;
    double _timeSinceUpdate;

    std::shared_ptr<Node> _root;
    std::shared_ptr<Node> _current;
};