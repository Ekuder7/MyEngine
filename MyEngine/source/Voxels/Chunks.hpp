#pragma once

#include "Chunk.hpp"

#include <glm/glm.hpp>

using namespace glm;

class Chunks
{
public:
	Chunk** chunks;
	size_t volume;
	int32_t width, height, depth;

	Chunks(int32_t w, int32_t h, int32_t d);
	~Chunks();

	Chunk* getChunk(int32_t x, int32_t y, int32_t z);
	voxel* get(int32_t x, int32_t y, int32_t z);
	void set(int32_t x, int32_t y, int32_t z, int32_t id);
	voxel* rayCast(vec3 start, vec3 dir, float maxLength, vec3& end, vec3& norm, vec3& iend);
};