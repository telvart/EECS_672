
#include "P4Controller.h"


P4Controller::P4Controller(const std::string& windowTitle, int rcFlags)
: GLFWController(windowTitle, rcFlags), shaderIF(nullptr)
{

}

P4Controller::~P4Controller()
{

}

void P4Controller::handleDisplay()
{

  if (theWindow == nullptr)
    return;
  glfwMakeContextCurrent(theWindow);
  int width, height;
  glfwGetFramebufferSize(theWindow, &width, &height);
  glViewport(0, 0, width, height);

  glClear(glClearFlags);

  glUseProgram(shaderIF->getShaderPgmID()); //workaround

  glDepthMask(GL_TRUE);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glUniform1i(shaderIF->ppuLoc("sceneHasTransluscentObjects"), 1);

  glDisable(GL_BLEND);
  glUniform1i(shaderIF->ppuLoc("drawingOpaqueObjects"), 1);
  drawAllObjects(); //drawing opaque objects

  glDepthMask(GL_FALSE);
  glEnable(GL_BLEND);
  glUniform1i(shaderIF->ppuLoc("drawingOpaqueObjects"), 0);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  drawAllObjects(); // drawing transluscent objects

  //
  checkForErrors(std::cout, "P4Controller::handleDisplay");

}

void P4Controller::setShaderIF(ShaderIF* sIF)
{
  shaderIF = sIF;
}

void P4Controller::drawAllObjects()
{
  // draw the collection of models
  for (std::vector<ModelView*>::iterator it=models.begin() ; it<models.end() ; it++)
    (*it)->render();
  glfwSwapBuffers(theWindow);

}
