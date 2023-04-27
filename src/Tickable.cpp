export module game;


export class ITickable {
	virtual void tick(float delta) = 0;
};