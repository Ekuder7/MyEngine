#include "Chunks.hpp"

Chunks::Chunks(int32_t w, int32_t h, int32_t d) : width(w), height(h), depth(d)
{
	volume = w * h * d;
	chunks = new Chunk* [volume];

	int index = 0;
	for (int y = 0; y < h; y++)
	{
		for (int z = 0; z < d; z++)
		{
			for (int x = 0; x < w; x++, index++)
			{
				Chunk* chunk = new Chunk(x, y, z);
				chunks[index] = chunk;
			}
		}
	}
}


Chunks::~Chunks()
{
	
	for (size_t i = 0; i < volume; i++)
	{
		if (chunks[i] != nullptr)
		{
			delete chunks[i];
		}
	}
	delete[] chunks;
}


Chunk* Chunks::getChunk(int32_t x, int32_t y, int32_t z)
{
	if (x < 0 || y < 0 || z < 0 || x >= width || y >= height || z >= depth)
		return nullptr;
	return chunks[(y * depth + z) * width + x];
}

voxel* Chunks::get(int32_t x, int32_t y, int32_t z)
{
	int cx = x / CHUNK_W;
	int cy = y / CHUNK_H;
	int cz = z / CHUNK_D;
	if (x < 0) cx--;
	if (y < 0) cy--;
	if (z < 0) cz--;
	if (cx < 0 || cy < 0 || cz < 0 || cx >= width || cy >= height || cz >= depth)
		return nullptr;
	Chunk* chunk = chunks[(cy * depth + cz) * width + cx];
	int lx = x - cx * CHUNK_W;
	int ly = y - cy * CHUNK_H;
	int lz = z - cz * CHUNK_D;

	return &chunk->voxels[(ly * CHUNK_D + lz) * CHUNK_W + lx];
}

void Chunks::set(int32_t x, int32_t y, int32_t z, int32_t id)
{
	int cx = x / CHUNK_W;
	int cy = y / CHUNK_H;
	int cz = z / CHUNK_D;
	if (x < 0) cx--;
	if (y < 0) cy--;
	if (z < 0) cz--;
	if (cx < 0 || cy < 0 || cz < 0 || cx >= width || cy >= height || cz >= depth)
		return;
	Chunk* chunk = chunks[(cy * depth + cz) * width + cx];
	int lx = x - cx * CHUNK_W;
	int ly = y - cy * CHUNK_H;
	int lz = z - cz * CHUNK_D;
	chunk->voxels[(ly * CHUNK_D + lz) * CHUNK_W + lx].id = id;
	chunk->modded = true;

	if (lx == 0 && (chunk = getChunk(cx - 1, cy, cz))) chunk->modded = true;
	if (ly == 0 && (chunk = getChunk(cx, cy - 1, cz))) chunk->modded = true;
	if (lz == 0 && (chunk = getChunk(cx, cy, cz - 1))) chunk->modded = true;

	if (lx == CHUNK_W - 1 && (chunk = getChunk(cx + 1, cy, cz))) chunk->modded = true;
	if (ly == CHUNK_H - 1 && (chunk = getChunk(cx, cy + 1, cz))) chunk->modded = true;
	if (lz == CHUNK_D - 1 && (chunk = getChunk(cx, cy, cz + 1))) chunk->modded = true;
}

voxel* Chunks::rayCast(vec3 start, vec3 dir, float maxLength, vec3& end, vec3& norm, vec3& iend)
{
	float px = start.x;
	float py = start.y;
	float pz = start.z;

	float dx = dir.x;
	float dy = dir.y;
	float dz = dir.z;

	float t = 0.0f;
	int ix = floor(px);
	int iy = floor(py);
	int iz = floor(pz);

	float stepx = (dx > 0.0f) ? 1.0f : -1.0f;
	float stepy = (dy > 0.0f) ? 1.0f : -1.0f;
	float stepz = (dz > 0.0f) ? 1.0f : -1.0f;

	constexpr float infinity = std::numeric_limits<float>::infinity();

	float txDelta = (dx == 0.0f) ? infinity : abs(1.0f / dx);
	float tyDelta = (dy == 0.0f) ? infinity : abs(1.0f / dy);
	float tzDelta = (dz == 0.0f) ? infinity : abs(1.0f / dz);

	float xdist = (stepx > 0) ? (ix + 1 - px) : (px - ix);
	float ydist = (stepy > 0) ? (iy + 1 - py) : (py - iy);
	float zdist = (stepz > 0) ? (iz + 1 - pz) : (pz - iz);

	float txMax = (txDelta < infinity) ? txDelta * xdist : infinity;
	float tyMax = (tyDelta < infinity) ? tyDelta * ydist : infinity;
	float tzMax = (tzDelta < infinity) ? tzDelta * zdist : infinity;

	int steppedIndex = -1;

	while (t <= maxLength)
	{
		voxel* voxel = get(ix, iy, iz);
		if (voxel == nullptr || voxel->id) {
			end.x = px + t * dx;
			end.y = py + t * dy;
			end.z = pz + t * dz;

			iend.x = ix;
			iend.y = iy;
			iend.z = iz;

			norm.x = norm.y = norm.z = 0.0f;
			if (steppedIndex == 0) norm.x = -stepx;
			if (steppedIndex == 1) norm.y = -stepy;
			if (steppedIndex == 2) norm.z = -stepz;
			return voxel;
		}
		if (txMax < tyMax) {
			if (txMax < tzMax) {
				ix += stepx;
				t = txMax;
				txMax += txDelta;
				steppedIndex = 0;
			}
			else {
				iz += stepz;
				t = tzMax;
				tzMax += tzDelta;
				steppedIndex = 2;
			}
		}
		else {
			if (tyMax < tzMax) {
				iy += stepy;
				t = tyMax;
				tyMax += tyDelta;
				steppedIndex = 1;
			}
			else {
				iz += stepz;
				t = tzMax;
				tzMax += tzDelta;
				steppedIndex = 2;
			}
		}
	}
	iend.x = ix;
	iend.y = iy;
	iend.z = iz;

	end.x = px + t * dx;
	end.y = py + t * dy;
	end.z = pz + t * dz;
	norm.x = norm.y = norm.z = 0.0f;
	return nullptr;
}