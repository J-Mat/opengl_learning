#include <sb7.h>
#include <shader.h>
#include <object.h>
#include <vmath.h>


class  GSExplode: public sb7::application
{
	void init()
	{
		static const char title[] = "OpenGL SuperBible - Exploder";

		sb7::application::init();

		memcpy(info.title, title, sizeof(title));
	}
	
	virtual void startup()
	{
		program = glCreateProgram();
		
		GLuint vs = sb7::shader::load("media/shaders/gs_explode/gs_explode.vs", GL_VERTEX_SHADER);
		GLuint gs = sb7::shader::load("media/shaders/gs_explode/gs_explode.gs", GL_GEOMETRY_SHADER);
		GLuint fs = sb7::shader::load("media/shaders/gs_explode/gs_explode.fs", GL_FRAGMENT_SHADER);
		
		glAttachShader(program, vs);
		glAttachShader(program, gs);
		glAttachShader(program, fs);
	
		glLinkProgram(program);
		
		mv_location = glGetUniformLocation(program, "mv_matrix");
		proj_location = glGetUniformLocation(program, "proj_matrix");
		explode_factor_location = glGetUniformLocation(program, "explode_factor");

		object.load("media/objects/torus.sbm");

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
	}
	
	virtual void render(double currentTime)
	{
		static const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		static const GLfloat one = 1.0f;
		float f = (float)currentTime;

		glViewport(0, 0, info.windowWidth, info.windowHeight);
		glClearBufferfv(GL_COLOR, 0, black);
		glClearBufferfv(GL_DEPTH, 0, &one);
		
		glUseProgram(program);
		
		vmath::mat4 proj_matrix = vmath::perspective(50.0f,
			(float)info.windowWidth / (float)info.windowHeight,
			0.1f,
			1000.0f);
		glUniformMatrix4fv(proj_location, 1, GL_FALSE, proj_matrix);

		vmath::mat4 mv_matrix = vmath::translate(0.0f, 0.0f, -3.0f) *
			vmath::rotate((float)currentTime * 45.0f, 0.0f, 1.0f, 0.0f) *
			vmath::rotate((float)currentTime * 81.0f, 1.0f, 0.0f, 0.0f);
		glUniformMatrix4fv(mv_location, 1, GL_FALSE, mv_matrix);

		glUniform1f(explode_factor_location, sinf((float)currentTime * 8.0f) * cosf((float)currentTime * 6.0f) * 0.7f + 0.1f);

		object.render();
	}
	 
	virtual void shutdown()
	{
		object.free();
		glDeleteProgram(program);
	}

private:
	GLuint          program;
	GLint           mv_location;
	GLint           proj_location;
	GLint           explode_factor_location;

	sb7::object     object;
};

DECLARE_MAIN(GSExplode)
