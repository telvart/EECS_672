// project2.c++: Starter for EECS 672 Project 2

#include "GLFWController.h"
#include "House.h"
#include "Tree.h"
#include "Plane.h"
void set3DViewingInformation(double xyz[6])
{
	ModelView::setMCRegionOfInterest(xyz);

	//NOTE: TIMS CODE

	//cryph::AffPoint eye(20, 20, 20);
	//cryph::AffPoint center(0.5*(xyz[0] + xyz[1]), 0.5*(xyz[2] + xyz[3]), 0.5*(xyz[4] + xyz[5]));
	// double xmid = 0.5 * (xyz[0] + xyz[1]);
	// double ymid = 0.5 * (xyz[2] + xyz[3]);
	// double zmid = 0.5 * (xyz[4] + xyz[5]);
	//
	// cryph::AffPoint center(xmid, ymid, zmid);
	// cryph::AffVector up = cryph::AffVector::yu;
	//
	// double r = 40;
	// double distance = 1.7 * r;
	// cryph::AffVector dir(1, 0, 0);
	//
	// cryph::AffPoint eye = center + distance*(dir);


	//NOTE: MILLERS CODE

	double maxDelta = xyz[1] - xyz[0];
	double delta = xyz[3] - xyz[2];
	if (delta > maxDelta)
		maxDelta = delta;
	delta = xyz[5] - xyz[4];
	if (delta > maxDelta)
		maxDelta = delta;
	// (ii) Determine the center of the created scene:
	double xmid = 0.5 * (xyz[0] + xyz[1]);
	double ymid = 0.5 * (xyz[2] + xyz[3]);
	double zmid = 0.5 * (xyz[4] + xyz[5]);

	// 1:
	cryph::AffPoint center(xmid, ymid, zmid);

	// 2:
	double distEyeCenter = 2 * maxDelta;
	cryph::AffVector dir(-2, -20, 10);
	dir.normalize();
	cryph::AffPoint eye = center + distEyeCenter*(dir);
	// 3:
	cryph::AffVector up = cryph::AffVector::zu;


	// Set values for eye-center-up to produce a reasonable off-axis
	// view of your scene, then:

	// Notify the ModelView of our MC->EC viewing requests:
	ModelView::setEyeCenterUp(eye, center, up);

	// double ecZpp, ecZmin, ecZmax;
	// ecZmin = -distance - r;
	// ecZmax = -distance + r;
	// ecZpp  = ecZmax;

	double ecZpp = -(distEyeCenter - 0.5*maxDelta);
	double ecZmin = ecZpp - maxDelta;
	double ecZmax = ecZpp + 0.5*maxDelta;


	// Set values for ecZpp, ecZmin, ecZmax that make sense in the context of
	// the EC system established above, then:

	ModelView::setProjection(PERSPECTIVE);
	ModelView::setProjectionPlaneZ(ecZpp);
	ModelView::setECZminZmax(ecZmin, ecZmax);
}

int main(int argc, char* argv[])
{

	cryph::AffPoint tl(-100, 100, -20);
	cryph::AffPoint tr(100, 100, -20);
	cryph::AffPoint	bl(-100, -100, -20);
	cryph::AffPoint br(100, -100, -20);
	cryph::AffVector dir(1,1,0);

	vec3 grass = {0.486, 0.988, 0};
	vec3 house = {0.862, 0.752, 0.415};
	vec3 pine =  {0.004, 0.475, 0.435};
	vec3 yellow = {1, 1, 0};
	vec3 orange = {1, 0.27, 0};
	vec3 gray = {0.86, 0.86, 0.86};


	GLFWController c("House and Trees", MVC_USE_DEPTH_BIT);
	c.reportVersions(std::cout);

	ShaderIF* sIF = new ShaderIF("shaders/basic.vsh", "shaders/phong.fsh");

	// create your scene, adding things to the Controller....

	glClearColor(1.0, 1.0, 1.0, 1.0);


  //c.addModel(new Plane(sIF, tl, tr, bl, br, color));
	//c.addModel(new Block(sIF, -10, -10, 0, 20, 20, 20));

	//NOTE: Block is defined in Plane.h/Plane.c++
	c.addModel(new Block(sIF, -200, -200, -1, 400, 400, 1, grass));
	c.addModel(new Block(sIF, -50, -50, 0, 100, 100, 50, house));
	c.addModel(new Block(sIF, -12.5, -200, 1, 25, 150, 1, gray));


//	c.addModel(new Trunk(sIF, 5, trunkBottom, trunkTop));
//	c.addModel(new TreeTop(sIF, treetopBottom, 20, 50));

	c.addModel(new Tree(sIF, 75, 150, 0, 100, 20, yellow));
	c.addModel(new Tree(sIF, -100, -75, 0, 75, 10, orange));
	c.addModel(new Tree(sIF, -140, 120, 0, 75, 30, pine));
	//c.addModel(new DirBlock(0, 0, 0, ));


	//c.addModel(new Tree(sIF));

	double xyz[6];
	c.getOverallMCBoundingBox(xyz);
	set3DViewingInformation(xyz);

	c.run();

	delete sIF;

	return 0;
}
