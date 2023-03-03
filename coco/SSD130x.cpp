#include "SSD130x.hpp"


namespace coco {

AwaitableCoroutine SSD130x::init() {
	bool externalVcc = (this->flags & Flags::EXTERNAL_VCC) != 0;
	
	auto d = this->buffer.data();
	int i = 0;
	if (this->i2c)
		d[i++] = 0; // Co = 0, D/C = 0
	
	d[i++] = DISPLAY_OFF;
	
	d[i++] = CLOCK_DIV;
	d[i++] = 0x80;

	d[i++] = MULTIPLEX;
	d[i++] = this->height - 1;

	d[i++] = DISPLAY_OFFSET;
	d[i++] = 0; // no offset

	d[i++] = START_LINE + 0; // line 0

	d[i++] = CHARGEPUMP;
	d[i++] = externalVcc ? 0x10 : 0x14; // enable when no external vcc

	d[i++] = ADDRESSING_MODE;
	d[i++] = 0; // horizontal

	d[i++] = SEGMENT_REMAP1;

	d[i++] = COM_SCAN_DEC;

	uint8_t comPins = 0x02;
	uint8_t contrast = 0x8F;
	if (this->width == 128 && this->height == 32) {
		comPins = 0x02;
		contrast = 0x8F;
	} else if (this->width == 128 && this->height == 64) {
		comPins = 0x12;
		contrast = externalVcc ? 0x9F : 0xCF;
	} else if ((this->width == 96) && (this->height == 16)) {
		comPins = 0x2; // ada x12
		contrast = externalVcc ? 0x10 : 0xAF;
	} else {
		// Other screen varieties -- TBD
	
	}

	d[i++] = COM_PINS_CONFIG;
	d[i++] = comPins;

	d[i++] = CONTRAST;
	d[i++] = contrast;

	d[i++] = PRECHARGE_PERIOD;
	d[i++] = externalVcc ? 0x22 : 0xF1;
	
	d[i++] = VCOM_DETECT;
	d[i++] = 0x40;

	d[i++] = ALL_ON_DISABLE;

	d[i++] = INVERT_DISABLE;

	//d[i++] = DISPLAY_ON;

	co_await buffer.transfer(Buffer::Op::COMMAND, i);
}

AwaitableCoroutine SSD130x::enable() {
	//display::enableVcc(true);
	this->buffer[0] = DISPLAY_ON;
	co_await buffer.transfer(Buffer::Op::COMMAND, 1);
	this->enabled = true;
}

AwaitableCoroutine SSD130x::disable() {
	this->buffer[0] = DISPLAY_OFF;
	co_await buffer.transfer(Buffer::Op::COMMAND, 1);
	this->enabled = true;
	//display::enableVcc(false);
}

AwaitableCoroutine SSD130x::setContrast(uint8_t contrast) {
	this->buffer[0] = CONTRAST;
	this->buffer[1] = contrast;
	co_await buffer.transfer(Buffer::Op::COMMAND, 2);
}

} // namespace coco
