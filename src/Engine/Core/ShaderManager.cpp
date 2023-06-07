#include <Engine/Core/ShaderManager.h>
#include <Engine/Core/IShader.h>
#include <Engine/Utils/File.h>
#include <asgl.h>
#ifdef LINUX
#include <unistd.h>
#endif
#include <Game/Core/Defines.h>
#include <cstdio>
#include <Game/Core/ResEnum.h>

ShaderManager* ShaderManager::get_instance ()
{
	static ShaderManager* instance = new ShaderManager ();

	return instance;
}

void ShaderManager::set_shader_size (const uint32_t count)
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
		programs[idx] = create_program (shader_info.enum_vert, shader_info.enum_frag);
		shaders[idx] = shader_info.shader;
		if (shader_info.shader)
			shaders[idx]->init ();
	}
}

uint32_t ShaderManager::create_shader (int type, uint64_t enum_pos)
{
	uint32_t shader = glCreateShader (type);

	uint64_t size;
	uint8_t* data = Utils::file_get_game_data (enum_pos, size, R_SHADERS);
	if (!data) {
		fprintf (stderr, "%lx: file not found\n", enum_pos);
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

uint32_t ShaderManager::create_program (uint64_t enum_vert, uint64_t enum_frag)
{
	uint32_t vertex = create_shader (GL_VERTEX_SHADER, enum_vert);
	uint32_t fragment = create_shader (GL_FRAGMENT_SHADER, enum_frag);

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
