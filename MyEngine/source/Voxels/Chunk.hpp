#pragma once

#include "voxel.hpp"

#define CHUNK_W 16
#define CHUNK_H 16
#define CHUNK_D 16
#define CHUNK_VOL (CHUNK_W * CHUNK_H * CHUNK_D)

class Chunk
{
public:
	Chunk(int xpos, int ypos, int zpos);
	~Chunk();

	voxel* voxels;
	int x, y, z;
	bool modded = true;
};