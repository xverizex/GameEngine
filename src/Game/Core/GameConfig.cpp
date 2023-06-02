#include <Game/Core/GameConfig.h>
#include <Engine/Core/ShaderManager.h>
#include <Engine/Core/ScreenManager.h>
#include <Game/Core/ShaderList.h>
#include <Game/Shaders/Shaders.h>
#include <Game/Core/LevelsEnum.h>
#include <vector>

#include <Game/Levels/GameLevel.h>
#include <cstdio>

GameConfig* GameConfig::get_instance ()
{
	static GameConfig* instance = new GameConfig ();

	return instance;
}

void GameConfig::init_shaders ()
{
	std::vector<ShaderInfoFile> list_shaders;
	list_shaders.push_back ({SHADER_RECT, "rect", new ShaderRect()});
	list_shaders.push_back ({SHADER_SPRITE, "sprite", new Shader2DSprite()});

	ShaderManager* shader_manager = ShaderManager::get_instance ();

	shader_manager->set_shaders_and_compile (list_shaders);
}

void GameConfig::init_screen ()
{
	ScreenManager* screen_manager = ScreenManager::get_instance ();

	screen_manager->set_screen_size (1024, 1024, false);
}

void GameConfig::list_of_levels ()
{
	n_levels.resize (N_LEVELS);
	n_levels[LEVEL_GAME] = new GameLevel ();
}

void GameConfig::entry_point ()
{
	cur_level = n_levels[LEVEL_GAME];

	cur_level->load_assets ();
}

void GameConfig::switch_level ()
{
	cur_level->unload_assets ();
	cur_level = n_levels[cur_level->new_level];
	cur_level->load_assets ();
}
