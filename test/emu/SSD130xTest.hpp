#pragma once

#include <coco/platform/SSD130x_emu.hpp>


using namespace coco;

// drivers for SSD130xTest
struct Drivers {
	Loop_emu loop;
	SSD130x_emu display{loop, 128, 64};
};
