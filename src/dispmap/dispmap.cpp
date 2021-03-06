#include <sb7.h>
#include <vmath.h>

#include <object.h>
#include <shader.h>
#include <sb7ktx.h>
#include <vector>
#include <sb7textoverlay.h>
#include <sb7color.h>


class DispMap : public sb7::application
{
public:
	void load_shaders()
	{
		if (program)
			glDeleteProgram(program);
		GLuint vs = sb7::shader::load("media/shaders/dispmap/dispmap.vs", GL_VERTEX_SHADER);
		GLuint tcs = sb7::shader::load("media/shaders/dispmap/dispmap.tcs", GL_TESS_CONTROL_SHADER);
		GLuint tes = sb7::shader::load("media/shaders/dispmap/dispmap.tes", GL_TESS_EVALUATION_SHADER);
		GLuint fs = sb7::shader::load("media/shaders/dispmap/dispmap.fs", GL_FRAGMENT_SHADER);
		
		program = glCreateProgram();
		
		glAttachShader(program, vs);
		glAttachShader(program, tes);
		glAttachShader(program, tes);
		glAttachShader(program, fs);
		
		glLinkProgram(program);
		
		uniforms.mv_matrix = glGetUniformLocation(program, "mv_matrix");
		uniforms.mvp_matrix = glGetUniformLocation(program, "mvp_matrix");
		uniforms.proj_matrix = glGetUniformLocation(program, "proj_matrix");
		uniforms.dmap_depth = glGetUniformLocation(program, "dmap_depth");
		uniforms.enable_fog = glGetUniformLocation(program, "enable_fog");
		dmap_depth = 6.0f;
		
	}

	virtual void startup()
	{
		load_shaders();
		
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		
		glPatchParameteri(GL_PATCH_VERTICES, 4);
		glEnable(GL_CULL_FACE);
		
		tex_displacement = sb7::ktx::file::load("media/textures/terragen1.ktx");
		glActiveTexture(GL_TEXTURE1);
		tex_color = sb7::ktx::file::load("media/textures/terragen_color.ktx");
	}

	virtual void render(double currentTime)
	{
		static const GLfloat black[] = { 0.85f, 0.95f, 1.0f, 1.0f };
		static const GLfloat one = 1.0f;
		static double last_time = 0.0;
		static double total_time = 0.0;

		if (!paused)
			total_time += (currentTime - last_time);
		last_time = currentTime;

		float t = (float)total_time * 0.03f;
		float r = sinf(t * 5.37f) * 15.0f + 16.0f;
		float h = cosf(t * 4.79f) * 2.0f + 3.2f;

		vmath::mat4 mv_matrix = /* vmath::translate(0.0f, 0.0f, -1.4f) *
						vmath::translate(0.0f, -0.4f, 0.0f) * */
						// vmath::rotate((float)currentTime * 6.0f, 0.0f, 1.0f, 0.0f) *
						vmath::lookat(vmath::vec3(sinf(t) * r, h, cosf(t) * r), vmath::vec3(0.0f), vmath::vec3(0.0f, 1.0f, 0.0f));
		vmath::mat4 proj_matrix = vmath::perspective(60.0f,
			(float)info.windowWidth / (float)info.windowHeight,
			0.1f, 1000.0f);
		
		glUseProgram(program);
		
		glUniformMatrix4fv(uniforms.mv_matrix, 1, GL_FALSE, mv_matrix);
		glUniformMatrix4fv(uniforms.proj_matrix, 1, GL_FALSE, proj_matrix);
		glUniformMatrix4fv(uniforms.mvp_matrix, 1, GL_FALSE, proj_matrix * mv_matrix);
		glUniform1f(uniforms.dmap_depth, enable_displacement ? dmap_depth : 0.0f);
		glUniform1i(uniforms.enable_fog, enable_fog ? 1 : 0);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		
		if (wireframe)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		glDrawArraysInstanced(GL_PATCHES, 0, 4, 64 * 64);
	}

	virtual void shutdown()
	{
		glDeleteVertexArrays(1, &vao);
		glDeleteProgram(program);
	}

	void onKey(int key, int action)
	{
		if (action == 1)
		{
			switch (key)
			{
			case GLFW_KEY_KP_ADD: dmap_depth += 0.1f;
				break;
			case GLFW_KEY_KP_SUBTRACT: dmap_depth -= 0.1f;
				break;
			case 'F': enable_fog = !enable_fog;
				break;
			case 'D': enable_displacement = !enable_displacement;
				break;
			case 'W': wireframe = !wireframe;
				break;
			case 'P': paused = !paused;
				break;
			case 'R':
				load_shaders();
				break;
			default:
				break;
			};
		}
	}
private: 
	struct
	{
		GLint mvp_matrix;
		GLint mv_matrix;
		GLint proj_matrix;
		GLint dmap_depth;
		GLint enable_fog;
	} uniforms;
	GLuint          program;
	GLuint          vao;
	GLuint          tex_displacement;
	GLuint          tex_color;
	float           dmap_depth;
	bool            enable_displacement;
	bool            wireframe;
	bool            enable_fog;
	bool            paused;
};

DECLARE_MAIN(DispMap);