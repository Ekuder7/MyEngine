#include "Chunk.hpp"
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/noise.hpp>

Chunk::Chunk(int xpos, int ypos, int zpos) : x(xpos), y(ypos), z(zpos)
{
	voxels = new voxel[CHUNK_VOL];

	for (int z = 0; z < CHUNK_D; z++)
	{
		for (int x = 0; x < CHUNK_W; x++)
		{
			int real_x = x + this->x * CHUNK_W;
			int real_z = z + this->z * CHUNK_D;
			float height = glm::perlin(glm::vec3(real_x*0.025f,real_z*0.025f, 0.0f));

			for (int y = 0; y < CHUNK_H; y++)
			{
				int real_y = y + this->y * CHUNK_H;
				int id = real_y <= (height) * 10 + 5;
				if (real_y <= 2)
					id = 2;
				voxels[(y * CHUNK_D + z) * CHUNK_W + x].id = id;
			}
		}
	}
}

Chunk::~Chunk()
{
	delete[] voxels;
}