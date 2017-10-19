
#include "House.h"

House::House(ShaderIF* sIF, cryph::AffPoint houseBottom, float width, float length, float height)
 : shaderIF(sIF)
{
	this->width = width;
	this->length = length;
	this->height = height;
	m_bottom = houseBottom;
}

House::~House()
{
}



void House::defineHouse()
{
	xyz[0] = m_bottom.x - (width/2);
	xyz[1] = m_bottom.x + (width/2);
	xyz[2] = m_bottom.y - (length/2);
	xyz[3] = m_bottom.y + (length/2);
	xyz[4] = m_bottom.z;
	xyz[5] = m_bottom.z + height;
}

// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
void House::getMCBoundingBox(double* xyzLimits) const
{
	xyzLimits[0] = -1000.0; // xmin  Give real values!
	xyzLimits[1] = 1000.0;  // xmax         |
	xyzLimits[2] = -1234.5; // ymin         |
	xyzLimits[3] = -1011.2; // ymax         |
	xyzLimits[4] = -3000.0; // zmin         |
	xyzLimits[5] = -2000.0; // zmax        \_/
}

void House::render()
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
