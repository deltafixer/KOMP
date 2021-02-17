run: build
	./compiler.exe

clean: 
	rm -f compiler.y.hpp compiler.y.cpp compiler.l.cpp compiler.exe

build: lex-compile parser-compile
	g++ -std=c++17 compiler.y.cpp compiler.l.cpp -lfl -o compiler.exe

lex-compile: parser-compile
	flex -o compiler.l.cpp compiler.l

parser-compile:
	bison -d compiler.y -o compiler.y.cpp