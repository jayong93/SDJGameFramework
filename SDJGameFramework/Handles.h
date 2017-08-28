#pragma once

struct Handle
{
	Handle(unsigned i = 0, unsigned c = 0) :
		index{ i },
		count{ c }
	{}

	bool operator==(const Handle& h) { return (index == h.index) && (count == h.count); }

	operator bool() const { return (count != 0); }
	operator uint64_t() const { return uint64_t(index) << 24 | count; }
	uint64_t ToUInt64() const { return uint64_t(*this); }

	unsigned index;
	unsigned count : 24;

	static Handle ToHandle(uint64_t h) { return Handle(h >> 24, h); }
};

using ObjectHandle = Handle;
using ComponentHandle = Handle;