#pragma once

struct Vector3D
{
	Vector3D(float x = 0, float y = 0, float z = 0) :
		x{ x },
		y{ y },
		z{ z }
	{}

	union
	{
		struct
		{
			float x, y, z;
		};
		float data[3];
	};

	void Set(float x, float y, float z)
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

	operator float*() { return data; }
};