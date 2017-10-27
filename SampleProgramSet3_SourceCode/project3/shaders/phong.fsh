#version 410 core


#define PERSPECTIVE 1
#define ORTHOGANAL  2
#define OBLIQUE     3
// phong.fsh - a fragment shader that implements a Phong Lighting model.

in PVA
{
	vec3 ecPosition;
	vec3 ecUnitNormal;
} pvaIn;

out vec4 fragmentColor;

const int MAX_LIGHTS = 3;

// Phong material properties (just kd for now; you will add the rest later):
// "kd" - diffuse reflectivity; basic object color

uniform vec3 kd = vec3(0.8, 0.0, 0.0); // default: darkish red
uniform vec3 ka = vec3(0.8, 0.0, 0.0);
uniform vec3 ks = vec3(0.8, 0.0, 0.0);
uniform int numLights;
uniform int projectionType;
uniform vec4 p_ecLightPositions[MAX_LIGHTS];
uniform vec3 lightStrengths[MAX_LIGHTS];
uniform vec3 globalAmbient = vec3(0.6, 0.6, 0.6);
uniform float spec_m;
uniform float alpha;



vec4 evaluateLightingModel(in vec3 Q, in vec3 normal)
{
	// THIS IS JUST A PLACEHOLDER FOR A LIGHTING MODEL.
	// It only currently implements simple Lambert shading.

	// NOTES:
	// 1. We assume for now a single directional light source defined in EC (liHat).
	// 2. We assume it will be "full strength" (see liStrength).
	//
	// In project 3, both #1 and #2 will be generalized by introducing uniform
	// arrays ("vec4 p_ecLightSourcePos" and "vec3 ecLightSourceStrength") and+
	// using them INSTEAD OF the liHat and liStrength you see here.
	//
	// 3. The use of "abs" here is a temporary hack. As we study the Phong
	//    lighting model more carefully, you will REMOVE "abs" since it will
	//    no longer be appropriate.

	vec3 ambientTotal;
	vec3 diffuseTotal;
	vec3 specularTotal;
	//
	vec3 v_hat;
	//
	if(projectionType == PERSPECTIVE)
	{
		//vHat = -(normalize(Q));
	}
	// else if(projectionType == ORTHOGONAL)
	// {
	//
	// }
	// else if(projectionType == OBLIQUE)
	// {
	//
	// }


	vec3 liHat = vec3(0, 0, 1);
	vec3 liStrength = vec3(1.0, 1.0, 1.0);
	vec3 norm = pvaIn.ecUnitNormal;

	vec3 globalAmbient = vec3(0.1, 0.1, 0.1);

	 if(dot(normalize(liHat), norm) < 0)
	 {
	 	norm = norm*-1;
	 }

	//float factor = abs(dot(liHat, norm));
	float factor = dot(liHat, norm);

	return vec4(factor * kd * liStrength, 1.0);
}

void main ()
{

	fragmentColor = evaluateLightingModel(pvaIn.ecPosition.xyz, pvaIn.ecUnitNormal);
}
