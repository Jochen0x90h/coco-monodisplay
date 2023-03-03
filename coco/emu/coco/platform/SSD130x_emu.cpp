#include "SSD130x_emu.hpp"
#include "Display.hpp"
#include <iostream>
#include <iomanip>


namespace coco {


SSD130x_emu::SSD130x_emu(Loop_emu &loop, int width, int height)
	: Buffer(new uint8_t[width * ((height + 7) >> 3)], width * ((height + 7) >> 3), State::READY), loop(loop), width(width), height(height)
	, data(new uint8_t[width * height])
{
	this->loop.guiHandlers.add(*this);
}

SSD130x_emu::~SSD130x_emu() {
	delete [] this->p.data;
	delete [] this->data;
}

bool SSD130x_emu::start(Op op, int size) {
	if (this->p.state != State::READY || ((op & Op::COMMAND) == 0) && (op & Op::WRITE) == 0) {
		assert(false);
		return false;
	}

	this->op = op;
	this->transferred = size;

	// set state
	setState(State::BUSY);

	return true;
}

void SSD130x_emu::cancel() {
	if (this->p.state == State::BUSY) {
		this->transferred = 0;
		setState(State::CANCELLED);
	}
}

void SSD130x_emu::handle(Gui &gui) {
	auto op = this->op;
	int transferred = this->transferred;

	if (op != Op::NONE) {
		if ((op & Op::COMMAND) != 0) {
			// command
			uint8_t *b = this->p.data;
			for (int i = 0; i < transferred; ++i) {
				switch (b[i]) {
					// set contrast control
				case 0x81:
					this->contrast = b[++i];
					break;

					// entire display on
				case 0xA4:
					this->allOn = false;
					break;
				case 0xA5:
					this->allOn = true;
					break;

					// set normal/inverse display
				case 0xA6:
					this->inverse = false;
					break;
				case 0xA7:
					this->inverse = true;
					break;

					// set display on/off
				case 0xAE:
					this->enabled = false;
					break;
				case 0xAF:
					this->enabled = true;
					break;
				}
			}
		} else {
			// data
			int width = this->width;
			int height = this->height;
			uint8_t foreground = !this->enabled ? 0 : this->contrast;
			uint8_t background = (!this->enabled || this->allOn) ? foreground : (48 * this->contrast) / 255;
			if (this->inverse)
				std::swap(foreground, background);
			for (int j = 0; j < height; ++j) {
				uint8_t *line = &this->data[width * j];
				for (int i = 0; i < width; ++i) {
					bool bit = (this->p.data[i + width * (j >> 3)] & (1 << (j & 7))) != 0;
					line[i] = bit ? foreground : background;
				}
			}
		}		
		
		completed(transferred);
		op = Op::NONE;
	}

	gui.draw<Display>(this->data, this->width, this->height);
}

} // namespace coco
