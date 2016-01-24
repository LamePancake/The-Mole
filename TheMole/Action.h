#pragma once
template <typename Action>
class Action
{
public:
	Action();
	~Action();
	virtual int Execute() = 0;
};

