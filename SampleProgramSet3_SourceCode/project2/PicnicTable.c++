// PicnicTable.c++

#include "PicnicTable.h"

PicnicTable::PicnicTable(ShaderIF* sIF, cryph::AffPoint bottom, float width, float length, float height) : shaderIF(sIF)
{
	this->width = width;
	this->length = length;
	this->height = height;
	m_bottom = bottom;
	definePT();
}

PicnicTable::~PicnicTable()
{
}

void PicnicTable::definePT()
{
	xyz[0] = m_bottom.x - (width/2); xyz[0] = m_bottom.x + (width/2);
	xyz[0] = m_bottom.x - (length/2); xyz[0] = m_bottom.x + (length/2);
	xyz[0] = m_bottom.z; xyz[5] = m_bottom.z + height;
	vec3 brown = {0.32, 0.2, 0.039};
	vec3 wood = {0.41, 0.086, 0.086};
	float benchLegRadius = width / 8;


	cryph::AffPoint leftBench(m_bottom.x-(width/2), m_bottom.y-(length/2), m_bottom.z+height/2);
	cryph::AffPoint lbLeg1(leftBench.x+width/8, leftBench.y+width/8, m_bottom.z);
	models.push_back(new Trunk(shaderIF, lbLeg1, benchLegRadius, (leftBench.z-lbLeg1.z), brown));



}

// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
void PicnicTable::getMCBoundingBox(double* xyzLimits) const
{
	xyzLimits[0] = xyz[0]; xyzLimits[1] = xyz[1];
	xyzLimits[2] = xyz[2]; xyzLimits[3] = xyz[3];
	xyzLimits[4] = xyz[4]; xyzLimits[5] = xyz[5];
}

void PicnicTable::render()
{
	for(int i=0; i<models.size(); i++)
	{
		models[i] -> render();
	}
}
