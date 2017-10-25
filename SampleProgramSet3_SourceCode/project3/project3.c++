// project2.c++: Starter for EECS 672 Project 2

#include "GLFWController.h"
#include "House.h"
#include "Tree.h"
#include "PicnicTable.h"

void set3DViewingInformation(double xyz[6])
{
	ModelView::setMCRegionOfInterest(xyz);

	double maxDelta = xyz[1] - xyz[0];
	double delta = xyz[3] - xyz[2];
	if (delta > maxDelta)
		maxDelta = delta;
	delta = xyz[5] - xyz[4];
	if (delta > maxDelta)
		maxDelta = delta;

	double xmid = 0.5 * (xyz[0] + xyz[1]);
	double ymid = 0.5 * (xyz[2] + xyz[3]);
	double zmid = 0.5 * (xyz[4] + xyz[5]);

	cryph::AffPoint center(xmid, ymid, zmid);

	double distEyeCenter = 2 * maxDelta;
	cryph::AffVector dir(0, -1, 0);
	dir.normalize();
	cryph::AffPoint eye = center + distEyeCenter*(dir);
	// 3:
	cryph::AffVector up = cryph::AffVector::zu;

	// Set values for eye-center-up to produce a reasonable off-axis
	// view of your scene, then:

	// Notify the ModelView of our MC->EC viewing requests:
	ModelView::setEyeCenterUp(eye, center, up);

	double ecZpp = -(distEyeCenter - 0.3*maxDelta);
	double ecZmin = ecZpp - maxDelta;
	double ecZmax = ecZpp + 0.3*maxDelta;

	// Set values for ecZpp, ecZmin, ecZmax that make sense in the context of
	// the EC system established above, then:

	ModelView::setProjection(PERSPECTIVE);
	ModelView::setProjectionPlaneZ(ecZpp);
	ModelView::setECZminZmax(ecZmin, ecZmax);
}

int main(int argc, char* argv[])
{

	cryph::AffPoint origin(0, 0, 0);
	cryph::AffPoint dogHouse(70, 10, 0);
	cryph::AffPoint table(100, -110, 0);
	cryph::AffPoint table2(-100, -100, 0);

	vec3 grass = {0.486, 0.988, 0};
	vec3 pine =  {0.004, 0.475, 0.435};
	vec3 yellow = {1, 1, 0};
	vec3 orange = {1, 0.27, 0};
	vec3 gray = {0.9607, 0.9607, 0.9607};

	GLFWController c("Project 3", MVC_USE_DEPTH_BIT);
	c.reportVersions(std::cout);

	ShaderIF* sIF = new ShaderIF("shaders/basic.vsh", "shaders/phong.fsh");

	std::cout<<"Time for Project 3, YAY\n";

	 c.addModel(new Block(sIF, -200, -200, -1, 400, 400, 1, grass)); //green underlay
	 c.addModel(new Block(sIF, -12.5, -200, 1, 25, 150, 1, gray)); //sidewalk
	//
	 c.addModel(new Tree(sIF, 75, 150, 0, 100, 20, yellow));
	 c.addModel(new Tree(sIF, -75, 150, 0, 75, 20, orange));
	 c.addModel(new Tree(sIF, -175, -85, 0, 75, 15, orange));
	 c.addModel(new Tree(sIF, -140, 120, 0, 75, 30, pine));
	 c.addModel(new Tree(sIF, 150, 100, 0, 150, 25, pine));
	 c.addModel(new Tree(sIF, -150, 20, 0, 75, 20, yellow));

	 c.addModel(new House(sIF, origin, 75, 75, 50, 40, false));
	 c.addModel(new House(sIF, dogHouse, 30, 30, 30, 10, true));
	//
	 c.addModel(new PicnicTable(sIF, table, 40, 80, 40));
	 c.addModel(new PicnicTable(sIF, table2, 40, 130, 40));

	//c.addModel(new Trunk(sIF, origin, 20, 50));
	double xyz[6];
	c.getOverallMCBoundingBox(xyz);
	set3DViewingInformation(xyz);

	glClearColor(1.0, 1.0, 1.0, 1.0);
	c.run();

	delete sIF;

	return 0;
}
