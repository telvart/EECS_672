// PicnicTable.c++

#include "PicnicTable.h"

PicnicTable::PicnicTable(ShaderIF* sIF, cryph::AffPoint bottom, float width, float height) : shaderIF(sIF)
{

}

PicnicTable::~PicnicTable()
{
}

// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
void PicnicTable::getMCBoundingBox(double* xyzLimits) const
{
	xyzLimits[0] = -1000.0;
	xyzLimits[1] = 1000.0;  // xmax         |
	xyzLimits[2] = -1234.5; // ymin         |
	xyzLimits[3] = -1011.2; // ymax         |
	xyzLimits[4] = -3000.0; // zmin         |
	xyzLimits[5] = -2000.0; // zmax        \_/
}

void PicnicTable::render()
{
	// 1. Save current and establish new current shader program
	// ...

	// 2. Establish "mc_ec" and "ec_lds" matrices
	// ...

	// 3. Set GLSL's "kd" variable using this object's "kd" instance variable
	// ...

	// 4. Establish any other attributes and make one or more calls to
	//    glDrawArrays and/or glDrawElements
	// ...

	// 5. Reestablish previous shader program
	// ...
}
