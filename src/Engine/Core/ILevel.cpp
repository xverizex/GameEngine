#include <Engine/Core/ILevel.h>
#include <Game/Core/GameConfig.h>

ILevel::ILevel ()
{
	cfg = GameConfig::get_instance ();
}
