#include "stdafx.h"
#include "Component.h"
#include "ObjectManager.h"

void EllipseComponent::Draw(HDC hdc)
{
	HPEN pen = CreatePen(PS_SOLID, penWidth, RGB(penColor.r, penColor.g, penColor.b));
	HBRUSH brush = CreateSolidBrush(RGB(brushColor.r, brushColor.g, brushColor.b));
	HGDIOBJ oldPen = SelectObject(hdc, pen);
	HGDIOBJ oldBrush = SelectObject(hdc, brush);
	Vector2D& ownerPos = OM.Get(owner)->position;

	Ellipse(hdc, ownerPos.x - radius, ownerPos.y - radius, ownerPos.x + radius, ownerPos.y + radius);

	SelectObject(hdc, oldPen);
	SelectObject(hdc, oldBrush);
	DeleteObject(pen);
	DeleteObject(brush);
}

void PolygonComponent::Draw(HDC hdc)
{
	HPEN pen = CreatePen(PS_SOLID, penWidth, RGB(penColor.r, penColor.g, penColor.b));
	HBRUSH brush = CreateSolidBrush(RGB(brushColor.r, brushColor.g, brushColor.b));
	HGDIOBJ oldPen = SelectObject(hdc, pen);
	HGDIOBJ oldBrush = SelectObject(hdc, brush);
	Vector2D& ownerPos = OM.Get(owner)->position;

	std::vector<Vector2D> movedPoints = points;
	std::for_each(movedPoints.begin(), movedPoints.end(), [&ownerPos](auto& p) {
		p = p + ownerPos;
	});

	Polygon(hdc, (POINT*)movedPoints.data(), movedPoints.size());

	SelectObject(hdc, oldPen);
	SelectObject(hdc, oldBrush);
	DeleteObject(pen);
	DeleteObject(brush);
}
