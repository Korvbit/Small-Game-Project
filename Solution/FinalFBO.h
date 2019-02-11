#ifndef FINALFBO_H
#define FINALFBO_H

#include <glew\glew.h>
#include <iostream>

class FinalFBO
{
public:
	enum FINALBUFFER_TEXTURE_TYPE
	{
		FINALFBO_TEXTURE_TYPE_DIFFUSE,
		FINALFBO_NUM_TEXTURES
	};

	FinalFBO();
	~FinalFBO();

	bool Init(unsigned int SCREENWIDTH, unsigned int SCREENHEIGHT);
	void BindForWriting();
	void BindForReading(GLuint textureUnit);

	void CopyDepth(unsigned int SCREENWIDTH, unsigned int SCREENHEIGHT, GLuint fboRead);

	GLuint GetFBO();
private:
	GLuint fbo;
	GLuint colorBuffers[FINALFBO_NUM_TEXTURES];
	GLuint depthTexture;
};

#endif