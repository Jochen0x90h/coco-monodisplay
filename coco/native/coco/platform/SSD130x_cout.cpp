#include "SSD130x_cout.hpp"
#include <iostream>
#include <iomanip>


namespace coco {


SSD130x_cout::SSD130x_cout(Loop_native &loop, int width, int height)
	: Buffer(new uint8_t[width * ((height + 7) >> 3)], width * ((height + 7) >> 3), State::READY), loop(loop), width(width), height(height) {
}

SSD130x_cout::~SSD130x_cout() {
	delete [] this->buffer;
}

bool SSD130x_cout::start(Op op, int size) {
	if (this->state != State::READY || ((op & Op::WRITE) == 0 && (op & Op::COMMAND) == 0)) {
		assert(false);
		return false;
	}

	this->op = op;
	this->transferred = size;

	if (!this->inList())
		this->loop.yieldHandlers.add(*this);

	// set state
	setState(State::BUSY);

	return true;
}

void SSD130x_cout::cancel() {
	if (this->state == State::BUSY) {
		this->transferred = 0;
		setState(State::CANCELLED);
	}
}

void SSD130x_cout::handle() {
	this->remove(); 

	auto op = this->op;
	int transferred = this->transferred;

	if ((op & Op::COMMAND) == 0) {
		std::cout << std::endl;
		for (int y = 0; y < this->height; ++y) {
			uint8_t *line = this->buffer + (y >> 3) * this->width;
			std::cout << std::setw(2) << y << ": ";
			for (int x = 0; x < this->width; ++x) {
				bool on = (line[x] & (1 << (y & 7))) != 0;
				std::cout << (on ? 'O' : ' ');
			}
			std::cout << std::endl;
		}
	}

	completed(transferred);
}

} // namespace coco
