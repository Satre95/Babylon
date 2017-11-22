#pragma once
#include <string>
#include "Color.hpp"


enum class TEXTURE_FORMAT:uint8_t {
	GREY = 1, RGB = 3, RGBA = 4
};

enum class TEXTURE_TYPE: uint8_t {
	DIFFUSE, SPECULAR, NORMAL
};

class Texture {
public:
	Texture(std::string path, TEXTURE_TYPE type = TEXTURE_TYPE::DIFFUSE);
	~Texture();
	
	int Width() const { return m_width;}
	int Height() const { return m_height; }
	int NumComponents() const { return m_numComponents; }
	/// Query a pixel color. Indices range from [0, width) & [0, height)
	Color GetPixelColor(int x, int y);
	const std::string & Name() const { return m_name; }
	bool operator ==(const Texture & other) const {
		return other.m_name == m_name;
	}

private:
	int m_width, m_height, m_numComponents;
	unsigned char * m_data = nullptr;
	TEXTURE_FORMAT m_format;
	TEXTURE_TYPE m_type;
	std::string m_name;
};


namespace std {
	template<>
	struct hash<Texture> {
		size_t operator()(const Texture & tex) const {
			return hash<string>()(tex.Name());
		}
	};
}