#pragma once

#include "Mesh.hpp"
#include "../Voxels/Chunk.hpp"

class VoxelRenderer
{
public:
	VoxelRenderer(size_t capacity);
	~VoxelRenderer();

	Mesh* render(Chunk* chunk, const Chunk** chunks, bool ambientOcclusion);

private:
	float* buffer;
	size_t capacity;
};