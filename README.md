# ChessWinAPI

Windows API chess app in C++20 using raw Win32 + GDI+. Features a simple UI with main menu, difficulty selection, **color selection (play as White or Black)**, save/load to JSON, and a pluggable AI that lives in a DLL.

## What’s here
- Win32 window + double-buffered GDI+ rendering of board/pieces.
- Screens: Main Menu → Difficulty → Color Select → Game → Game Over.
- Player color picker flips board orientation and turn handling.
- Save/resume unfinished games (`savegame.json`) with persisted difficulty and chosen color.
- Basic AI bot invoked through `sillyBot.dll`.

## Implementation notes
- UI: raw Win32 messages with custom buttons drawn via GDI+; back-buffer to reduce flicker.
- Board rendering: coordinates flip when playing as Black (display ↔ board transforms).
- Input: only the player’s turn accepts clicks; AI moves scheduled via a custom window message.
- Persistence: JSON-like save (no external lib) storing board, turn, difficulty, and player color.
- AI: DLL exposes `chooseMoveFromList`; `Chess.exe` loads it at startup and delegates move choice.


## DLL / AI note
- The chess AI is shipped as `sillyBot.dll` in this repo for convenience.
- The bot source lives in my separate ChessBot repo; update/rebuild the DLL there if you want to change AI behavior.
- `Chess.exe` loads the DLL at startup and calls `chooseMoveFromList`.

## Requirements
### Windows (MSYS2 recommended)
- Install MSYS2, then in the **MinGW64** shell:
  ```bash
  pacman -S --needed mingw-w64-x86_64-gcc mingw-w64-x86_64-binutils make
  ```
  Optional: VS Code + C/C++ extension.

### Linux (cross-compile for Windows)
- Install MinGW and Wine:
  ```bash
  sudo pacman -S mingw-w64-gcc make wine
  # or on Debian/Ubuntu:
  sudo apt install mingw-w64 make wine
  ```

## Build
### Windows
```bash
make
./Chess.exe
```

### Linux
```bash
make
wine Chess.exe
```

## Clean
```bash
make clean
```

## Notes
- Makefile picks the right compiler automatically (g++ on Windows, x86_64-w64-mingw32-g++ on Linux).
- GDI+ and Win32 headers are provided via MinGW packages.
- If Wine or a DLL is missing on Linux, install via your package manager.

## TODO / roadmap
- Rules: en passant, castling, and a proper promotion chooser (likely modal or inline buttons); add per-side timers.
- AI: more randomness/strength options, and additional bots selectable from the difficulty screen.
- UX: Settings screen (audio, themes, controls), richer visuals, and better bot-selection UX (show names/descriptions).
- Polish: guard saves when a game ends, add tests for move legality, and surface error prompts when the DLL is missing.

## Folder structure
```
ChessWinAPI/
├── src/
├── Resources/
├── Include/
├── ChessBot/        # AI DLL source lives elsewhere; DLL binary is included here
├── makefile
└── README.md
```
