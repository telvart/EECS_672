
#ifndef Cylinder_H
#define Cylinder_H


#include "ModelView.h"
#include "ShaderIF.h"

class Trunk : public ModelView
{
public:

  Trunk(ShaderIF* sIF, cryph::AffPoint bottom, float radius, float height);

  virtual ~Trunk();

	void getMCBoundingBox(double* xyzLimitsF) const;
	void render();

private:

  void defineTrunk();


	GLuint vao[1];
	GLuint vbo[2]; // 0: coordinates; 1: normal vectors

  const int PointsAroundCircle;
  cryph::AffPoint m_bottom, m_top;
  float xyz[6], kd[3], radius;
  ShaderIF* sIF;

};

#endif
