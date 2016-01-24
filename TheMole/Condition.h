#pragma once
template <typename Condition>
class Condition
{
public:
	Condition();
	~Condition();
	virtual int Test() = 0;
};

