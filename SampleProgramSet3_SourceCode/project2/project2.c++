// project2.c++: Starter for EECS 672 Project 2

#include "GLFWController.h"
#include "House.h"
#include "Tree.h"
void set3DViewingInformation(double xyz[6])
{
	ModelView::setMCRegionOfInterest(xyz);

	//cryph::AffPoint eye(20, 20, 20);
	cryph::AffPoint center(0.5*(xyz[0] + xyz[1]), 0.5*(xyz[2] + xyz[3]), 0.5*(xyz[4] + xyz[5]));
	cryph::AffVector up = cryph::AffVector::zu;

	double r = 20;
	double distance = 1.7 * r;
	cryph::AffVector dir(1, 0, 0);

	cryph::AffPoint eye = center + distance*(dir);
	// Set values for eye-center-up to produce a reasonable off-axis
	// view of your scene, then:

	// Notify the ModelView of our MC->EC viewing requests:
	ModelView::setEyeCenterUp(eye, center, up);

	double ecZpp, ecZmin, ecZmax;
	ecZmin = -distance - r;
	ecZmax = -distance + r;
	ecZpp  = ecZmax;
	// Set values for ecZpp, ecZmin, ecZmax that make sense in the context of
	// the EC system established above, then:

	ModelView::setProjection(PERSPECTIVE);
	ModelView::setProjectionPlaneZ(ecZpp);
	ModelView::setECZminZmax(ecZmin, ecZmax);
}

int main(int argc, char* argv[])
{
	GLFWController c("House and Trees", MVC_USE_DEPTH_BIT);
	c.reportVersions(std::cout);

	ShaderIF* sIF = new ShaderIF("shaders/basic.vsh", "shaders/phong.fsh");

	// create your scene, adding things to the Controller....

	glClearColor(1.0, 1.0, 1.0, 1.0);

	double xyz[6];
	c.getOverallMCBoundingBox(xyz);
	set3DViewingInformation(xyz);

	c.run();

	delete sIF;

	return 0;
}
