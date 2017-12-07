
#include "TriPrism.h"

TriPrism::TriPrism(ShaderIF* sIF, PhongMaterial& mat, cryph::AffPoint loc, float height, float width, float length, bool onXaxis)
 : SceneElement(sIF, mat)
{
  this->loc = loc;
  this->height = height;
  this->width = width;
  this->length = length;
  alongX = onXaxis;
  definePrism();
}

TriPrism::TriPrism(ShaderIF* sIF, PhongMaterial& mat, cryph::AffPoint loc,
   float height, float width, float length, bool onXaxis, const char* texImgName)
 : SceneElement(sIF, mat)
{
  this->loc = loc;
  this->height = height;
  this->width = width;
  this->length = length;
  alongX = onXaxis;
  definePrism();
}

TriPrism::~TriPrism()
{
}

// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
void TriPrism::getMCBoundingBox(double* xyzLimits) const
{
  xyzLimits[0] = xyz[0]; xyzLimits[1] = xyz[1];
	xyzLimits[2] = xyz[2]; xyzLimits[3] = xyz[3];
	xyzLimits[4] = xyz[4]; xyzLimits[5] = xyz[5];
}

void TriPrism::definePrism()
{
  xyz[0] = loc.x - (width/2); xyz[1] = loc.x + (width/2);
  xyz[2] = loc.y - (length/2); xyz[3] = loc.y + (length/2);
  xyz[4] = loc.z; xyz[5] = loc.z + height;

  bl = cryph::AffPoint(loc.x - (width/2), loc.y -(length/2), loc.z);
  br = cryph::AffPoint(loc.x + (width/2), loc.y -(length/2), loc.z);
  tl = cryph::AffPoint(loc.x - (width/2), loc.y +(length/2), loc.z);
  tr = cryph::AffPoint(loc.x + (width/2), loc.y +(length/2), loc.z);

  vec3* coords = new vec3[6];

  bl.aCoords(coords, 0);
  br.aCoords(coords, 1);
  tl.aCoords(coords, 2);
  tr.aCoords(coords, 3);

  if(!alongX)
  {
    tt1 = cryph::AffPoint((bl.x+br.x)/2, bl.y, bl.z+height);
    tt2 = cryph::AffPoint((tl.x+tr.x)/2, tl.y, tt1.z);
  }
  else
  {
    tt1 = cryph::AffPoint(bl.x, (bl.y+tl.y)/2, bl.z+height);
    tt2 = cryph::AffPoint(br.x, (br.y+tr.y)/2, tt1.z);
  }
  tt1.aCoords(coords, 4);
  tt2.aCoords(coords, 5);

  glGenVertexArrays(1, vao);
  glGenBuffers(1, vbo);

  glBindVertexArray(vao[0]);
  glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

  int bSize = 6 * sizeof(vec3);

  glBufferData(GL_ARRAY_BUFFER, bSize, coords, GL_STATIC_DRAW);
  glVertexAttribPointer(shaderIF->pvaLoc("mcPosition"), 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(shaderIF->pvaLoc("mcPosition"));

  glGenBuffers(4, ebo);
  if(!alongX)
  {
    for(int i=0; i<2; i++)
    {
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[i]);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3*sizeof(GLuint), yTris[i], GL_STATIC_DRAW);
    }
    for(int i=2; i<4; i++)
    {
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[i]);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4*sizeof(GLuint), yFaces[i-2], GL_STATIC_DRAW);
    }
  }
  else
  {
    for(int i=0; i<2; i++)
    {
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[i]);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3*sizeof(GLuint), xTris[i], GL_STATIC_DRAW);
    }
    for(int i=2; i<4; i++)
    {
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[i]);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4*sizeof(GLuint), xFaces[i-2], GL_STATIC_DRAW);
    }
  }

  glDisableVertexAttribArray(shaderIF->pvaLoc("mcNormal"));

  delete[] coords;

}

GLuint TriPrism::yTris[2][3] = {
  {0, 1, 4},
  {2, 3, 5}
};

GLuint TriPrism::xTris[2][3] = {
  {0, 2, 4},
  {1, 3, 5}
};

GLuint TriPrism::xFaces[2][4] = {
  {0, 4, 5, 1},
  {2, 4, 5, 3}
};

GLuint TriPrism::yFaces[2][4] = {
  {0, 2, 5, 4},
  {1, 4, 5, 3}
};

void TriPrism::renderAlongX()
{
  cryph::AffVector norm, temp1, temp2;
  temp1 = tl - bl;
  temp2 = br - bl;
  norm = temp1.cross(temp2);
  norm.normalize();

  glBindVertexArray(vao[0]);
  glVertexAttrib3f(shaderIF->pvaLoc("mcNormal"), norm.dx, norm.dy, norm.dz);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  temp1 = tt1 - bl;
  temp2 = tl - bl;
  norm = temp1.cross(temp2);
  norm.normalize();

  glVertexAttrib3f(shaderIF->pvaLoc("mcNormal"), norm.dx, norm.dy, norm.dz);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[0]);
  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[1]);
  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

  temp1 = tt1 - bl;
  temp2 = br - bl;
  norm = temp1.cross(temp2);
  norm.normalize();

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[2]);
  glVertexAttrib3f(shaderIF->pvaLoc("mcNormal"), norm.dx, norm.dy, norm.dz);
  glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, nullptr);

  temp1 = tt1 - tl;
  temp2 = tr - tl;
  norm = temp1.cross(temp2);
  norm.normalize();

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[3]);
  glVertexAttrib3f(shaderIF->pvaLoc("mcNormal"), norm.dx, norm.dy, norm.dz);
  glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, nullptr);

}

void TriPrism::renderAlongy()
{
  cryph::AffVector norm, temp1, temp2;
  temp1 = tl - bl;
  temp2 = br - bl;
  norm = temp1.cross(temp2);
  norm.normalize();

  glBindVertexArray(vao[0]);
  glVertexAttrib3f(shaderIF->pvaLoc("mcNormal"), norm.dx, norm.dy, norm.dz);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  temp1 = tt1 - bl;
  temp2 = br - bl;
  norm = temp1.cross(temp2);
  norm.normalize();

  glVertexAttrib3f(shaderIF->pvaLoc("mcNormal"), norm.dx, norm.dy, norm.dz);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[0]);
  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[1]);
  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

  temp1 = tt1 - bl;
  temp2 = tl - bl;
  norm = temp1.cross(temp2);
  norm.normalize();

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[2]);
  glVertexAttrib3f(shaderIF->pvaLoc("mcNormal"), norm.dx, norm.dy, norm.dz);
  glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, nullptr);

  temp1 = tt1 - br;
  temp2 = tr - br;
  norm = temp1.cross(temp2);
  norm.normalize();

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[3]);
  glVertexAttrib3f(shaderIF->pvaLoc("mcNormal"), norm.dx, norm.dy, norm.dz);
  glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, nullptr);


}

void TriPrism::render()
{
  GLint program;
	glGetIntegerv(GL_CURRENT_PROGRAM, &program);
	glUseProgram(shaderIF->getShaderPgmID());

	establishView();
	establishMaterial();
	establishLightingEnvironment();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  if(alongX)
    renderAlongX();
  else
    renderAlongy();

	glUseProgram(program);
}
