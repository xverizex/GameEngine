#include <Game/Core/GameConfig.h>
#include <Engine/Core/ShaderManager.h>
#include <Engine/Core/Global.h>
#include <vector>

void GameConfig::init_shaders ()
{
	std::vector<ShaderInfoFile> list_shaders;
	list_shaders.push_back ({SHADER_3D_WITHOUT_LIGHT, "shader_3d_without_light"});

	ShaderManager* shader_manager = Global::get_singleton<ShaderManager> ();

	shader_manager->set_shaders_and_compile (list_shaders);
}
