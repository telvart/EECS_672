// Tree.h

#ifndef TREE_H
#define TREE_H

#include "ModelView.h"
#include "ShaderIF.h"

class Tree : public ModelView
{
public:
	// As before: you will likely want to add parameters to the constructor
	Tree(ShaderIF* sIF);
	virtual ~Tree();

	// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
	void getMCBoundingBox(double* xyzLimitsF) const;
	void render();
private:
	ShaderIF* shaderIF;
	float kd[3];
};

#endif
