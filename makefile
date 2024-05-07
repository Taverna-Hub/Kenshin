all:
	gcc main.c ./src/*.c -I include -o kenshin

run:
	./kenshin

clean:
	rm kenshin