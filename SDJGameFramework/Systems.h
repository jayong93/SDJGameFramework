#pragma once

#include "ComponentManager.h"

struct Message;

class Graphic
{
public:
	Graphic() :
		hWnd{ 0 },
		hdc{ 0 },
		bitmap{ 0 }
	{}
	virtual ~Graphic();

	void Init(HWND hWnd, HDC& memDC);
	void Update();

private:
	HWND hWnd;
	HDC hdc;
	HBITMAP bitmap;

	CompoList<EllipseComponent> ellipseList;
	CompoList<PolygonComponent> polygonList;
};