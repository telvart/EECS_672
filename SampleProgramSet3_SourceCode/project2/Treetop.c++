// TreeTop.c++

#include "Treetop.h"

TreeTop::TreeTop(ShaderIF* sIF, cryph::AffPoint bottom, double baseRadius, double height)
:PointsAroundBase(20), radius(baseRadius)
{
	// DON'T FORGET TO SET INSTANCE VARIABLE "kd", PERHAPS USING
	// SOME CONSTRUCTOR PARAMETERS
	kd[0] = 0.004; kd[1] = 0.475; kd[2] = 0.435;
	m_bottom = bottom;
	m_top.x = bottom.x;
	m_top.y = bottom.y;
	m_top.z = bottom.z + height;
	axis = m_top - m_bottom;
	defineTreeTop();
}

TreeTop::~TreeTop()
{
	glDeleteBuffers(2, vbo);
	glDeleteVertexArrays(1, vao);
}

void TreeTop::defineTreeTop()
{

	int totalPoints = (2 * PointsAroundBase) + 1;

	vec3* verticies = new vec3[totalPoints];
	vec3* normals = new vec3[totalPoints];

	xyz[0] = m_bottom.x - radius;
	xyz[1] = m_bottom.x + radius;
	xyz[2] = m_bottom.y - radius;
	xyz[3] = m_bottom.y + radius;
	xyz[4] = m_bottom.z;
	xyz[5] = m_top.z;

	double 	theta 	= 0.0;
	double 	dTheta 	= 2.0 * M_PI / PointsAroundBase;

	//cryph::AffVector toPoint = cryph::AffVector(axis.dx - 1, axis.dy, axis.dz + 1).cross(axis);
	//cryph::AffPoint  b = m_bottom + radius * toPoint;
	//cryph::AffVector bPerpendicular = (b - m_bottom).cross(m_top - b).cross(m_top - b);
	//cryph::AffPoint  t = m_top + radius * toPoint;

	cryph::AffVector U(1, 0, 0);
	cryph::AffVector V(0, 1, 0);


	std::cout<<"Top ("<<m_top.x<<", "<<m_top.y<<", "<<m_top.z<<")\n\n";
	// std::cout

	for(int i=0; i<PointsAroundBase; i++)
	{
		cryph::AffPoint p = m_bottom + radius * (cos(theta)*U + sin(theta)*V);
		std::cout<<"Point ("<<p.x<<", "<<p.y<<", "<<p.z<<")\n";
		p.aCoords(verticies, 2*i);
		m_top.aCoords(verticies, 2*i+1);
		theta += dTheta;
		vec3 n = {1,1,1};
		normals[2*i][0] = n[0]; normals[2*i][1] = n[1]; normals[2*i][2] = n[2];
		normals[2*i+1][0] = n[0]; normals[2*i+1][1] = n[1]; normals[2*i+1][2] = n[2];

		// toPoint = (b - m_bottom);
		// toPoint.normalize();
		//
		// cryph::AffVector helperV = (b - m_bottom).cross(m_top - b);
		// bPerpendicular = helperV.cross(m_top - b);
	}

	glGenVertexArrays(1, vao);
	glGenBuffers(2, vbo);

	glBindVertexArray(vao[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

	glBufferData(GL_ARRAY_BUFFER, totalPoints * sizeof(vec3), verticies, GL_STATIC_DRAW);
	glVertexAttribPointer(shaderIF->pvaLoc("mcPosition"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	//glEnableVertexAttribArray(shaderIF->pvaLoc("mcPosition"));

	// glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	// glBufferData(GL_ARRAY_BUFFER, totalPoints * sizeof(vec3), normals, GL_STATIC_DRAW);
	// glVertexAttribPointer(shaderIF->pvaLoc("mcNormal"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	// glEnableVertexAttribArray(shaderIF->pvaLoc("mcNormal"));




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
	// 1. Save current and establish new current shader program
	// ...
	// GLint program;
	// glGetIntegerv(GL_CURRENT_PROGRAM, &program);
	// glUseProgram(shaderIF->getShaderPgmID());
	//
	// cryph::Matrix4x4 mc_ec, ec_lds;
	// getMatrices(mc_ec, ec_lds);
	// float mat[16];
	// glUniformMatrix4fv(shaderIF->ppuLoc("mc_ec"), 	1, false, mc_ec.	extractColMajor(mat));
	// glUniformMatrix4fv(shaderIF->ppuLoc("ec_lds"), 	1, false, ec_lds.	extractColMajor(mat));
	//
	// //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//
	// glUniform3fv(shaderIF->ppuLoc("kd"), 1, kd);
	// glBindVertexArray(vao[0]);
	// glDrawArrays(GL_TRIANGLE_STRIP, 0, (2*PointsAroundBase) +1);
	//
	// glUseProgram(program);

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
