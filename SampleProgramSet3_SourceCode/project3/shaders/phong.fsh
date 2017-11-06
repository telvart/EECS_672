#version 410 core

#define PERSPECTIVE 1
#define ORTHOGANAL  2
// phong.fsh - a fragment shader that implements a Phong Lighting model.

in PVA
{
	vec3 ecPosition;
	vec3 ecUnitNormal;
	vec3 obliqueVhat;
} pvaIn;

out vec4 fragmentColor;

const int MAX_LIGHTS = 3;

// Phong lighting model values

uniform vec3 kd = vec3(0.8, 0.0, 0.0); // default: darkish red
uniform vec3 ka = vec3(0.8, 0.0, 0.0); // " "      " "     " "
uniform vec3 ks = vec3(0.8, 0.0, 0.0); // " "      " "     " "
uniform int numLights;
uniform int projectionType;
uniform vec4 p_ecLightPositions[MAX_LIGHTS];
uniform vec3 lightStrengths[MAX_LIGHTS];
uniform vec3 globalAmbient;
uniform float spec_m;
uniform float alpha;

float attenuation(vec3 light, vec3 mcPos)
{
		return 1 / distance(light, mcPos);
}

vec4 evaluateLightingModel()
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

	vec3 normal = pvaIn.ecUnitNormal; // unit normal vector to Q
	vec3 Q = pvaIn.ecPosition; //point of evaluation

	vec3 ambientTotal = ka * globalAmbient;
	vec3 diffuseTotal = vec3(0.0, 0.0, 0.0);
	vec3 specularTotal = vec3(0.0, 0.0, 0.0);

	vec3 v_hat; // unit vector towards eye

	if(projectionType == PERSPECTIVE)
		v_hat = normalize(-Q);

	else if(projectionType == ORTHOGANAL)
		v_hat = vec3(0.0, 0.0, 1.0);

	else //must be oblique
		v_hat = pvaIn.obliqueVhat;

	if(dot(normal, v_hat) < 0) //check if normal in right direction
		normal = -normal;

	bool isPositional = false;

	 for(int i = 0; i<numLights; i++) //for each light source
	 {
		 	vec4 currentLightPos = p_ecLightPositions[i];
			vec3 li_hat;

			if(currentLightPos.w == 0.0) //directional light
			{
				li_hat = normalize(currentLightPos.xyz);
			}
			else //positional light
			{
				li_hat = normalize(currentLightPos.xyz - Q);
				isPositional = true;
			}

			float atten = attenuation(currentLightPos.xyz, Q) * 20;

			if(dot(li_hat, normal) > 0)
			{
				if(!isPositional)
					diffuseTotal += kd * lightStrengths[i] * dot(li_hat, normal);
				else
					diffuseTotal += atten * kd * lightStrengths[i] * dot(li_hat, normal);
			}

			vec3 ri_hat = normalize(reflect(li_hat, normal));
			if(dot(ri_hat, v_hat) > 0)
			{
				float rdotv = dot(ri_hat, v_hat);

				if(!isPositional)
					specularTotal += ks * lightStrengths[i] * pow(rdotv, spec_m);

				else
					specularTotal += atten * ks * lightStrengths[i] * pow(rdotv, spec_m);

			}
	 }

	vec3 totalLight = ambientTotal + diffuseTotal + specularTotal;

	for(int i=0; i<3; i++)
		if(totalLight[i] >= 1.0)
			totalLight[i] = 1.0;

	//return vec4(ambientTotal + (factor * kd * liStrength), alpha);
	return vec4(totalLight, alpha);
}

void main ()
{
	fragmentColor = evaluateLightingModel();
}
