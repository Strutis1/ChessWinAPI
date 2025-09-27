make:
	windres Resources/main.rc Resources/res.o
	g++ -c src/main.cpp -o src/main.o
	g++ -c src/utilz.cpp -o src/utilz.o
	g++ src/main.o src/utilz.o Resources/res.o -o chess.exe -mwindows -municode -lgdi32 -lgdiplus -lole32
