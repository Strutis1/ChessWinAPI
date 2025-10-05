# --- Detect OS ---
ifeq ($(OS),Windows_NT)
    CC_WIN = g++
    RC_WIN = windres
    WIN_LIBS = -mwindows -municode -lgdi32 -lgdiplus -lole32
    RM = del /Q
else
    CC_WIN = x86_64-w64-mingw32-g++
    RC_WIN = x86_64-w64-mingw32-windres
    WIN_LIBS = -static -static-libgcc -static-libstdc++ -mwindows -municode -lgdi32 -lgdiplus -lole32
    RM = rm -f
endif

CFLAGS = -std=c++20 -O2

SRC = src/main.cpp src/utilz.cpp
OBJ = $(SRC:.cpp=.o)
RES = Resources/res.o

all: windows

windows:
	$(RC_WIN) Resources/main.rc -O coff -o $(RES)
	$(CC_WIN) $(CFLAGS) $(SRC) $(RES) -o Chess.exe $(WIN_LIBS)

clean:
	$(RM) $(OBJ) $(RES) Chess.exe
