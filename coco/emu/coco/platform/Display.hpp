#pragma once

#include <coco/platform/Gui.hpp>


namespace coco {

class Display : public Gui::Renderer {
public:
	Display();

	Gui::Size draw(float x, float y, const uint8_t *buffer, int width, int height);

protected:
	GLuint texture;
};

} // namespace coco
