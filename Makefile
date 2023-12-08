all: write

write: write.c
	gcc write.c -o write

clean:
	rm -f *.txt write *.o