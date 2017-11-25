#include <iostream>
#include "Image.hpp"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "Utilities.hpp"

////////////////////////////////////////////////////////////////////////////////

Image::Image(int x, int y, Components comp, Format format): 
m_format(format), m_components(comp), m_width(x), m_height(y) {
	m_pixelData = new uint8_t[x * y * RawType(comp)];
	std::fill(m_pixelData, m_pixelData + (x * y * RawType(comp)), 0);
}

////////////////////////////////////////////////////////////////////////////////

Image::~Image() {
	delete[] m_pixelData;
}

////////////////////////////////////////////////////////////////////////////////

void Image::SetPixel(size_t x, size_t y, uint8_t * data) {
    if(x >= m_width || y  >= m_height) {
        std::cout << "Invalid image indices!" << std::endl;
        return;
    }

	size_t numComps = RawType(m_components);
	size_t startIndex = y * m_width * numComps + x;
	std::copy(data, data + numComps, m_pixelData + startIndex);
}

////////////////////////////////////////////////////////////////////////////////

bool Image::Write(std::string file) {
	int retval;

	switch(m_format) {
		case Format::PNG: {
			file = file + ".png";
			retval = stbi_write_png(
				file.c_str(), 
				m_width, m_height, 
				RawType(m_components), 
				m_pixelData, 
				int(m_width) * RawType(m_components) );
			break;
		}

		case Format::BMP: {
			file = file + ".bmp";
			retval = stbi_write_bmp(
				file.c_str(), 
				m_width, m_height,
				RawType(m_components), 
				m_pixelData);
			break;
		}

		case Format::TGA: {
			file = file + ".tga";
			retval = stbi_write_tga(
				file.c_str(), 
				m_width, m_height,
				RawType(m_components),
				m_pixelData);
			break;
		}

		default: {
			file = file + ".jpeg";
			retval = stbi_write_jpg(
				file.c_str(), m_width, m_height,
				RawType(m_components), 
				m_pixelData, 
				100);
			break;
		}
	}

	return (retval != 0);
}

////////////////////////////////////////////////////////////////////////////////
