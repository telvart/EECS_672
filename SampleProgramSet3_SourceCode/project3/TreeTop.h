// TreeTop.h

#ifndef TreeTop_H
#define TreeTop_H

#include "SceneElement.h"

typedef float vec3[3];

class TreeTop : public SceneElement
{
public:
	TreeTop(ShaderIF* sIF, cryph::AffPoint bottom, double baseRadius, double height, PhongMaterial& mat);
	virtual ~TreeTop();

	void getMCBoundingBox(double* xyzLimitsF) const;
	void render();
private:

	void defineTreeTop();
	const int PointsAroundBase;
	const int radius;

	GLuint vao[1];
	GLuint vbo[2];

	float xyz[6];

	BasicShape* theCone;
	BasicShapeRenderer* theConeR;

	cryph::AffPoint m_bottom;
	cryph::AffPoint m_top;
	cryph::AffVector axis;

};

#endif
