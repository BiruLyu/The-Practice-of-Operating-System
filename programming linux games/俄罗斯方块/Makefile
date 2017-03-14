test: main.o block.o
	gcc -o test main.o block.o
%.c:%.o
	gcc -c $@ $<
.PHONY:clean
clean:
	rm -rf main.o block.o test
