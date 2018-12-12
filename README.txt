UTFC
====

Universal Tibetan Font Converter


## Synopsis

The Universal Tibetan Font Converter was created by Tashi Tsering with funding from Trace Foundation. The objective of the development is to provide the community a tool for converting files between different Tibetan encoding schemes with different file formats. The converter also can translate Tibetan script with different encodings to variant Wylie transliterations, and vice versa.

The converter covers 17 different Tibetan encoding schemes including 4 Tibetan transliteration schemes (ACIP, ALA-LC, THL Wylie, Wylie), Tibetan Unicode, and other 12 legacy Tibetan fonts including:

Sambhota 1.0 (Sama)
Sambhota 2.0 (Dedris)
Bandrida
Tongyuan
Beida Founder
Huanguang
LTibetan
Jamyang
TCRC Bod-Yig
Tibetan Machine
Tibetan Machine Web 
National Standard Extended

Their name used in the command are:

OldSambhota
NewSambhota
Bzd
Ty
Fz
Hg
LTibetan
Jamyang
TCRCBodYig
TM
TMW
NS

Also include others

ACIP
Wylie
THDLWylie
LCWylie

The converter also covers four common file formats including txt, Unicode txt, rich text file format (RTF) and HTML web page. You can convert file between arbitrary two encoding schemes from 16 ones mentioned above with certain file formats.

The converter has some bugs and additional information on the file format supported can be found at http://www.trace.org/utfc. Any improvement of the application would be very much appreciated.

## Installation

The application is programmed in C, you can compile the application with the following:

gcc -o converter Converter_20181128.c

In order to run the application use the following sintax:

./converter INPUT_FILE OUTPUT_FILE INPUT_ENCODING OUTPUT_ENCODING INPUT_FORMT OUTPUT_FORMAT

for example:

./converter file.rtf file_converted.rtf Unicode Wylie rtf rtf

For converting to Unicode:

By default, Unicode will use "Microsoft Himalaya" font in both Unix and Windows.

If prefer other Unicode, such as "SambhotaUnicode" and convert from rtf to rtf, the conversion 
command will be: 

./converter file.rtf file_converted.rtf NewSambhota SambhotaUnicode rtf rtf

If not from rtf to rtf,
you can just use "Unicode" and edit the target file to change "Microsoft Himalaya" to the the 
Unicode font you prefer, such as "SambhotaUnicode"

## Additional notes

You can find an online version of the application at http://www.trace.org/utfc

You can download a standalone version from: http://www.yalasoo.com/English/docs/yalasoo_en_utfc.html

## Contributors

Author: Tashi Tsering
Assistant: Nyima Droma
Supported by: Trace Foundation

## License

The application has been licensed GPL v3, you can find the full license in the LICENSE.txt file.
