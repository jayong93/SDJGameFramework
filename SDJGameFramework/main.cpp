#include "stdafx.h"
#pragma comment(lib, "luajit/lib/lua51.lib")

#ifndef SDJ_TEST_BUILD

#include "Framework.h"
constexpr char* WINDOW_TITLE = "Framework Test App";

void DrawScene();
void Update();
void Reshape(int w, int h);
void FpsUpdate(int val);

int main()
{
	FW.Init();
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(1280, 720);
	glutInitWindowPosition(250, 250);
	glutCreateWindow(WINDOW_TITLE);
	glutDisplayFunc(DrawScene);
	glutReshapeFunc(Reshape);
	glutIdleFunc(Update);
	glutTimerFunc(1000, FpsUpdate, 0);
	glutMainLoop();
}

void DrawScene()
{
	FW.Render();
}

void Update()
{
	FW.MainLoop();
}

void Reshape(int w, int h)
{
	FW.SetView(w, h);
}

void FpsUpdate(int val)
{
	auto fps = 1/FW.timer.GetFPS();
	char titleWithFPS[40];
	snprintf(titleWithFPS, sizeof(titleWithFPS), "%s(%.1lffps)", WINDOW_TITLE, fps);
	glutSetWindowTitle(titleWithFPS);
	glutTimerFunc(1000, FpsUpdate, 0);
}

#endif
