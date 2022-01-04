#include <sb7.h>


class HelloOpenGL : public sb7::application
{
	void init() 
	{
		static const char title[] = "Hello OpenGL";
		sb7::application::init();
		memcpy(info.title, title, sizeof(title));
	}
	
	void startup(void)
	{
		static const char* vertexShaderSource[] = { "#version 330 core\n"
			"layout (location = 0) in vec2 aPos;\n"
			"void main()\n"
			"{\n"
			"   gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);\n"
			"}\0" };
		const char* fragmentShaderSource[] = {"#version 330 core\n"
			"out vec4 FragColor;\n"
			"void main()\n"
			"{\n"
			"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
			"}\n\0" };

		program = glCreateProgram();
		GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fs, 1, fragmentShaderSource, NULL);
		glCompileShader(fs);


		GLuint vs = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vs, 1, vertexShaderSource, NULL);
		glCompileShader(vs);


		glAttachShader(program, vs);
		glAttachShader(program, fs);

		glLinkProgram(program);


		static const GLfloat grass_blade[] =
		{
			-0.3f, 0.0f,
			 0.3f, 0.0f,
			-0.20f, 1.0f,
			 0.1f, 1.3f,
			-0.05f, 2.3f,
			 0.0f, 3.3f
		};
		glGenVertexArrays(1, &grass_vao);
		glGenBuffers(1, &grass_buffer);

		glBindVertexArray(grass_vao);
		glBindBuffer(GL_ARRAY_BUFFER, grass_buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(grass_blade), grass_blade, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
		glEnableVertexAttribArray(0);
		
		glBindVertexArray(0);
	}
	
	virtual void render(double currentTime)
	{
		static const GLfloat red[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, red);
		
		glUseProgram(program);
		glBindVertexArray(grass_vao);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
	}
protected:    
	GLuint      grass_buffer;
	GLuint      grass_vao;
	GLuint program;
};

DECLARE_MAIN(HelloOpenGL)