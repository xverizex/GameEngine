#ifndef SHADER_MANAGER_H
#define SHADER_MANAGER_H
#include <cstdint>
#include <vector>
#include <string>

class IShader;

struct ShaderInfoFile {
	uint32_t index_program;
	IShader* shader;
	uint64_t enum_vert;
	uint64_t enum_frag;
};

class ShaderManager {
	public:
		static ShaderManager* get_instance ();
		void set_shader_size (const uint32_t count);

		void set_shaders_and_compile (std::vector<ShaderInfoFile>& list);

		uint32_t get_program (uint32_t idx);
		IShader* get_shader (uint32_t idx);


	protected:
		uint32_t* programs;
		IShader** shaders;
		uint32_t max_count;

	private:
		uint32_t create_program (uint64_t enum_vert, uint64_t enum_frag);
		uint32_t create_shader (int type, uint64_t enum_pos);
};

#endif
