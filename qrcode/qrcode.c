/* 
 * QR Code generator demo (C)
 * 
 * Run this command-line program with no arguments. The program
 * computes a demonstration QR Codes and print it to the console.
 * 
 * Copyright (c) Project Nayuki. (MIT License)
 * https://www.nayuki.io/page/qr-code-generator-library
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 * - The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 * - The Software is provided "as is", without warranty of any kind, express or
 *   implied, including but not limited to the warranties of merchantability,
 *   fitness for a particular purpose and noninfringement. In no event shall the
 *   authors or copyright holders be liable for any claim, damages or other
 *   liability, whether in an action of contract, tort or otherwise, arising from,
 *   out of or in connection with the Software or the use or other dealings in the
 *   Software.
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "qrcodegen.h"

// hex dump - bottom left pixel first
static void dumphex(const uint8_t qrcode[], unsigned char p)
{
    int c = 0, y;
    int size = qrcodegen_getSize(qrcode);
    for (y = size-1; y >= 0; y--) {
        int v = 0, x, b = 128;
        for (x = 0; x < size; x++) {
            if ( qrcodegen_getModule(qrcode, x, y) )
                v |= b;
            b >>= 1;
            if (!b) {
                printf("%02X", v ^ p);
                v = 0;
                b = 128;
                c++;
            }
        }
        if (b != 128) {
            printf("%02X", v ^ p);
            c++;
        }
        printf(" ");
        c++;
        if (c >= 80) {
            printf("\n");
            c = 0;
        }
    }
    if (c)
        printf("\n");
}

void usage(void)
{
    printf("Usage: ./qrcode -c [text] -e [errorCorrrectionLevel] > test.eps\n");
    printf("qrcode generates an EPS file of QR codes from the text input\n");
    printf("  -c: text to generate barcodes\n");
    printf("  -t: Integar, whether to print a text ouptut on screen (stderr)\n");
    printf("  -e: Integar (1-4), Error correction level for QR codes (default 2).\n");
    printf("        1 = Level L - up to 7%% damage\n");
    printf("        2 = Level M - up to 15%% damage\n");
    printf("        3 = Level Q - up to 25%% damage\n");
    printf("        4 = Level H - up to 30%% damage\n");
    exit (8);
}

// Prints the given QR Code to the stderr.
static void printQr(const uint8_t qrcode[]) {
    int size = qrcodegen_getSize(qrcode);
    int border = 1;
    for (int y = -border; y < size + border; y++) {
        for (int x = -border; x < size + border; x++) {
            fputs( (qrcodegen_getModule(qrcode, x, y) ? " " : "█"), stderr);
        }
        fputs("\n", stderr);
    }
    fputs("\n", stderr);
}

// Creates a single QR Code, return a matrix (actually a vector in the C code) of 0 and 1.
int main(int argc, char *argv[])
{
   int ecc = 2;
   int text = 0; // whether print data matrix code on screen. You can replace them with 0 and 1 for other uses. print on stderr.
   char *barcode = NULL;
   int option;
   while((option = getopt(argc, argv, "c:e:t")) != -1){ //get option from the getopt() method
      switch(option){
         case 'c':
            barcode = optarg;
            break;
         case 'e':
            ecc = atoi(optarg);
            break;
         case 't':
            text = 1;
            break;
         case '?':
            if (optopt == 'c' || optopt == 'e')
               fprintf (stderr, "Option -%c requires an argument.\n", optopt);
            else
               fprintf (stderr, "Unknown option `-%c'.\n", optopt);
            // return 1;
            usage();
         default:
            usage();
      }
   }
    if (!barcode)
        usage();
    fprintf(stderr, "Barcode : %s\n", barcode);
    enum qrcodegen_Ecc errCorLvl; // = qrcodegen_Ecc_LOW;  // Error correction level
    if (ecc == 1) errCorLvl = qrcodegen_Ecc_LOW;
    else if (ecc == 2) errCorLvl = qrcodegen_Ecc_MEDIUM;
    else if (ecc == 3) errCorLvl = qrcodegen_Ecc_QUARTILE;
    else if (ecc == 4) errCorLvl = qrcodegen_Ecc_HIGH;
    else {
        errCorLvl = qrcodegen_Ecc_LOW;
        fprintf(stderr, "Wrong number for error correction!\nPlease provide number 1 to 4.\nUse level 1.\n");
    }
    // Make and print the QR Code symbol
    uint8_t qrcode[qrcodegen_BUFFER_LEN_MAX];
    uint8_t tempBuffer[qrcodegen_BUFFER_LEN_MAX];
    bool ok = qrcodegen_encodeText(barcode, tempBuffer, qrcode, errCorLvl,
        qrcodegen_VERSION_MIN, qrcodegen_VERSION_MAX, qrcodegen_Mask_AUTO, true);
    if (ok) {
        int size = qrcodegen_getSize(qrcode);
        fprintf(stderr, "Size    : %d x %d\n", size, size);
        int border = 1;
        if (text) printQr(qrcode);
        // print EPS output to stdout
        printf("%%!PS-Adobe-3.0 EPSF-3.0\n"
            "%%%%Creator: IEC16022 barcode/stamp generator\n"
            "%%%%BarcodeData: %s\n" "%%%%BarcodeSize: %dx%d\n"
            "%%%%BarcodeFormat: ECC200\n"
            "%%%%DocumentData: Clean7Bit\n" "%%%%LanguageLevel: 1\n"
            "%%%%Pages: 1\n" "%%%%BoundingBox: 0 0 %d %d\n"
            "%%%%EndComments\n" "%%%%Page: 1 1\n"
            "%d %d 1[1 0 0 1 -1 -1]{<\n", barcode, size, size, size + 2*border,
            size + 2*border, size, size);
        dumphex(qrcode, 0xFF);
        printf(">}image\n");
        return 0;
    }
    return -1;
}
