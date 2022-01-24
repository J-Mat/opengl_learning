#include <sb7.h>
#include <sb7ktx.h>
#include <vmath.h>
#include <shader.h>
#include <sb7ktx.h>
#include <cmath>
#include <iostream>
#include <object.h>

class BasicFBO : public sb7::application
{
public:
	BasicFBO()
	{}

	void init()
	{
		static const char title[] = "OpenGL SuperBible - Basic FBO";

		sb7::application::init();

		memcpy(info.title, title, sizeof(title));
	}
	
	virtual void startup()
	{
		program1 = glCreateProgram();

		GLuint vs = sb7::shader::load("media/shaders/basic_fbo/basic_fbo.vs", GL_VERTEX_SHADER);
		glCompileShader(vs);
		GLuint fs_1 = sb7::shader::load("media/shaders/basic_fbo/basic_fbo_1.fs", GL_FRAGMENT_SHADER);
		glCompileShader(fs_1);
		GLuint fs_2 = sb7::shader::load("media/shaders/basic_fbo/basic_fbo_2.fs", GL_FRAGMENT_SHADER);
		glCompileShader(fs_2);
		
		glAttachShader(program1, vs);
		glAttachShader(program1, fs_1);
		
		glLinkProgram(vs);
		glLinkProgram(fs_1);

		program2 = glCreateProgram();
		glAttachShader(program2, vs);
		glAttachShader(program2, fs_2);

		glLinkProgram(program2);
		
		glDeleteShader(vs);
		glDeleteShader(fs_1);
		glDeleteShader(fs_2);
		
		mv_location = glGetUniformLocation(program1, "mv_matrix");
		proj_location = glGetUniformLocation(program1, "proj_matrix");
		mv_location2 = glGetUniformLocation(program2, "mv_matrix");
		proj_location2 = glGetUniformLocation(program2, "proj_matrix");
		
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		
		static const GLushort vertex_indices[] =
		{
			0, 1, 2,
			2, 1, 3,
			2, 3, 4,
			4, 3, 5,
			4, 5, 6,
			6, 5, 7,
			6, 7, 0,
			0, 7, 1,
			6, 0, 2,
			2, 4, 6,
			7, 5, 3,
			7, 3, 1
		};

		static const GLfloat vertex_data[] =
		{
			// Position                 Tex Coord
		   -0.25f, -0.25f,  0.25f,      0.0f, 1.0f,
		   -0.25f, -0.25f, -0.25f,      0.0f, 0.0f,
			0.25f, -0.25f, -0.25f,      1.0f, 0.0f,

			0.25f, -0.25f, -0.25f,      1.0f, 0.0f,
			0.25f, -0.25f,  0.25f,      1.0f, 1.0f,
		   -0.25f, -0.25f,  0.25f,      0.0f, 1.0f,

			0.25f, -0.25f, -0.25f,      0.0f, 0.0f,
			0.25f,  0.25f, -0.25f,      1.0f, 0.0f,
			0.25f, -0.25f,  0.25f,      0.0f, 1.0f,

			0.25f,  0.25f, -0.25f,      1.0f, 0.0f,
			0.25f,  0.25f,  0.25f,      1.0f, 1.0f,
			0.25f, -0.25f,  0.25f,      0.0f, 1.0f,

			0.25f,  0.25f, -0.25f,      1.0f, 0.0f,
		   -0.25f,  0.25f, -0.25f,      0.0f, 0.0f,
			0.25f,  0.25f,  0.25f,      1.0f, 1.0f,

		   -0.25f,  0.25f, -0.25f,      0.0f, 0.0f,
		   -0.25f,  0.25f,  0.25f,      0.0f, 1.0f,
			0.25f,  0.25f,  0.25f,      1.0f, 1.0f,

		   -0.25f,  0.25f, -0.25f,      1.0f, 0.0f,
		   -0.25f, -0.25f, -0.25f,      0.0f, 0.0f,
		   -0.25f,  0.25f,  0.25f,      1.0f, 1.0f,

		   -0.25f, -0.25f, -0.25f,      0.0f, 0.0f,
		   -0.25f, -0.25f,  0.25f,      0.0f, 1.0f,
		   -0.25f,  0.25f,  0.25f,      1.0f, 1.0f,

		   -0.25f,  0.25f, -0.25f,      0.0f, 1.0f,
			0.25f,  0.25f, -0.25f,      1.0f, 1.0f,
			0.25f, -0.25f, -0.25f,      1.0f, 0.0f,

			0.25f, -0.25f, -0.25f,      1.0f, 0.0f,
		   -0.25f, -0.25f, -0.25f,      0.0f, 0.0f,
		   -0.25f,  0.25f, -0.25f,      0.0f, 1.0f,

		   -0.25f, -0.25f,  0.25f,      0.0f, 0.0f,
			0.25f, -0.25f,  0.25f,      1.0f, 0.0f,
			0.25f,  0.25f,  0.25f,      1.0f, 1.0f,

			0.25f,  0.25f,  0.25f,      1.0f, 1.0f,
		   -0.25f,  0.25f,  0.25f,      0.0f, 1.0f,
		   -0.25f, -0.25f,  0.25f,      0.0f, 0.0f,
		};
		 
		glGenBuffers(1, &position_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, position_buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), nullptr);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT)
	}

private: 
	GLuint          program1;
	GLuint          program2;
	GLuint          vao;
	GLuint          position_buffer;
	GLuint          index_buffer;
	GLuint          fbo;
	GLuint          color_texture;
	GLuint          depth_texture;
	GLint           mv_location;
	GLint           proj_location;
	GLuint          mv_location2;
	GLuint          proj_location2;
};


DECLARE_MAIN(BasicFBO)