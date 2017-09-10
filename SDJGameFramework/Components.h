#pragma once
#include "Component.h"

struct Shape : public Component
{
    virtual ~Shape() {}

    union {
        float cubeSize;
        float teapotSize;
        float sphereRadius;
        float torusInnerRadius;
        float coneBase;
    };
    union {
        float sphereSlice;
        float torusOuterRadius;
        float coneHeight;
    };
    union {
        float sphereStack;
        float torusSide;
        float coneSlice;
    };
    union {
        float torusRing;
        float coneStack;
    };

    Vector3D color;

    enum class Type { NONE, CUBE, SPHERE, CONE, TORUS, TEAPOT } type;
    static const char* stringFromType[];
    static const std::map<std::string, Type> stringToType;

    static MessageMap InitMsgMap() {return MessageMap();}
    static void RegisterInLua();

private:
    static std::map<std::string, Type> GetEnumTypeMap();
};

