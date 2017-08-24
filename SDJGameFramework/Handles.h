#pragma once

struct Handle
{
	Handle(unsigned i = 0, unsigned c = 0) :
		index{ i },
		count{ c }
	{}

	bool operator==(const Handle& h) { return (index == h.index) && (count == h.count); }

	operator bool() const { return (count != 0); }
	operator uint64_t() const { return uint64_t(index) << 24 | (0xffffff & count); }

	unsigned index;
	unsigned count;
};

using ObjectHandle = Handle;
using ComponentHandle = Handle;