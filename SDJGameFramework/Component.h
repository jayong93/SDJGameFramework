#pragma once

#include "Handles.h"
#include "Color.h"
#include "Vector2D.h"

struct Message;

class Component
{
public:
	Component() {}
	virtual ~Component() {}

	virtual void SendMessage(Message& msg) = 0;

	ObjectHandle owner;
	ComponentHandle handle;
};

class GraphicComponent : public Component
{
public:
	GraphicComponent() :
		penWidth{ 1 },
		penColor{ 0,0,0 },
		brushColor{ 255,255,255 },
		order{ 0 }
	{}
	virtual ~GraphicComponent() {}

	virtual void SendMessage(Message& msg) {}
	virtual void Draw(HDC hdc) = 0;

	unsigned penWidth;
	int order;
	Color penColor, brushColor;
};

class EllipseComponent : public GraphicComponent
{
public:
	virtual void Draw(HDC hdc);

	unsigned radius;
};

class PolygonComponent : public GraphicComponent
{
public:
	virtual void Draw(HDC hdc);

	std::vector<Vector2D> points;
};