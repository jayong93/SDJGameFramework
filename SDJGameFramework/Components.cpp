#include "stdafx.h"
#include "Components.h"
#include "HandleManagers.h"
#include "Framework.h"

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
    FW.typeTable.new_usertype<Shape>("Shape",
        "new", sol::no_constructor,
        "get", [](uint64_t id) {return CM.GetBy<Shape>(id);},
        "handle", &Shape::handle,
        "owner", &Shape::owner,
        "color", sol::property([](Shape& compo){return compo.color;}, [](Shape& c, sol::object v) { if (v.is<Vector3D>()) c.color = v.as<Vector3D>(); else if(v.is<sol::table>()) c.color = v.as<sol::table>(); }),
        "cubeSize", &Shape::cubeSize,
        "teapotSize", &Shape::teapotSize,
        "sphereRadius", &Shape::sphereRadius,
        "torusInnerRadius", &Shape::torusInnerRadius,
        "coneBase", &Shape::coneBase,
        "sphereSlice", &Shape::sphereSlice,
        "torusOuterRadius", &Shape::torusOuterRadius,
        "coneHeight", &Shape::coneHeight,
        "sphereStack", &Shape::sphereStack,
        "torusSide", &Shape::torusSide,
        "coneSlice", &Shape::coneSlice,
        "torusRing", &Shape::torusRing,
        "coneStack", &Shape::coneStack,
        "type", sol::property([](Shape& c) { return std::string(Shape::stringFromType[size_t(c.type)]);},
            [](Shape& c, std::string& str){auto it = Shape::stringToType.find(str);if(it != Shape::stringToType.end())c.type = it->second;})
    );
}

