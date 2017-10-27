// TreeTop.c++

#include "TreeTop.h"

TreeTop::TreeTop(ShaderIF* sIF, cryph::AffPoint bottom, double baseRadius, double height, PhongMaterial& mat)
:PointsAroundBase(20), radius(baseRadius), SceneElement(sIF, mat)
{
	m_bottom = bottom;
	m_top.x = bottom.x;
	m_top.y = bottom.y;
	m_top.z = bottom.z + height;
	axis = m_bottom - m_top;
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

	int totalPoints = PointsAroundBase + 2;

	vec3* verticies = new vec3[totalPoints];
	vec3* normals = new vec3[totalPoints];

	double 	theta 	= 0.0;
	double 	dTheta 	= 2.0 * M_PI / PointsAroundBase;

	cryph::AffVector U(1, 0, 0); //x-axis
	cryph::AffVector V(0, 1, 0); //y-axis

	cryph::AffPoint first = m_bottom + radius * (cos(theta)*U + sin(theta)*V);
	cryph::AffVector first2 = first - m_bottom;
	first2.normalize();

	m_top.aCoords(verticies, 0);
	first.aCoords(verticies, totalPoints-1);

	normals[0][0] = axis.dx; normals[0][1] = axis.dy; normals [0][2] = axis.dz;
	normals[totalPoints-1][0] = first2.dx; normals[totalPoints-1][1] = first2.dy; normals[totalPoints-1][2] = first2.dz;


	for(int i=1; i<=PointsAroundBase; i++)
	{
		cryph::AffPoint p = m_bottom + radius * (cos(theta)*U + sin(theta)*V);

		p.aCoords(verticies, i);

		theta += dTheta;

		cryph::AffVector Vhat = m_top - p;
		cryph::AffVector VParallel = (axis.dot(Vhat)) * axis;
		cryph::AffVector vPerp = Vhat - VParallel;

		vPerp.normalize();
		normals[i][0] = vPerp.dx;
		normals[i][1] = vPerp.dy;
		normals[i][2] = vPerp.dz;
	}

	glGenVertexArrays(1, vao);
	glGenBuffers(2, vbo);

	glBindVertexArray(vao[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

	glBufferData(GL_ARRAY_BUFFER, totalPoints * sizeof(vec3), verticies, GL_STATIC_DRAW);
	glVertexAttribPointer(shaderIF->pvaLoc("mcPosition"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(shaderIF->pvaLoc("mcPosition"));

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, totalPoints * sizeof(vec3), normals, GL_STATIC_DRAW);
	glVertexAttribPointer(shaderIF->pvaLoc("mcNormal"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(shaderIF->pvaLoc("mcNormal"));

	delete[] verticies;
	delete[] normals;

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

	glBindVertexArray(vao[0]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, PointsAroundBase+2);

	glUseProgram(program);

}
