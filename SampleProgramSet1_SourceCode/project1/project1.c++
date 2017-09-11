// project1.c++

#include "GLFWController.h"
#include "ModelView.h"
#include <iostream>
#include <fstream>

int main(int argc, char* argv[])
{

	if (argc < 2)
	{
		std::cout<<"Usage: ./project1 <filename>\n";
		return 0;
	}

	std::ifstream fileIn(argv[1]);

	if (!fileIn.good())
	{
		std::cout << "Could not open file \"" << argv[1] << "\" for reading\n";
		return 0;
	}

	GLFWController c(argv[0]);
	c.reportVersions(std::cout);
	ShaderIF* sIF = new ShaderIF("shaders/project1.vsh", "shaders/project1.fsh");
	float a0, a1, a2, a3, b0, b1, b2, b3, tmin, tmax;
	int numPoints;

	while (fileIn >> a0 >> a1 >> a2 >> a3)
	{
		fileIn >> b0 >> b1 >> b2 >> b3 >> tmin >> tmax >> numPoints;

		vec2 thisCurve[numPoints];
		float t = tmin;
		double dt = (tmax - tmin) / ( (double)numPoints - 1);

		for(int i = 0; i < numPoints; i++)
		{
			thisCurve[i][0] = a0 + (a1*t) + (a2*t*t) + (a3*t*t*t);
			thisCurve[i][1] = b0 + (b1*t) + (b2*t*t) + (b3*t*t*t);
			t += dt;
		}

		c.addModel(new ModelView(sIF, thisCurve, numPoints));
	}
	
	fileIn.close();

	// initialize 2D viewing information:
	// Get the overall scene bounding box in Model Coordinates:
	double xyz[6]; // xyz limits, even though this is 2D
	c.getOverallMCBoundingBox(xyz);

	// Tell class ModelView we initially want to see the whole scene:
	ModelView::setMCRegionOfInterest(xyz);
	//glClearColor(1,1,1,1);
	c.run();

	delete sIF;

	return 0;
}
