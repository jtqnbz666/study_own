elf:main.o add.o sub.o 
	g++ *.o -o $@ -I.
%.o:%.c 
	g++ -c $< -o $@
clean:
	rm *.o elf

