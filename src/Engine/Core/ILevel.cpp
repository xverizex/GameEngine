#include <Engine/Core/ILevel.h>
#include <Game/Core/GameConfig.h>

ILevel::ILevel ()
{
	cfg = GameConfig::get_instance ();
}

void ILevel::clear_screen ()
{
}

void ILevel::load_assets ()
{
}

void ILevel::load_stats ()
{

}

void ILevel::save_stats ()
{

}

void ILevel::load_level ()
{
}

void ILevel::unload_level ()
{

}

void ILevel::unload_assets ()
{
}

void ILevel::tick ()
{
}

void ILevel::mouse_click_down (int x, int y)
{
}

void ILevel::mouse_click_up (int x, int y)
{
}

void ILevel::mouse_motion (int x, int y)
{
}

void ILevel::key_click (SDL_Keysym sym)
{
}

void ILevel::render ()
{
}
