#include <sb7.h>


class HelloOpenGL : public sb7::application
{
	void init() 
	{
		static const char title[] = "Hello OpenGL";
		sb7::application::init();
		memcpy(info.title, title, sizeof(title));
	}
	
	virtual void render(double currentTime)
	{
		static const GLfloat red[] = { 1.0f, 0.0f, 0.0f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, red);
	}
};

DECLARE_MAIN(HelloOpenGL)