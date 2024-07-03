#pragma once

#include <string>

#include "Prerequisitos.h"

class Texture;

class TextureManage
{
public:
	TextureManage() = default;
	~TextureManage() = default;

	Texture* loafTextureFromFile(const Path& felieName);
};

