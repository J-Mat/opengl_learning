#include <sb7.h>
#include <vmath.h>

#include <string>


static void print_shader_log(GLuint shader)
{
	std::string str;
	GLint len;

	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
	if (len != 0)
	{
		str.resize(len);
		glGetShaderInfoLog(shader, len, NULL, &str[0]);
	}

#ifdef _WIN32
	OutputDebugStringA(str.c_str());
#endif

 }

static const char* vs_source[] =
{
	"#version 420 core                                                              \n"
	"                                                                               \n"
	"void main(void)                                                                \n"
	"{                                                                              \n"
	"    const vec4 vertices[] = vec4[](vec4( 0.75, -0.75, 0.5, 1.0),               \n"
	"                                   vec4(-0.75, -0.75, 0.5, 1.0),               \n"
	"                                   vec4( 0.75,  0.75, 0.5, 1.0));              \n"
	"                                                                               \n"
	"    gl_Position = vertices[gl_VertexID];                                       \n"
	"}                                                                              \n"
};

static const char* fs_source[] =
{
	"#version 430 core                                                              \n"
	"                                                                               \n"
	"uniform sampler2D s;                                                           \n"
	"                                                                               \n"
	"out vec4 color;                                                                \n"
	"                                                                               \n"
	"void main(void)                                                                \n"
	"{                                                                              \n"
	"    color = texture(s, gl_FragCoord.xy / textureSize(s, 0));                   \n"
	"}                                                                              \n"
};



class SimpleTexture : public sb7::application
{
public:
	void init()
	{
		static const char title[] = "OpenGL SuperBible - Simple Texturing";

		sb7::application::init();

		memcpy(info.title, title, sizeof(title));
	}

	void generate_texture(float* data, int width, int height)
	{
		for (int x = 0; x < width; ++x)
		{
			for (int y = 0; y < height; ++y)
			{
				data[(y * width + x) * 4 + 0] = (float)((x & y) & 0xFF) / 255.0f;
				data[(y * width + x) * 4 + 1] = (float)((x | y) & 0xFF) / 255.0f;
				data[(y * width + x) * 4 + 2] = (float)((x ^ y) & 0xFF) / 255.0f;
				data[(y * width + x) * 4 + 3] = 1.0f;
			}
		}
	}

	void startup(void)
	{
		glGenTextures(1, &texture);
		
		glBindTexture(GL_TEXTURE_2D, texture);
		
		glTexStorage2D(GL_TEXTURE_2D, 8, GL_RGBA32F, 256, 256);

		float* data = new float[256 * 256 * 4];
		
		generate_texture(data, 256, 256);

		glTexSubImage2D(
			GL_TEXTURE_2D,
			0,
			0, 0,
			256, 256,
			GL_RGBA,
			GL_FLOAT,
			data
		);
		delete[] data;
		
		program = glCreateProgram();
		GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fs, 1, fs_source, NULL);
		glCompileShader(fs);

		print_shader_log(fs);

		GLuint vs = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vs, 1, vs_source, NULL);
		glCompileShader(vs);

		print_shader_log(vs);

		glAttachShader(program, vs);
		glAttachShader(program, fs);

		glLinkProgram(program);

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

	}

	void shutdown(void)
	{
		glDeleteProgram(program);
		glDeleteVertexArrays(1, &vao);
		glDeleteTextures(1, &texture);
	}

	void render(double t)
	{
		static const GLfloat green[] = { 0.0f, 0.25f, 0.0f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, green);
	
		glUseProgram(program);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}


private:
	GLuint texture;
	GLuint program;
	GLuint vao;
};

DECLARE_MAIN(SimpleTexture);

