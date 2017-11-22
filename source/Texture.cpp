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

Color Texture::GetPixelColor(int x, int y) {
	switch(m_format) {
		case TEXTURE_FORMAT::GREY: {
			auto grey = m_data[y * m_width * m_numComponents + x];
			return Color(float(grey) / 255.f);
		}

		case TEXTURE_FORMAT::RGBA: {
			float red = float(m_data[y * m_width *m_numComponents + x]) / 255.f;
			float blue = float(m_data[y * m_width *m_numComponents + x + 1]) / 255.f;
			float green = float(m_data[y * m_width *m_numComponents + x + 2]) / 255.f;
			float alpha = float(m_data[y * m_width *m_numComponents + x + 3]) / 255.f;
			return Color(red, blue, green, alpha);
		}

		default: {
			float red = float(m_data[y * m_width *m_numComponents + x]) / 255.f;
			float blue = float(m_data[y * m_width *m_numComponents + x + 1]) / 255.f;
			float green = float(m_data[y * m_width *m_numComponents + x + 2]) / 255.f;
			return Color(red, blue, green);
		}
	}
}