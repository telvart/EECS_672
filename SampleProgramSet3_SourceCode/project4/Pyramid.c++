// Pyramid.c++

#include "Pyramid.h"

Pyramid::Pyramid(ShaderIF* sIF, cryph::AffPoint bottom, float height, float width, PhongMaterial& mat)
: SceneElement(sIF, mat)
{
	m_bottom = bottom;
	m_top = cryph::AffPoint(bottom.x, bottom.y, bottom.z+height);
	this->height = height;
	this->width = width;
	definePyramid();
}

Pyramid::~Pyramid()
{
	glDeleteBuffers(4, ebo);
	glDeleteBuffers(1, vbo);
	glDeleteVertexArrays(1, vao);
}

void Pyramid::definePyramid()
{
	cryph::AffPoint bl(m_bottom.x-(width/2), m_bottom.y-(width/2), m_bottom.z);
	cryph::AffPoint br(m_bottom.x+(width/2), m_bottom.y-(width/2), m_bottom.z);
	cryph::AffPoint tl(m_bottom.x-(width/2), m_bottom.y+(width/2), m_bottom.z);
	cryph::AffPoint tr(m_bottom.x+(width/2), m_bottom.y+(width/2), m_bottom.z);

	m_tl = tl; m_tr = tr;
	m_bl = bl; m_br = br;

	xyz[0] = bl.x; xyz[1] = br.x;
	xyz[2] = bl.y; xyz[3] = tl.y;
	xyz[4] = m_bottom.z; xyz[5] = m_bottom.z+height;

	vec3* coords = new vec3[5];

	bl.aCoords(coords, 0);
	br.aCoords(coords, 1);
	tl.aCoords(coords, 2);
	tr.aCoords(coords, 3);
	m_top.aCoords(coords, 4);

	glGenVertexArrays(1, vao);
	glGenBuffers(2, vbo);

	glBindVertexArray(vao[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

	int bSize = 5 * sizeof(vec3);

	glBufferData(GL_ARRAY_BUFFER, bSize, coords, GL_STATIC_DRAW);
	glVertexAttribPointer(shaderIF->pvaLoc("mcPosition"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(shaderIF->pvaLoc("mcPosition"));

	glGenBuffers(4, ebo);
	for(int i=0; i<4; i++)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[i]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3*sizeof(GLuint), indexList[i], GL_STATIC_DRAW);
	}

	glDisableVertexAttribArray(shaderIF->pvaLoc("mcNormal"));

	delete[] coords;

}

void Pyramid::getMCBoundingBox(double* xyzLimits) const
{
	xyzLimits[0] = xyz[0]; xyzLimits[1] = xyz[1];
	xyzLimits[2] = xyz[2]; xyzLimits[3] = xyz[3];
	xyzLimits[4] = xyz[4]; xyzLimits[5] = xyz[5];
}

GLuint Pyramid::indexList[4][3] = {
	{ 1, 3, 4 },
	{ 0, 1, 4 },
	{ 0, 2, 4 },
	{ 2, 3, 4 }
};

void Pyramid::render()
{

	GLint pgm;
	glGetIntegerv(GL_CURRENT_PROGRAM, &pgm);
	glUseProgram(shaderIF->getShaderPgmID());

	glBindVertexArray(vao[0]);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	establishMaterial();
	establishView();
	establishLightingEnvironment();

	cryph::AffVector temp, temp2, n;
	temp = m_tl - m_bl;
	temp2 = m_br - m_bl;
	n = temp.cross(temp2);
	n.normalize();
	glVertexAttrib3f(shaderIF->pvaLoc("mcNormal"), n.dx, n.dy, n.dz);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	temp = m_top - m_br;
	temp2 = m_top - m_tr;
	n = temp.cross(temp2);
	n.normalize();
	glVertexAttrib3f(shaderIF->pvaLoc("mcNormal"), n.dx, n.dy, n.dz);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[0]);
	glDrawElements(GL_TRIANGLE_STRIP, 3, GL_UNSIGNED_INT, nullptr);

	temp = m_top - m_bl;
	temp2 = m_top - m_br;
	n = temp.cross(temp2);
	n.normalize();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[1]);
	glVertexAttrib3f(shaderIF->pvaLoc("mcNormal"), n.dx, n.dy, n.dz);
	glDrawElements(GL_TRIANGLE_STRIP, 3, GL_UNSIGNED_INT, nullptr);

	temp = m_top - m_bl;
	temp2 = m_top - m_tl;
	n = temp.cross(temp2);
	n.normalize();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[2]);
	glVertexAttrib3f(shaderIF->pvaLoc("mcNormal"), n.dx, n.dy, n.dz);
	glDrawElements(GL_TRIANGLE_STRIP, 3, GL_UNSIGNED_INT, nullptr);

	temp = m_top - m_tl;
	temp2 = m_top - m_tr;
	n = temp.cross(temp2);
	n.normalize();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[3]);
	glVertexAttrib3f(shaderIF->pvaLoc("mcNormal"), n.dx, n.dy, n.dz);
	glDrawElements(GL_TRIANGLE_STRIP, 3, GL_UNSIGNED_INT, nullptr);

	glUseProgram(pgm);

}
