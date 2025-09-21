make:
	windres Resources/main.rc Resources/res.o
	g++ -c src/main.cpp -o src/main.o
	g++ src/main.o Resources/res.o -o chess.exe -mwindows -municode -lgdi32 -lgdiplus
