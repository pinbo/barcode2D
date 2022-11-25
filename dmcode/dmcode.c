// IEC16022 bar code generation
// Copyright (c) 2007 Adrian Kennard Andrews & Arnold Ltd
// This software is provided under the terms of the GPL v2 or later.
// This software is provided free of charge with a full "Money back" guarantee.
// Use entirely at your own risk. We accept no liability. If you don't like that - don't use it.

// TBA, structured append, and ECI options.
// TBA, library for odd size ECC000-140 codes

// Junli Zhang 2022
// This version  uses C codes from https://github.com/rdoeffinger/iec16022
// but added square argument to make square barcodes

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include "iec16022ecc200.h"

// hex dump - bottom left pixel first
static void dumphex(const unsigned char *grid, int W, int H, unsigned char p)
{
	int c = 0, y;
	for (y = 0; y < H; y++) {
		int v = 0, x, b = 128;
		for (x = 0; x < W; x++) {
			if (grid[y * W + x])
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
	printf("Usage: ./dmcode -c [text] -q > test.eps\n");
   printf("dmcode generates an EPS file of DataMatrix codes from the text input");
	printf("  -c: text to generate barcodes\n");
	printf("  -q: Integar, whether to generate only square data matrix codes\n");
   printf("  -g: Integar, whether to generate GS1 data matrix codes\n");
   printf("  -t: Integar, whether to print a text ouptut on screen (stderr)\n");
	exit (8);
}

int main(int argc, char *argv[])
{
   int W = 0, H = 0;
   int square = 0;
   int gs1 = 0; // IEC16022_FLAG_GS1
   int text = 0; // whether print data matrix code on screen. You can replace them with 0 and 1 for other uses. print on stderr.
   char *barcode = NULL;
   int option;
   while((option = getopt(argc, argv, "c:qgt")) != -1){ //get option from the getopt() method
      switch(option){
         case 'c':
            barcode = optarg;
            break;
         case 'q':
            square = 1;
            break;
         case 'g':
            gs1 = 1;
            break;
         case 't':
            text = 1;
            break;
         case '?':
            if (optopt == 'c')
               fprintf (stderr, "Option -%c requires an argument.\n", optopt);
            else if (isprint (optopt))
               fprintf (stderr, "Unknown option `-%c'.\n", optopt);
            else
               fprintf (stderr,"Unknown option character `\\x%x'.\n",optopt);
            // return 1;
            usage();
         default:
            usage();
      }
   }
   if (!barcode)
      usage();
   // int flags = 0;
   // flags |= gs1 ? IEC16022_FLAG_GS1 : 0;
   int barcodelen = 0;
   char *encoding = NULL;
   // char *barcode = argv[1]; // input text
   int len = 0,
       maxlen = 0,
       ecclen = 0;
   unsigned char *grid = 0;
   barcodelen = strlen(barcode);
   // make grid
   //  grid = iec16022ecc200(&W, &H, &encoding, barcodelen, ( char *) barcode, &len, &maxlen, &ecclen, square: square, noquiet:noquiet);
    grid = iec16022ecc200f(&W, &H, &encoding, barcodelen, (const unsigned char *) barcode, &len, &maxlen, &ecclen, gs1, square);
   // output
   if (!grid || !W)
      fprintf(stderr,"No barcode produced\n");
   
   fprintf(stderr,"Size    : %dx%d\n", W, H);
   fprintf(stderr,"Encoded : %d of %d bytes with %d bytes of ecc\n", len, maxlen, ecclen);
   fprintf(stderr,"Barcode : %s\n", barcode);
   fprintf(stderr,"Encoding: %s\n\n", encoding ? encoding : "");

   // Sanitize bar code from all control characters as it is not
	// only used for comment-like usage.
	// Avoids issues like broken EPS files but also PNG comments
	// some programs might have issues with.
   for (char *c = barcode; *c; c++) {
		if (*c < 32 || *c == 127) *c = ' ';
	}
   // write to text on stderr
   if (text) {
      for (int i = 0; i < W+2; i++)
         fprintf(stderr, "█"); // padding 1 row of quiet zone
       fprintf(stderr,"\n");
      int y;
      for (y = H - 1; y >= 0; y--) {
         fprintf(stderr, "█");
         int x;
         for (x = 0; x < W; x++)
            fprintf(stderr,"%s", grid[W * y + x] ? " " : "█");
         fprintf(stderr, "█");
         fprintf(stderr,"\n");
      }
      for (int i = 0; i < W+2; i++)
         fprintf(stderr, "█"); // padding 1 row of quiet zone
      fprintf(stderr,"\n");
   }
   // write to stdout for EPS output
   printf("%%!PS-Adobe-3.0 EPSF-3.0\n"
         "%%%%Creator: IEC16022 barcode/stamp generator\n"
         "%%%%BarcodeData: %s\n" "%%%%BarcodeSize: %dx%d\n"
         "%%%%BarcodeFormat: ECC200\n"
         "%%%%DocumentData: Clean7Bit\n" "%%%%LanguageLevel: 1\n"
         "%%%%Pages: 1\n" "%%%%BoundingBox: 0 0 %d %d\n"
         "%%%%EndComments\n" "%%%%Page: 1 1\n"
         "%d %d 1[1 0 0 1 -1 -1]{<\n", barcode, W, H, W + 2,
         H + 2, W, H);
   dumphex(grid, W, H, 0xFF);
   printf(">}image\n");

   free(grid);
   free(encoding);

   return 0;
}
