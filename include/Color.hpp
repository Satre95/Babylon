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

	void Add(const Color & c);
	void AddScaled(const Color & c, float s);
    Color & Scale(float s);
	void Scale(const Color c, float s);
	void Multiply(const Color c);

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

	int ToInt();
	void FromInt(int c);

	static Color AverageColors(std::vector<Color> & colors);

	static Color WHITE, GREY, BLACK;
	static Color RED, YELLOW, BLUE, GREEN;

private:
	float Red, Green, Blue;
};

////////////////////////////////////////////////////////////////////////////////
