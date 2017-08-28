#pragma once

struct Handle
{
	Handle() : index{ 0 }, count{ 0 } {}
	Handle(unsigned i, unsigned c) :
		index{ i },
		count{ c }
	{}
	Handle(uint64_t h) : Handle{ h >> 24, h }
	{}

	bool operator==(const Handle& h) { return (index == h.index) && (count == h.count); }

	operator bool() const { return (count != 0); }
	operator uint64_t() const { return uint64_t(index) << 24 | count; }
	uint64_t ToUInt64() const { return uint64_t(*this); }

	unsigned index;
	unsigned count : 24;
};

using ObjectHandle = Handle;
using ComponentHandle = Handle;