#include <Game/Core/GameConfig.h>
#include <Engine/Core/ShaderManager.h>
#include <Engine/Core/ScreenManager.h>
#include <Game/Core/ShaderList.h>
#include <Game/Shaders/Shaders.h>
#include <Game/Core/LevelsEnum.h>
#include <vector>

#include <Game/Levels/GameLevel.h>
#include <Game/Levels/SelectLevel.h>
#include <Game/Levels/Menu.h>
#include <Game/Levels/Tutorial.h>

#include <cstdio>

GameConfig* GameConfig::get_instance ()
{
	static GameConfig* instance = new GameConfig ();

	return instance;
}

void GameConfig::init_shaders ()
{
	std::vector<ShaderInfoFile> list_shaders;
	list_shaders.push_back ({SHADER_RECT, new ShaderRect(), RES_RECT_VERT, RES_RECT_FRAG});
	list_shaders.push_back ({SHADER_SPRITE, new Shader2DSprite(), RES_SPRITE_VERT, RES_SPRITE_FRAG});
	list_shaders.push_back ({SHADER_SPRITE_WITH_CAMERA, new Shader2DSpriteWithCamera(), RES_SPRITE_WITH_CAMERA_VERT, RES_SPRITE_WITH_CAMERA_FRAG});
	list_shaders.push_back ({SHADER_TEXT, nullptr, RES_TEXT_VERT, RES_TEXT_FRAG});

	ShaderManager* shader_manager = ShaderManager::get_instance ();

	shader_manager->set_shaders_and_compile (list_shaders);
}

void GameConfig::init_screen ()
{
	ScreenManager* screen_manager = ScreenManager::get_instance ();

	//screen_manager->set_screen_size (1920, 1080, false);
	screen_manager->set_desktop_size (0);
}

void GameConfig::list_of_levels ()
{
	n_levels.resize (N_LEVELS);
#if 0
	n_levels[LEVEL_GAME] = new GameLevel ();
	n_levels[LEVEL_SELECT_GAME] = new SelectLevel ();
	n_levels[LEVEL_MENU] = new MenuLevel ();
	n_levels[LEVEL_TUTORIAL] = new TutorialLevel ();
#endif
}

void GameConfig::entry_point ()
{
#if 0
	cur_level = n_levels[LEVEL_MENU];

	cur_level->load_assets ();
#endif
}

void GameConfig::switch_level ()
{
	cur_level->unload_assets ();
	cur_level = n_levels[cur_level->new_level];
	cur_level->load_assets ();
}
