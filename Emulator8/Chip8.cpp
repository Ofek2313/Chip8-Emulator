#include "Chip8.h"
#include <fstream>

Chip8::Chip8()
{
	memset(registers, 0, sizeof(registers)); //Set all registers as empty
	memset(keys, 0, sizeof(keys)); //Set all keys down
	memset(display, 0, sizeof(display)); //Set display as blank
	PC = 0x200; // Program Counter Starts at 0x200
	I = 0x000;
	delayTimer = 60;
	soundTimer = 60;
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
void Chip8::UpdateTimers()
{
	if (delayTimer > 0)
		delayTimer--;
	if (soundTimer > 0)
		soundTimer--;
}
void Chip8::EmulateCycle()
{
	
	opcode = memory[PC] << 8 | memory[PC + 1];
	PC += 2;

	switch (opcode & 0xF000 )
	{
		case 0x0000:
			switch (opcode & 0x00FF)
			{
				case 0x00E0:
					disp_clear();
					break;
				case 0x00EE:
					Op_00EE();
					break;
				default:
					break;
			}
			break;
		case 0x1000:
			Op_1NNN();
			break;
		case 0x2000:
			Op_2NNN();
			break;
		case 0x3000:
			Op_3XNN();
			break;
		case 0x4000:
			Op_4XNN();
			break;
		case 0x5000:
			Op_5XY0();
			break;
		case 0x6000:
			Op_6XNN();
			break;
		case 0x7000:
			Op_7XNN();
			break;
		case 0x8000:
			switch (opcode & 0xF00F)
			{
				case 0x8000:
					Op_8XY0();
					break;
				case 0x8001:
					Op_8XY1();
					break;
				case 0x8002:
					Op_8XY2();
					break;
				case 0x8003:
					Op_8XY3();
					break;
				case 0x8004:
					Op_8XY4();
					break;
				case 0x8005:
					Op_8XY5();
					break;
				case 0x8006:
					Op_8XY6();
					break;
				case 0x8007:
					Op_8XY7();
					break;
				case 0x800E:
					Op_8XYE();
					break;
				default:
					break;
			}
			
			break;
		case 0x9000:
			Op_9XY0();
			break;
		case 0xA000:
			Op_ANNN();
			break;
		case 0xB000:
			Op_BNNN();
			break;
		case 0xC000:
			Op_CXNN();
			break;
		case 0xD000:
			Op_DXYN();
			break;
		case 0xE000:
			switch (opcode & 0x00FF)
			{
				case 0x009E:
					Op_EX9E();
					break;
				case 0x00A1:
					Op_EXA1();
					break;
				default:
					break;
			}
			break;
		case  0xF000:
			switch (opcode & 0x00FF)
			{
				case 0x0007:
					Op_FX07();
					break;
				case 0x000A:
					Op_FX0A();
					break;
				case 0x0015:
					Op_FX15();
					break;
				case 0x0018:
					Op_FX18();
					break;
				case 0x001E:
					Op_FX1E();
					break;
				case 0x0029:
					Op_FX29();
					break;
				/*case 0x0033:
					Op_FX33();
					break;*/
				case 0x0055:
					Op_FX55();
					break;
				case 0x0065:
					Op_FX65();
					break;
				default:
					break;
			}
			break;
		default:
			break;
	}
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
void Chip8::Op_00EE()
{
	SP -= 1;
	PC = stack[SP];
}
void Chip8::Op_1NNN()
{
	uint16_t NNN = opcode & 0x0FFF;
	PC = NNN;
}

void Chip8::Op_2NNN()
{
	uint16_t NNN = opcode & 0x0FFF;
	stack[SP] = PC;
	SP+=1;
	PC = NNN;
	
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
	uint16_t sum = registers[Vx] + registers[Vy];
	if (sum > 0xFF)
		registers[0xF] = 1;
	else
		registers[0xF] = 0;
	registers[Vx] = sum & 0xFF;
}
void Chip8::Op_8XY5() {
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	uint8_t Vy = (opcode & 0x00F0) >> 4;
	
	if (Vy > Vx)
		registers[0xF] = 0;
	else
		registers[0xF] = 1;
	registers[Vx] -= registers[Vy];
}
void Chip8::Op_8XY6()
{
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	uint8_t LSB = (registers[Vx] & 0x1);
	registers[0xF] = LSB;
	registers[Vx] >>= 1;
}
void Chip8::Op_8XY7()
{
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	uint8_t Vy = (opcode & 0x00F0) >> 4;
	if (Vx > Vy)
		registers[0xF] = 0;
	else
		registers[0xF] = 1;

	registers[Vx] = registers[Vy]-registers[Vx];
}
void Chip8::Op_8XYE()
{
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	uint8_t MSB = (registers[Vx] & 0x80) >> 7;
	registers[0xF] = MSB;
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
	PC = address + registers[0];
}
void Chip8::Op_CXNN()
{
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	uint8_t NN = opcode & 0x00FF;
	registers[Vx] = (rand() & 0xFF) & NN;
}
void Chip8::Op_DXYN()
{
	uint8_t X = (opcode & 0x0F00) >> 8;
	uint8_t Y= (opcode & 0x00F0) >> 4;
	uint8_t N = opcode & 0x000F;
	uint8_t Vx = registers[X];
	uint8_t Vy = registers[Y];
	for (int i = 0; i < N; i++)
	{
		uint8_t row = memory[I + i];
		for (int j = 0; j < 8; j++)
		{
			uint8_t bit = (row & (1 << (7-j))) >> (7-j);
			if (display[(Vy + i)%64][(Vx + j)%32] == bit && bit == 1)
				registers[0xF] = 0;
			display[(Vy + i) % 32][(Vx + j) % 64] ^= bit;
		}
	}
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
void Chip8::Op_FX0A() {
	uint8_t X = (opcode & 0x0F00) >> 8;
	for (int i = 0; i < 16; i++)
	{
		if (keys[i] == 1)
		{
			registers[X] = i;
			return;
		}
			
	}
	PC -= 2;
}
void Chip8::Op_FX15()
{
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	delayTimer = registers[Vx];
}
void Chip8::Op_FX18()
{
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	soundTimer = registers[Vx];
}
void Chip8::Op_FX1E()
{
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	I += registers[Vx];
}
void Chip8::Op_FX29()
{
	uint8_t X = (opcode & 0x0F00) >> 8;
	I = memory[registers[X]];
}
void Chip8::Op_FX55()
{
	uint8_t X = (opcode & 0x0F00) >> 8;
	for (int i = 0; i <= X; i++)
	{
		memory[I + i] = registers[i];
	}
}
void Chip8::Op_FX65()
{
	uint8_t X = (opcode & 0x0F00) >> 8;
	for (int i = 0; i <= X; i++)
	{
		registers[i] = memory[I + i];
	}
}