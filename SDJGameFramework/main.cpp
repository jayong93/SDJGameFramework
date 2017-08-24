#include "stdafx.h"
#pragma comment(lib, "luajit/lib/lua51.lib")

#ifndef SDJ_TEST_BUILD

#include "Framework.h"

Framework framework;

void DrawScene();
void Update();
void Reshape(int w, int h);

int main()
{
	sol::state lua;
	framework.Init();
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(1280, 720);
	glutInitWindowPosition(250, 250);
	glutCreateWindow("Framework Test App");
	glutDisplayFunc(DrawScene);
	glutReshapeFunc(Reshape);
	glutIdleFunc(Update);
	glutMainLoop();
}

#endif

void DrawScene()
{
	framework.Render();
}

void Update()
{
	framework.MainLoop();
}

void Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glOrtho(-10., 10., -10., 10., -1., 1.);
}
