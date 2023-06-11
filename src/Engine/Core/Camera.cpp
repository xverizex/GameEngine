#include <Engine/Core/Camera.h>

static glm::mat4 cam;

glm::mat4& get_cam ()
{
	return cam;
}
