#include <Game/Levels/GameLevel.h>
#include <Game/Core/SpritesEnum.h>
#include <Engine/Core/InfoSprite.h>
#include <Engine/Core/Downloader.h>
#include <vector>
#include <Game/Sprites/Tile.h>

GameLevel::GameLevel ()
{
}

void GameLevel::load_assets ()
{
	std::vector<InfoSprite> list;
	list.push_back ({"tile", TILE_SPRITE});
	downloader_init_sprite_array (N_SPRITES);

	downloader_download_all_sprites_without_progressbar (list);

	Tile* tile = new Tile ();
	tile->setPos (glm::vec3 (4.f, 4.f, 0.f));

	sprites.push_back (tile);
}

void GameLevel::unload_assets ()
{
}

void GameLevel::tick ()
{
}

void GameLevel::mouse_click (int x, int y)
{
}

void GameLevel::render ()
{
	sprites[0]->draw ();
}
