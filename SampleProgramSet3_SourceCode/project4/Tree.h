// Tree.h

#ifndef TREE_H
#define TREE_H

#include "SceneElement.h"
#include "Trunk.h"
#include "TreeTop.h"

typedef float vec3[3];

class Tree : public ModelView
{
public:
	// As before: you will likely want to add parameters to the constructor
	Tree(ShaderIF* sIF, double bottomx, double bottomy, double bottomz, double height, double treeRadius, PhongMaterial& top);
	virtual ~Tree();

	// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
	void getMCBoundingBox(double* xyzLimitsF) const;
	void render();
private:

	void defineTree();

	TreeTop* top;
	Trunk* trunk;
	PhongMaterial topMat;

	float xyz[6];

	cryph::AffPoint treeBottom;
	double m_height;
	double m_baseRadius;

	ShaderIF* shaderIF;

};

#endif
