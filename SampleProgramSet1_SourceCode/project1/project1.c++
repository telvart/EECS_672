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

	vec2 lineCoords[2] =
	{
		{-0.75, -0.75}, {0.75, -0.75}
	};

	vec2 lineCoords2[2] =
	{
		{0, 0.5}, {0, -0.5}
	};

	vec2 lineCoords3[2] =
	{
		{-1, -1}, {1, 1}
	};

	float a0, a1, a2, a3, b0, b1, b2, b3, tmin, tmax;
	int numPoints;

	while(fileIn>>a0>>a1>>a2>>a3)
	{
		fileIn>>b0>>b1>>b2>>b3>>tmin>>tmax>>numPoints;
		std::cout<<"\n\na0: "<<a0<<" a1: "<<a1<<" a2: "<<a2<<" a3: "<<a3<<"\n";
		std::cout<<"b0: "<<b0<<" b1: "<<b1<<" b2: "<<b2<<" b3: "<<b3<<"\n";
		std::cout<<"tmin: "<<tmin<<" tmax: "<<tmax<<" numPoints "<<numPoints<<"\n";

		vec2 thisLine[numPoints];
		float t = tmin;
		double deltaT = (tmax - tmin)/((double)numPoints-1);
		//std::cout<<"DELTA T: "<<deltaT<<"\n";

		std::cout<<"\nPoints added:\n\n";
		for(int i=0; i<numPoints; i++)
		{
			thisLine[i][0] = a0 + (a1*t) + (a2*t*t) + (a3*t*t*t);
			thisLine[i][1] = b0 + (b1*t) + (b2*t*t) + (b3*t*t*t);
			t += deltaT;
			//std::cout<<"("<<thisLine[i][0]<<", "<<thisLine[i][1]<<") ";
		}
	c.addModel(new ModelView(sIF, thisLine, numPoints));;
	}

  //c.addModel(new ModelView(sIF, lineCoords, 2));
	//c.addModel(new ModelView(sIF, lineCoords2, 2));
	//c.addModel(new ModelView(sIF, lineCoords3, 2));
	//NOTE: Currently adds the three lines and each one is a different color.
	//			Scale and Trans is working correctly, all left to do is figure out how to draw the curves

	// TODO: one or more ModelView dynamic allocations, adding
	//       each to the Controller using "c.addModel(...);"

	// initialize 2D viewing information:
	// Get the overall scene bounding box in Model Coordinates:
	double xyz[6]; // xyz limits, even though this is 2D
	c.getOverallMCBoundingBox(xyz);
	std::cout<<xyz[0] <<" "<<xyz[1]<<"\n"
					 <<xyz[2] <<" "<<xyz[3]<<"\n"
					 <<xyz[4] <<" "<<xyz[5]<<"\n";


	// Tell class ModelView we initially want to see the whole scene:
	ModelView::setMCRegionOfInterest(xyz);

	c.run();

	delete sIF;

	return 0;
}
