#pragma once

struct Vector3D
{
	Vector3D(float x = 0, float y = 0, float z = 0) :
		x{ x },
		y{ y },
		z{ z }
	{}
	Vector3D(sol::table args)
	{
		int limit = (args.size() > 3) ? 3 : args.size();
		for (int i = 0; i < limit; ++i)
			data[i] = args[i + 1];
	}

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

	Vector3D operator-(const Vector3D& v) const
	{
		return Vector3D(x - v.x, y - v.y, z - v.z);
	}

	Vector3D operator*(const Vector3D& v) const
	{
		return Vector3D(y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x);
	}

	float Dot(const Vector3D& v) const
	{
		return (x*v.x + y*v.y + z*v.z);
	}

	float Length() const
	{
		return sqrtf(x*x + y*y + z*z);
	}

	Vector3D Normal() const
	{
		float leng = Length();
		return Vector3D(x / leng, y / leng, z / leng);
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

	operator float*() { return data; }

	static void RegisterInLua(sol::state_view& lua);
};