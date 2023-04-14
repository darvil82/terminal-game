#pragma once


class ITickable {
	virtual void tick(float delta) = 0;
};