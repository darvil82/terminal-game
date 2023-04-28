export module game.tickable;


export class ITickable {
	virtual void tick(float delta) = 0;
};