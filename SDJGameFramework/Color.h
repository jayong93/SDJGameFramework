#pragma once

struct Color
{
	Color(UCHAR r, UCHAR g, UCHAR b) : 
		r{ r }, g{ g }, b{ b }
	{}

	union
	{
		struct
		{
			unsigned char r, g, b;
		};
		unsigned char data[3];
	};

	void Set(unsigned char r, unsigned char g, unsigned char b)
	{
		this->r = r; this->g = g; this->b = b;
	}
};