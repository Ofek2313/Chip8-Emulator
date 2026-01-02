#pragma once
#include <cstdint>
#include <string>
enum class RomErrCd {
	None,
	FailedToLoad,
	FileSize,
	FailedToRead

};
class Chip8
{
private:

	uint8_t memory[4096];
	uint8_t registers[16];
	uint16_t I;
	uint16_t PC;
	uint8_t keys[16];
	uint16_t stack[16];
	uint8_t SP;
	uint8_t display[64 * 32];
	uint8_t delayTimer;
	uint8_t soundTimer;
	uint16_t opcode;


public:
	RomErrCd loadROM(const std::string& filename);
	void disp_clear();

	void Op_1NNN();
	void Op_2NNN();
	void Op_3XNN();
	void Op_4XNN();
	void Op_5XY0();
	void Op_6XNN();
	void Op_7XNN();
	void Op_8XY0();
	void Op_8XY1();
	void Op_8XY2();
	void Op_8XY3();
	void Op_8XY4(); // Add Vy To Vx
	void Op_8XY5(); // Subtract Vy From Vx

};

