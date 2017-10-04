// House.h

#ifndef HOUSE_H
#define HOUSE_H

#include "ModelView.h"
#include "ShaderIF.h"

class House : public ModelView
{
public:
	// As before: you will likely want to add parameters to the constructor
	House(ShaderIF* sIF);
	virtual ~House();

	// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
	void getMCBoundingBox(double* xyzLimitsF) const;
	void render();
private:
	ShaderIF* shaderIF;
	float kd[3];
};

#endif
