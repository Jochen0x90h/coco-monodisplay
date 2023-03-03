#pragma once

#include <coco/platform/SSD130x_cout.hpp>


using namespace coco;

// drivers for SSD130xTest
struct Drivers {
	Loop_native loop;
	SSD130x_cout display{loop, 128, 64};
};
