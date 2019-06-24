#pragma once
#include<functional>

template<typename T>
class Trigger
{
public:
	T after;
	T before;
	//std::function<void(Trigger& trigger)> events;

public:
	Trigger(T&& state)
		: after(std::move(state))
		, before(after) {}
	Trigger(const T& state)
		: after(state)
		, before(state) {}
	Trigger() {}

	Trigger& operator=(T&& state)
	{
		before = std::move(after);
		after = std::move(state);
		return *this;
	}

	Trigger& operator=(const T& state)
	{
		before = std::move(after);
		after = state;
		return *this;
	}

	T& poll()
	{
		before = after;
		return after;
	}

	bool changed() const
	{
		return after != before;
	}

	operator T&()
	{
		return after;
	}
};

