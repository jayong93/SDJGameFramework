#pragma once

#define START_GETSET_DEF_(compoType) \
	using TYPE = compoType;\
		size_t typeNum = GetTypeHash<TYPE>();\
		sol::table getT = FW.lua["Component"]["get"][typeNum];\
		sol::table setT = FW.lua["Component"]["set"][typeNum];
#define END_GETSET_DEF_() \
	return
#define SIMPLE_GETSET_DEF_(name, var) \
		getT[#name] =\
		[](uint64_t h, sol::this_state l) -> sol::object {\
			sol::state_view lua(l);\
			auto c = CM.GetBy<TYPE>(h);\
			if(c) return sol::make_object(lua, c->var);\
			return sol::nil;\
		};\
		setT[#name] =\
		[](uint64_t h, sol::object v, sol::this_state l) {\
			sol::state_view lua(l);\
			auto c = CM.GetBy<TYPE>(h);\
			if(c && v.is<std::remove_reference_t<decltype(c->var)>>()) c->var = v.as<std::remove_reference_t<decltype(c->var)>>();\
		}
#define START_COMPLEX_GET_DEF_(name, objVar) \
		getT[#name] =\
		[](uint64_t h, sol::this_state l) -> sol::object {\
			sol::state_view lua(l);\
			auto objVar = CM.GetBy<TYPE>(h);\
			if(objVar) {\
				auto f = [&]() {0
#define END_COMPLEX_GET_DEF_() \
				};\
				return sol::make_object(lua, f());\
			}\
			return sol::nil;\
		}
#define START_COMPLEX_SET_DEF_(name, objVar, valVar) \
		setT[#name] =\
		[](uint64_t h, sol::object valVar, sol::this_state l) {\
			sol::state_view lua(l);\
			auto objVar = CM.GetBy<TYPE>(h);\
			if(objVar) {0
#define END_COMPLEX_SET_DEF_() \
			}\
		}
#define START_MULTI_GET_DEF_(name, objVar, listVar) \
		getT[#name] =\
		[](uint64_t h, sol::this_state l) {\
			sol::state_view lua(l);\
			auto objVar = CM.GetBy<TYPE>(h);\
			sol::variadic_results t;\
			sol::variadic_results& listVar=t;\
			if(objVar) {0
#define END_MULTI_GET_DEF_() \
			}\
			return t;\
		}

#define START_GETSET_DEF(compoType) START_GETSET_DEF_(compoType)
#define END_GETSET_DEF() END_GETSET_DEF_()
#define SIMPLE_GETSET_DEF(name, var) SIMPLE_GETSET_DEF_(name, var)
#define START_COMPLEX_GET_DEF(name, objVar) START_COMPLEX_GET_DEF_(name, objVar)
#define END_COMPLEX_GET_DEF() END_COMPLEX_GET_DEF_()
#define START_COMPLEX_SET_DEF(name, objVar, valVar) START_COMPLEX_SET_DEF_(name, objVar, valVar)
#define END_COMPLEX_SET_DEF() END_COMPLEX_SET_DEF_()
#define START_MULTI_GET_DEF(name, objVar, listVar) START_MULTI_GET_DEF_(name, objVar, listVar)
#define END_MULTI_GET_DEF() END_MULTI_GET_DEF_()