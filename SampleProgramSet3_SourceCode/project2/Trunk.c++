
#include "Trunk.h"

cryph::AffVector dir;

#define N_POINTS_AROUND_SLICE 20

typedef float vec3[3];

Trunk::Trunk(ShaderIF* sIF, float radius, const cryph::AffPoint& b, const cryph::AffPoint& t)
{
  this->sIF = sIF;
	this->radius = radius;
	m_bottom = b;
	m_top = t;
	dir = (m_top - m_bottom);
	dir.normalize();

	defineTrunk();

  kd[0] = 0.32; kd[1] = 0.2; kd[2] = 0.039;
}

Trunk::~Trunk()
{
	glDeleteBuffers(2, vbo);
	glDeleteVertexArrays(1, vao);
}

void Trunk::getMCBoundingBox(double* xyzLimits) const
{
	xyzLimits[0] = xyz[0]; xyzLimits[1] = xyz[1];
	xyzLimits[2] = xyz[2]; xyzLimits[3] = xyz[3];
	xyzLimits[4] = xyz[4]; xyzLimits[5] = xyz[5];
}

void Trunk::defineTrunk()
{
	xyz[0] = m_bottom.x 	- radius;
	xyz[1] = m_bottom.x 	+ radius;
	xyz[2] = m_bottom.y 	- radius;
	xyz[3] = m_top.y 		+ radius;
	xyz[4] = m_bottom.z 	- radius;
	xyz[5] = m_bottom.z 	+ radius;

	double 	theta 	= 0.0;
	double 	dTheta 	= 2.0 * M_PI / N_POINTS_AROUND_SLICE;

	int 	nPoints = 2 * (N_POINTS_AROUND_SLICE + 1);
	vec3 	coords[nPoints];
	vec3 	normals[nPoints];

	cryph::AffVector toPoint 			= cryph::AffVector(dir.dx - 1, dir.dy, dir.dz + 1).cross(dir);
	cryph::AffPoint  b 				= m_bottom 	+ radius * toPoint;
	cryph::AffPoint  t 				= m_top 		+ radius * toPoint;

	cryph::AffVector bPerpendicular = (b - m_bottom).cross(t - b).cross(t - b);

	for (int i=0 ; i <= N_POINTS_AROUND_SLICE ; i++)
	{
		/* Set the values for the new coordinates after moving to the next point */
		coords[2*i][0] 		= b.x;
		coords[2*i][1] 		= b.y;
		coords[2*i][2] 		= b.z;
		coords[2*i+1][0] 	= t.x;
		coords[2*i+1][1] 	= t.y;
		coords[2*i+1][2] 	= t.z;

		/* Set the values for the normal vectors */
		normals[2*i][0] = bPerpendicular.dx;
		normals[2*i][1] = bPerpendicular.dy;
		normals[2*i][2] = bPerpendicular.dz;

		normals[2*i+1][0] = bPerpendicular.dx;
		normals[2*i+1][1] = bPerpendicular.dy;
		normals[2*i+1][2] = bPerpendicular.dz;

		b = m_bottom 	+ ((radius * cos(dTheta)) * toPoint + ((radius * sin(dTheta)) * dir.cross(toPoint)));
		t = m_top + ((radius * cos(dTheta)) * toPoint + ((radius * sin(dTheta)) * dir.cross(toPoint)));
		toPoint = (b - m_bottom);
		toPoint.normalize();

		cryph::AffVector helperV = (b - m_bottom).cross(t - b);
		bPerpendicular = helperV.cross(t - b);

	}

	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);

	glGenBuffers(2, vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, nPoints * sizeof(vec3), coords, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(sIF->pvaLoc("mcPosition"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(sIF->pvaLoc("mcPosition"));

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, nPoints * sizeof(vec3), normals, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(sIF->pvaLoc("mcNormal"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(sIF->pvaLoc("mcNormal"));

}

void Trunk::render()
{
	GLint pgm;
	glGetIntegerv(GL_CURRENT_PROGRAM, &pgm);
	glUseProgram(sIF->getShaderPgmID());

	cryph::Matrix4x4 mc_ec, ec_lds;
	getMatrices(mc_ec, ec_lds);
	float mat[16];
	glUniformMatrix4fv(sIF->ppuLoc("mc_ec"), 	1, false, mc_ec.	extractColMajor(mat));
	glUniformMatrix4fv(sIF->ppuLoc("ec_lds"), 	1, false, ec_lds.	extractColMajor(mat));

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glUniform3fv(sIF->ppuLoc("kd"), 1, kd);
	glBindVertexArray(vao[0]);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 2*(N_POINTS_AROUND_SLICE+1));

	glUseProgram(pgm);
}
