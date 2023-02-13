#pragma once

class Renderer;

class Body {
public:
	Body();
	virtual void update(double dt);
	virtual void render(Renderer* renderer);
};