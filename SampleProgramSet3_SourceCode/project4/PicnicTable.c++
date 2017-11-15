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
	for(int i=0; i<models.size(); i++)
		delete models[i];
}

void PicnicTable::definePT()
{
	xyz[0] = m_bottom.x - (width/2); xyz[0] = m_bottom.x + (width/2);
	xyz[0] = m_bottom.x - (length/2); xyz[0] = m_bottom.x + (length/2);
	xyz[0] = m_bottom.z; xyz[5] = m_bottom.z + height;

	//vec3 brown = {0.32, 0.2, 0.039};
	//vec3 wood = {0.41, 0.086, 0.086};
	PhongMaterial wood(0.41, 0.086, 0.086, 0.41, 0.086, 0.086, 0.41, 0.086, 0.086, 10, 1);

	float legRadius = width / 16;
	float benchWidth = width/6;
	float tableWidth = 0.5*width;

	cryph::AffPoint table(m_bottom.x-tableWidth/2, m_bottom.y-(length/2), m_bottom.z+height-(height/5));
	cryph::AffPoint tablel1(table.x+legRadius, table.y+legRadius, m_bottom.z);
	cryph::AffPoint tablel2(table.x+tableWidth-legRadius, table.y+legRadius, m_bottom.z);
	cryph::AffPoint tablel3(tablel1.x, table.y+length-legRadius, m_bottom.z);
	cryph::AffPoint tablel4(tablel2.x, tablel3.y, m_bottom.z);

	cryph::AffPoint leftBench(m_bottom.x-(width/2)-5, m_bottom.y-(length/2), m_bottom.z+height/2);
	cryph::AffPoint rightBench(m_bottom.x+(width/2)+5, m_bottom.y-(length/2), m_bottom.z+height/2);

	cryph::AffPoint lbLeg1(leftBench.x+legRadius, leftBench.y+legRadius, m_bottom.z);
	cryph::AffPoint lbLeg2(leftBench.x+legRadius, leftBench.y+length-legRadius, m_bottom.z);
	cryph::AffPoint rbLeg1(rightBench.x-legRadius, rightBench.y+legRadius, m_bottom.z);
	cryph::AffPoint rbLeg2(rightBench.x-legRadius, rightBench.y+length-legRadius, m_bottom.z);

	models.push_back(new Block(shaderIF, table.x, table.y, table.z, tableWidth, length, 5, wood));//the table and two benches
	models.push_back(new Block(shaderIF, leftBench.x, leftBench.y, leftBench.z, benchWidth, length, 5, wood));
	models.push_back(new Block(shaderIF, rightBench.x, rightBench.y, rightBench.z, -1*benchWidth, length, 5, wood));

	models.push_back(new Trunk(shaderIF, tablel1, legRadius, table.z-m_bottom.z, wood));//four legs off table
	models.push_back(new Trunk(shaderIF, tablel2, legRadius, table.z-m_bottom.z, wood));
	models.push_back(new Trunk(shaderIF, tablel3, legRadius, table.z-m_bottom.z, wood));
	models.push_back(new Trunk(shaderIF, tablel4, legRadius, table.z-m_bottom.z, wood));

	models.push_back(new Trunk(shaderIF, lbLeg1, legRadius, (leftBench.z-lbLeg1.z), wood));//four legs of the benches
	models.push_back(new Trunk(shaderIF, lbLeg2, legRadius, (leftBench.z-lbLeg2.z), wood));
	models.push_back(new Trunk(shaderIF, rbLeg1, legRadius, (leftBench.z-rbLeg1.z), wood));
	models.push_back(new Trunk(shaderIF, rbLeg2, legRadius, (leftBench.z-rbLeg2.z), wood));

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
		models[i] -> render();

}
