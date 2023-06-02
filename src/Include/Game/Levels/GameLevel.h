#ifndef GAME_LEVEL_H
#define GAME_LEVEL_H

#include <Engine/Core/ILevel.h>
#include <Engine/Core/Sprite.h>
#include <vector>

class GameLevel: public ILevel {
	public:
		GameLevel ();
		void load_assets () override;
		void unload_assets () override;
		void tick () override;
		void mouse_click (int x, int y) override;
		void render () override;

		std::vector<Sprite*> sprites;
};

#endif
