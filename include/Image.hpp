#pragma once

#include <cstdint>
#include <memory>

class Image
{
public:
	enum class Format: uint8_t {
		PNG = 0 , BMP = 1, TGA = 2, JPEG = 3
	};

	enum class Components: uint8_t {
		GREY = 1, GREYALPHA = 2, RGB = 3, RGBA = 4
	};

	Image(int x, int y, Components comp = Components::RGB, Format Format = Format::JPEG);
	~Image();

	int GetWidth() const		{ return m_width; 	}
	int GetHeight() const 		{ return m_height; 	}
	void SetFormat(Format f)	{ m_format = f;		}
	/// Sets a particular pixel in the image. 
	/// The data arg is expected to be a pointer to a contiguous set of memory of 
	/// the set number of components.
	/// (0, 0) is at the top left corner.
	void SetPixel(size_t x, size_t y, unsigned char * data);
	/// Writes the image out to file.
	bool Write(std::string file);

private:
	Format m_format;
	const Components m_components;
	const int m_width;
	const int m_height;
	unsigned char * m_pixelData = nullptr;
	
};

typedef std::shared_ptr<Image> ImageRef;