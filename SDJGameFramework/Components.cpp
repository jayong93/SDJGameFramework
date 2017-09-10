#include "stdafx.h"
#include "Components.h"
#include "HandleManagers.h"
#include "Framework.h"
#include "LuaInterfaceType.h"

const char* Shape::stringFromType[] = { "NONE", "CUBE", "SPHERE", "CONE", "TORUS", "TEAPOT" };
const std::map<std::string, Shape::Type> Shape::stringToType = Shape::GetEnumTypeMap();
std::map<std::string, Shape::Type> Shape::GetEnumTypeMap()
{
    std::map<std::string, Shape::Type> map;
    int i=0;
    for (auto n : stringFromType)
        map[n] = Shape::Type(i++);
    return map;
}

void Shape::RegisterInLua()
{
    auto multiVarProperties1 = sol::property([](ComponentInLua& c) { auto& s = *CM.GetBy<Shape>(c.handle); return s.cubeSize;},
        [](ComponentInLua& c, float v) { auto& s = *CM.GetBy<Shape>(c.handle); s.cubeSize = v;});
    auto multiVarProperties2 = sol::property([](ComponentInLua& c) { auto& s = *CM.GetBy<Shape>(c.handle); return s.sphereSlice;},
        [](ComponentInLua& c, float v) { auto& s = *CM.GetBy<Shape>(c.handle); s.sphereSlice = v;});
    auto multiVarProperties3 = sol::property([](ComponentInLua& c) { auto& s = *CM.GetBy<Shape>(c.handle); return s.sphereStack;},
        [](ComponentInLua& c, float v) { auto& s = *CM.GetBy<Shape>(c.handle); s.sphereStack = v;});
    auto multiVarProperties4 = sol::property([](ComponentInLua& c) { auto& s = *CM.GetBy<Shape>(c.handle); return s.torusRing;},
        [](ComponentInLua& c, float v) { auto& s = *CM.GetBy<Shape>(c.handle); s.torusRing = v;});

    FW.componentTable.new_usertype<ComponentInLua>("Shape",
        "new", sol::constructors<ComponentInLua(ComponentHandle, ObjectHandle)>(),
        "handle", &ComponentInLua::handle,
        "owner", &ComponentInLua::owner,
        "color", sol::property([](ComponentInLua& c) { auto& s = *CM.GetBy<Shape>(c.handle); return s.color;},
            [](ComponentInLua& c, sol::object v) { auto& s = *CM.GetBy<Shape>(c.handle); if (v.is<Vector3D>()) s.color = v.as<Vector3D>(); else if(v.is<sol::table>()) s.color = v.as<sol::table>(); }),
        "cubeSize", multiVarProperties1,
        "teapotSize", multiVarProperties1,
        "sphereRadius", multiVarProperties1,
        "torusInnerRadius", multiVarProperties1,
        "coneBase", multiVarProperties1,
        "sphereSlice", multiVarProperties2,
        "torusOuterRadius", multiVarProperties2,
        "coneHeight", multiVarProperties2,
        "sphereStack", multiVarProperties3,
        "torusSide", multiVarProperties3,
        "coneSlice", multiVarProperties3,
        "torusRing", multiVarProperties4,
        "coneStack", multiVarProperties4,
        "type", sol::property([](ComponentInLua& c) { auto& s = *CM.GetBy<Shape>(c.handle); return std::string(Shape::stringFromType[size_t(s.type)]);},
            [](ComponentInLua& c, std::string& str){auto& s = *CM.GetBy<Shape>(c.handle);auto it = Shape::stringToType.find(str);if(it != Shape::stringToType.end())s.type = it->second;})
    );
}

