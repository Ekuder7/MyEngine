#include "Camera.hpp"

#include <glm/ext.hpp>

#include "../globals.hpp"

Camera::Camera(vec3 pos, float fov) : position(pos), fov(fov), rotation(1.0f)
{
	UpdateVectors();
}

mat4 Camera::getProjection()
{
	float aspect = (float)globals::winow_width / (float)globals::winow_height;
	return glm::perspective(fov, aspect, 0.1f, 500.0f);
}

mat4 Camera::getView()
{
	return glm::lookAt(position, position + front, up);
}

void Camera::rotate(float x, float y, float z)
{
	rotation = glm::rotate(rotation, z, vec3(0, 0, 1));
	rotation = glm::rotate(rotation, y, vec3(0, 1, 0));
	rotation = glm::rotate(rotation, x, vec3(1, 0, 0));

	UpdateVectors();
}

void Camera::UpdateVectors()
{
	front = vec3(rotation * vec4(0, 0, -1, 1));
	right = vec3(rotation * vec4(1, 0, 0, 1));
	up = vec3(rotation * vec4(0, 1, 0, 1));
}