
#include "Tree.h"

Tree::Tree(ShaderIF* sIF, double bottomx, double bottomy, double bottomz, double height, double treeRadius, vec3 color)
: shaderIF(sIF)
{
	treeBottom = cryph::AffPoint(bottomx, bottomy, bottomz);
	m_height = height;
	m_baseRadius = treeRadius;
	topColor[0] = color[0]; topColor[1] = color[1]; topColor[2] = color[2];
	defineTree();
}

Tree::~Tree()
{
	delete trunk;
	delete top;
}

void Tree::defineTree()
{

	double trunkHeight = 0.25 * m_height;
	double trunkRadius = 0.25 * m_baseRadius;
	double topHeight = m_height - trunkHeight;
	cryph::AffPoint trunkTop(treeBottom.x, treeBottom.y, treeBottom.z + trunkHeight);

	trunk = new Trunk(shaderIF, trunkRadius, treeBottom, trunkTop);
	top = new TreeTop(shaderIF, trunkTop, m_baseRadius, topHeight, topColor);

}

// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
void Tree::getMCBoundingBox(double* xyzLimits) const
{
	double* temp = new double[6];
	double* temp2 = new double[6];
	trunk->getMCBoundingBox(temp);
	top->getMCBoundingBox(temp2);
	xyzLimits[0] = temp2[0];
	xyzLimits[1] = temp2[1];
	xyzLimits[2] = temp2[2];
	xyzLimits[3] = temp2[3];
	xyzLimits[4] = temp[4];
	xyzLimits[5] = temp2[5];

	delete[] temp;
	delete[] temp2;


}

void Tree::render()
{

	trunk->render();
	top->render();

}
