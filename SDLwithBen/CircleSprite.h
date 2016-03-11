#pragma once
#include <GL\glew.h>
#include <math.h>

#define PI				3.14159265
#define NUM_TRIANGLES	360

class CircleSprite
{
public:
	CircleSprite();
	~CircleSprite();

	void init(float x, float y, float r);
	void draw();

private:
	float _x;
	float _y;
	float _r;
	GLuint _vboID;
};

