#include <sb7.h>
#include <vmath.h>

#include <object.h>
#include <shader.h>
#include <sb7ktx.h>
#include <vector>
#include <sb7textoverlay.h>
#include <sb7color.h>
using namespace std;

class  TessMode : public sb7::application
{
public:
	TessMode()
		: program_index(0)
	{

	}
	void init()
	{
		static const char title[] = "OpenGL SuperBible - Simple Texturing";

		sb7::application::init();

		memcpy(info.title, title, sizeof(title));
	}
	
	virtual void startup()
	{
		overlay.init(80, 50);
		GLuint vs, fs;
		vs = sb7::shader::load("media/shaders/tess_mode/tess_mode.vs", GL_VERTEX_SHADER);
		fs = sb7::shader::load("media/shaders/tess_mode/tess_mode.fs", GL_FRAGMENT_SHADER);
		
		static vector<GLuint> vs_vec = { vs, vs, vs, vs };
		static vector<GLuint> fs_vec = { fs, fs, fs, fs };
		
		static vector<GLuint> tcs_vec = {
			sb7::shader::load("media/shaders/tess_mode/tess_mode_quads.tcs", GL_TESS_CONTROL_SHADER),
			sb7::shader::load("media/shaders/tess_mode/tess_mode_triangle.tcs", GL_TESS_CONTROL_SHADER),
			sb7::shader::load("media/shaders/tess_mode/tess_mode_triangle.tcs", GL_TESS_CONTROL_SHADER),
			sb7::shader::load("media/shaders/tess_mode/tess_mode_isolines.tcs", GL_TESS_CONTROL_SHADER),
		};
		 
		static vector<GLuint> tes_vec = {
			sb7::shader::load("media/shaders/tess_mode/tess_mode_quads.tes", GL_TESS_EVALUATION_SHADER),
			sb7::shader::load("media/shaders/tess_mode/tess_mode_triangle.tes", GL_TESS_EVALUATION_SHADER),
			sb7::shader::load("media/shaders/tess_mode/tess_mode_point.tes", GL_TESS_EVALUATION_SHADER),
			sb7::shader::load("media/shaders/tess_mode/tess_mode_isolines.tes", GL_TESS_EVALUATION_SHADER),
		};
		
		
		for (int i = 0; i < 4; ++i)
		{
			program[i] = glCreateProgram();
			glCompileShader(vs_vec[i]);
			glCompileShader(tcs_vec[i]);
			glCompileShader(tes_vec[i]);
			glCompileShader(fs_vec[i]);
			
			glAttachShader(program[i], vs_vec[i]);
			glAttachShader(program[i], tcs_vec[i]);
			glAttachShader(program[i], tes_vec[i]);
			glAttachShader(program[i], fs_vec[i]);
			glLinkProgram(program[i]);
		}
		
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		
		glPatchParameteri(GL_PATCH_VERTICES, 4);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	virtual void render(double currentTime)
	{
		glClearBufferfv(GL_COLOR, 0, sb7::color::Black);
		
		glUseProgram(program[program_index]);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawArrays(GL_PATCHES, 0, 4);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


		static const char* mode_names[] =
		{
			"QUADS", "TRIANGLES", "QUADS AS POINTS", "ISOLINES"
		};

		overlay.clear();
		overlay.print("Mode: ");
		overlay.print(mode_names[program_index]);
		overlay.print(" (M toggles)");

		overlay.draw();

	}

	virtual void shutdown()
	{
		glDeleteVertexArrays(1, &vao);

		for (int i = 0; i < 4; i++)
		{
			glDeleteProgram(program[i]);
		}
	}
	
	void onKey(int key, int action)
	{
		if (!action)
			return;

		switch (key)
		{
		case 'M': program_index = (program_index + 1) % 4;
			break;
		}
	}
	
private:
	GLuint program[4];
	int    program_index;
	GLuint vao;
	sb7::text_overlay   overlay;
};

DECLARE_MAIN(TessMode);


