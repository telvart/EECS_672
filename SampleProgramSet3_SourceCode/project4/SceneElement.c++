// SceneElement.c++

#include "SceneElement.h"
#include "ImageReader.h"

// Are coordinates in "lightPos" stored in MC or EC?
bool SceneElement::posInModelCoordinates[MAX_NUM_LIGHTS] =
	{ true, true, true };

float SceneElement::lightStrength[3*MAX_NUM_LIGHTS] =
	{
		0.6, 0.6, 0.6,
		0.2, 0.2, 0.2,
		1.0, 0.2, 0.2
	};

float SceneElement::lightPos[4*MAX_NUM_LIGHTS] =
	{
		 0.0, -20.0, 40.0, 0.0,
		 0.0,  20.0, 0.0, 0.0,
		 150.0, 0.0, 20.0, 1.0
	};

float SceneElement::globalAmbient[] = { 0.1, 0.1, 0.1};

SceneElement::SceneElement(ShaderIF* sIF, const PhongMaterial& matlIn) :
	shaderIF(sIF), matl(matlIn), texID(0), colorGenerationMode(-1),
	textureSource(-1), isVisible(true)
{
}

SceneElement::~SceneElement()
{
}

void SceneElement::establishLightingEnvironment()
{
	int actualNumLights = 2;

	cryph::Matrix4x4 mc_ec, ec_lds;
	getMatrices(mc_ec, ec_lds);

	// The following is the buffer actually sent to GLSL. It will contain a copy of
	// the (x,y,z,w) for light sources defined in EC; it will contain the coordinates
	// after transformation to EC if the position was originally specified in MC.
	float lightECs[actualNumLights*4];

	for(int i=0; i<(actualNumLights*4); i+=4)
	{
		if(posInModelCoordinates[i/4] == true)
		{
			if(lightPos[i+3] == 1)
			{
				cryph::AffPoint p(lightPos[i], lightPos[i+1], lightPos[i+2]);
				p = mc_ec * p;
				lightECs[i] = p.x; lightECs[i+1] = p.y; lightECs[i+2] = p.z; lightECs[i+3] = lightPos[i+3];
			}
			else
			{
				cryph::AffVector v(lightPos[i], lightPos[i+1], lightPos[i+2]);
				v = mc_ec * v;
				lightECs[i] = v.dx; lightECs[i+1] = v.dy; lightECs[i+2] = v.dz; lightECs[i+3] = lightPos[i+3];
			}
		}
		else
		{
			lightECs[i] = lightPos[i];
			lightECs[i+1] = lightPos[i+1];
			lightECs[i+2] = lightPos[i+2];
			lightECs[i+3] = lightPos[i+3];
		}
	}

	glUniform1i (shaderIF->ppuLoc("numLights"), actualNumLights);
	glUniform3fv(shaderIF->ppuLoc("lightStrengths"), actualNumLights, lightStrength);
	glUniform3fv(shaderIF->ppuLoc("globalAmbient"), 1, globalAmbient);
	glUniform4fv(shaderIF->ppuLoc("p_ecLightPositions"), actualNumLights, lightECs);
}

void SceneElement::establishMaterial()
{
	glUniform3fv(shaderIF->ppuLoc("kd"), 1, matl.kd);
	glUniform3fv(shaderIF->ppuLoc("ka"), 1, matl.ka);
	glUniform3fv(shaderIF->ppuLoc("ks"), 1, matl.ks);
	glUniform1f(shaderIF->ppuLoc("alpha"), matl.alpha);
	glUniform1f(shaderIF->ppuLoc("spec_m"), matl.shininess);
}

void SceneElement::establishTexture()
{
	// Unless you are texture-mapping onto faces of BasicShape instances,
	// this method should be called from your render method, and it should
	// set texture-related parameters like:
	// "colorGenerationMode", "textureSource", "textureMap"
	// It should also do the appropriate call to glBindTexture.
	// (If you are texture-mapping onto faces of BasicShape instances,
	// you use the "prepareForFace" callback which may or may not be
	// implemented by calling this method.)

	int activeTexture = 0;
	glActiveTexture(GL_TEXTURE0 + activeTexture);
	glUniform1i(shaderIF->ppuLoc("textureMap"), activeTexture);
	glBindTexture(GL_TEXTURE_2D, texID);
}

void SceneElement::useTexture()
{
	glUniform1i(shaderIF->ppuLoc("modelHasTexture"), 1);
}

void SceneElement::disableTexture()
{
	glUniform1i(shaderIF->ppuLoc("modelHasTexture"), 0);
}

void SceneElement::establishView()
{
	// Line of sight, dynamic view controls, 3D-2D projection, & mapping to LDS:
	cryph::Matrix4x4 mc_ec, ec_lds;
	ModelView::getMatrices(mc_ec, ec_lds);
	float m[16];
	glUniformMatrix4fv(shaderIF->ppuLoc("mc_ec"), 1, false, mc_ec.extractColMajor(m));
	glUniformMatrix4fv(shaderIF->ppuLoc("ec_lds"), 1, false, ec_lds.extractColMajor(m));

	switch(ModelView::projType)
	{
		case PERSPECTIVE:
			glUniform1i(shaderIF->ppuLoc("projectionType"), 1);
			break;
		case ORTHOGONAL:
			glUniform1i(shaderIF->ppuLoc("projectionType"), 2);
			break;
		case OBLIQUE:
			glUniform1i(shaderIF->ppuLoc("projectionType"), 3);
			break;
	}
}

bool SceneElement::handleCommand(unsigned char anASCIIChar, double ldsX, double ldsY)
{
	if (anASCIIChar == 'O')
		ModelView::setProjection(ORTHOGONAL);
	else if (anASCIIChar == 'P')
		ModelView::setProjection(PERSPECTIVE);
	else if (anASCIIChar == 'Q')
		ModelView::setProjection(OBLIQUE);
	else
		return ModelView::handleCommand(anASCIIChar, ldsX, ldsY);
	Controller::getCurrentController()->redraw();
	return true;
}

void SceneElement::setColorGenerationMode(int mode, int onFace)
{
	colorGenerationMode = mode;
}

void SceneElement::setTextureImage(const std::string& imgFileName, int onFace)
{
	ImageReader* ir = ImageReader::create(imgFileName.c_str());
	if (ir == nullptr)
	{
		std::cerr << "Could not open '" << imgFileName << "' for texture map.\n";
		return;
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);
	float white[] = { 1.0, 1.0, 1.0, 1.0 }; //border color
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, white);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); //GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); //GL_CLAMP_TO_BORDER);

	GLint level = 0;
	int pw = ir->getWidth();
	int ph = ir->getHeight();
	GLint iFormat = ir->getInternalFormat();
	GLenum format = ir->getFormat();
	GLenum type = ir->getType();
	const GLint border = 0; // must be zero (only present for backwards compatibility)
	const void* pixelData = ir->getTexture();
	glTexImage2D(GL_TEXTURE_2D, level, iFormat, pw, ph, border, format, type, pixelData);
	delete ir;

  Controller::checkForErrors(std::cout, "SceneElement::setTextureImage");
}

void SceneElement::setTextureSource(int source, int onFace)
{
	textureSource = source;
}
