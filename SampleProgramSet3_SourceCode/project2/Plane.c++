#include "Plane.h"

#include <iostream>
#include <math.h>

typedef float vec3[3];
Plane::Plane(ShaderIF* sIF, cryph::AffPoint tl, cryph::AffPoint tr,
	 cryph::AffPoint bl, cryph::AffPoint br, vec3 color)
{
	shaderIF = sIF;
	kd[0] = color[0]; kd[1] = color[1]; kd[2] = color[2];
	tLeft = tl;
	tRight = tr;
	bLeft = bl;
	bRight = br;
	definePlane();
}

Plane::~Plane()
{
	glDeleteBuffers(2, vbo);
	glDeleteVertexArrays(1, vao);
}

void Plane::definePlane()
{
	cryph::AffPoint temp[4];
	temp[0] = tLeft; temp[1] = tRight;
	temp[2] = bRight; temp[3] = bLeft;

	xyz[0] = xyz[1] = tLeft.x;
	xyz[2] = xyz[3] = tLeft.y;
	xyz[4] = xyz[5] = tLeft.z;
	for(int i=1; i<4; i++)
	{
		 if (temp[i].x < xyz[0])
		 	xyz[0] = temp[i].x;
		 else if (temp[i].x > xyz[1])
		 	xyz[1] = temp[i].x;
		 if (temp[i].y < xyz[2])
		 	xyz[2] = temp[i].y;
		 else if (temp[i].y > xyz[3])
		 	xyz[3] = temp[i].y;
		 if (temp[i].z < xyz[2])
		 	xyz[2] = temp[i].z;
		 else if (temp[i].z > xyz[3])
		 	xyz[3] = temp[i].z;
	}
	// std::cout<<"Plane Bounding box: ("<<xyz[0]<<", "<<xyz[1]<<") ("
	// 																	<<xyz[2]<<", "<<xyz[3]<<") ("
	// 																	<<xyz[4]<<", "<<xyz[5]<<")\n";

	cryph::AffVector U, V;
	V = tLeft - bLeft;
	U = bRight - bLeft;
	//U.normalize();
	//V.normalize();
	normal = U.cross(V);
	normal.normalize();
	// std::cout<<"U = ("<<U.dx<<", "<<U.dy<<", "<<U.dz<<")\n";
	// std::cout<<"V = ("<<V.dx<<", "<<V.dy<<", "<<V.dz<<")\n";
	// std::cout<<"N = ("<<normal.dx<<", "<<normal.dy<<", "<<normal.dz<<")\n";

	vec3* verticies = new vec3[4];
	vec3* normals = new vec3[4];
	for(int i=0; i<4; i++)
	{
		temp[i].aCoords(verticies, i);
		normals[i][0] = normal.dx;
		normals[i][1] = normal.dy;
		normals[i][2] = normal.dz;
	}
	// verticies[4][0] = tLeft.x;
	// verticies[4][1] = tLeft.y;
	// verticies[4][2] = tLeft.z;
	//
	// normals[4][0] = normal.dx;
	// normals[4][1] = normal.dy;
	// normals[4][2] = normal.dz;

	// for(int i=0; i<5; i++)
	// {
	// 	std::cout<<"Point: ("<<verticies[i][0]<<", "<<verticies[i][1]<<", "<<verticies[i][2]<<")\n"
	// 					 <<"Normal:"<<normals[i][0]<<", "<<normals[i][1]<<", "<<normals[i][2]<<")\n\n";
	// }

	glGenVertexArrays(1, vao);
	glGenBuffers(2, vbo);

	int bufferSize = 5 * sizeof(vec3);

	glBindVertexArray(vao[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

	glBufferData(GL_ARRAY_BUFFER, bufferSize, verticies, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(shaderIF->pvaLoc("mcPosition"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(shaderIF->pvaLoc("mcPosition"));

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, bufferSize, normals, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(shaderIF->pvaLoc("mcNormal"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(shaderIF->pvaLoc("mcNormal"));

	delete[] verticies;
	delete[] normals;

}


// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
void Plane::getMCBoundingBox(double* xyzLimits) const
{
	xyzLimits[0] = xyz[0]; xyzLimits[1] = xyz[1];
	xyzLimits[2] = xyz[2]; xyzLimits[3] = xyz[3];
  xyzLimits[4] = xyz[4]; xyzLimits[5] = xyz[5];
}

void Plane::render()
{
	// 1. Save current and establish new current shader program
	// ...
	GLint program;
	glGetIntegerv(GL_CURRENT_PROGRAM, &program);

	glUseProgram(shaderIF->getShaderPgmID());

	// 2. Establish "mc_ec" and "ec_lds" matrices
	// ...
	cryph::Matrix4x4 mc_ec, ec_lds;
	getMatrices(mc_ec, ec_lds);
	float mat[16];
	glUniformMatrix4fv(shaderIF->ppuLoc("mc_ec"), 	1, false, mc_ec.	extractColMajor(mat));
	glUniformMatrix4fv(shaderIF->ppuLoc("ec_lds"), 	1, false, ec_lds.	extractColMajor(mat));

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	// 3. Set GLSL's "kd" variable using this object's "kd" instance variable
	// ...
	glUniform3fv(shaderIF->ppuLoc("kd"), 1, kd);

	// 4. Establish any other attributes and make one or more calls to
	//    glDrawArrays and/or glDrawElements
	// ...
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	// 5. Reestablish previous shader program
	glUseProgram(program);
	// ...
}

// index lists for the three faces that can't be drawn with glDrawArrays
GLuint Block::indexList[3][4] = {
	{ 6, 7, 0, 1 }, // xmin face
	{ 6, 0, 4, 2 }, // ymin face
	{ 1, 7, 3, 5 }  // ymax face
};

Block::Block(ShaderIF* sIF, float cx, float cy, float cz, float lx, float ly, float lz, vec3 color) :
	shaderIF(sIF)
{
	xmin = cx; xmax = cx + lx;
	ymin = cy; ymax = cy + ly;
	zmin = cz; zmax = cz + lz;

	kd[0] = color[0]; kd[1] = color[1]; kd[2] = color[2];
	ka[0] = color[0]; ka[1] = color[1]; ka[2] = color[2];

	defineBlock();
}

Block::~Block()
{
	glDeleteBuffers(3,ebo);
	glDeleteBuffers(1, vbo);
	glDeleteVertexArrays(1, vao);
}

void Block::defineBlock()
{
	vec3 vtx[] = { // The 8 unique vertices (Note the order)
		{xmin ,ymin, zmax}, {xmin, ymax, zmax},
		{xmax, ymin, zmax}, {xmax, ymax, zmax},
		{xmax, ymin, zmin}, {xmax, ymax, zmin},
		{xmin, ymin, zmin}, {xmin, ymax, zmin}
	};
	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);

	glGenBuffers(1, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, 8*sizeof(vec3), vtx, GL_STATIC_DRAW);
	glVertexAttribPointer(shaderIF->pvaLoc("mcPosition"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(shaderIF->pvaLoc("mcPosition"));

	glGenBuffers(3, ebo);
	for (int i=0 ; i<3 ; i++)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[i]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4*sizeof(GLuint), indexList[i], GL_STATIC_DRAW);
	}

	glDisableVertexAttribArray(shaderIF->pvaLoc("mcNormal"));
}

// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
void Block::getMCBoundingBox(double* xyzLimits) const
{
	xyzLimits[0] = xmin;
	xyzLimits[1] = xmax;
	xyzLimits[2] = ymin;
	xyzLimits[3] = ymax;
	xyzLimits[4] = zmin;
	xyzLimits[5] = zmax;
}

bool Block::handleCommand(unsigned char anASCIIChar, double ldsX, double ldsY)
{
	// Block does not look for events; just hand off to inherited handleCommand.
	return this->ModelView::handleCommand(anASCIIChar, ldsX, ldsY);
}

void Block::renderBlock()
{
	glBindVertexArray(vao[0]);
	glUniform3fv(shaderIF->ppuLoc("kd"), 1, kd);
  //glUniform3fv(shaderIF->ppuLoc("ka"), 1, ka);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// The three faces that can be drawn with glDrawArrays
	glVertexAttrib3f(shaderIF->pvaLoc("mcNormal"), 0.0, 0.0, 1.0);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glVertexAttrib3f(shaderIF->pvaLoc("mcNormal"), 1.0, 0.0, 0.0);
	glDrawArrays(GL_TRIANGLE_STRIP, 2, 4);
	glVertexAttrib3f(shaderIF->pvaLoc("mcNormal"), 0.0, 0.0, -1.0);
	glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);

	// The three faces that are drawn with glDrawElements
	glVertexAttrib3f(shaderIF->pvaLoc("mcNormal"), -1.0, 0.0, 0.0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[0]);
	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, nullptr);
	glVertexAttrib3f(shaderIF->pvaLoc("mcNormal"), 0.0, -1.0, 0.0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[1]);
	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, nullptr);
	glVertexAttrib3f(shaderIF->pvaLoc("mcNormal"), 0.0, 1.0, 0.0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[2]);
	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, nullptr);
}

void Block::render()
{
	GLint pgm;
	glGetIntegerv(GL_CURRENT_PROGRAM, &pgm);
	glUseProgram(shaderIF->getShaderPgmID());

	cryph::Matrix4x4 mc_ec, ec_lds;
	getMatrices(mc_ec, ec_lds);
	float mat[16];
	glUniformMatrix4fv(shaderIF->ppuLoc("mc_ec"), 1, false, mc_ec.extractColMajor(mat));
	glUniformMatrix4fv(shaderIF->ppuLoc("ec_lds"), 1, false, ec_lds.extractColMajor(mat));

	renderBlock();

	glUseProgram(pgm);
}
