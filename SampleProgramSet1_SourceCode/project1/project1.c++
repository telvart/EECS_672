// project1.c++

#include "GLFWController.h"
#include "ModelView.h"

int main(int argc, char* argv[])
{

	// vec2 vertexPositions[][3] =
	// {
	// 	// triangle 1:
	// 	{ { -6.0, 137.0 }, { 2.0, 137.0 }, { -2.0, 145.0 } },
	// 	// triangle 2:
	// 	{ { -6.0, 135.0 }, { 2.0, 135.0 }, { -2.0, 127.0 } }
	// };
	vec2 lineCoords[2] =
	{
		{-0.5, -0.5}, {0.5, 0.5}
	};

	vec2 lineCoords2[2] =
	{
		{0,1}, {-1,0}
	};

	GLFWController c(argv[0]);
	c.reportVersions(std::cout);

	ShaderIF* sIF = new ShaderIF("shaders/project1.vsh", "shaders/project1.fsh");
	c.addModel(new ModelView(sIF, lineCoords));
	c.addModel(new ModelView(sIF, lineCoords2));



	// TODO: one or more ModelView dynamic allocations, adding
	//       each to the Controller using "c.addModel(...);"
	//
	//       NOTE: You will likely want to modify the ModelView
	//             constructor to take additional parameters.

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
