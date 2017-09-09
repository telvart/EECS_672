// project1.c++

#include "GLFWController.h"
#include "ModelView.h"
#include <iostream>
#include <fstream>

int main(int argc, char* argv[])
{

	std::ifstream fileIn("data.txt");
	GLFWController c(argv[0]);
	c.reportVersions(std::cout);
	ShaderIF* sIF = new ShaderIF("shaders/project1.vsh", "shaders/project1.fsh");

	float a0, a1, a2, a3, b0, b1, b2, b3, tmin, tmax;
	int numPoints;

	while(fileIn>>a0>>a1>>a2>>a3)
	{
		fileIn>>b0>>b1>>b2>>b3>>tmin>>tmax>>numPoints;

		vec2 thisLine[numPoints];
		float t = tmin;
		double deltaT = (tmax - tmin)/((double)numPoints-1);

		for(int i=0; i<numPoints; i++)
		{
			thisLine[i][0] = a0 + (a1*t) + (a2*t*t) + (a3*t*t*t);
			thisLine[i][1] = b0 + (b1*t) + (b2*t*t) + (b3*t*t*t);
			t += deltaT;
		}

	c.addModel(new ModelView(sIF, thisLine, numPoints));;

	}

	//NOTE: Currently adds the three lines and each one is a different color.
	//			Scale and Trans is working correctly, all left to do is figure out how to draw the curves

	// TODO: one or more ModelView dynamic allocations, adding
	//       each to the Controller using "c.addModel(...);"

	// initialize 2D viewing information:
	// Get the overall scene bounding box in Model Coordinates:
	double xyz[6]; // xyz limits, even though this is 2D
	c.getOverallMCBoundingBox(xyz);

	// Tell class ModelView we initially want to see the whole scene:
	ModelView::setMCRegionOfInterest(xyz);

	c.run();

	delete sIF;

	return 0;
}
