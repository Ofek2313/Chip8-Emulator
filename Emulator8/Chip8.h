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
	
	uint8_t delayTimer;
	uint8_t soundTimer;
	uint16_t opcode;


public:

	uint8_t display[32][64];

	Chip8();
	RomErrCd loadROM(const std::string& filename); // Loads ROM into memory
	void EmulateCycle();
	void UpdateTimers();

	//Input Handling
	bool IsKeyDown(uint8_t key) const;
	void SetKeyDown(uint8_t key);
	void SetKeyUp(uint8_t key);
	

	//Chip8 Functions
	void disp_clear();
	void Op_00EE();
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
	void Op_8XY4(); // Add Vy to Vx
	void Op_8XY5(); // Subtract Vy from Vx
	void Op_8XY6(); // Shift Vx to the right by 1
	void Op_8XY7(); // Subtract Vx from Vy and store the result inside Vx
	void Op_8XYE(); // Shift Vx to the left by 1
	void Op_9XY0(); // Skips the next instruction if VX does not equal VY
	void Op_ANNN(); // Sets I to the given address
	void Op_BNNN(); // Set Address To given address + V0;
	void Op_CXNN();
	void Op_DXYN();
	void Op_EX9E();
	void Op_EXA1();
	void Op_FX07();
	void Op_FX0A();
	void Op_FX15();
	void Op_FX18();
	void Op_FX1E();
	void Op_FX29();
	void Op_FX55();
	void Op_FX65();
};

