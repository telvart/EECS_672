#version 410 core

// phong.fsh - a fragment shader that implements the Phong Lighting model.

#define PERSPECTIVE 1
#define ORTHOGANAL  2

in PVA
{
	vec3 ecPosition;
	vec3 ecUnitNormal;
} pvaIn;

uniform mat4 ec_lds = // (W-V map) * (projection matrix)
	mat4(1.0, 0.0, 0.0, 0.0, // initialize to (almost) identity matrix
	     0.0, 1.0, 0.0, 0.0, // ==> ORTHOGONAL projection -AND- EC = LDS
	     0.0, 0.0, -1.0, 0.0,
	     0.0, 0.0, 0.0, 1.0);

out vec4 fragmentColor;

const int MAX_LIGHTS = 3;
uniform int sceneHasTransluscentObjects = 0;
uniform int drawingOpaqueObjects = 1;

// Phong lighting model values
uniform vec3 kd = vec3(0.8, 0.0, 0.0); // default: darkish red
uniform vec3 ka = vec3(0.8, 0.0, 0.0); // " "      " "     " "
uniform vec3 ks = vec3(0.8, 0.0, 0.0); // " "      " "     " "
uniform vec3 lightStrengths[MAX_LIGHTS];
uniform vec3 globalAmbient;
uniform vec4 p_ecLightPositions[MAX_LIGHTS];
uniform float spec_m;
uniform float alpha;
uniform int numLights;
uniform int projectionType;


float attenuation(vec3 light, vec3 ecPos)
{
		return 1 / ( 0.5 * distance(light, ecPos));
}

vec4 evaluateLightingModel()
{

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
		v_hat = normalize(vec3((-ec_lds[2][0])/ec_lds[0][0], (-ec_lds[2][1])/ec_lds[1][1], 1.0)); //COLUMN MAJOR IS ANNOYING

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

		vec3 ri_hat = normalize(reflect(-li_hat, normal));

		if(dot(ri_hat, v_hat) > 0)
		{
			if(!isPositional)
				specularTotal += ks * lightStrengths[i] * pow(dot(ri_hat, v_hat), spec_m);

			else
				specularTotal += atten * ks * lightStrengths[i] * pow(dot(ri_hat, v_hat), spec_m);
		}
 }

	vec3 Iq = ambientTotal + diffuseTotal + specularTotal;

	for(int i=0; i<3; i++)
		if(Iq[i] >= 1.0)
			Iq[i] = 1.0;

	//return vec4(ambientTotal + (factor * kd * liStrength), alpha);
	return vec4(Iq, alpha);
}

void main ()
{
	vec4 color = evaluateLightingModel();
	if(sceneHasTransluscentObjects == 1)
	{
		if(drawingOpaqueObjects == 1)
		{
			if(color.w < 1.0)
				discard;
			else
				fragmentColor = color;
		}
		else if(color.w < 1.0)
			fragmentColor = color;
		else
			discard;
	}
	else
		fragmentColor = color;
}
