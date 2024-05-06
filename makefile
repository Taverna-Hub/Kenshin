all:
	gcc main.c ./src/*.c -I include -o main.exe

run:
	./main.exe

clean:
	rm main.exe