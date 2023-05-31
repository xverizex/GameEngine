#ifndef ENGINE_RECT_HEADER_H
#define ENGINE_RECT_HEADER_H
#include <stdint.h>
#include <Engine/Core/UI.h>

class ShaderRect;

class Rect: public UI {
	public:
		Rect ();
		Rect(uint32_t width, uint32_t height);
		virtual void setSize(uint32_t _width, uint32_t _height);
		virtual void resized(uint32_t w, uint32_t h);
		virtual void set_radius (float r);
		void setColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);
		const glm::vec4 &getColor() const;
		void draw() override;
		float top_left_radius = 0;
		float top_right_radius = 0;
		float bottom_left_radius = 0;
		float bottom_right_radius = 0;

	protected:
		bool is_resized = {false};
		
	private:
		glm::vec4 color;
};


#endif
