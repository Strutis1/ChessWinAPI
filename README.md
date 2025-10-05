# ♟️ ChessWinAPI

A Windows API–based chess application built in modern C++ (C++20) using GDI+ for graphics.  
This project supports both **native Windows builds** and **cross-compilation from Linux** using MinGW.

---

## 🧰 Requirements

### 🪟 On Windows
Make sure you have:

- **[MSYS2](https://www.msys2.org/)** (recommended)
- Inside MSYS2, install the required packages:

  ```bash
  pacman -S --needed mingw-w64-x86_64-gcc mingw-w64-x86_64-binutils make

    Optional (for code editing):

        Visual Studio Code + C/C++ extension

    After installation, open MSYS2 MinGW64 shell — not the default MSYS shell.

🐧 On Linux (cross-compile for Windows)

Install MinGW and Wine:

sudo pacman -S mingw-w64-gcc make wine

On Debian/Ubuntu-based systems:

sudo apt install mingw-w64 make wine

⚙️ Build Instructions
🪟 On Windows

In the MSYS2 MinGW64 shell:

make

This will generate:

Chess.exe

Run it directly:

./Chess.exe

🐧 On Linux

In your terminal:

make

Then run the Windows executable using Wine:

wine Chess.exe

🧹 Cleaning the Build

To remove all compiled objects and the executable:

make clean

🪶 Notes

    The Makefile automatically detects the OS and uses the correct compiler:

        Windows: g++ (MSYS2 MinGW)

        Linux: x86_64-w64-mingw32-g++

    GDI+ and Windows headers are included automatically via MinGW.

    The build produces a standalone Chess.exe on Linux (linked statically).

    Works in both VS Code and terminal-only environments.

🧩 Folder Structure

ChessWinAPI/
│
├── src/
│   ├── main.cpp
│   └── utilz.cpp
│
├── Resources/
│   ├── main.rc
│   └── resource.h
│
├── makefile
└── README.md

💡 Tips

    If you get a wine or DLL missing error on Linux:

    sudo pacman -S wine

    or install via your distro’s package manager.

    On Windows, ensure you’re compiling inside the MSYS2 MinGW64 shell (g++ --version should mention MinGW).

✅ Example Commands Recap
Command	Purpose
make	Build the Windows executable
make clean	Remove compiled files
wine Chess.exe	Run on Linux
./Chess.exe	Run on Windows

Author: wind
License: MIT