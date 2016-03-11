#include "CircleSprite.h"



CircleSprite::CircleSprite()
{
	_vboID = 0;
}


CircleSprite::~CircleSprite()
{
	if (_vboID != 0)
	{
		glDeleteBuffers(1, &_vboID);
	}
}

void CircleSprite::init(float x, float y, float r)
{
	_x = x;
	_y = y;
	_r = r;

	if (_vboID == 0)
	{
		glGenBuffers(1, &_vboID);
	}

	float vertexData[NUM_TRIANGLES*6];

	for (int n = 0; n < NUM_TRIANGLES*6; n += 6) 
	{
		float theta1 = ((n / 6)) * 360 / NUM_TRIANGLES;
		float theta2 = ((n / 6) + 1) * 360 / NUM_TRIANGLES;
		vertexData[n] = x;
		vertexData[n + 1] = y;

		vertexData[n + 2] = x + r*cos(theta1*PI / 180.0);
		vertexData[n + 3] = y + r*sin(theta1*PI / 180.0);

		vertexData[n + 4] = x + r*cos(theta2*PI / 180.0);
		vertexData[n + 5] = y + r*sin(theta2*PI / 180.0);
	}
	

	glBindBuffer(GL_ARRAY_BUFFER, _vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void CircleSprite::draw()
{
	glBindBuffer(GL_ARRAY_BUFFER, _vboID);

	glEnableVertexAttribArray(0);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_TRIANGLES, 0, NUM_TRIANGLES*3);

	glDisableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}