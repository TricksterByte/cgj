#pragma once
class IUpdatable {
public:
	virtual void update(float elapsed) = 0;
};