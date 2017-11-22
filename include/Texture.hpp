#pragma once
#include <string>
#include "Color.hpp"


enum class TEXTURE_FORMAT:uint8_t {
	GREY = 1, RGB = 3, RGBA = 4
};

class Texture {
public:
	Texture(std::string path);
	~Texture();
	
	int Width() const { return m_width;}
	int Height() const { return m_height; }
	int NumComponents() const { return m_numComponents; }
	/// Query a pixel color. Indices range from [0, width) & [0, height)
	Color GetPixelColor(int x, int y);
	const std::string & Name() { return m_name; }

private:
	int m_width, m_height, m_numComponents;
	unsigned char * m_data = nullptr;
	TEXTURE_FORMAT m_format;
	std::string m_name;
};