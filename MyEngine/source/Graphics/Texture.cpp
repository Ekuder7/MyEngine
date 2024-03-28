#include "Texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

static GLuint load_texture(std::string filename, int& width, int& height)
{
	GLuint texture;

	int channels;

	stbi_uc* image = stbi_load(filename.c_str(), &width, &height, &channels, STBI_rgb_alpha);
	stbi__vertical_flip(image, width, height, 4);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)image);

	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(image);

	return texture;
}

Texture::Texture(std::string filename)
{
	_TextureId = load_texture(filename, width, height);
}

Texture::~Texture()
{
	glDeleteTextures(1, &_TextureId);
}

void Texture::bind()
{
	glBindTexture(GL_TEXTURE_2D, _TextureId);
}