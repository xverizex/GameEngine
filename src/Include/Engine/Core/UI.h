#ifndef UI_H
#define UI_H
#include <Engine/Core/Object.h>


class UI: public Object {
	protected:


		virtual void handleClickDown(int x, int y);
		virtual void handleClickUp(int x, int y);
		virtual void handleMove(int x, int y);
		virtual void handleKeyDown(uint8_t key);
	public:
		uint32_t width;
		uint32_t height;
		UI();
};



#endif
