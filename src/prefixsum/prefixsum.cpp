#include <sb7.h>
#include <vmath.h>
#include <sb7textoverlay.h>

#include <shader.h>
#include <iostream>

#include <cstdio>

#define NUM_ELEMENTS 2048

static inline float random_float()
{
	static unsigned int seed = 0x13371337;

	float res;
	unsigned int tmp;

	seed *= 16807;

	tmp = seed ^ (seed >> 4) ^ (seed << 15);

	*((unsigned int*)&res) = (tmp >> 9) | 0x3F800000;

	return (res - 1.0f);
}

class PrefixSum : public sb7::application
{
public:
	PrefixSum()
		: prefix_sum_prog(0)
	{

	}
protected:
	void init()
	{
		static const char title[] = "OpenGL SuperBible - 1D Prefix Sum";

		sb7::application::init();

		memcpy(info.title, title, sizeof(title));
	}
	
	void load_shader()
	{
		GLuint cs = sb7::shader::load("media/shaders/prefixsum/prefixsum.cs", GL_COMPUTE_SHADER);
		prefix_sum_prog = sb7::program::link_from_shaders(&cs, 1, true);
		
		glShaderStorageBlockBinding(prefix_sum_prog, 0, 0);
		glShaderStorageBlockBinding(prefix_sum_prog, 1, 1);
	}

	
	void startup()
	{
		overlay.init(80, 30);
		glGenBuffers(2, data_buffer);
		
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, data_buffer[0]);
		glBufferData(GL_SHADER_STORAGE_BUFFER, NUM_ELEMENTS * sizeof(float), nullptr, GL_DYNAMIC_DRAW);
		
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, data_buffer[1]);
		glBufferData(GL_SHADER_STORAGE_BUFFER, NUM_ELEMENTS * sizeof(float), nullptr, GL_DYNAMIC_DRAW);
		
		for (int i = 0; i < NUM_ELEMENTS; ++i)
		{
			input_data[i] = random_float();
			std::cout << input_data[i] << " ";
		}
		
		load_shader();
	}
	
	void render(double currentTime)
	{
		GLsizeiptr len = sizeof(float) * NUM_ELEMENTS;
		glBindBufferRange(GL_SHADER_STORAGE_BUFFER, 0, data_buffer[0], 0, len);
		glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, len, input_data); 
		
		
		glBindBufferRange(GL_SHADER_STORAGE_BUFFER, 1, data_buffer[1], 0, len);
		
		glUseProgram(prefix_sum_prog);
		glDispatchCompute(1, 1, 1);
		
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
		glFinish();
		
	
		glBindBufferRange(GL_SHADER_STORAGE_BUFFER, 0, data_buffer[1], 0, len);
		float* ptr = (float*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, len, GL_MAP_READ_BIT);
		
		char buffer[1024];

		sprintf(buffer, "SUM: %2.2f %2.2f %2.2f %2.2f %2.2f %2.2f %2.2f %2.2f "
			"%2.2f %2.2f %2.2f %2.2f %2.2f %2.2f %2.2f %2.2f",
			input_data[0], input_data[1], input_data[2], input_data[3], input_data[4], input_data[5], input_data[6], input_data[7],
			input_data[8], input_data[9], input_data[10], input_data[11], input_data[12], input_data[13], input_data[14], input_data[15]);


		char buffer_1[1024];
		sprintf(buffer_1, "\nSUM: %2.2f %2.2f %2.2f %2.2f %2.2f %2.2f %2.2f %2.2f "
			"%2.2f %2.2f %2.2f %2.2f %2.2f %2.2f %2.2f %2.2f",
			ptr[0], ptr[1], ptr[2], ptr[3], ptr[4], ptr[5], ptr[6], ptr[7],
			ptr[8], ptr[9], ptr[10], ptr[11], ptr[12], ptr[13], ptr[14], ptr[15]);

		
		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

		overlay.clear();
		overlay.drawText(buffer, 0, 0);
		overlay.drawText(buffer_1, 0, 10);
		overlay.draw();
	}
protected:
	GLuint  data_buffer[2];

	float input_data[NUM_ELEMENTS];
	float output_data[NUM_ELEMENTS];

	GLuint  prefix_sum_prog;
	sb7::text_overlay overlay;
};

DECLARE_MAIN(PrefixSum);
