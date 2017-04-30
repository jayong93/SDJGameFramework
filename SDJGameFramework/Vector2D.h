#pragma once

struct Vector2D
{
	Vector2D(int x = 0, int y = 0) :
		x{ x },
		y{ y }
	{}

	union
	{
		struct
		{
			int x, y;
		};
		int data[2];
	};

	void Set(int x, int y)
	{
		this->x = x; this->y = y;
	}

	Vector2D operator+(const Vector2D& v) const
	{
		return Vector2D(x + v.x, y + v.y);
	}

	operator POINT() const
	{
		return POINT{ x,y };
	}
};