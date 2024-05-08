all:
	gcc ./src/*.c -I include -o kenshin.out

run:
	./kenshin.out

clean:
	rm kenshin.out
