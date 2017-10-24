
#ifndef Cylinder_H
#define Cylinder_H


#include "ModelView.h"
#include "ShaderIF.h"

typedef float vec3[3];

class Trunk : public ModelView
{
public:

  Trunk(ShaderIF* sIF, cryph::AffPoint bottom, float radius, float height, vec3 color);

  virtual ~Trunk();

	void getMCBoundingBox(double* xyzLimitsF) const;
	void render();

private:

  void defineTrunk();


	GLuint vao[1];
	GLuint vbo[2]; // 0: coordinates; 1: normal vectors

  const int PointsAroundCircle;
  cryph::AffPoint m_bottom, m_top;
  float xyz[6], kd[3], ka[3], radius;
  ShaderIF* sIF;

};

#endif
