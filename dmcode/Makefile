CC = gcc
CFLAGS = -g -O -Wall

all: dmcode

dmcode: dmcode.c reedsol.c iec16022ecc200.c
	$(CC) $(CFLAGS) -o $@ dmcode.c reedsol.c iec16022ecc200.c

# Delete build output
clean:
	rm dmcode
