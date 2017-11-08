// TreeTop.c++

#include "TreeTop.h"

TreeTop::TreeTop(ShaderIF* sIF, cryph::AffPoint bottom, double baseRadius, double height, PhongMaterial& mat)
:PointsAroundBase(40), radius(baseRadius), SceneElement(sIF, mat)
{
	m_bottom = bottom;
	m_top = cryph::AffPoint(bottom.x, bottom.y, bottom.z+height);
	axis = m_top - m_bottom;
	axis.normalize();
	defineTreeTop();
}

TreeTop::~TreeTop()
{
	glDeleteBuffers(2, vbo);
	glDeleteVertexArrays(1, vao);
}

void TreeTop::defineTreeTop()
{
	xyz[0] = m_bottom.x - radius; xyz[1] = m_bottom.x + radius;
	xyz[2] = m_bottom.y - radius; xyz[3] = m_bottom.y + radius;
	xyz[4] = m_bottom.z; 					xyz[5] = m_top.z;

	// int totalPoints = 2 * (PointsAroundBase + 1);
	//
	// vec3* verticies = new vec3[totalPoints];
	// vec3* normals = new vec3[totalPoints];
	//
	// double 	theta 	= 0.0;
	// double 	dTheta 	= 2.0 * M_PI / PointsAroundBase;
	//
	// cryph::AffVector U(1, 0, 0); //x-axis
	// cryph::AffVector V(0, 1, 0); //y-axis
	//
	//
	// for (int i=0 ; i <= PointsAroundBase ; i++)
	// {
  //   cryph::AffPoint b = m_bottom + radius * (cos(theta)*U + sin(theta)*V);
  //   cryph::AffPoint t = m_top    + 0.1    * (cos(theta)*U + sin(theta)*V);
  //   theta += dTheta;
  //   int j = 2*i;
	//
  //   b.aCoords(verticies, j);
  //   t.aCoords(verticies, j+1);
	//
	//
	// 	cryph::AffVector eHat = b - t;
	//
  //   cryph::AffVector norm = axis -(axis.dot(eHat) * eHat);
	// 	norm.normalize();
  //   normals[j][0] = norm.dx; normals[j][1] = norm.dy; normals[j][2] = norm.dz;
  //   normals[j+1][0] = norm.dx; normals[j+1][1] = norm.dy; normals[j+1][2] = norm.dz;
	// }

	//NOTE: The basic shape utilities are very helpful
	cryph::AffVector dir(20,20,0);

	theCone = BasicShape::makeBoundedCone(m_bottom, m_top, radius, 0.1, 35, 2, BasicShape::CAP_AT_NEITHER);
	theConeR = new BasicShapeRenderer(shaderIF, theCone);

	// glGenVertexArrays(1, vao);
	// glGenBuffers(2, vbo);
	//
	// glBindVertexArray(vao[0]);
	// glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	//
	// glBufferData(GL_ARRAY_BUFFER, totalPoints * sizeof(vec3), verticies, GL_STATIC_DRAW);
	// glVertexAttribPointer(shaderIF->pvaLoc("mcPosition"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	// glEnableVertexAttribArray(shaderIF->pvaLoc("mcPosition"));
	//
	// glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	// glBufferData(GL_ARRAY_BUFFER, totalPoints * sizeof(vec3), normals, GL_STATIC_DRAW);
	// glVertexAttribPointer(shaderIF->pvaLoc("mcNormal"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	// glEnableVertexAttribArray(shaderIF->pvaLoc("mcNormal"));
	//
	// delete[] verticies;
	// delete[] normals;

}

// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
void TreeTop::getMCBoundingBox(double* xyzLimits) const
{
	xyzLimits[0] = xyz[0]; xyzLimits[1] = xyz[1];
	xyzLimits[2] = xyz[2]; xyzLimits[3] = xyz[3];
	xyzLimits[4] = xyz[4]; xyzLimits[5] = xyz[5];
}

void TreeTop::render()
{

	GLint program;
	glGetIntegerv(GL_CURRENT_PROGRAM, &program);
	glUseProgram(shaderIF->getShaderPgmID());

	establishView();
	establishMaterial();
	establishLightingEnvironment();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	theConeR -> drawShape();

	//glBindVertexArray(vao[0]);
	//glDrawArrays(GL_TRIANGLE_STRIP, 0, 2*(PointsAroundBase+1));

	glUseProgram(program);

}
