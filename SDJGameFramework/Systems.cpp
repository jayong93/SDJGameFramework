#include "stdafx.h"
#include "Systems.h"
#include "HandleManagers.h"

using namespace std;

void RenderSystem::Init()
{
	CM.RegisterComponentList(this->shapesList);
}

void RenderSystem::Render()
{
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);
	for (auto& s : shapesList)
	{
		Object* owner = OM.Get(s.owner);
		glPushMatrix();
		{
			glTranslatef(owner->position.x, owner->position.y, owner->position.z);
			if (s.shapeType == Shape::CUBE)
			{
				glutWireCube(s.drawParam[0]);
			}
			else if (s.shapeType == Shape::SPHERE)
			{
				glutWireSphere(s.drawParam[0], s.drawParam[1], s.drawParam[2]);
			}
		}
		glPopMatrix();
	}
	glFlush();
}
