#include <coco/Buffer.hpp>
#include <coco/platform/Loop_emu.hpp>
#include <string>


namespace coco {

/**
 * Implementation of an SSD1306/SSD1309 emulator that prints the dipslay contents to std::cout
 */
class SSD130x_emu : public Buffer, public Loop_emu::GuiHandler {
public:
	/**
	 * Constructor
	 * @param loop event loop
	 * @param width width of emulated display
	 * @param height height of emulated display
	 */
	SSD130x_emu(Loop_emu &loop, int width, int height);
	~SSD130x_emu() override;

	bool start(Op op, int size) override;
	void cancel() override;

protected:
	void handle(Gui &gui) override;

	Loop_emu &loop;
	int width;
	int height;

	Op op = Op::NONE;
	int transferred;

	bool enabled;
	uint8_t contrast = 255;
	bool allOn = false;
	bool inverse = false;
	uint8_t *data;

};

} // namespace coco
