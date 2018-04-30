all: haifu

haifu: Haifu.exe

clean:
	del Haifu.exe
	del *.o

run: Haifu.exe
	Haifu.exe

Haifu.exe: main.cpp WordData.o SyllableParser.o TokenGenerator.o ProgramExecutor.o funcs.o elements.o
	g++ -o Haifu.exe -DUSE_G_COMPILER main.cpp WordData.o SyllableParser.o TokenGenerator.o ProgramExecutor.o funcs.o elements.o

WordData.o: WordData.h WordData.cpp funcs.o elements.o
	g++ -DUSE_G_COMPILER -c WordData.cpp

SyllableParser.o: SyllableParser.h SyllableParser.cpp WordData.o funcs.o
	g++ -DUSE_G_COMPILER -c SyllableParser.cpp

TokenGenerator.o: TokenGenerator.h TokenGenerator.cpp WordData.o funcs.o elements.o
	g++ -DUSE_G_COMPILER -c TokenGenerator.cpp

ProgramExecutor.o: ProgramExecutor.h ProgramExecutor.cpp TokenGenerator.o elements.o
	g++ -DUSE_G_COMPILER -c ProgramExecutor.cpp

funcs.o: funcs.h funcs.cpp
	g++ -DUSE_G_COMPILER -c funcs.cpp

elements.o: elements.h elements.cpp
	g++ -DUSE_G_COMPILER -c elements.cpp