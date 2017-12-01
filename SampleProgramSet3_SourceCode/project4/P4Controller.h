#ifndef P4CONTROLLER_H
#define P4CONTROLLER_H

#include "GLFWController.h"
#include "SceneElement.h"

class P4Controller : public GLFWController
{

public:

  P4Controller(const std::string& windowTitle, int rcFlags = 0);
  ~P4Controller();
  virtual void handleDisplay();
  void setShaderIF(ShaderIF* sIF);

private:

  void drawAllObjects();

  ShaderIF* shaderIF;




};

#endif
