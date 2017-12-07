
#include "Block.h"

Block::Block(ShaderIF* sIF, float cx, float cy, float cz, float lx, float ly, float lz, PhongMaterial& mat) :
	SceneElement(sIF, mat)
{
	xmin = cx; xmax = cx + lx;
	ymin = cy; ymax = cy + ly;
	zmin = cz; zmax = cz + lz;
	hasTexture = false;
	defineBlock();
}

Block::Block(ShaderIF* sIF, float cx, float cy, float cz, float lx, float ly, float lz,
	 PhongMaterial& mat, const char* texImgName) :
	SceneElement(sIF, mat)
{
	xmin = cx; xmax = cx + lx;
	ymin = cy; ymax = cy + ly;
	zmin = cz; zmax = cz + lz;
	hasTexture = true;
	defineBlock();
	//setTextureWrapMode(GL_REPEAT, GL_REPEAT);
	setTextureImage(texImgName, 0);
}

Block::~Block()
{
	glDeleteBuffers(3,ebo);
	glDeleteBuffers(1, vbo);
	glDeleteVertexArrays(1, vao);
}

// index lists for the three faces that can't be drawn with glDrawArrays
GLuint Block::indexList[3][4] = {
	{ 6, 7, 0, 1 }, // xmin face
	{ 6, 0, 4, 2 }, // ymin face
	{ 1, 7, 3, 5 }  // ymax face
};

void Block::defineBlock()
{
	vec3 vtx[] = { // The 8 unique vertices (Note the order)
		{xmin ,ymin, zmax}, {xmin, ymax, zmax},
		{xmax, ymin, zmax}, {xmax, ymax, zmax},
		{xmax, ymin, zmin}, {xmax, ymax, zmin},
		{xmin, ymin, zmin}, {xmin, ymax, zmin}
	};

	vec2 firstTex[] = {
		{0, 0},{0, 1},
		{1, 0},{1, 1},
		{0, 0},{0, 1},
		{1, 0},{1, 1} };



	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);

	glGenBuffers(2, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, 8*sizeof(vec3), vtx, GL_STATIC_DRAW);
	glVertexAttribPointer(shaderIF->pvaLoc("mcPosition"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(shaderIF->pvaLoc("mcPosition"));

	if(hasTexture)
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glBufferData(GL_ARRAY_BUFFER, 8*sizeof(vec3), firstTex, GL_STATIC_DRAW);
		glVertexAttribPointer(shaderIF->pvaLoc("texCoords"), 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(shaderIF->pvaLoc("texCoords"));
	}


	glGenBuffers(3, ebo);
	for (int i=0 ; i<3 ; i++)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[i]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4*sizeof(GLuint), indexList[i], GL_STATIC_DRAW);
	}

	glDisableVertexAttribArray(shaderIF->pvaLoc("mcNormal"));
}

void Block::getMCBoundingBox(double* xyzLimits) const
{
	xyzLimits[0] = xmin; xyzLimits[1] = xmax;
	xyzLimits[2] = ymin; xyzLimits[3] = ymax;
	xyzLimits[4] = zmin;xyzLimits[5] = zmax;
}

void Block::sendTextureData(vec2 tex[])
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 8*sizeof(vec2), tex);
}

bool Block::handleCommand(unsigned char anASCIIChar, double ldsX, double ldsY)
{
	// Block does not look for events; just hand off to inherited handleCommand.
	return this->ModelView::handleCommand(anASCIIChar, ldsX, ldsY);
}

void Block::renderBlock()
{
	glBindVertexArray(vao[0]);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// The three faces that can be drawn with glDrawArrays
	glVertexAttrib3f(shaderIF->pvaLoc("mcNormal"), 0.0, 0.0, 1.0);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
 	glVertexAttrib3f(shaderIF->pvaLoc("mcNormal"), 1.0, 0.0, 0.0);
 	glDrawArrays(GL_TRIANGLE_STRIP, 2, 4);
 	glVertexAttrib3f(shaderIF->pvaLoc("mcNormal"), 0.0, 0.0, -1.0);
 	glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
//
// 	// The three faces that are drawn with glDrawElements
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

void Block::renderBlockWithTexture()
{
	glBindVertexArray(vao[0]);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	establishTexture();
	useTexture();

	glVertexAttrib3f(shaderIF->pvaLoc("mcNormal"), 0.0, 0.0, 1.0);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	vec2 tex2[] = {{0,0}, {0,0}, {0,1}, {1,1}, {0, 0}, {1,0}, {0,0}, {0,0}};
	sendTextureData(tex2);

	glVertexAttrib3f(shaderIF->pvaLoc("mcNormal"), 1.0, 0.0, 0.0);
	glDrawArrays(GL_TRIANGLE_STRIP, 2, 4);

	vec2 tex3[] = { {0,0}, {0,0}, {0,0}, {0,0}, {1,0}, {1,1}, {0,0}, {0,1}};
	sendTextureData(tex3);

	glVertexAttrib3f(shaderIF->pvaLoc("mcNormal"), 0.0, 0.0, -1.0);
	glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);

	vec2 tex4[] = { {1,1}, {0,1}, {0,0}, {0,0}, {0,0}, {0,0}, {1,0}, {0,0}};
	sendTextureData(tex4);

	glVertexAttrib3f(shaderIF->pvaLoc("mcNormal"), -1.0, 0.0, 0.0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[0]);
 	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, nullptr);

  vec2 tex5[] = { {0,1}, {1,1}, {1,1}, {0,0}, {1,0}, {0,0}, {0,0}, {1,0}};
  sendTextureData(tex5);

 	glVertexAttrib3f(shaderIF->pvaLoc("mcNormal"), 0.0, -1.0, 0.0);
 	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[1]);
	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, nullptr);

  vec2 tex6[] = { {0,1}, {1,1}, {1,1}, {1,0}, {1,0}, {0,0}, {0,0}, {1,0}};
	sendTextureData(tex6);

 	glVertexAttrib3f(shaderIF->pvaLoc("mcNormal"), 0.0, 1.0, 0.0);
 	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[2]);
 	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, nullptr);

	disableTexture();
}

void Block::render()
{
	GLint pgm;
	glGetIntegerv(GL_CURRENT_PROGRAM, &pgm);
	glUseProgram(shaderIF->getShaderPgmID());

	establishView();
	establishMaterial();
	establishLightingEnvironment();

	if(hasTexture)
	{
		setTextureWrapMode(GL_REPEAT, GL_REPEAT);
		renderBlockWithTexture();
	}
	else
		renderBlock();

	glUseProgram(pgm);
}
