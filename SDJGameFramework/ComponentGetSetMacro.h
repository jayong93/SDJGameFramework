#pragma once

#define START_GETSET_DEF_(compoType) \
	using TYPE = compoType;\
	sol::state& lua = FW.lua;\
		size_t typeNum = GetTypeHash<TYPE>();\
		sol::table getT = lua["Component"]["get"][typeNum];\
		sol::table setT = lua["Component"]["set"][typeNum];
#define END_GETSET_DEF_() \
	return
#define SIMPLE_GETSET_DEF_(name, var) \
		getT[#name] =\
		[&lua](uint64_t h) -> sol::object {\
			auto c = CM.GetBy<TYPE>(h);\
			if(c) return sol::make_object(lua, c->var);\
			return sol::nil;\
		};\
		setT[#name] =\
		[](uint64_t h, sol::object v) {\
			auto c = CM.GetBy<TYPE>(h);\
			if(c && v.is<std::remove_reference_t<decltype(c->var)>>()) c->var = v.as<std::remove_reference_t<decltype(c->var)>>();\
		}

#define START_GETSET_DEF(compoType) START_GETSET_DEF_(compoType)
#define END_GETSET_DEF() END_GETSET_DEF_()
#define SIMPLE_GETSET_DEF(name, var) SIMPLE_GETSET_DEF_(name, var)