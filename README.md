# barcode2D
Generate QR or Data Matrix codes.

The C codes are from [QR-Code-generator](https://github.com/nayuki/QR-Code-generator) and [iec16022](https://github.com/rdoeffinger/iec16022). I simplilied the codes to only print on screen or create an EPS file of the 2D barcodes.

To create labels on Avery Label sheets, please use R package [barcodeLabel](https://github.com/pinbo/barcodeLabel) or the Shiny App: https://junli.shinyapps.io/label_barcodes

## Compile
```sh
# build QR code generator
cd qrcode
make
# build Data Matrix code generator
cd dmcode
make
```
## Usage
```sh
# see Help
./qrcode
# or
./dmcode

# QR codes
./qrcode -c "Hello world!" > hello.eps

./qrcode -c 'abcdefgggggggggggggghijkLMNIOPQSTRSUVMWWMMMMMMMMMMMMMMMWWWWLMMMMMMMMMMMMMOOOOOOOOOOOOOQQQQQQ!12233$556@#$%^&*()_+-=;:"[]{}/?<>|\\\\///~`````AAAAAAABBmmmmmmmmmmmmmmmMMMMMMMMMMMMMMMMM11111111111111111' > long.eps
# Data Matrix code
./dmcode -c "Hello world!" > hello.eps

./dmcode -c 'abcdefgggggggggggggghijkLMNIOPQSTRSUVMWWMMMMMMMMMMMMMMMWWWWLMMMMMMMMMMMMMOOOOOOOOOOOOOQQQQQQ!12233$556@#$%^&*()_+-=;:"[]{}/?<>|\\\\///~`````AAAAAAABBmmmmmmmmmmmmmmmMMMMMMMMMMMMMMMMM11111111111111111' > long.eps

```

## Acknowledgements
Special thanks to the authors of following packages and repositories.
1. [QR-Code-generator](https://github.com/nayuki/QR-Code-generator): I get the codes for the C version qrcode function.
1. [iec16022](https://github.com/rdoeffinger/iec16022): get the C codes for making Data Matrix barcodes.
1. [Datamatrix](https://github.com/revk/Datamatrix): get the idea how to make square Data Matrix.