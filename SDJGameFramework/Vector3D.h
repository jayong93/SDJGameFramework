#pragma once

struct Vector3D
{
	Vector3D(double x = 0, double y = 0, double z = 0) :
		x{ x },
		y{ y },
		z{ z }
	{}

	union
	{
		struct
		{
			double x, y, z;
		};
		double data[3];
	};

	void Set(double x, double y, double z)
	{
		this->x = x; this->y = y; this->z = z;
	}

	Vector3D operator+(const Vector3D& v) const
	{
		return Vector3D(x + v.x, y + v.y, z + v.z);
	}

	Vector3D& operator+=(const Vector3D& v)
	{
		x += v.x; y += v.y; z += v.z;
		return *this;
	}

	bool operator==(const Vector3D& v) const
	{
		return (x == v.x && y == v.y && z == v.z);
	}

	operator double*() { return data; }
};