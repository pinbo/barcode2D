CC = gcc
CFLAGS = -g -O -Wall

all: qrcode

qrcode: qrcode.c qrcodegen.c
	$(CC) $(CFLAGS) -o $@ qrcode.c qrcodegen.c

# Delete build output
clean:
	rm qrcode
