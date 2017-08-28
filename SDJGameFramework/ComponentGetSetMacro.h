#pragma once

#define START_GETSET_DEF_(compoType) \
	using TYPE = compoType;\
	static bool init = true;\
	sol::state& lua = FW.lua;\
	if(init){\
		std::string typeName = GetTypeName<TYPE>()
#define END_GETSET_DEF_() \
	init = false; \
	}\
	return
#define SIMPLE_GETSET_DEF_(name, var) \
		lua["Component"]["get"][typeName][#name] =\
		[&lua](uint64_t h) -> sol::object {\
			auto c = CM.GetBy<TYPE>(h);\
			if(c) return sol::make_object(lua, c->var);\
			return sol::nil;\
		};\
		lua["Component"]["set"][typeName][#name] =\
		[](uint64_t h, sol::object v) {\
			auto c = CM.GetBy<TYPE>(h);\
			if(c && v.is<decltype(c->var)>()) c->var = v.as<decltype(c->var)>();\
		}

#define START_GETSET_DEF(compoType) START_GETSET_DEF_(compoType)
#define END_GETSET_DEF() END_GETSET_DEF_()
#define SIMPLE_GETSET_DEF(name, var) SIMPLE_GETSET_DEF_(name, var)