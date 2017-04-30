#pragma once

struct Handle
{
	Handle(unsigned i = 0, unsigned c = 0) :
		index{ i },
		count{ c }
	{}

	bool operator==(const Handle& h) { return (index == h.index) && (count == h.count); }

	operator bool() { return (count != 0); }

	unsigned index;
	unsigned count;
};

using ObjectHandle = Handle;
using ComponentHandle = Handle;