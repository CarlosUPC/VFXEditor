#pragma once

#include "Globals.h"
class Application;


struct Image
{
	void* pixels;
	float2 size;
	int size_x;
	int size_y;
	int   nchannels;
	int   stride;
};

struct Texture
{
	unsigned int handle;
	std::string filepath;
};

Image LoadImage2D(const char* filename);
void FreeImage(Image image);


uint CreateTexture2DFromImage(Image image);

int LoadTexture2D(Application* app, const char* filepath);

