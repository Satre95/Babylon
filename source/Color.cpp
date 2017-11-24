////////////////////////////////////////
// Color.cpp
////////////////////////////////////////

#include "Color.hpp"

////////////////////////////////////////////////////////////////////////////////

Color Color::WHITE(1.0, 1.0, 1.0);
Color Color::GREY(0.25, 0.25, 0.25);
Color Color::BLACK(0.0, 0.0, 0.0);
Color Color::RED(1.0, 0.0, 0.0);
Color Color::YELLOW(1.0, 1.0, 0.0);
Color Color::BLUE(0.0, 0.0, 1.0);
Color Color::GREEN(0.0, 0.75, 0.0);

////////////////////////////////////////////////////////////////////////////////

void Color::Add(const Color & c) { 
	Red += c.Red; Green += c.Green; Blue += c.Blue; 
}

////////////////////////////////////////////////////////////////////////////////

void Color::AddScaled(const Color & c, float s) { 
	Red += s*c.Red; Green += s*c.Green; Blue += s*c.Blue; 
}
////////////////////////////////////////////////////////////////////////////////

Color & Color::Scale(float s) {
	Red *= s; Green *= s; Blue *= s; return *this;
}

////////////////////////////////////////////////////////////////////////////////

void Color::Scale(const Color c, float s) {
	Red = s*c.Red; Green = s*c.Green; Blue = s*c.Blue;
}
////////////////////////////////////////////////////////////////////////////////

void Color::Multiply(const Color c) { 
	Red *= c.Red; Green *= c.Green; Blue *= c.Blue; 
}

////////////////////////////////////////////////////////////////////////////////

int Color::ToInt() {
	int r = (Red < 0) ? 0 : ((Red >= 1.0) ? 255 : int(Red*256.0f));
	int g = (Green < 0) ? 0 : ((Green >= 1.0) ? 255 : int(Green*256.0f));
	int b = (Blue < 0) ? 0 : ((Blue >= 1.0) ? 255 : int(Blue*256.0f));
	return (r << 16) | (g << 8) | b;
}

////////////////////////////////////////////////////////////////////////////////

void Color::FromInt(int c) { 
	Set(float((c >> 16) & 0xff) / 255.0f, float((c >> 8) & 0xff) / 255.0f, float(c & 0xff) / 255.0f); 
}

////////////////////////////////////////////////////////////////////////////////

Color Color::AverageColors(std::vector<Color> & colors) {
	Color avg = Color::BLACK;
	for (Color & aColor : colors)
		avg.Add(aColor);
	avg.Scale(1.f / float(colors.size()));
	return avg;
}

////////////////////////////////////////////////////////////////////////////////

std::array<uint8_t, 4> Color::ToSeparateComponents() {
	uint8_t r = (Red < 0) ? 0 : ((Red >= 1.0) ? 255 : uint8_t(Red*256.0f));
	uint8_t g = (Green < 0) ? 0 : ((Green >= 1.0) ? 255 : uint8_t(Green*256.0f));
	uint8_t b = (Blue < 0) ? 0 : ((Blue >= 1.0) ? 255 : uint8_t(Blue*256.0f));
	uint8_t a = (Alpha < 0) ? 0 : ((Alpha >= 1.0) ? 255 : uint8_t(Alpha*256.0f));

	return {r, g, b, a};

}



////////////////////////////////////////////////////////////////////////////////
