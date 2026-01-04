#include "Chip8.h"
#include <fstream>

Chip8::Chip8()
{
	memset(registers, 0, sizeof(registers)); //Set all registers as empty
	memset(keys, 0, sizeof(keys)); //Set all keys down
	memset(display, 0, sizeof(display)); //Set display as blank
	PC = 0x200; // Program Counter Starts at 0x200
	I = 0x000;
}

//ROM Loading
RomErrCd Chip8::loadROM(const std::string& filename)
{
	std::ifstream file(filename, std::ios::binary | std::ios::ate);
	if (!file.is_open())
		return RomErrCd::FailedToLoad;

	std::streamsize size = file.tellg();
	if (size > 4096 - 0x200)
		return RomErrCd::FileSize;
	file.seekg(0, std::ios::beg);

	if (!file.read((char*)&memory[0x200], size))
		return RomErrCd::FailedToRead;

	file.close();

	return RomErrCd::None;
}

//Input Handling
void Chip8::SetKeyDown(uint8_t key)
{
	keys[key] = true;
}
void Chip8::SetKeyUp(uint8_t key)
{
	keys[key] = false;
}
bool Chip8::IsKeyDown(uint8_t key) const
{
	return keys[key];
}


//Op Codes
void Chip8::disp_clear()
{
	memset(display, 0, sizeof(display));	
	
}
void Chip8::Op_3XNN()
{
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	uint8_t byte = opcode & 0x00FF;
	if (registers[Vx] == byte)
		PC += 2;
}
void Chip8::Op_4XNN()
{
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	uint8_t byte = opcode & 0x00FF;
	if (registers[Vx] != byte)
		PC += 2;
}
void Chip8::Op_5XY0()
{
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	uint8_t Vy = (opcode & 0x00F0) >> 4;
	if (registers[Vx] == registers[Vy])
		PC += 2;
}
void Chip8::Op_6XNN()
{
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	uint8_t byte = opcode & 0x00FF;
	registers[Vx] = byte;
}
void Chip8::Op_7XNN()
{
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	uint8_t byte = opcode & 0x00FF;
	registers[Vx] += byte;
}
void Chip8::Op_8XY0()
{
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	uint8_t Vy = (opcode & 0x00F0) >> 4;
	registers[Vx] = registers[Vy];
}
void Chip8::Op_8XY1()
{
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	uint8_t Vy = (opcode & 0x00F0) >> 4;
	registers[Vx] |= registers[Vy];
}

void Chip8::Op_8XY2()
{
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	uint8_t Vy = (opcode & 0x00F0) >> 4;
	registers[Vx] &= registers[Vy];
}
void Chip8::Op_8XY3()
{
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	uint8_t Vy = (opcode & 0x00F0) >> 4;
	registers[Vx] ^= registers[Vy];
}
void Chip8::Op_8XY4() {
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	uint8_t Vy = (opcode & 0x00F0) >> 4;
	registers[Vx] += registers[Vy];
}
void Chip8::Op_8XY5() {
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	uint8_t Vy = (opcode & 0x00F0) >> 4;
	registers[Vx] -= registers[Vy];
}
void Chip8::Op_8XY6()
{
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	registers[Vx] >>= 1;
}
void Chip8::Op_8XY7()
{
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	uint8_t Vy = (opcode & 0x00F0) >> 4;
	registers[Vx] = registers[Vy]-registers[Vx];
}
void Chip8::Op_8XYE()
{
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	registers[Vx] <<= 1;
}
void Chip8::Op_9XY0()
{
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	uint8_t Vy = (opcode & 0x00F0) >> 4;
	if (registers[Vx] != registers[Vy])
		PC += 2;
}
void Chip8::Op_ANNN()
{
	uint16_t address = opcode & 0x0FFF;
	I = address;
}
void Chip8::Op_BNNN()
{
	uint16_t address = opcode & 0x0FFF;
	I = address + registers[0];
}
void Chip8::Op_EX9E()
{
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	if (keys[registers[Vx]])
		PC += 2;

}
void Chip8::Op_EXA1()
{
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	if (!keys[registers[Vx]])
		PC += 2;
}
void Chip8::Op_FX07()
{
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	registers[Vx] = delayTimer;

}