////////////////////////////////////////
// Color.h
////////////////////////////////////////

#pragma once
#include <vector>

////////////////////////////////////////////////////////////////////////////////

class Color {
public:
	Color() { Red = Green = Blue = 1.0; }
	Color(float r, float g, float b) { Red = r; Green = g; Blue = b; }
	Color(float val) { Red = Blue = Green = val; }

	void Set(float r, float g, float b) { Red = r; Green = g; Blue = b; }

	void Add(const Color & c) { Red += c.Red; Green += c.Green; Blue += c.Blue; }
	void AddScaled(const Color & c, float s) { Red += s*c.Red; Green += s*c.Green; Blue += s*c.Blue; }
	void Scale(float s) { Red *= s; Green *= s; Blue *= s; }
	void Scale(const Color c, float s) { Red = s*c.Red; Green = s*c.Green; Blue = s*c.Blue; }
	void Multiply(const Color c) { Red *= c.Red; Green *= c.Green; Blue *= c.Blue; }
	const float & GetRed() { return Red; }
	const float & GetGreen() { return Green; }
	const float & GetBlue() { return Blue; }
	Color operator+ (const Color & other)
	{
		Color t = Color::BLACK;
		t.Add(*this);
		t.Add(other);
		return t;
	}

	Color operator*(const Color & other)
	{
		return Color(Red * other.Red, Green * other.Green, Blue * other.Blue);
	}

	Color operator*(const float & scale)
	{
		return Color(Red * scale, Blue * scale, Green * scale);
	}

	int ToInt() {
		int r = (Red < 0) ? 0 : ((Red >= 1.0) ? 255 : int(Red*256.0f));
		int g = (Green < 0) ? 0 : ((Green >= 1.0) ? 255 : int(Green*256.0f));
		int b = (Blue < 0) ? 0 : ((Blue >= 1.0) ? 255 : int(Blue*256.0f));
		return (r << 16) | (g << 8) | b;
	}
	void FromInt(int c) { Set(float((c >> 16) & 0xff) / 255.0f, float((c >> 8) & 0xff) / 255.0f, float(c & 0xff) / 255.0f); }

	static Color AverageColors(std::vector<Color> & colors);

	static Color WHITE, GREY, BLACK;
	static Color RED, YELLOW, BLUE, GREEN;

private:
	float Red, Green, Blue;
};

////////////////////////////////////////////////////////////////////////////////