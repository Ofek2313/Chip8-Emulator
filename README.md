# CHIP-8 Emulator

A CHIP-8 emulator written in C++ using SDL3 for rendering and input handling.

## About

CHIP-8 is an interpreted programming language from the 1970s, originally designed for 8-bit microcomputers. This project implements a full CHIP-8 interpreter capable of running classic ROMs such as Pong, Tetris, and Space Invaders.

## Features

- Full CHIP-8 instruction set (35 opcodes)
- 64×32 pixel display rendered at 640×320 (10× scale)
- Hexadecimal keypad input mapped to a QWERTY keyboard
- Delay and sound timers running at 60 Hz
- ~20 CPU cycles emulated per frame

## Requirements

- Windows
- Visual Studio 2022 (or later)
- SDL3 (included in the `SDL/` directory)

## Building

1. Clone the repository:
   ```
   git clone https://github.com/Ofek2313/Chip8-Emulator.git
   ```
2. Open `Emulator8.sln` in Visual Studio.
3. Set the build configuration to **Release** or **Debug**.
4. Press **Ctrl+Shift+B** to build.

SDL3 is bundled in the `SDL/` folder and is already linked in the project — no additional setup is required.

## Usage

Run the emulator from the command line, passing a CHIP-8 ROM as an argument:

```
Emulator8.exe path\to\rom.ch8
```

## Keypad Mapping

The original CHIP-8 hexadecimal keypad is mapped to the following keyboard layout:

| CHIP-8 | `1` | `2` | `3` | `C` |
|--------|-----|-----|-----|-----|
| **Keyboard** | `1` | `2` | `3` | `4` |

| CHIP-8 | `4` | `5` | `6` | `D` |
|--------|-----|-----|-----|-----|
| **Keyboard** | `Q` | `W` | `E` | `R` |

| CHIP-8 | `7` | `8` | `9` | `E` |
|--------|-----|-----|-----|-----|
| **Keyboard** | `A` | `S` | `D` | `F` |

| CHIP-8 | `A` | `0` | `B` | `F` |
|--------|-----|-----|-----|-----|
| **Keyboard** | `Z` | `X` | `C` | `V` |

## Project Structure

```
Chip8-Emulator/
├── Emulator8/
│   ├── main.cpp       # Entry point and SDL rendering loop
│   ├── Chip8.h        # Chip8 class declaration
│   └── Chip8.cpp      # Opcode implementations, timers, input
├── SDL/               # Bundled SDL3 library
└── Emulator8.sln      # Visual Studio solution file
```

## Supported Opcodes

| Opcode | Description |
|--------|-------------|
| `00E0` | Clear the display |
| `00EE` | Return from subroutine |
| `1NNN` | Jump to address NNN |
| `2NNN` | Call subroutine at NNN |
| `3XNN` | Skip next instruction if Vx == NN |
| `4XNN` | Skip next instruction if Vx != NN |
| `5XY0` | Skip next instruction if Vx == Vy |
| `6XNN` | Set Vx = NN |
| `7XNN` | Set Vx = Vx + NN |
| `8XY0` | Set Vx = Vy |
| `8XY1` | Set Vx = Vx OR Vy |
| `8XY2` | Set Vx = Vx AND Vy |
| `8XY3` | Set Vx = Vx XOR Vy |
| `8XY4` | Set Vx = Vx + Vy, VF = carry |
| `8XY5` | Set Vx = Vx - Vy, VF = not borrow |
| `8XY6` | Shift Vx right by 1, VF = LSB |
| `8XY7` | Set Vx = Vy - Vx, VF = not borrow |
| `8XYE` | Shift Vx left by 1, VF = MSB |
| `9XY0` | Skip next instruction if Vx != Vy |
| `ANNN` | Set I = NNN |
| `BNNN` | Jump to NNN + V0 |
| `CXNN` | Set Vx = random byte AND NN |
| `DXYN` | Draw N-byte sprite at (Vx, Vy), VF = collision |
| `EX9E` | Skip next instruction if key Vx is pressed |
| `EXA1` | Skip next instruction if key Vx is not pressed |
| `FX07` | Set Vx = delay timer |
| `FX0A` | Wait for key press, store in Vx |
| `FX15` | Set delay timer = Vx |
| `FX18` | Set sound timer = Vx |
| `FX1E` | Set I = I + Vx |
| `FX29` | Set I = font sprite address for digit Vx |
| `FX55` | Store V0–Vx in memory at I |
| `FX65` | Read V0–Vx from memory at I |
