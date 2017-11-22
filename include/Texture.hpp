#pragma once
#include <string>


enum class TEXTURE_FORMAT:uint8_t {
	GREY, RGB, RGBA
};

class Texture {
public:
	Texture(std::string path);
	~Texture();
	
	int Width() const { return m_width;}
	int Height() const { return m_height; }
	int NumComponents() const { return m_numComponents; }
	const std::string & Name() { return m_name; }

private:
	int m_width, m_height, m_numComponents;
	unsigned char * m_data = nullptr;
	TEXTURE_FORMAT m_format;
	std::string m_name;
};