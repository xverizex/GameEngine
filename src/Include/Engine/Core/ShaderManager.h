#ifndef SHADER_MANAGER_H
#define SHADER_MANAGER_H
#include <cstdint>
#include <vector>
#include <string>

struct ShaderInfoFile {
	uint32_t index_program;
	std::string name;
};

class ShaderManager {
	public:
		ShaderManager (const uint32_t count);

		void set_shaders_and_compile (std::vector<ShaderInfoFile>& list);

		uint32_t operator[](uint32_t idx);

	protected:
		uint32_t *programs;
		uint32_t max_count;

	private:
		uint32_t create_program (std::string& name);
		uint32_t create_shader (int type, std::string& name);
};

#endif
