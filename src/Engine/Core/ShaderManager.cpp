#include <Engine/Core/ShaderManager.h>
#include <Engine/Core/IShader.h>
#include <Engine/Utils/File.h>
#include <GLES3/gl3.h>
#include <unistd.h>
#include <cstdio>

ShaderManager::ShaderManager (const uint32_t count)
{
	programs = new uint32_t[count];
	if (!programs) {
		exit (-1);
	}
	shaders = new IShader*[count];

	max_count = count;
}


IShader* ShaderManager::get_shader (uint32_t idx)
{
	if (idx >= max_count) {
		exit (-1);
	}
	return shaders[idx];
}

uint32_t ShaderManager::get_program (uint32_t idx)
{
	if (idx >= max_count) {
		exit (-1);
	}
	return programs[idx];
}

void ShaderManager::set_shaders_and_compile (std::vector<ShaderInfoFile>& list)
{
	for (ShaderInfoFile shader_info: list) {
		uint32_t idx = shader_info.index_program;
		programs[idx] = create_program (shader_info.name);
		shaders[idx] = shader_info.shader;
		shaders[idx]->init ();
	}
}

uint32_t ShaderManager::create_shader (int type, std::string& name)
{
	uint32_t shader = glCreateShader (type);

	uint8_t* data = Utils::file_get_data (name);
	if (!data) {
		fprintf (stderr, "%s: file not found\n", name.c_str());
		exit (-1);
	}

	glShaderSource (shader, 1, (const char* const* ) &data, nullptr);

	delete[] data;

	glCompileShader (shader);

	int value;
	glGetShaderiv (shader, GL_COMPILE_STATUS, &value);

	if (value == GL_FALSE) {
		glGetShaderiv (shader, GL_INFO_LOG_LENGTH, &value);

		char *info = new char[value + 1];
		glGetShaderInfoLog (shader, value, &value, info);
		info[value] = 0;
		fprintf (stderr, "compile shader: [%s]\n", info);

		delete[] info;
		exit (-1);
	}

	return shader;
}

uint32_t ShaderManager::create_program (std::string& name)
{
	std::string vertex_str = name + ".vert";
	std::string fragment_str = name + ".frag";

	uint32_t vertex = create_shader (GL_VERTEX_SHADER, vertex_str);
	uint32_t fragment = create_shader (GL_FRAGMENT_SHADER, fragment_str);

	uint32_t program = glCreateProgram ();

	glAttachShader (program, vertex);
	glAttachShader (program, fragment);
	glLinkProgram (program);

	int value;
	glGetProgramiv (program, GL_LINK_STATUS, &value);
	if (value == GL_FALSE) {
		glGetProgramiv (program, GL_INFO_LOG_LENGTH, &value);
		char *info = new char[value + 1];
		glGetProgramInfoLog (program, value, &value, info);
		info[value] = 0;
		fprintf (stderr, "link program: [%s]\n", info);
		delete[] info;
		exit (-1);
	}

	return program;
}
