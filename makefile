all:
	gcc main.c ./src/*.c -I include -o kenshin.out

run:
	./kenshin.out

clean:
	rm kenshin.out