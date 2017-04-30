#include "stdafx.h"
#include "Systems.h"
#include "HandleManagers.h"

using namespace std;

Graphic::~Graphic()
{
	if (bitmap)
		DeleteObject(bitmap);
	if (hdc)
		DeleteDC(hdc);
}

void Graphic::Init(HWND hWnd, HDC & memDC)
{
	this->hWnd = hWnd;

	HDC orgDC = GetDC(hWnd);
	hdc = CreateCompatibleDC(orgDC);

	RECT crt;
	GetClientRect(hWnd, &crt);
	bitmap = CreateCompatibleBitmap(orgDC, crt.right - crt.left, crt.bottom - crt.top);

	SelectObject(hdc, bitmap);
	memDC = hdc;

	ReleaseDC(hWnd, orgDC);

	RegisterComponent(ellipseList);
	RegisterComponent(polygonList);
}

void Graphic::Update()
{
	RECT crt;
	GetClientRect(hWnd, &crt);
	FillRect(hdc, &crt, (HBRUSH)GetStockObject(WHITE_BRUSH));

	struct OrderComp
	{
	public:
		bool operator() (const pair<int, ComponentHandle>& a, const pair<int, ComponentHandle>& b)
		{
			return (a.first < b.first);
		}
	};

	priority_queue<pair<int, ComponentHandle>, vector<pair<int, ComponentHandle>>, OrderComp> orderHeap;

	for_each(ellipseList.arr.begin(), ellipseList.arr.end(), [&orderHeap](const auto& c) {
		orderHeap.emplace(c.order, c.handle);
	});
	for_each(polygonList.arr.begin(), polygonList.arr.end(), [&orderHeap](const auto& c) {
		orderHeap.emplace(c.order, c.handle);
	});

	while (orderHeap.empty() == false)
	{
		const ComponentHandle& handle = orderHeap.top().second;
		GraphicComponent* compo = (GraphicComponent*)CM.Get(handle);
		compo->Draw(hdc);
		orderHeap.pop();
	}

	InvalidateRect(hWnd, nullptr, false);
}
