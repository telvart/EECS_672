// PicnicTable.h

#ifndef PicnicTable_H
#define PicnicTable_H

#include "ModelView.h"
#include "ShaderIF.h"
#include "Plane.h"

class PicnicTable : public ModelView
{
public:
	// As before: you will likely want to add parameters to the constructor
	PicnicTable(ShaderIF* sIF, cryph::AffPoint bottom, float width, float height);
	virtual ~PicnicTable();

	// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
	void getMCBoundingBox(double* xyzLimitsF) const;
	void render();


private:

	ShaderIF* shaderIF;
	cryph::AffPoint m_bottom;
	float width, height;
	float xyz[6];

};

#endif
