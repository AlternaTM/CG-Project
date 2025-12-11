#pragma once

#include <iostream>

class Texture
{
private:
	unsigned int renderedID;
	std::string filePath;
	unsigned char* localBuffer;
	int width, height, BPP;
public:
	Texture(const std::string& path);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;
};
