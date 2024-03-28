#pragma once

#include "glm/glm.hpp"

using namespace glm;

class Camera
{
public:

	Camera(vec3 position, float fov);

	vec3 position;
	float fov;

	vec3 front;
	vec3 up;
	vec3 right;

	mat4 rotation;

	mat4 getProjection();
	mat4 getView();

	void rotate(float x, float y, float z);

private:

	void UpdateVectors();
};