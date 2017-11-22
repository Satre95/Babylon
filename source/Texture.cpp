#include "Texture.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Utilities.hpp"

Texture::Texture(std::string path) {
	m_data = stbi_load(path.c_str(), &m_width, &m_height, &m_numComponents, 0);
	if(m_numComponents == 1)
		m_format = TEXTURE_FORMAT::GREY;
	else if(m_numComponents == 3)
		m_format = TEXTURE_FORMAT::RGB;
	else if( m_numComponents == 4)
		m_format = TEXTURE_FORMAT::RGBA;
	m_name = Utilities::FilenameFromPath(path);
}

Texture::~Texture() {
	 stbi_image_free(m_data);
}

unsigned char Texture::GetPixelColor(int x, int y) {
	return m_data[y * m_width + x];
}