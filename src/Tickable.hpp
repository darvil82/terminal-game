#pragma once


class ITickable {
protected:
	virtual void tick(float delta) = 0;
};