
#include "Trunk.h"

cryph::AffVector dir;

typedef float vec3[3];

Trunk::Trunk(ShaderIF* sIF, cryph::AffPoint bottom, float radius, float height)
  :PointsAroundCircle(20)
{
  this->sIF = sIF;
	this->radius = radius;
	m_bottom = bottom;
  m_top = cryph::AffPoint(m_bottom.x, m_bottom.y, m_bottom.z+height);
	//dir.normalize();

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
	xyz[0] = m_bottom.x - radius; xyz[1] = m_bottom.x + radius;
	xyz[2] = m_bottom.y - radius;	xyz[3] = m_bottom.y + radius;
	xyz[4] = m_bottom.z - radius;	xyz[5] = m_bottom.z + radius;

	int 	nPoints = 2 * (PointsAroundCircle + 1);
	vec3* 	coords = new vec3[nPoints];
	vec3* 	normals = new vec3[nPoints];

  double 	theta 	= 0.0;
  double 	dTheta 	= 2.0 * M_PI / PointsAroundCircle;
  cryph::AffVector U(1, 0, 0); //x-axis
  cryph::AffVector V(0, 1, 0); //y-axis

	for (int i=0 ; i <= PointsAroundCircle ; i++)
	{
    cryph::AffPoint b = m_bottom + radius * (cos(theta)*U + sin(theta)*V);
    cryph::AffPoint t = m_top    + radius * (cos(theta)*U + sin(theta)*V);
    theta += dTheta;
    int j = 2*i;

    b.aCoords(coords, j);
    t.aCoords(coords, j+1);

    cryph::AffVector norm = b - m_bottom;
    norm.normalize();
    normals[j][0] = norm.dx; normals[j][1] = norm.dy; normals[j][2] = norm.dz;
    normals[j+1][0] = norm.dx; normals[j+1][1] = norm.dy; normals[j+1][2] = norm.dz;
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

	glUniform3fv(sIF->ppuLoc("kd"), 1, kd);
	glBindVertexArray(vao[0]);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 2*(PointsAroundCircle+1));

	glUseProgram(pgm);
}
