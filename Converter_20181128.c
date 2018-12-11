/**
 *  Universal Tibetan Font Converter  Version 2.0
 *
 *  Copyright 2003-2008 Trace Foundation
 *
 *  Licensed under GNU General Public License 3.0. 
 *  Author: Tashi Tsering
 * 	Research Assistant: Nyima Droma
 *  Supported by: Trace Foundation
 *
 * @license GPL-3.0+ <http://spdx.org/licenses/GPL-3.0+>
 */
 
//----------------------------------------------------
//----------include-----------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
//#include <process.h>  //For Windows, not for Unix
//#include <malloc.h>
#include <stdlib.h> //For Mac //20180302
#include <string.h>
//#include <wchar.h>  //Only for Unix    //20090325


//-------------OS--------------------------------------
//#define Windows	//This is for Windows system!!!!!
//define Mac	//This is for Macintosh system!!!!!
#define Unix	//This is for Unix or Linux system!!!!!
//#define OS390	//This is for OS/390 system!!!!!

//--------------Language Edition-----------------------
//#define CHINESE //This is for Chinese Edition
#define ENGLISH //This is for English Edition

//--------------Other Defines--------------------------
#define Yes 1
#define No 0

#define Latin 100
#define Tibetan 101
#define Chinese 102
#define Sanskrit 103

//------------------------------------------------------
#define Unicode 0		//ID--0
#define TMW 1			//ID--1
#define TM 2			//ID--2
#define Fz 3			//ID--3
#define Hg 4			//ID--4
#define ACIP 5			//ID--5
#define Wylie 6			//ID--6
#define LTibetan 7		//ID--7
#define OldSambhota 8	//ID--8
#define NewSambhota 9	//ID--9
#define THDLWylie 10	//ID--10
#define LCWylie 11		//ID--11
#define TCRCBodYig	12	//ID--12
#define Bzd 13			//ID--13 //2007
#define Ty 14			//ID--14
#define NS 15			//ID--15
#define Jamyang 16			//ID--16  //20080924
//--------------------------------------------
#define TXT 0
#define RTF 1
#define UnicodeTXT 2
#define WebPage 3

#define UnicodeToOthers 0
#define OthersToUnicode 1
#define EncodingToEncoding 2
#define OthersToOthers 3

//-----------------------------------------------------
char m_UTFCdirectory[1000]; //The directory of UTFC.exe
int TBracketLeft = No;  //add bracket for Tibetan so that it can be lower by using \dn
//Unicode to others Mapping Table File Names
char *Uni2othersMTFNames[] =
{
"Uni2Uni.tbl",
"Uni2TMW.tbl",
"Uni2TM.tbl",
"Uni2Fz.tbl",
"Uni2Hg.tbl",
"Uni2ACIP.tbl",
"Uni2Wylie.tbl",
"Uni2LTibetan.tbl",
"Uni2OldSambhota.tbl",
"Uni2NewSambhota.tbl",
"Uni2THDLWylie.tbl",
"Uni2LCWylie.tbl",
"Uni2TCRCBodYig.tbl",
"Uni2Bzd.tbl", //2007
"Uni2Ty.tbl",
"Uni2NS.tbl",
"Uni2Jamyang.tbl"//20080924
};

//Others to Unicode  Mapping Table File Names
char *Others2UniMTFNames[] =
{
"Uni2Uni.tbl",
"TMW2Uni.tbl",
"TM2Uni.tbl",
"Fz2Uni.tbl",
"Hg2Uni.tbl",
"ACIP2Uni.tbl",
"Wylie2Uni.tbl",
"LTibetan2Uni.tbl",
"OldSambhota2Uni.tbl",
"NewSambhota2Uni.tbl",
"THDLWylie2Uni.tbl",
"LCWylie2Uni.tbl",
"TCRCBodYig2Uni.tbl",
"Bzd2Uni.tbl", //2007
"Ty2Uni.tbl",
"NS2Uni.tbl",
"Jamyang2Uni.tbl"//20080924
};

//Tibetan Syllable Table File Names for different transliteration scheme
char *TibetanSyllableTableFileNames[] =
{
"Uni2Uni.tbl",
"TMW2Uni.tbl",
"TM2Uni.tbl",
"Fz2Uni.tbl",
"Hg2Uni.tbl",
"ACIPTibetanSyllable.tbl",
"WylieTibetanSyllable.tbl",
"LTibetan2Uni.tbl",
"OldSambhota2Uni.tbl",
"NewSambhota2Uni.tbl",
"THDLWylieTibetanSyllable.tbl",
"LCWylieTibetanSyllable.tbl",
"TCRCBodYig2Uni.tbl",
"Bzd2Uni.tbl", //2007
"Ty2Uni.tbl",
"NS2Uni.tbl",
"Jamyang2Uni.tbl"//20080924
};


char *VowelsForTSLtableNames[] =
{
"VowelsForUni.tbl",//No use
"VowelsForTMW.tbl",
"VowelsForTM.tbl",
"VowelsForFz.tbl",//No use
"VowelsForHg.tbl",//No use
"VowelsForACIP.tbl",//No use
"VowelsForWylie.tbl",//No use
"VowelsForLTibetan.tbl",
"VowelsForOldSambhota.tbl",
"VowelsForNewSambhota.tbl",
"VowelsForTHDLWylie.tbl", //No use
"VowelsForLCWylie.tbl", //No use
"VowelsForTCRCBodYig.tbl",
"VowelsForBzd.tbl", //2007
"VowelsForTy.tbl",
"VowelsForNS.tbl",
"VowelsForJamyang.tbl"//20080924
};


//--------------------------------------
//Those that can be adjusted
//
#define DeffFontSize 16
#define MAXSTACKHEIGHT 100
#define SZBuffer 1000	//Source Text Buffer Size was 1000
#define FontBufferSize 300	//Using for hunting fonttbl
#define BaseNumber 33	//For setting up rtf head when writing a new rtf file
//---------------------------------------------------------------------------

//Default Unicode tibetan font name in the system
#ifdef Windows
char TibetanUnicodeFontName[]="Microsoft Himalaya";

//char TibetanUnicodeFontName[]="SambhotaUnicode";  // 20180304
#endif


#ifdef Mac
char TibetanUnicodeFontName[]="kalaso";//TO BE DETERMINED   20080924
//char TibetanUnicodeFontName[]="SambhotaUnicode";//TO BE DETERMINED   20180304
#endif


#ifdef Unix
char TibetanUnicodeFontName[]="Microsoft Himalaya";   //UnixTibetanFont";//TO BE DETERMINED
//char TibetanUnicodeFontName[]="SambhotaUnicode";   //UnixTibetanFont";//20180304
#endif

#define TEMP_FILE "TMP/tempFile"		//mk parameterized tempFile location

//------------------------------
#define PUSHED 0
#define STACKFULL 1
#define NOTHINGONSTACK 2
#define POPED 3
#define ControlSymbol 4
#define ControlWord 5
#define SpecialControlWord 6
#define TextBufferEmpty 7
#define TranslationDone 8
#define NoFontFileID 9
#define NotRtfFile 10
#define ThisIsRtfFile 11
#define NoDeff 12
#define FonttblProcessed 13
#define EndOfBuffer 14
#define NoCertainTibetanFont 15
#define NoFonttbl 16
#define Exception 17
#define ConversionDone 18
#define NoSourceFile 19
#define NoTargetFile 20
#define WrongSourceEncoding 21
#define WrongTargetEncoding 22
#define Initialized 23
#define WrongSourceFileFormat 24
#define WrongTargetFileFormat 25
#define UnknowConversionDirection 26
#define SameEncoding 27
#define EndOfFile 28
#define GotTheDeff 29
#define NoTableFile 30
#define WrongBytesAppeared 31
#define EmptyFile 32
#define NoMemory 33
#define ReachFontTbl 34
#define MappingTableLoaded 35
#define MemoryFreed 36
#define WrongSourceEncodingOrFileFormat 37
#define WrongTargetEncodingOrFileFormat 38
#define MSoffice 39
#define BodyTagFound 40
#define FontTagFound 41
#define OtherTagFound 42
#define NoVowelTableFile 43
#define VowelTableLoaded 44
#define NoUnicodeToAWTableFile 45
#define UnicodeToAWTableLoaded 46
#define Translated 47
#define AWToUnicodeTableLoaded 48
#define NeedFileName 49
#define ClosingTagFound 50
#define NonLatinCharacterCodeValue 51
#define NoTibetanSyllableTableFile 52
#define LinkTagFound 53
#define StyleTagFound 54
#define ClosingStyleTagFound 55
#define CommentTagFound 56
#define FontStyleTagFound 57
#define SimpleTag 58
#define TagWithAttributes 59
#define NoStyleSheetFile 60
#define NoAWToUnicodeTableFile 61
#define CP936TableUnavailable 62  //GB2312
#define CP936TableBuilt 63  //GB2312
#define UnicodeBigEndian 64 //200802
#define UTF8 65 //200802
#define DOCTYPETagFound 66 //20080229


#ifdef CHINESE
//For output debug information
char *DebugInfo[] = {
"PUSHED",
"STACKFULL",
"NOTHINGONSTACK",
"POPED",
"ControlSymbol",
"ControlWord",
"SpecialControlWord",
"TextBufferEmpty",
"翻译结束。",
"No Font File ID",
"源文件不是RTF文件。",
"ThisIsRtfFile",
"No Deff",
"Fonttbl Processed",
"End Of Buffer",
"No Certain Tibetan Font",
"No Fonttbl",
"Exception Happened!",
"      转换结束！",
"没有源文件！",
"没有目标文件！",
"源文件编码或字体名称不正确！",
"目标文件编码或字体名称不正确！",
"Initialized",
"源文件格式不正确！",
"目标文件格式不正确！",
"Unknow Conversion Direction!",
"相同编码或字体！",
"文件结束！",
"Got The Deff",
"影射表文件不存在！",
"Wrong Bytes Appeared",
"源文件是空的！",
"没有足够的内存空间！",
"Font table presents!",
"Mapping Table Loaded!",
"Memory Is Freed!",
"源文件编码或格式不正确！",  //"Wrong Source Encoding Or Source File Format!",
"目标文件编码或格式不正确！",  //"Wrong Target Encoding Or Target File Format!",
"微软Office就可以做这个转换，请用它转换！",
"Body tag found!",
"Font tag found!",
"Other tag found!",
"缺影射表文件VowelsForTSL.tbl!",
"VowelsForTSL table loaded!",
"缺Unicode 到AW影射表文件!",
"Unicode to AW table loaded!",
"Unicode 到AW转换结束!",
"AW to Unicode table loaded!",
"请指定文件名！",
"Closing tag found!",
"Non Latin Character Code Value!",
"No Tibetan Syllable TableFile!",
"Link tag found!",
"Style tag found!",
"Closing Style Tag Found!",
"Comment Tag Found!",
"Style Tag That Has Font is Found!",
"A simple tag!",
"Tag With Attributes Found!",
"没有Stylesheet文件！",
"缺AW到Unicode影射表文件!",
"CP936toUnicodeTable.TXT 不存在!请检查！",//GB2312
"CP936toUnicode Table and Unicode to CP936 Table are already being built!",//GB2312
"您的源文件的格式是 Unicode Big Endian, 无法进行转换。请将其格式用其它软件如MS Notepad转存为Unicode, 然后再来转换!", //200802
"您的源文件的格式是 UTF8, 无法进行转换。请将其格式用其它软件如MS Notepad转存为Unicode, 然后再来转换!", //200802
"A DOCTYPE Tag Found."
};
#endif

#ifdef ENGLISH
//For output debug information
char *DebugInfo[] = {
"PUSHED",
"STACKFULL",
"NOTHINGONSTACK",
"POPED",
"ControlSymbol",
"ControlWord",
"SpecialControlWord",
"TextBufferEmpty",
"Translation Done",
"No Font File ID",
"Source File Is Not Rtf File!",
"ThisIsRtfFile",
"No Deff",
"Fonttbl Processed",
"End Of Buffer",
"No Certain Tibetan Font",
"No Fonttbl",
"Exception Happened!",
"      Conversion Is Done!",
"No Source File!",
"No Target File!",
"Wrong Source Encoding Name!",
"Wrong Target Encoding Name!",
"Initialized",
"WrongSourceFileFormat",
"Wrong Target File Format Name!",
"Unknow Conversion Direction!",
"They Are The Same Encoding!",
"End Of File!",
"Got The Deff",
"There Is No Table File!",
"Wrong Bytes Appeared",
"Source File Is a Empty File!",
"There is no enough memory!",
"Font table presents!",
"Mapping Table Loaded!",
"Memory Is Freed!",
"Wrong Source File! It's not in the specified format!",  //"Wrong Source Encoding Or Source File Format!",
"Wrong Target File Format!",  //"Wrong Target Encoding Or Target File Format!",
"MS Office Can Do This! Please use it to do this!",
"Body tag found!",
"Font tag found!",
"Other tag found!",
"There is no VowelsForTSL table file",
"VowelsForTSL table loaded!",
"There is no Unicode to AW table!",
"Unicode to AW table loaded!",
"Unicode to AW Translation done!",
"AW to Unicode table loaded!",
"Need file name!",
"Closing tag found!",
"Non Latin Character Code Value!",
"No Tibetan Syllable TableFile!",
"Link tag found!",
"Style tag found!",
"Closing Style Tag Found!",
"Comment Tag Found!",
"Style Tag That Has Font is Found!",
"A simple tag!",
"Tag With Attributes Found!",
"There Is No Style Sheet File!",
"There is no AW to Unicode table!",
"Table file CP936toUnicodeTable.TXT is unavailable!",//GB2312
"CP936toUnicode Table and Unicode to CP936 Table are already being built!",//GB2312
"Source file format is Unicode Big Endian, please contert it into Unicode by other software such as Microsoft Notepad, and then come back and do the conversion here again!", //200802
"Source file format is UTF8, please contert it into Unicode by other software such as Microsoft Notepad, and then come back and do the conversion here again!", //200802
"A DOCTYPE Tag Found." //20080229
};
#endif

//-----------------------------------------------------------------------
//Pointers to TMW font names
char *TibetanFontNames[] = {
	//Unicode font
	"Microsoft Himalaya",
	"SambhotaUnicode", //20181201
	"CTRC-Uchen",  //20090325
	"CTRC-Betsu",  //20090325
	"CTRC-Drutsa",  //20090325
	"CTRC-Tsumachu",  //20090325
	"藏研乌坚体",  //20090325
	"藏研簇玛丘体",  //20090325
	"藏研柏簇体",  //20090325
	"藏研珠擦体",  //20090325
	"藏研乌金体",  //20090325
	//TMW font
	"TibetanMachineWeb",
	"TibetanMachineWeb1",
	"TibetanMachineWeb2",
	"TibetanMachineWeb3",
	"TibetanMachineWeb4",
	"TibetanMachineWeb5",
	"TibetanMachineWeb6",
	"TibetanMachineWeb7",
	"TibetanMachineWeb8",
	"TibetanMachineWeb9",
	//TM font
	"TibetanMachine",
	"TibetanMachineSkt1",
	"TibetanMachineSkt2",
	"TibetanMachineSkt3",
	"TibetanMachineSkt4",
	//Fz
	//Hg

	//ACIP
	"Arial",
	"Times New Roman",
	//Wylie
	"Arial",
	"Times New Roman",
	//LTibetan
	"LTibetan",
	"LMantra",
	//OldSambhota
	"Sama",
	"Samb",
	"Samc",
	"Esama",
	"Esamb",
	"Esamc",
	//NewSambhota
	"Dedris-a",
	"Dedris-a1",
	"Dedris-a2",
	"Dedris-a3",
	"Dedris-b",
	"Dedris-b1",
	"Dedris-b2",
	"Dedris-b3",
	"Dedris-c",
	"Dedris-c1",
	"Dedris-c2",
	"Dedris-c3",
	"Dedris-d",
	"Dedris-d1",
	"Dedris-d2",
	"Dedris-d3",
	"Dedris-e",
	"Dedris-e1",
	"Dedris-e2",
	"Dedris-e3",
	"Dedris-f",
	"Dedris-f1",
	"Dedris-f2",
	"Dedris-f3",
	"Dedris-g",
	"Dedris-g1",
	"Dedris-g2",
	"Dedris-g3",
	"Dedris-syma",
	"Dedris-vowa",

	"Ededris-a",
	"Ededris-a1",
	"Ededris-a2",
	"Ededris-a3",
	"Ededris-b",
	"Ededris-b1",
	"Ededris-b2",
	"Ededris-b3",
	"Ededris-c",
	"Ededris-c1",
	"Ededris-c2",
	"Ededris-c3",
	"Ededris-d",
	"Ededris-d1",
	"Ededris-d2",
	"Ededris-d3",
	"Ededris-e",
	"Ededris-e1",
	"Ededris-e2",
	"Ededris-e3",
	"Ededris-f",
	"Ededris-f1",
	"Ededris-f2",
	"Ededris-f3",
	"Ededris-g",
	"Ededris-g1",
	"Ededris-g2",
	"Ededris-g3",
	"Ededris-syma",
	"Ededris-vowa",
	//THDLWylie
	"Arial",
	"Times New Roman",
	//LCWylie
	"Arial",
	"Times New Roman",
	//TCRCBodYig
	"TCRC Bod-Yig",
	"TCRC Youtso",
	"TCRC Youtsoweb",
	//Bzd
	"BZDBT", //2007
	"BZDHT",
	"BZDMT",
	//Ty
	"TIBETBT",
	"TIBETHT",
	"CHINATIBET",//20090325
	//NS
	"藏文吾坚琼体",
	//Jamyang   20080924
	"DBu-can",
	"Tibetisch dBu-can",
	"Tibetisch dBu-can Overstrike"
};//TO BE EXTENDED


char *AllTibetanFontNames[] = {
	//Unicode font
	"Microsoft Himalaya", //2007
    "SambhotaUnicode", //20181201
	"CTRC-Uchen",  //20090325
	"CTRC-Betsu",  //20090325
	"CTRC-Drutsa",  //20090325
	"CTRC-Tsumachu",  //20090325
	"藏研乌坚体",  //20090325
	"藏研簇玛丘体",  //20090325
	"藏研柏簇体",  //20090325
	"藏研珠擦体",  //20090325
	"藏研乌金体",  //20090325
	//TMW font
	"TibetanMachineWeb",
	"TibetanMachineWeb1",
	"TibetanMachineWeb2",
	"TibetanMachineWeb3",
	"TibetanMachineWeb4",
	"TibetanMachineWeb5",
	"TibetanMachineWeb6",
	"TibetanMachineWeb7",
	"TibetanMachineWeb8",
	"TibetanMachineWeb9",
	//TM font
	"TibetanMachine",
	"TibetanMachineSkt1",
	"TibetanMachineSkt2",
	"TibetanMachineSkt3",
	"TibetanMachineSkt4",
	//Fz
	//Hg
	//Chinese
	//"宋体", //20080229  //20090325

	//LTibetan
	"LTibetan",
	"LMantra",
	//OldSambhota
	"Sama",
	"Samb",
	"Samc",
	"Esama",
	"Esamb",
	"Esamc",
	//NewSambhota
	"Dedris-a",
	"Dedris-a1",
	"Dedris-a2",
	"Dedris-a3",
	"Dedris-b",
	"Dedris-b1",
	"Dedris-b2",
	"Dedris-b3",
	"Dedris-c",
	"Dedris-c1",
	"Dedris-c2",
	"Dedris-c3",
	"Dedris-d",
	"Dedris-d1",
	"Dedris-d2",
	"Dedris-d3",
	"Dedris-e",
	"Dedris-e1",
	"Dedris-e2",
	"Dedris-e3",
	"Dedris-f",
	"Dedris-f1",
	"Dedris-f2",
	"Dedris-f3",
	"Dedris-g",
	"Dedris-g1",
	"Dedris-g2",
	"Dedris-g3",
	"Dedris-syma",
	"Dedris-vowa",

	"Ededris-a",
	"Ededris-a1",
	"Ededris-a2",
	"Ededris-a3",
	"Ededris-b",
	"Ededris-b1",
	"Ededris-b2",
	"Ededris-b3",
	"Ededris-c",
	"Ededris-c1",
	"Ededris-c2",
	"Ededris-c3",
	"Ededris-d",
	"Ededris-d1",
	"Ededris-d2",
	"Ededris-d3",
	"Ededris-e",
	"Ededris-e1",
	"Ededris-e2",
	"Ededris-e3",
	"Ededris-f",
	"Ededris-f1",
	"Ededris-f2",
	"Ededris-f3",
	"Ededris-g",
	"Ededris-g1",
	"Ededris-g2",
	"Ededris-g3",
	"Ededris-syma",
	"Ededris-vowa",

	"TCRC Bod-Yig",
	"TCRC Youtso",
	"TCRC Youtsoweb"
	//Bzd
	"BZDBT", //2007
	"BZDHT",
	"BZDMT",
	//Ty
	"TIBETBT",
	"TIBETHT",
	"CHINATIBET",//20090325
	//NS
	"藏文吾坚琼体",
	//Jamyang   20080924
	"DBu-can",
	"Tibetisch dBu-can",
	"Tibetisch dBu-can Overstrike"
};//TO BE EXTENDED



int TotalTibetanFontNumber = 107; // = 95 - 8 + 1; //# of all Tibetan fonts over all encodings  //2007  20080924 //20090325 //was 106 before 20181201
								 //TO BE DETERMINED
//--------------------------------------------------------------------------
//Tibetan encodings. Index is the ID of the encoding
char *EncodingNames[] = {
	"Unicode",		//ID--0
	"TMW",			//ID--1
	"TM",			//ID--2
	"Fz",			//ID--3
	"Hg",			//ID--4
	"ACIP",			//ID--5
	"Wylie",		//ID--6
	"LTibetan",		//ID--7
	"OldSambhota",	//ID--8
	"NewSambhota",	//ID--9
	"THDLWylie",	//ID--10
	"LCWylie",		//ID--11
	"TCRCBodYig",	//ID--12
	"Bzd",			//ID--13 //2007
	"Ty",			//ID--14
	"NS",			//ID--15
	"Jamyang"		//ID--16 //20080924
};
int Encodings = 17; //How many Encodings are there!!!!!  //2007  20080924

//int BaseIndex[]={0, 10, 20, 0, 0, 25, 27, 29, 31, 37, 97, 99, 101, 104, 107, 110, 111};//Actual index of each encoding in the array "TibetanFontNames"  //2007  20080924 //20090325
int BaseIndex[]={0, 11, 21, 0, 0, 26, 28, 30, 32, 38, 98, 100, 102, 105, 108, 111, 112};//Actual index of each encoding in the array "TibetanFontNames"  //2007  20080924 //20090325 //add SambhotaUnicode on 20181201


//int EncodingFontNumber[] = {10, 10, 5, 0, 0, 2, 2, 2, 6, 60, 2, 2, 3, 3, 3, 1, 3}; //Number of Tibetan font in a certain font encoding //2007	20080924  //20090325
int EncodingFontNumber[] = {11, 10, 5, 0, 0, 2, 2, 2, 6, 60, 2, 2, 3, 3, 3, 1, 3}; //Number of Tibetan font in a certain font encoding //2007	20080924  //20090325 // add SambhotaUnicode on 20181201

int TargetFileBaseFontNumberRTF2RTF; //20090427
//
//---------------------------------------------------------------------
//Files

FILE *sourceFile;
FILE *targetFile;

FILE *fTable;	//For mapping tables
FILE *fTibetanSyllableTable; //For Tibetan Syllable

char *styleSheetFileName;
FILE *stylesheetFile;
//---------------------------------------------------------------------
int SourceEncoding = -1;//To be initiallized as -1
int TargetEncoding = -1;

int ConversionDirection = -1; //Conversion direction
							//0---Unicode to others
							//1---Others to Unicode
							//2---between same encodings, doesn't make sense
							//3---Others to others
int SourceFileFormat;	//0---TXT
						//1---RTF
						//2---UnicodeTXT
						//3---webpage
int TargetFileFormat;	//0---TXT
						//1---RTF
						//2---UnicodeTXT
						//3---webpage
int SourceFileType = -1;		//0---TXT			//200802
								//1---RTF
								//2---UnicodeTXT
								//3---webpage
int TargetFileType = -1;		//0---TXT			//200802
								//1---RTF
								//2---UnicodeTXT
								//3---webpage
//---------------------------------------------------------------------

char ChineseFontName[]="宋体"; //20080229

char RtfFontSize[] = "\\fs16";

//---------------------------------------------------------------------
//Global variables for parsing fonttbl

#define fTrue 1
#define fFalse 0

int holdTibetanBuffer = No;//For making Unicode sequence continuous when convert from Unicode to others.

int FonttblPast = No;

long lParam;
int param;
int fParam = fFalse; //Does the control word have a parameter?
char szKeyword[30];	//Buffer for the keyword
char szParameter[20];	//Buffer for the parameter

typedef struct    // font info structure to be on stack
{
    int fontSize;

    int fontID;

	int isTibetan;

} FONTINFO;

FONTINFO myStack[MAXSTACKHEIGHT];

int stackIndex = 0;

FONTINFO currentFontInfo;

FONTINFO deffFontInfo;

//----------------------------------------------------
//The buffer holding characters from RTF files!
typedef struct
{
	int ch;

	int fontID;

} CharWithFontInfo;//Character with its font info


CharWithFontInfo TibetanBuffer[SZBuffer + 1];

int nTibetan = 0; //Number of Tibetan Characters In Source Text Buffer;
int nChinese = 0;
int nLatin = 0;
int nTibetanInAW = 0;
int nFormatBuffer = 0;
int nGBCode = 0;

int LastCh;

int LatinBuffer[SZBuffer + 1];
int ChineseBuffer[SZBuffer + 1];
int TibetanInAWBuffer[SZBuffer + 1];

int FormatBuffer[SZBuffer + 1];
char GBCode[20];

int header = No;//Don't process control words in header
//--------------------------------------------------------------------------

#define MaxEncodingFontNumber 200 //The max number in all. TO BE ADJUSTED from time to time
//Source and target font info, set up when parsing fonttbl, used during
//parsing rtf file

int sourceFontIDs[MaxEncodingFontNumber+1] = {-1}; //set up when parsing fonttbl, used when parsing text
//int sourceFontNumber;

int TibetanFontIDs[MaxEncodingFontNumber+1] = {-1}; //201004
int nTibetanFontIDs = 0; //201004

int targetFontIDs[MaxEncodingFontNumber+1]; //set up when parsing fonttbl, used when translating
//int targetFontBaseIndex;
//int targetFontNumber;


int Western[101];//Western font IDs, special for ACIP and Wylie
int nWestern = 0;

int ChineseFontID[10];//Chinese font IDs, special for ACIP and Wylie
//---------------------------------------------------------------------------
//Omapping table

char *pTable; //The mapping table entry. ----global

int *pVowel; //The vowel table entry. ----global

int *pAW2Unicode; //The vowel table entry. ----global

char *pUnicodeToAW; //The Unicode to AW table entry. ----global

char *pTibetanSyllable; //Tibetan syllable table entry. ----global
char **pTSLookupTable; //For quick look up Tibetan syllable in the table pTibetanSyllable.

int TCRCvowelChart[300][6];//Combination chart for TCRC to pick up right vowels
int TCRCconstantChart[50][50][3];//Combination chart for TCRC to pick up right low level constant

//--------------------------DATA STRUCTURES-----------------------------------
//Data structures for lookup tables for converting from Unicode to
//others
typedef struct
{
	int FileNumber;
	char theChar;
}LEVEL;


typedef struct
{
	char FzHg[3];

	int FileNumber;	//Correspounding file number

	char theChar;

	LEVEL level[5];

}NODEDATA; //Data that stored at B-tree node


typedef struct node
{
	NODEDATA data;

	struct node *NextNode[126]; //[126]; //Only concern Unicode rank: 0f40--0fbd

} BTREE; //The tree that stores mapping tables


BTREE *TableHead;	//The root of the mapping table tree.
					//The entry point of the table
BTREE *currentNode;	//current working node

BTREE *AllocatedNodes[6000];//For freeing those allocated memory;
int nAllocatedNodes = 0;//Only for delocating purpose

NODEDATA otherCells[208];	//This is for hodling Unicode points that are not
							// considered in the BTREE. u<0f40 && u>0fbd


//-----------------For web page----------------------------------------
char TagName[50];
char FontSize[30];
char FontName[100];
char RestFontInfo[200];

typedef struct    // font info structure to be on stack
{
	char tagName[50];

    char fontSize[30];

    char fontName[50];

	int isTibetan;

} WebFONTINFO;

WebFONTINFO currentWebFontInfo;

WebFONTINFO deffWebFontInfo;

WebFONTINFO myWebFontInfoStack[MAXSTACKHEIGHT];


typedef struct    // font info structure to be on stack
{
    int ch;

	char fontSize[30];

    char fontName[50];

} WebTEXTWITHFONTNAME;

WebTEXTWITHFONTNAME TibetanBufferForWebPage[SZBuffer];


//-----------------------for HTML 3.20 over------------------------------

#define MAXATTRIBUTENUMBER 10

char AttributeName[MAXATTRIBUTENUMBER][50];
char AttributeContent[MAXATTRIBUTENUMBER][200];
int nAttributes = 0; //number of attributes in a tag;

//Current Tibetan Font Tag Attribute Names
char CurrentTibetanFontTagName[50];
char CurrentTibetanFontTagAttributeName[MAXATTRIBUTENUMBER][50];
char CurrentTibetanFontTagAttributeContent[MAXATTRIBUTENUMBER][200];
int nCurrentTibetanFontTagAttributes = 0;

int nStyleTags = 0;
char StyleTags[50][50];

int nClassNames = 0;
char ClassNames[50][50];

char ClassName[50];

char FontNameInStyles[50][50][30];//FontNameInStyles[TagName][ClassName][].

int WayToChangeFontAttribute = 0; //Remeber the way the tag changes the font attribute of text
							//1---by <font ...>;
							//2---by class or inline style --->Write back inline style
char lastClosingTag[50];//For buffering format


//--------------------------------------//GB2312---------------------------------------------
//Conversion table from CP936 or GB2312 to Unicode
unsigned short GB2Unicode[0xffff]; //CP936 to Unicode table
unsigned short Unicode2GB[0xffff]; //Unicode to CP936 table
int CP936GBTablesBuilt = No; //

//--------------------------End of Header-----------------------------------------------------




//////////////////////////////////////////////////////
//													//
//			All Routines							//
//													//
//////////////////////////////////////////////////////


//----------------------------Function declaration--------------------------------------------
int TranslateAndWriteBack(void);
int WriteBackFormat(void);


int axtoi(char *hexStg) {
  int n = 0;         // position in string
  int m = 0;         // position in digit[] to shift
  int count;         // loop index
  int intValue = 0;  // integer value of hex string
  int digit[5];      // hold values to convert
  while (n < 4) {
     if (hexStg[n]=='\0')
        break;
     if (hexStg[n] > 0x29 && hexStg[n] < 0x40 ) //if 0 to 9
        digit[n] = hexStg[n] & 0x0f;            //convert to int
     else if (hexStg[n] >='a' && hexStg[n] <= 'f') //if a to f
        digit[n] = (hexStg[n] & 0x0f) + 9;      //convert to int
     else if (hexStg[n] >='A' && hexStg[n] <= 'F') //if A to F
        digit[n] = (hexStg[n] & 0x0f) + 9;      //convert to int
     else break;
    n++;
  }
  count = n;
  m = n - 1;
  n = 0;
  while(n < count) {
     // digit[n] is value of hex digit at position n
     // (m << 2) is the number of positions to shift
     // OR the bits into return value
     intValue = intValue | (digit[n] << (m << 2));
     m--;   // adjust the position to set
     n++;   // next digit to process
  }
  return (intValue);
}



#ifdef Unix

#define __strrev strrev

char* strrev(char* szT)
{
    int i, j, k, t;
    if ( !szT )                 // handle null passed strings.
        return "";
    i = strlen(szT);
    t = !(i%2)? 1 : 0;      // check the length of the string .
    for(j = i-1 , k = 0 ; j > (i/2 -t) ; j-- )
    {
        char ch  = szT[j];
        szT[j]   = szT[k];
        szT[k++] = ch;
    }
    return szT;
}


#define _itoa   itoa

char* itoa(int value, char*  str, int radix)
{
    int  rem = 0;
    int  pos = 0;
    char ch  = '!' ;
    do
    {
        rem    = value % radix ;
        value /= radix;
        if ( 16 == radix )
        {
            if( rem >= 10 && rem <= 15 )
            {
                switch( rem )
                {
                    case 10:
                        ch = 'a' ;
                        break;
                    case 11:
                        ch ='b' ;
                        break;
                    case 12:
                        ch = 'c' ;
                        break;
                    case 13:
                        ch ='d' ;
                        break;
                    case 14:
                        ch = 'e' ;
                        break;
                    case 15:
                        ch ='f' ;
                        break;
                }
            }
        }
        if( '!' == ch )
        {
            str[pos++] = (char) ( rem + 0x30 );
        }
        else
        {
            str[pos++] = ch ;
        }
    }while( value != 0 );
    str[pos] = '\0' ;
    return strrev(str);
}

#endif


//GB2312
int BuildCp936UnicodeTables() //20080229
{
	char s[20][20];
	int i, j, n;
	char ss[200];
	char theChar[3];
	unsigned short l, m;
	unsigned short iUnicode;
	unsigned short iCP936;

	FILE *CP936UnicodeTableFile;

	char tempPath1[1000];
	strcpy(tempPath1, m_UTFCdirectory);
	strcat(tempPath1, "CP936toUnicodeTable.TXT");

	CP936UnicodeTableFile = fopen(tempPath1, "rb");
    if (!CP936UnicodeTableFile) return CP936TableUnavailable;

	for(i=0;i<0xffff;i++) GB2Unicode[i] = 0;//Initializating the tables
	for(i=0;i<0xffff;i++) Unicode2GB[i] = 0;

	while (fgets(ss, 200, CP936UnicodeTableFile))
	{
		j = 0; n = 0;

		for(i=0; i<strlen(ss); i++ )
		{
			if (ss[i]==0x09)
			{
				s[j][n] = '\0';
				j++; n = 0;
			}
			else s[j][n++]= ss[i];
		}
		if(j<2) continue;

		theChar[2] = '\0';
		//Converting GB string to long
		theChar[0] = s[0][0];
		theChar[1] = s[0][1];
		m = axtoi(theChar);
		m = m<<8;
		theChar[0] = s[0][2];
		theChar[1] = s[0][3];
		l = axtoi(theChar);
		iCP936 = l + m;
		//Converting Unicode string to long
		theChar[0] = s[1][0];
		theChar[1] = s[1][1];
		m = axtoi(theChar);
		m = m<<8;
		theChar[0] = s[1][2];
		theChar[1] = s[1][3];
		l = axtoi(theChar);
		iUnicode = l + m;
		//Build the tables
		GB2Unicode[iCP936] = iUnicode;
		Unicode2GB[iUnicode] = iCP936;
	}

	fclose(CP936UnicodeTableFile);

	CP936GBTablesBuilt = Yes;
	return CP936TableBuilt;
}


//English punctuations.
int IsPunctuation(int character)
{
	if( character == ',' ||
		character == ';' ||
		character == '?' ||
		character == '!' ||
		character == ':' ||
		character == ')' ||
		character == ']' ||
		character == '}' ||
		character == '/' )
		return Yes;
	else if( character == 0x20)//For '.'
	{
		if(nTibetan>1)
		{
			if(SourceFileFormat==WebPage)
			{
				if(TibetanBufferForWebPage[nTibetan-2].ch == '.') return Yes;
				else return No;
			}
			else //Other file formats
			{
				if(TibetanBuffer[nTibetan-2].ch == '.') return Yes;
				else return No;
			}
		}
		else return No;
	}
	else return No;
}


//For processing web page
char *CharacterValue2ItsName(int ch)
{
	//Frequently used characters
	if(ch == 160) return "nbsp";
	else if(ch == 38) return "amp";
	else if(ch == 60) return "lt";
	else if(ch == 62) return "gt";
	else if(ch == 34) return "quot";
	else return "";//Return empty string
}


//For processing web page
int CharacterName2ItsValue(char *ChName)
{
	//Frequently used characters
	if(!strcmp(ChName, "nbsp")) return 160;
	else if(!strcmp(ChName, "amp")) return 38;
	else if(!strcmp(ChName, "lt")) return 60;
	else if(!strcmp(ChName, "gt")) return 62;
	else if(!strcmp(ChName, "quot")) return 34;
	else
	{	//Non-ferquently used characters
		if(!strcmp(ChName, "iexcl")) return 161;
		else if(!strcmp(ChName, "curren")) return 164;
		else if(!strcmp(ChName, "cent")) return 162;
		else if(!strcmp(ChName, "pound")) return 163;
		else if(!strcmp(ChName, "yen")) return 165;
		else if(!strcmp(ChName, "brvbar")) return 166;
		else if(!strcmp(ChName, "sect")) return 167;
		else if(!strcmp(ChName, "uml")) return 168;
		else if(!strcmp(ChName, "copy")) return 169;
		else if(!strcmp(ChName, "ordf")) return 170;
		else if(!strcmp(ChName, "laquo")) return 171;
		else if(!strcmp(ChName, "not")) return 172;
		else if(!strcmp(ChName, "shy")) return 173;
		else if(!strcmp(ChName, "reg")) return 174;
		else if(!strcmp(ChName, "trade")) return -1;
		else if(!strcmp(ChName, "macr")) return 175;
		else if(!strcmp(ChName, "deg")) return 176;
		else if(!strcmp(ChName, "plusmn")) return 177;
		else if(!strcmp(ChName, "sup2")) return 178;
		else if(!strcmp(ChName, "sup3")) return 179;
		else if(!strcmp(ChName, "acute")) return 180;
		else if(!strcmp(ChName, "micro")) return 181;
		else if(!strcmp(ChName, "para")) return 182;
		else if(!strcmp(ChName, "middot")) return 183;
		else if(!strcmp(ChName, "cedil")) return 184;
		else if(!strcmp(ChName, "sup1")) return 185;
		else if(!strcmp(ChName, "ordm")) return 186;
		else if(!strcmp(ChName, "raquo")) return 187;
		else if(!strcmp(ChName, "frac14")) return 188;
		else if(!strcmp(ChName, "frac12")) return 189;
		else if(!strcmp(ChName, "frac34")) return 190;
		else if(!strcmp(ChName, "iquest")) return 191;
		else if(!strcmp(ChName, "times")) return 215;
		else if(!strcmp(ChName, "divide")) return 247;
		else if(!strcmp(ChName, "Agrave")) return 192;
		else if(!strcmp(ChName, "Aacute")) return 193;
		else if(!strcmp(ChName, "Acirc")) return 194;
		else if(!strcmp(ChName, "Atilde")) return 195;
		else if(!strcmp(ChName, "Auml")) return 196;
		else if(!strcmp(ChName, "Aring")) return 197;
		else if(!strcmp(ChName, "AElig")) return 198;
		else if(!strcmp(ChName, "Ccedil")) return 199;
		else if(!strcmp(ChName, "Egrave")) return 200;
		else if(!strcmp(ChName, "Eacute")) return 201;
		else if(!strcmp(ChName, "Ecirc")) return 202;
		else if(!strcmp(ChName, "Euml")) return 203;
		else if(!strcmp(ChName, "Igrave")) return 204;
		else if(!strcmp(ChName, "Iacute")) return 205;
		else if(!strcmp(ChName, "Icirc")) return 206;
		else if(!strcmp(ChName, "Iuml")) return 207;
		else if(!strcmp(ChName, "ETH")) return 208;
		else if(!strcmp(ChName, "Ntilde")) return 209;
		else if(!strcmp(ChName, "Ograve")) return 210;
		else if(!strcmp(ChName, "Oacute")) return 211;
		else if(!strcmp(ChName, "Ocirc")) return 212;
		else if(!strcmp(ChName, "Otilde")) return 213;
		else if(!strcmp(ChName, "Ouml")) return 214;
		else if(!strcmp(ChName, "Oslash")) return 215;
		else if(!strcmp(ChName, "Ugrave")) return 216;
		else if(!strcmp(ChName, "Uacute")) return 217;
		else if(!strcmp(ChName, "Ucirc")) return 218;
		else if(!strcmp(ChName, "Uuml")) return 219;
		else if(!strcmp(ChName, "Yacute")) return 220;
		else if(!strcmp(ChName, "THORN")) return 221;
		else if(!strcmp(ChName, "szlig")) return 223;
		else if(!strcmp(ChName, "agrave")) return 224;
		else if(!strcmp(ChName, "aacute")) return 225;
		else if(!strcmp(ChName, "acirc")) return 226;
		else if(!strcmp(ChName, "atilde")) return 227;
		else if(!strcmp(ChName, "auml")) return 228;
		else if(!strcmp(ChName, "aring")) return 229;
		else if(!strcmp(ChName, "aelig")) return 230;
		else if(!strcmp(ChName, "ccedil")) return 231;
		else if(!strcmp(ChName, "egrave")) return 232;
		else if(!strcmp(ChName, "eacute")) return 233;
		else if(!strcmp(ChName, "ecirc")) return 234;
		else if(!strcmp(ChName, "euml")) return 235;
		else if(!strcmp(ChName, "igrave")) return 236;
		else if(!strcmp(ChName, "iacute")) return 237;
		else if(!strcmp(ChName, "icirc")) return 238;
		else if(!strcmp(ChName, "iuml")) return 239;
		else if(!strcmp(ChName, "eth")) return 240;
		else if(!strcmp(ChName, "ntilde")) return 241;
		else if(!strcmp(ChName, "ograve")) return 242;
		else if(!strcmp(ChName, "oacute")) return 243;
		else if(!strcmp(ChName, "ocirc")) return 244;
		else if(!strcmp(ChName, "otilde")) return 245;
		else if(!strcmp(ChName, "ouml")) return 246;
		else if(!strcmp(ChName, "oslash")) return 248;
		else if(!strcmp(ChName, "ugrave")) return 249;
		else if(!strcmp(ChName, "uacute")) return 250;
		else if(!strcmp(ChName, "ucirc")) return 251;
		else if(!strcmp(ChName, "uuml")) return 252;
		else if(!strcmp(ChName, "yacute")) return 253;
		else if(!strcmp(ChName, "thorn")) return 254;
		else if(!strcmp(ChName, "yuml")) return 255;

		else if(!strcmp(ChName, "OElig")) return 338;
		else if(!strcmp(ChName, "oelig")) return 339;
		else if(!strcmp(ChName, "Scaron")) return 352;
		else if(!strcmp(ChName, "scaron")) return 353;
		else if(!strcmp(ChName, "Yuml")) return 376;
		else if(!strcmp(ChName, "circ")) return 710;
		else if(!strcmp(ChName, "tilde")) return 732;
		else ;


		if(!strcmp(ChName, "ensp")) return 8194;
		else if(!strcmp(ChName, "emsp")) return 8195;
		else if(!strcmp(ChName, "thinsp")) return 8201;
		else if(!strcmp(ChName, "zwnj")) return 8204;
		else if(!strcmp(ChName, "zwj")) return 8205;
		else if(!strcmp(ChName, "lrm")) return 8206;
		else if(!strcmp(ChName, "rlm")) return 8207;
		else if(!strcmp(ChName, "ndash")) return 8211;
		else if(!strcmp(ChName, "mdash")) return 8212;
		else if(!strcmp(ChName, "lsquo")) return 8216;
		else if(!strcmp(ChName, "rsquo")) return 8217;
		else if(!strcmp(ChName, "sbquo")) return 8218;
		else if(!strcmp(ChName, "ldquo")) return 8220;
		else if(!strcmp(ChName, "rdquo")) return 8221;
		else if(!strcmp(ChName, "bdquo")) return 8222;
		else if(!strcmp(ChName, "dagger")) return 8224;
		else if(!strcmp(ChName, "Dagger")) return 8225;
		else if(!strcmp(ChName, "permil")) return 8240;
		else if(!strcmp(ChName, "lsaquo")) return 8249;
		else if(!strcmp(ChName, "rsaquo")) return 8250;
		else if(!strcmp(ChName, "euro")) return 8364;
		else ;

		return -1;
	}
}




//-----------------------------------------------------------
//Lookup font file ID for the font ID
int SourceFontFileID( int fontID)
{
	int i;
	for (i=0;i<EncodingFontNumber[SourceEncoding]; i++)
	{
		if(sourceFontIDs[i] == fontID) return i;
	}
	return 0; // There is no such font
}



int SourceFontFileIDforWebPage(char *fontName)
{
	int i;
	for (i=0;i<EncodingFontNumber[SourceEncoding]; i++)
	{
		if(!strcmp(TibetanFontNames[BaseIndex[SourceEncoding] + i], fontName)) return i;
	}
	return 0; // There is no such font
}



//Shared Routines
int
PushFontinfoOnStack(void)
{
    if (stackIndex >= MAXSTACKHEIGHT)
        return STACKFULL;
	//myStack[stackIndex] = currentFontInfo;
	myStack[stackIndex].fontID = currentFontInfo.fontID;
	myStack[stackIndex].fontSize = currentFontInfo.fontSize;
	myStack[stackIndex].isTibetan = currentFontInfo.isTibetan;
	stackIndex ++;
	return PUSHED;
}


int
PopFontinfoFromStack(void)
{
	stackIndex --;
    if (stackIndex < 0)
        return NOTHINGONSTACK;

	currentFontInfo.fontID = myStack[stackIndex].fontID;
	currentFontInfo.fontSize = myStack[stackIndex].fontSize;
	currentFontInfo.isTibetan = myStack[stackIndex].isTibetan;
	return POPED;
}



int
PushWebFontinfoOnStack(void)
{
	strcpy(myWebFontInfoStack[stackIndex].tagName, currentWebFontInfo.tagName);
    strcpy(myWebFontInfoStack[stackIndex].fontName, currentWebFontInfo.fontName);
	strcpy(myWebFontInfoStack[stackIndex].fontSize, currentWebFontInfo.fontSize);
	myWebFontInfoStack[stackIndex].isTibetan = currentWebFontInfo.isTibetan;
	if (stackIndex < MAXSTACKHEIGHT-2) stackIndex ++;//Provent from ignoring too many closing tags.
	return PUSHED;
}



int
PopWebFontinfoFromStack(void)
{
	int i;
	char closingTagName[50];
	//Take off the closing tag flag '/'
	for(i=0;TagName[i+1]!='\0' && i<48;i++) closingTagName[i] = TagName[i+1];
	closingTagName[i] = '\0';

	if(!strcmp(currentWebFontInfo.tagName, closingTagName)) stackIndex --;//closing tag has font attributes
	else
	{
		for(i=stackIndex-1; i>=0; i--)
		{
			if(!strcmp(myWebFontInfoStack[i].tagName, closingTagName)) break;
		}
		if(i>=0) stackIndex = i;//Previous tag is the one we need.
		else return POPED;//closing tag has no font attributes. Don't pop.
	}

    if (stackIndex < 0) stackIndex = 0;

	strcpy(currentWebFontInfo.tagName, myWebFontInfoStack[stackIndex].tagName);
	strcpy(currentWebFontInfo.fontName, myWebFontInfoStack[stackIndex].fontName);
	strcpy(currentWebFontInfo.fontSize, myWebFontInfoStack[stackIndex].fontSize);
	currentWebFontInfo.isTibetan = myWebFontInfoStack[stackIndex].isTibetan;
	return POPED;
}


//200802
int WriteBackLatin(void)
{
	int i;
	int ch;
	if(holdTibetanBuffer == Yes)//Tibetan is held, process held Tibetan first.
	{
		holdTibetanBuffer = No;
		if(nTibetan>0) TranslateAndWriteBack();//Process Tibetan text
	}

	if(nFormatBuffer>0) WriteBackFormat();//Write back any format in the format buffer before writing Chinese

	if(nLatin<=0) return TextBufferEmpty;

	if(TargetFileFormat == RTF )
	{
		if(SourceFileFormat != RTF)
		{
			fputs("\\f", targetFile);
			fprintf(targetFile, "%d", Western[0]);
			fputc(0x20, targetFile);
		}
		else if(SourceEncoding == ACIP || SourceEncoding == Wylie || SourceEncoding == THDLWylie || SourceEncoding == LCWylie)//AW fonts
		{
			fputs("\\f", targetFile);
			fprintf(targetFile, "%d", Western[0]);
			fputc(0x20, targetFile);
		}
		else ;
	}

	if(TargetFileFormat == WebPage && SourceFileFormat != WebPage)
	{
		fputs("<font face=\"Times New Roman\"", targetFile);
		fputs(" size=5>", targetFile);
	}

	for(i=0; i<nLatin; i++)
	{
		ch = LatinBuffer[i];

		if(ch <= 0 || ch >= 0xff) continue; //Take off those "bad guys". They are no Latin characters.

		if(TargetFileFormat == TXT)//200802
		{
			if( ch == 0x0d || ch==0x0a)
			{
#ifdef Windows
				fputc(0x0d, targetFile);
//				fputc(0x0a, targetFile); //Can not do this in Windows
#endif

#ifdef Unix
				fputc(0x0a, targetFile);  //For Unix or Linux
#endif

				if(ch == 0x0d && (i+1)<nLatin) // Deal with Windows Line-feed
				{
					if(LatinBuffer[i+1] == 0x0a) i++; //Skip 0x0a
				}
			}
			else fputc(ch, targetFile);
		}

		else if(TargetFileFormat == UnicodeTXT) //200802
		{
			if( ch == 0x0d || ch==0x0a)
			{
#ifdef Windows
				fputc(0x0d, targetFile);
				fputc(0x00, targetFile);

//				fputc(0x0a, targetFile);
//				fputc(0x00, targetFile);
#endif

#ifdef Unix
				fputc(0x0a, targetFile);  //For Unix or Linux
				fputc(0x00, targetFile);  //For Unix or Linux
#endif

				if(ch == 0x0d && (i+1)<nLatin)// Deal with Windows Line-feed
				{
					if(LatinBuffer[i+1] == 0x0a) i++; //Skip 0x0a
				}
			}
			else
			{
				fputc(ch, targetFile);
				fputc(0x00, targetFile);
			}
		}

		else if(TargetFileFormat == RTF)
		{
			if(ch==0x0d || ch==0x0a) //Carriage-return or Line-feed encounter
			{
//				fputs("\\par", targetFile); // original
//				fputs("\\\n", targetFile); // 20181110 instead use \\par for \\, but keep \\ and add \n  add at line 4675
				if(ch == 0x0d && (i+1)<nLatin)// Deal with Windows Line-feed
				{
					if(LatinBuffer[i+1] == 0x0a) i++; //Skip 0x0a
				}
			}
			else if(ch == '\\' || ch == '}' || ch =='{')
			{
				fputc('\\', targetFile);
				fputc(ch, targetFile);
			}
			else fputc(ch, targetFile);
		}

		else //(TargetFileFormat == WebPage)
		{
			if(ch==0x0d || ch==0x0a)
			{
				fputs("<br>", targetFile);
				if(ch == 0x0d && (i+1)<nLatin)// Deal with Windows Line-feed
				{
					if(LatinBuffer[i+1] == 0x0a) i++; //Skip 0x0a
				}
			}
			else
			{
				if(ch==34 || ch==38 || ch==39 || ch==60 || ch==62 || ch > 159)
				{
					fputs("&#", targetFile);
					fprintf(targetFile, "%d", ch);
					fputc(';', targetFile);
				}
				else fputc(ch, targetFile);
			}
		}
	}

	if(TargetFileFormat == WebPage && SourceFileFormat != WebPage)
	{
		fputs("</font>", targetFile);
	}

	nLatin = 0;
	return TranslationDone;
}


int
WriteBackFormat(void)
{
	int i, j = 0;

	char lastBlock[200];
	char currentBlock[200];

	if(SourceFileFormat != TargetFileFormat) return TranslationDone;//It's only for SourceEncoding == TargetEncoding

	lastBlock[0] = '\0';//Initialize it!
	FormatBuffer[nFormatBuffer] = '\0';

	for(i=0;i<nFormatBuffer; i++)
	{
		if(FormatBuffer[i] == '}')
		{
			currentBlock[j++] = FormatBuffer[i];
			currentBlock[j] = '\0';
			j = 0;//Reset counter j
			if(strcmp(lastBlock, currentBlock))//They are not same!
			{
				fputs(lastBlock, targetFile);
				strcpy(lastBlock, currentBlock);
			}
		}
		else currentBlock[j++] = FormatBuffer[i];
	}
	fputs(lastBlock, targetFile);
	if(j > 0)
	{
		currentBlock[j] = '\0';
		fputs(currentBlock, targetFile);
	}

	nFormatBuffer = 0;

	return TranslationDone;
}


//Write back DOCTYPE tag!
int WriteBackDOCTYPETag() //20080229
{
	int ch;

	if(holdTibetanBuffer == Yes)
	{
		holdTibetanBuffer = No;
		if(nTibetan>0) TranslateAndWriteBack();
	}

	if(TargetFileFormat == WebPage)
	{
		fputc('<', targetFile);
		fputc('!', targetFile);
	}
	while ((ch = getc(sourceFile)) != EOF)
	{
		if(ch != '>')
		{
			if(TargetFileFormat == WebPage) fputc(ch, targetFile);
		}
		else
		{
			if(TargetFileFormat == WebPage) fputc(ch, targetFile);
			return Yes;
		}
	}
	return EndOfFile;
}


//Write back comment!
int WriteBackCommentTag() //20080229
{
	int ch;
	static int ch1, ch2;

	if(holdTibetanBuffer == Yes)
	{
		holdTibetanBuffer = No;
		if(nTibetan>0) TranslateAndWriteBack();
	}

	if(TargetFileFormat == WebPage)
	{
		fputc('<', targetFile);
		fputc('!', targetFile);
	}
	while ((ch = getc(sourceFile)) != EOF)
	{
		if(ch != '>')
		{
			if(TargetFileFormat == WebPage) fputc(ch, targetFile);
		}
		else
		{
			if(TargetFileFormat == WebPage) fputc(ch, targetFile);
			if(ch1=='-' && ch2=='-') return Yes;
		}
		ch1 = ch2; //Keep previous two characters
		ch2 = ch;
	}
	return EndOfFile;
}


//Write back current tag to target file
int WriteBackTag(void)
{
	int i;

	if(TargetFileFormat != WebPage) return No;

	if(holdTibetanBuffer == Yes)//Output the buffered Tibetan text
	{
		holdTibetanBuffer = No;
		if(nTibetan>0) TranslateAndWriteBack();

		if(strlen(lastClosingTag) > 0)//There is a tag need to be written back.
		{
			fputc('<', targetFile);
			fputs(lastClosingTag, targetFile);
			fputc('>', targetFile);
		}
	}

	fputc('<', targetFile);
	fputs(TagName, targetFile);

	if(nAttributes > 0)
	{
		for(i=0; i<nAttributes; i++)
		{
			fputc(0x20, targetFile);

			fputs(AttributeName[i], targetFile);
			fputc('=', targetFile);

			if(AttributeContent[i][198] == '\'' ||
			   AttributeContent[i][198] == '\"')
			   fputc(AttributeContent[i][198], targetFile);//There is quotation.

			fputs(AttributeContent[i], targetFile);

			if(AttributeContent[i][198] == '\'' ||
			   AttributeContent[i][198] == '\"')
			   fputc(AttributeContent[i][198], targetFile);//There is quotation.
		}
		fputc('>', targetFile);//Close the tag
	}
	else//Simple tag or closing tag
	{
		fputc('>', targetFile);
	}
	return Yes;
}



//Write back current Tibetan font tag to target file
//With the font file number
int WriteBackTibetanFontTag()
{
	int i;

	if(TargetFileFormat != WebPage) return No;
	if(WayToChangeFontAttribute == 0) return No;//Don't write back tag when "nTibetan >= SZBuffer" occur.

	fputc('<', targetFile);
	fputs(CurrentTibetanFontTagName, targetFile);

	switch(WayToChangeFontAttribute)//Check to see what tag to be written back
	{
	case 1://It's <font...> tag
		fputs(" face=\"", targetFile);
		fputs(TibetanFontNames[BaseIndex[TargetEncoding]], targetFile);
		fputc('"', targetFile);
		fputs(" size=5>", targetFile);
		break;

	case 2://Write back font attributes in an inline style in the tag
		if(nCurrentTibetanFontTagAttributes > 0)
		{
			for(i=0; i<nCurrentTibetanFontTagAttributes; i++)
			{
				fputc(0x20, targetFile);

				if(	!strcmp(CurrentTibetanFontTagAttributeName[i], "class") ||
					!strcmp(CurrentTibetanFontTagAttributeName[i], "style") )
					//write inline style to file
				{
					fputs("style=\'font-size:", targetFile);
					fputs(currentWebFontInfo.fontSize, targetFile);
					fputs(";font-family:\"", targetFile);
					fputs(TibetanFontNames[BaseIndex[TargetEncoding]], targetFile);
					fputs("\"\'", targetFile);
					continue;
				}

				fputs(CurrentTibetanFontTagAttributeName[i], targetFile);
				fputc('=', targetFile);

				if(CurrentTibetanFontTagAttributeContent[i][198] == '\'' ||
				   CurrentTibetanFontTagAttributeContent[i][198] == '\"')
				   fputc(CurrentTibetanFontTagAttributeContent[i][198], targetFile);//There is quotation.

				fputs(CurrentTibetanFontTagAttributeContent[i], targetFile);

				if(CurrentTibetanFontTagAttributeContent[i][198] == '\'' ||
				   CurrentTibetanFontTagAttributeContent[i][198] == '\"')
				   fputc(CurrentTibetanFontTagAttributeContent[i][198], targetFile);//There is quotation.
			}//End of for()
			fputc('>', targetFile);//Close the tag
		}
		else//Simple style tag
		{
			fputc(0x20, targetFile);

			fputs("style=\'font-size:", targetFile);
			fputs(currentWebFontInfo.fontSize, targetFile);
			fputs(";font-family:\"", targetFile);
			fputs(TibetanFontNames[BaseIndex[TargetEncoding]], targetFile);
			fputs("\"\'>", targetFile);
		}
		break;

	default:
		fputc('>', targetFile);//Close the tag
		break;
	}//End of switch()

	WayToChangeFontAttribute = 0;//Reset this!

	return Yes;
}


int CheckSpace() //20100307
{
	int ch;

	if ((ch = getc(sourceFile)) == EOF)
        return EndOfFile;
	else if (ch == 0x20); //take off the space
	else ungetc(ch, sourceFile);

	return Yes;
}



//Convert Chinese characters between Unicode and CP936, and write back them
int
WriteBackChinese(void) //GB2312
{
	int i;
	int ch;

	if(holdTibetanBuffer == Yes)//Tibetan is held, process held Tibetan first.
	{
		holdTibetanBuffer = No;
		if(nTibetan>0) TranslateAndWriteBack();//Process Tibetan text
	}

	if(nFormatBuffer>0) WriteBackFormat();//Write back any format in the format buffer before writing Chinese

	if(nChinese<=0) return TextBufferEmpty;

	if(TargetFileFormat == RTF )
	{
		if(SourceFileFormat != RTF)
		{
			fputs("\\f31", targetFile); //why it's \f31   ?????????????
		}
	}

	if(TargetFileFormat == WebPage && SourceFileFormat != WebPage)
	{
		fputs("<font face=\"", targetFile);
		fputs(ChineseFontName, targetFile);
		fputs("\" size=5>", targetFile);
	}

	for(i=0; i<nChinese; i++)
	{
		ch = ChineseBuffer[i];

		if(ConversionDirection == UnicodeToOthers &&
			TargetEncoding != NS &&
			TargetFileFormat != UnicodeTXT)
		{
			if(Unicode2GB[ch] != 0) ch = Unicode2GB[ch];//Convert from Unicode to CP936 (GB2312)
		}
//		else if (ConversionDirection == OthersToUnicode && //  commented on 20181110 because it will generate unknown characters
//			SourceEncoding != NS &&
//			SourceFileFormat != UnicodeTXT)
//		{
//	    	if(GB2Unicode[ch] != 0) ch = GB2Unicode[ch];//Convert from CP936 (GB2312) to Unicode //  commented on 20181110 because it will generate unknown characters
//		}
		else;//No conversion.

		if(TargetFileFormat == UnicodeTXT)
		{
#ifdef Unix
			fputc((ch & 0xff), targetFile);
			fputc((ch>>8), targetFile);
#endif
#ifdef Windows
			if((ch & 0xff) != 0x0a)//For avoiding 0A -->  0D 0A  in Windows, throw away some Unicode Chinese characters. Not necessary in Linux //200806
			{
				fputc((ch & 0xff), targetFile);
				fputc((ch>>8), targetFile);
			}
			else//20090918
			{
				fputc('\n', targetFile);
				fputc((ch>>8), targetFile);
			}
#endif
		}
		else if(TargetFileFormat == TXT)
		{
			fputc((ch>>8), targetFile);
			fputc((ch & 0xff), targetFile);
		}
		else if(TargetFileFormat == RTF)
		{
			if(ConversionDirection == UnicodeToOthers &&
				TargetEncoding != NS )//UnicodeToOthers.  //Write back to ANSI file
			{
//				fputs("\\\'", targetFile);            //Output  \' for: GB2312 code such as \'B2\'D8
//				fprintf(targetFile, "%x", (ch >> 8));
//				fputs("\\\'", targetFile);            //Output  \' for: \'B2\'D8
//				fprintf(targetFile, "%x", (ch & 0xff));

				fputc((ch>>8), targetFile);  //20080229
				fputc((ch & 0xff), targetFile);
			}
			else //OthersToUnicode or TargetEncoding == NS
			{
				fputs("\\u", targetFile);  //Usualy there is \'3f before \u3954
				fprintf(targetFile, "%d", ch);
			}
		}
		else //(TargetFileFormat == WebPage )
		{
			if(ConversionDirection == UnicodeToOthers &&
				TargetEncoding != NS )//UnicodeToOthers. Write back to ANSI file
			{
				fputc((ch>>8), targetFile);
				fputc((ch & 0xff), targetFile);
			}
			else //OthersToUnicode
			{
				//Web page file format is ANSI
				fputs("&#", targetFile);
				fprintf(targetFile, "%d", ch);
				fputc(';', targetFile);

				//Web page itself's file format is Unicode.    i.e. Head by FF FE
//				fputc((ch & 0xff), targetFile);
//				fputc((ch>>8), targetFile);
			}
		}
	}

	if(TargetFileFormat == WebPage && SourceFileFormat != WebPage)
	{
		fputs("</font>", targetFile);
	}

	nChinese = 0;
	return TranslationDone;
}



int
WriteBackTibetanInAW(void) //Write back Tibetan that is in AW back to Unicode encoding
{
	int i;
	int ch;

	if(nTibetanInAW<=0) return TextBufferEmpty;

	if(TargetFileFormat == RTF )
	{
		if(SourceFileFormat != RTF)
		{
			fputs("\\f30", targetFile);
			fputs("\\fs16", targetFile);
		}
	}

	if(TargetFileFormat == WebPage && SourceFileFormat != WebPage)
	{
		fputs("<font face=\"", targetFile);
		fputs(TibetanUnicodeFontName, targetFile);
		fputs("\" size=5>", targetFile);
	}


	for(i=0; i<nTibetanInAW; i++)
	{
		ch = TibetanInAWBuffer[i];

		//Take off those "bad guys", they are not Chinese characters
		if(ch < 0xf00 || ch > 0xfff) continue;

		if(TargetFileFormat == UnicodeTXT)
		{
			fputc((ch & 0xff), targetFile);
			fputc((ch>>8), targetFile);
		}
		else if(TargetFileFormat == TXT)//shouldn't appear this case!!!
		{
			fputc((ch>>8), targetFile);
			fputc((ch & 0xff), targetFile);
		}
		else if(TargetFileFormat == RTF)
		{
			fputs("\\u", targetFile);
			fprintf(targetFile, "%d", ch);
		}
		else //(TargetFileFormat == WebPage )
		{
			fputs("&#", targetFile);
			fprintf(targetFile, "%d", ch);
			fputc(';', targetFile);
		}
	}

	if(TargetFileFormat == WebPage && SourceFileFormat != WebPage)
	{
		fputs("</font>", targetFile);
	}

	nTibetanInAW = 0;
	return TranslationDone;
}


void WriteBackFileNumberToTSLFile(int preFileNumber, int currentFileNumber)
{
	if(TargetFileFormat == RTF)
	{
		if(preFileNumber != currentFileNumber)
		{
			fputs("\\f", targetFile);

			if(SourceFileFormat == RTF) fprintf(targetFile, "%d", targetFontIDs[currentFileNumber]);
			else //SourceFileFormat == UnicodeTXT || SourceFileFormat == WebPage
			{
				fprintf(targetFile, "%d", BaseNumber + currentFileNumber);
			}

			fputc(0x20, targetFile);
		}
	}
	else if(TargetFileFormat == WebPage)
	{
		if(preFileNumber != currentFileNumber)
		{
			if(preFileNumber != -1) fputs("</font>", targetFile);
			fputs("<font face=\"", targetFile);
			fputs(TibetanFontNames[BaseIndex[TargetEncoding] + currentFileNumber], targetFile);
			fputs("\">", targetFile);
		}
	}
	else //TXT,  UnicodeTXT
	{
		//Do nothing!
	}
}


void WriteBackCharacterToTSLFile(int currentChar)
{
	if(TargetFileFormat == RTF)
	{
		if(currentChar=='\\' || currentChar=='{' || currentChar=='}')
			fputc('\\', targetFile);
		fputc(currentChar, targetFile);//Translating..........
	}
	else if(TargetFileFormat == WebPage)
	{
		if(currentChar==34 || currentChar==38 || currentChar==39 || currentChar==60 || currentChar==62 || currentChar > 0x7f) //20090218
		{
			fputs("&#", targetFile);
			fprintf(targetFile, "%d", (int)currentChar);
			fputc(';', targetFile);
		}
/*		else if(TargetEncoding == TCRCBodYig && currentChar >= 126 && currentChar <= 159)  //20090218
		{
			fputs("&#", targetFile);
			fprintf(targetFile, "%d", (int)currentChar);
			fputc(';', targetFile);
		}*/  //20090218
		else fputc(currentChar, targetFile);
	}
	else //TXT,  UnicodeTXT
	{
		//Do nothing. Never come to this block
	}

}


//Reselect characters for TCRC stacks
int CombinationForTCRC(int lastLetter, int currentLetter,
					   unsigned char *preChar, unsigned char *currentChar)
{
	unsigned char temp = 0;

	if(currentLetter==0xf74 || currentLetter==0xf71 || currentLetter==0xfb0 ||
			currentLetter==0xf75 || currentLetter==0xfad )//Low level vowel or subscript appear
	{
		if(*preChar < 0x20 || *preChar > 0xff) return No;

		switch(currentLetter)
		{
		case 0xf74:
			temp = TCRCvowelChart[*preChar][1];
			break;
		case 0xf71:
		case 0xfb0:
			temp = TCRCvowelChart[*preChar][2];
			break;
		case 0xfad:
			temp = TCRCvowelChart[*preChar][4];
			break;
		case 0xf75:
			temp = TCRCvowelChart[*preChar][3];
			break;
		default:
			break;
		}

		if((lastLetter==0xf71 || lastLetter==0xfb0) && currentLetter==0xf74)//A-Chung+Au
		{
			temp = *preChar + 18;
			*preChar = 0;//Don't output preChar.
		}

		if(temp != 0) *currentChar = temp;//Avoid to assign zero to the variable
	}
	else if( (lastLetter>=0xf40 && lastLetter<=0xf6a) &&
		(currentLetter>=0xf90 && currentLetter<=0xfbc) )//Two constants combine into a stack
	{
		if(TCRCconstantChart[lastLetter-0xf40][currentLetter-0xf90][0] != 0 &&
		   TCRCconstantChart[lastLetter-0xf40][currentLetter-0xf90][1] != 0 )
		{
			*preChar = TCRCconstantChart[lastLetter-0xf40][currentLetter-0xf90][0];
			*currentChar = TCRCconstantChart[lastLetter-0xf40][currentLetter-0xf90][1];
		}
	}
	else;//Do nothing!

	return Yes;
}



//Routine for picking up right combining font cell
int PickUpRightVowelForTSL(int currentLetter,
						   int lastLetter,
						   int baseLetter,
						   int preFileNumber,
						   unsigned char preChar,
						   int *currentFileNumber,
						   unsigned char *currentChar, //20090302
						   int nLastStack)   //20090302
{
	int fileNumber = 0;

	if(TargetEncoding == LTibetan || TargetEncoding == TCRCBodYig || TargetEncoding == Jamyang) //20090302
	{
		if(currentLetter == 0xf74)  //Shbuju
		{
			if(TargetEncoding == LTibetan)
			{
				if(nLastStack == 1) *currentChar = 0x24;
				else if(nLastStack == 2) *currentChar = 0x25;
				else  *currentChar = 0x26;
			}
			else if (TargetEncoding == TCRCBodYig)
			{
				if(nLastStack == 1) *currentChar = 0xde;
				else if(nLastStack == 2) *currentChar = 0xdd;
				else  *currentChar = 0x8d;
			}
			else //TargetEncoding == Jamyang
			{
				if(nLastStack == 1) *currentChar = 0x2f;
				else if(nLastStack == 2) *currentChar = 0x75;
				else  *currentChar = 0x5f;
			}
		}
		return TranslationDone;
	}//20090302

	if(currentLetter == 0xf72)//Gigu
	{
		if(baseLetter >= 0xf59 && baseLetter <= 0xf5c)
		{
			if(TargetEncoding == OldSambhota) *currentChar = *currentChar + 2; //Use the second one
			else *currentChar = *currentChar + 1; //Use the second one
		}
	}
	else if(currentLetter == 0xf74 && TargetEncoding != NewSambhota)//Shbuju, exclude NewSambhota
	{
		if(lastLetter != 0xf71 && lastLetter != 0xfb0)//Not Ar-shbuju. It's Shbuju.
		{
			fileNumber = *(pVowel + preFileNumber*256*6 + preChar*6);
			if(fileNumber >= 0 && fileNumber < 11) //Take off those exceptions!
			{
				*currentFileNumber = fileNumber;
				*currentChar = *(pVowel + preFileNumber*256*6 + preChar*6 + 1);
			}
		}
		else //It's Ar-Shbuju
		{
			fileNumber = *(pVowel + preFileNumber*256*6 + preChar*6 + 4);
			if(fileNumber >= 0 && fileNumber < 11) //Take off those exceptions!
			{
				*currentFileNumber = fileNumber;
				*currentChar = *(pVowel + preFileNumber*256*6 + preChar*6 + 5);
			}
		}
	}
	else if(currentLetter == 0xf7a)//Zhembu
	{
		if(baseLetter >= 0xf59 && baseLetter <= 0xf5c)
			*currentChar = *currentChar + 1; //Use the second one
	}
	else if(currentLetter == 0xf7c)//Naro
	{
		if(baseLetter >= 0xf59 && baseLetter <= 0xf5c)
			*currentChar = *currentChar + 1; //Use the second one
	}
	else if(currentLetter == 0xf7e)//Top Ma
	{
		if(baseLetter >= 0xf59 && baseLetter <= 0xf5c)
			*currentChar = *currentChar + 1; //Use the second one
	}
	else if((currentLetter == 0xf71 || currentLetter == 0xfb0) && TargetEncoding != NewSambhota)//Achung, exclude NewSambhota
	{
		 fileNumber = *(pVowel + preFileNumber*256*6 + preChar*6 + 2);
		 if(fileNumber >= 0 && fileNumber < 11) //Take off those exceptions!
		 {
			*currentFileNumber = fileNumber;
			*currentChar = *(pVowel + preFileNumber*256*6 + preChar*6 + 3);
		 }
	}
	else //Non-vowel letters
	{
		//Do nothing!
	}

	return TranslationDone;
}



//---------------------RTF Utilities----------------------------
int CollectKeyword()  //20080229
{
    int ch, anotherCh;
    char *pch;
    char fNeg = fFalse;

	param = 0;	//global
	fParam = fFalse;//global
    szKeyword[0] = '\0';	//global
    szParameter[0] = '\0';	//global

	if ((ch = getc(sourceFile)) == EOF)
        return EndOfFile;

	if (!isalpha(ch))  // a control symbol; no delimiter.
    {
		LastCh = ch;//Remeber the last character that is read in

        szKeyword[0] = (char) ch;
        szKeyword[1] = '\0';
        if(ch == '*') return SpecialControlWord;
		else if(ch == '\'') //20080229
		{
			nGBCode = 0;
#ifdef Windows	//20080229
			while(true)
#endif
#ifdef Unix
			while(1)
#endif
#ifdef Mac
			while(1)
#endif
			{
				if(nGBCode >= 2) break;
				ch = getc(sourceFile);
				if(ch == EOF) return EndOfFile;
				else if (isalpha(ch) || isdigit(ch)) GBCode[nGBCode++] = ch;
				else { ungetc(ch, sourceFile); break; }  //Some thing like '\'
			}
			if(nGBCode == 0) return ControlSymbol;
			else
			{
				GBCode[nGBCode] = '\0';
				return NonLatinCharacterCodeValue;
			}
		}
	    else return ControlSymbol;
    }

    for (pch = szKeyword; isalpha(ch); ch = getc(sourceFile))
	{
        *pch++ = (char) ch;
		LastCh = ch;
	}
    *pch = '\0';

	if (ch == '-')
    {
        fNeg  = fTrue;
        if ((ch = getc(sourceFile)) == EOF)
            return EndOfFile;
    }

	if (isdigit(ch))
    {
        fParam = fTrue; // a digit after the control means we have a parameter
        for (pch = szParameter; isdigit(ch); ch = getc(sourceFile))
		{
            *pch++ = (char) ch;
			LastCh = ch;
		}
        *pch = '\0';
        param = atoi(szParameter);
        if (fNeg)
            param = -param;
        lParam = atol(szParameter);
        if (fNeg)
			lParam = -lParam;
    }

	if(ch==0x20)//Special process for space following a key word
	{
		if(TargetFileFormat == RTF)//Take off space here for non RTF
		{
			ungetc(ch, sourceFile); //20100307
/*			if(header == Yes) ungetc(ch, sourceFile);
			else //header == No
			{
				if ((anotherCh = getc(sourceFile)) == EOF)
					return EndOfFile;
				else
				{
					if(anotherCh == '\\') //Assume another keyword is following
					//Take off space between two keywords
					{
						ungetc(anotherCh, sourceFile);//Don't write back the space.
					//	ungetc(ch, sourceFile);
					}
					else
					{	//Leave the space.
						ungetc(anotherCh, sourceFile);
						ungetc(ch, sourceFile);
					}
				}
			} */ //20100307
		}
	}
	else ungetc(ch, sourceFile);//ch != 0x20

    return ControlWord;
}


//Work on the buffer holding Tibetan characters to be translated.
int CollectKeywordFromFontBuffer(char *p)
{
    int ch;
	char fNeg = fFalse;
    char *pch;
	char *FontBuffer;

    fParam = fFalse;	//global
	param = 0;	//global
	FontBuffer = p;

    szKeyword[0] = '\0';	//global
    szParameter[0] = '\0';	//global

	if ((ch = *FontBuffer++) == '\0')
        return EndOfBuffer;

	if (!isalpha(ch))           // a control symbol; no delimiter.
    {
        szKeyword[0] = (char) ch;
        szKeyword[1] = '\0';
        if(ch == '*') return SpecialControlWord;
	    else return ControlSymbol;
    }

    for (pch = szKeyword; isalpha(ch); ch = *FontBuffer++)
        *pch++ = (char) ch;
    *pch = '\0';

	if (ch == '-')
    {
        fNeg  = fTrue;
        if ((ch = *FontBuffer++) == '\0')
            return EndOfBuffer;
    }

	if (isdigit(ch))
    {
        fParam = fTrue;         // a digit after the control means we have a parameter
        for (pch = szParameter; isdigit(ch); ch = *FontBuffer++)
            *pch++ = (char) ch;
        *pch = '\0';
        param = atoi(szParameter);
        if (fNeg)
            param = -param;
        lParam = atol(szParameter);
        if (fNeg)
            lParam = -lParam;
    }

    return ControlWord;
}


//---------------------------------Web Page----------------------------------
//This is for web page
int IsTibetanFontWebPage(void)
{
	int i;
	for (i=0;i<EncodingFontNumber[SourceEncoding]; i++)
	{
		if(!strcmp(TibetanFontNames[BaseIndex[SourceEncoding] + i], FontName)) return Yes;
	}
	return No;
}


int IsAnyTibetanFont(void)
{
	int i;
	for (i=0;i<TotalTibetanFontNumber; i++)
	{
		if(!strcmp(AllTibetanFontNames[i], FontName)) return Yes;
	}
	return No;
}


int
BufferControlWords(int ch) //Buffering the control word for Unicode2Others
{
	int nKeyword = 0;
	int nParameter = 0;

	if( !strcmp(szKeyword, "u") ||
		!strcmp(szKeyword, "f") ||
		!strcmp(szKeyword, "hich") || // TAKE OFF THEM!!!
		!strcmp(szKeyword, "loch") ||
		!strcmp(szKeyword, "dbch") ||
		!strcmp(szKeyword, "af") ) return 0;

	FormatBuffer[nFormatBuffer++] = ch;

	while(szKeyword[nKeyword]!='\0') FormatBuffer[nFormatBuffer++] = szKeyword[nKeyword++];

	if(fParam)
	{
		while(szParameter[nParameter]!='\0') FormatBuffer[nFormatBuffer++] = szParameter[nParameter++];
	}

	if(nFormatBuffer > SZBuffer-20)
	{
		if(nTibetan>0) TranslateAndWriteBack();
		if(nLatin>0) WriteBackLatin();
		if(nChinese>0) WriteBackChinese();
		if(nTibetanInAW>0) WriteBackTibetanInAW();

		WriteBackFormat();
	}

	return 0;
}


//See if a tag is defined in style sheet
int IsStyleSheetTag(char *tagName)
{
	int i;

	if(nStyleTags==0) return No;

	for(i=1; i<nStyleTags+1; i++)
	{
		if(!strcmp(StyleTags[i], tagName)) break;
	}
	if(i<nStyleTags+1) return Yes;//It's a style tag
	else return No;//No such tag
}


//Store font name of a style in "char FontName[]"
int StoreStyleFontNameInFontName(char *StyleTagName, char *StyleClassName)
{
	int tagNameIndex;
	int classNameIndex;
	int i;

	if(!strcmp(StyleTagName, "")) return No;//No tag name --> It's not possible.

	for(i=1; i<nStyleTags+1; i++)
	{
		if(!strcmp(StyleTags[i], StyleTagName)) break;
	}
	if(i<nStyleTags+1) tagNameIndex = i;
	else tagNameIndex = 0;//No such tag in style sheet --> All tags could be style tags

	if(strlen(StyleClassName)==0)//Empty string
	{
		classNameIndex = 0;
		if(!strcmp(FontNameInStyles[tagNameIndex][classNameIndex], "")) return No;
		strcpy(FontName, FontNameInStyles[tagNameIndex][classNameIndex]);
		return Yes;
	}

	for(i=1; i<nClassNames+1; i++)
	{
		if(!strcmp(ClassNames[i], StyleClassName)) break;
	}
	if(i<nClassNames+1)
	{
		classNameIndex = i;
		if(!strcmp(FontNameInStyles[tagNameIndex][classNameIndex], "")) return No;
		strcpy(FontName, FontNameInStyles[tagNameIndex][classNameIndex]);
		return Yes;
	}
	else return No;
}


//Parse a tag in web page. RETURN:	SimpleTag; ClosingTagFound; TagWithAttributes;
//									EndOfFile; CommentTagFound;
//If there is any attribute in the tag, then nAttributes > 0
int ParseTag()
{
	int iTagName;
	int ch, tempCH;
	int QuotationAppear;
	int ret = SimpleTag;//Assume the tag is a simple tag

	int iAttributeName = 0;
	int iAttributeContent = 0;
	int gotTagName = No;
	int CollectingAttributeName = Yes;
	int CollectingAttributeContent = No;

	QuotationAppear = No;
	iTagName = 0;
	nAttributes = 0;//This is very important!

	//start to parse the tag
	while ((ch = getc(sourceFile)) != EOF)//20080229
	{
		if(ch == '\n' || ch == 0x0d)
		{
			if(TargetFileFormat == WebPage) fputc(ch, targetFile); //20081124
		}

		if(gotTagName == No)
		{
			if(iTagName == 0)
			{
				if(ch == '!')
				{
					if((tempCH = getc(sourceFile)) != EOF)
					{
						if(tempCH=='-')
						{
							if((tempCH = getc(sourceFile)) != EOF)
							{
								ungetc(tempCH, sourceFile);
								ungetc('-', sourceFile);
								if(tempCH=='-')
								{
									return CommentTagFound;
								}
								else return DOCTYPETagFound;
							}
							else return EndOfFile;
						}
						else
						{
							ungetc(tempCH, sourceFile);
							return DOCTYPETagFound;
						}
					}
					else return EndOfFile;
				}
				else if(ch == 0x20); //Take off the space following '<'
				else if(ch == '/') //Closing tag appears
				{
					TagName[iTagName++] = ch;
					ret = ClosingTagFound;
				}
				else TagName[iTagName++] = ch;
			}
			else if(TagName[0] == '/' && ch == 0x20); //Take of the space following '/'
			else if(ch == 0x20 || ch == '>')//got the tag name
			{
				TagName[iTagName] = '\0';
				gotTagName = Yes;
				if(ch == '>') return ret;
			}
			else TagName[iTagName++] = ch;
		}
		else//Already got the tag name, start to parse the attributes.
		{
			if(CollectingAttributeName == Yes) //Collect Attribute name
			{
				if(ch=='>') return ret;
				else if(ch == 0x20 || ch == '\n') continue;
				else if(ch == '=' || iAttributeName > 48)
				{
					AttributeName[nAttributes][iAttributeName] = '\0';
					iAttributeName = 0; //Start to collect a new attribute
					CollectingAttributeName = No;//Start to collect attribute name
					QuotationAppear = No;//Set this at here!
					ret = TagWithAttributes;//Set this!!
				}
				else AttributeName[nAttributes][iAttributeName ++] = ch;
			}
			else //CollectingAttributeName == No -- collect Attribute content
			{
				if(CollectingAttributeContent == Yes)//Collecting attribute content
				{
					if( (ch == '\"' || ch == '\'') && ch != AttributeContent[nAttributes][198])
						AttributeContent[nAttributes][iAttributeContent ++] = ch;
					else if( ch == '\"' || ch == '\'' || ch == '>' || iAttributeContent > 197 ||
						   ( ch == 0x20 && QuotationAppear == No) ||
						   ( ch == '\n' && QuotationAppear == No) )//end the content
					{
						AttributeContent[nAttributes][iAttributeContent] = '\0';

						iAttributeContent = 0; //Start to collect a new attribute
						CollectingAttributeName = Yes;
						CollectingAttributeContent = No;

						nAttributes ++;//Increase the attribute number
						ret = TagWithAttributes;//Set this!!
						if(ch == '>') return ret;
					}
					else if(ch == '\n') ; //Take off '\n'
					else AttributeContent[nAttributes][iAttributeContent ++] = ch;
				}
				else //Not start to collect attribute content yet
				{
					if(ch == '\"' || ch == '\'')
					{
						QuotationAppear = Yes;
						CollectingAttributeContent = Yes;//Start the content

						AttributeContent[nAttributes][198] = ch;//Record the quotation in this slot!!!
					}
					else if(ch == 0x20 || ch == '\n') continue;
					else//Other characters
					{
						CollectingAttributeContent = Yes;
						AttributeContent[nAttributes][iAttributeContent ++] = ch;
						AttributeContent[nAttributes][198] = No;
					}
				}
			}
		}//End of parsing attributes
	}//End of while()

	return EndOfFile;
}


//Collect the info between style tags, build the style sheet
int CollectStyleSheetTagInfo()
{
	int i, j, k;
	int ch;
	int comment = No;
	int nTagAndClass = 0;
	int ret;
	int FoundPeriod = No;
	int gotFontFamilyAttribute = No;

	int iTagAndClass = 0;
	int iFontFamily = 0;
	int iFontFamilyName = 0;
	int CollectingTagAndClass = Yes;
	int CollectingFontFamilyName = No;

	char TagAndClass[10][100];
	char FontFamilyAttribute[100];
	char FontFamilyName[100];

	char TheTag[50];
	char TheClass[50];

	//Get the tag names, their classes, and font-family names
	while ((ch = getc(sourceFile)) != EOF)
	{
		if(ch == '*')
		{
			if(TargetFileFormat == WebPage) fputc(ch, targetFile);
			ch = getc(sourceFile);//get one more character
			if(ch == '/')
			{
				if(TargetFileFormat == WebPage) fputc(ch, targetFile);
				comment = No;//End of a comment /* ..... */
			}
			else ungetc(ch, sourceFile);
			continue;
		}
		else if(comment == Yes)
		{
			if(TargetFileFormat == WebPage) fputc(ch, targetFile);
			continue;
		}
		else if(ch=='<')
		{
			if(TargetFileFormat == WebPage) fputc(ch, targetFile);

			ch = getc(sourceFile);
			if(ch != '!') //other tag
			{
				ungetc(ch, sourceFile);
				ret = ParseTag();
				if(ret == ClosingTagFound)
				{
					WriteBackTag();
					if(!strcmp(TagName, "/style")) return ret;//End of the style sheet!!
					else continue;//Other tag found!
				}
				else { WriteBackTag(); continue;} //Other tag found!
			}

			//Found comment tag!!  <!-- ...
			do
			{
				if(TargetFileFormat == WebPage) fputc(ch, targetFile);
				ch = getc(sourceFile);
			}while(ch=='-' || ch==0x20 || ch=='\n');
			ungetc(ch, sourceFile);
			continue;
		}
		else if(ch=='>') //The end of the style sheet
		{
			if(TargetFileFormat == WebPage) fputc('>', targetFile);
			return Yes;
		}
		else if(ch == 0x0d || ch == 0x0a || ch == '\n')
		{
			if(TargetFileFormat == WebPage) fputc(ch, targetFile);
			continue; // Ignore line breaker
		}
		else if(ch < 0x20)// Ignore these characters
		{
			if(TargetFileFormat == WebPage) fputc(ch, targetFile);
			continue;
		}
		else if(ch == '/')//Comment appears
		{
			if(TargetFileFormat == WebPage) fputc(ch, targetFile);
			ch = getc(sourceFile);
			if(ch == '*')
			{
				if(TargetFileFormat == WebPage) fputc(ch, targetFile);
				comment = Yes;//Start of a comment
			}
			else ungetc(ch, sourceFile);
			continue;
		}
		else ; //Go down there!!!


		if(CollectingTagAndClass == Yes) //Collect Tag And Class
		{
			if(ch=='{')
			{
				TagAndClass[nTagAndClass][iTagAndClass] = '\0';
				if(nTagAndClass < 9) nTagAndClass ++;
				iTagAndClass = 0;
				CollectingTagAndClass = No;//End of collecting TagAndClass.
			}
			else if(ch == 0x20);
			else if(ch == ',' || iTagAndClass > 88)
			{
				TagAndClass[nTagAndClass][iTagAndClass] = '\0';
				iTagAndClass = 0; //Start to collect a new TagAndClass
				if(nTagAndClass < 9) nTagAndClass ++;
			}
			else
			{
				TagAndClass[nTagAndClass][iTagAndClass ++] = ch;
			}

			if(TargetFileFormat == WebPage) fputc(ch, targetFile);//?????????????????????
		}
		else //CollectingTagAndClass == No -- collect font-family name
		{
			if(CollectingFontFamilyName == Yes)//Collecting font-family name
			{
				if(ch == '\"') {if(TargetFileFormat == WebPage) fputc(ch, targetFile);}
				else if(ch == 0x20 && iFontFamilyName == 0)// Ignore leading space
				{
					if(TargetFileFormat == WebPage) fputc(ch, targetFile);
				}
				else if(ch == ';') //Got the font-family name
				{
					FontFamilyName[iFontFamilyName] = '\0';
					CollectingFontFamilyName = No; //collect font-family attribute
					if(TargetFileFormat == WebPage) fputc(ch, targetFile);
					iFontFamilyName = 0;

					if(gotFontFamilyAttribute == Yes)//Collect font style info
					{
						for(i=0; i<nTagAndClass; i++)//Set up the style sheet
						{
							j = 0; FoundPeriod = No; k = 0;

							do
							{
								if(TagAndClass[i][j] == '.') { TheTag[k] = '\0'; FoundPeriod = Yes; k = 0; }
								else if(FoundPeriod == No) TheTag[k++] = TagAndClass[i][j];
								else if(FoundPeriod == Yes) TheClass[k++] = TagAndClass[i][j];
								else ;
							}while(TagAndClass[i][j++] != '\0');

							if(FoundPeriod == Yes) TheClass[k] = '\0';
							else TheTag[k] = '\0';

							//Remember the style tag if it's new
							for(j=0; j<nStyleTags; j++)
							{
								if(!strcmp(StyleTags[j], TheTag)) break;
							}
							if(j==nStyleTags)//New tag!
							{
								strcpy(StyleTags[j], TheTag);
								if(nStyleTags<48) nStyleTags ++;
							}

							if(FoundPeriod == Yes)
							{
								//Remember the class if it's new
								for(k=1; k<nClassNames+1; k++)
								{
									if(!strcmp(ClassNames[k], TheClass)) break;
								}
								if(k==nClassNames+1)//New tag!
								{
									strcpy(ClassNames[k], TheClass);
									if(nClassNames<48) nClassNames ++;
								}
							}
							else k = 0;

							strcpy(FontNameInStyles[j][k], FontFamilyName);//Store the font family name.
						}
						gotFontFamilyAttribute = No; //Reset this
					}
				}
				else
				{
					if(iFontFamilyName < 28) FontFamilyName[iFontFamilyName ++] = ch;
					if(TargetFileFormat == WebPage) fputc(ch, targetFile);
				}
			}
			else //CollectingFontFamilyName = No,  collect font-family attribute
			{
				if(ch == ':') //Got the font-family attribute
				{
					FontFamilyAttribute[iFontFamily] = '\0';
					CollectingFontFamilyName = Yes; //collect font-family name
					iFontFamily = 0;

					if(!strcmp(FontFamilyAttribute, "font-family")) gotFontFamilyAttribute = Yes;
				}
				else if(ch == 0x20 || ch<0x20 || ch == '\n'); // Ignore space
				else if(ch == '}') //Out of the block. Start to collect new style.
				{
					CollectingTagAndClass = Yes;
					CollectingFontFamilyName = No;
					nTagAndClass = 0;

					gotFontFamilyAttribute = No;//Reset this!
				}
				else
				{
					if(iFontFamily < 28) FontFamilyAttribute[iFontFamily ++] = ch;
				}

				if(TargetFileFormat == WebPage) fputc(ch, targetFile);
			}
		}
	}//End of while()
	return EndOfFile;
}



//Collect the style sheet info from external style sheet file, build the style sheet
int CollectStyleSheetTagInfoFromExternalFile()
{
	int i, j, k;
	int ch;
	int comment = No;
	int nTagAndClass = 0;
	int ret;
	int FoundPeriod = No;
	int gotFontFamilyAttribute = No;

	int iTagAndClass = 0;
	int iFontFamily = 0;
	int iFontFamilyName = 0;
	int CollectingTagAndClass = Yes;
	int CollectingFontFamilyName = No;

	char TagAndClass[10][100];
	char FontFamilyAttribute[100];
	char FontFamilyName[100];

	char TheTag[50];
	char TheClass[50];

	//Get the tag names, their classes, and font-family names
	while ((ch = getc(stylesheetFile)) != EOF)
	{
		if(ch == '*')
		{
			ch = getc(stylesheetFile);
			if(ch == '/') comment = No;//End of a comment
			else ungetc(ch, stylesheetFile);
			continue;
		}
		else if(comment == Yes) continue;
		else if(ch=='<')
		{
			ch = getc(stylesheetFile);
			if(ch != '!')
			{
				ungetc(ch, stylesheetFile);
				ret = ParseTag();
				if(ret == ClosingTagFound)
				{
					if(!strcmp(TagName, "/style")) return ret;//End of the style sheet!!
					else continue;//Other tag found!
				}
				else continue;//Other tag found!
			}

			//Found comment tag!!  <!-- ...
			do
			{
				ch = getc(sourceFile);
			}while(ch=='-' || ch==0x20 || ch=='\n');
			ungetc(ch, stylesheetFile);
			continue;
		}
		else if(ch=='>') return Yes;//The end of the style sheet
		else if(ch == 0x0d || ch == 0x0a || ch == '\n') continue;// Ignore line breaker
		else if(ch < 0x20) continue; // Ignore these characters
		else if(ch == '/')//Comment appears
		{
			ch = getc(stylesheetFile);
			if(ch == '*') comment = Yes;//Start of a comment
			else ungetc(ch, stylesheetFile);
			continue;
		}
		else ; //Go down there!!!


		//Collect Tag And Class
		if(CollectingTagAndClass == Yes)
		{
			if(ch=='{')
			{
				TagAndClass[nTagAndClass][iTagAndClass] = '\0';
				if(nTagAndClass < 9) nTagAndClass ++;
				iTagAndClass = 0;
				CollectingTagAndClass = No;//End of collecting TagAndClass.
			}
			else if(ch == 0x20);
			else if(ch == ',' || iTagAndClass > 88)
			{
				TagAndClass[nTagAndClass][iTagAndClass] = '\0';
				iTagAndClass = 0; //Start to collect a new TagAndClass
				if(nTagAndClass < 9) nTagAndClass ++;
			}
			else
			{
				TagAndClass[nTagAndClass][iTagAndClass ++] = ch;
			}
		}
		else //CollectingTagAndClass == No -- collect font-family name
		{
			if(CollectingFontFamilyName == Yes)//Collecting font-family name
			{
				if(ch == '\"') ;
				else if(ch == 0x20 && iFontFamilyName == 0) ; // Ignore leading space
				else if(ch == ';' || ch == '}') //Got the font-family name
				{
					FontFamilyName[iFontFamilyName] = '\0';
					CollectingFontFamilyName = No; //collect font-family attribute
					iFontFamilyName = 0;

					if(gotFontFamilyAttribute == Yes)//Collect font style info
					{
						for(i=0; i<nTagAndClass; i++)//Set up the style sheet
						{
							j = 0; FoundPeriod = No; k = 0;

							do
							{
								if(TagAndClass[i][j] == '.') { TheTag[k] = '\0'; FoundPeriod = Yes; k = 0; }
								else if(FoundPeriod == No) TheTag[k++] = TagAndClass[i][j];
								else if(FoundPeriod == Yes) TheClass[k++] = TagAndClass[i][j];
								else ;
							}while(TagAndClass[i][j++] != '\0');

							if(FoundPeriod == Yes) TheClass[k] = '\0';
							else TheTag[k] = '\0';

							if(strcmp(TheTag, "") != 0)//TheTag is not empty string
							{
								//Remember the style tag if it's new
								for(j=1; j<nStyleTags+1; j++)
								{
									if(!strcmp(StyleTags[j], TheTag)) break;
								}
								if(j==nStyleTags+1)//New tag!
								{
									strcpy(StyleTags[j], TheTag);
									if(nStyleTags<48) nStyleTags ++;
								}
							}
							else //TheTag is a empty string --> No tag is assigned --> Aplly to all tags.
							{
								j = 0;//Aplly to all tags
							}

							if(FoundPeriod == Yes)
							{
								//Remember the class if it's new
								for(k=1; k<nClassNames+1; k++)
								{
									if(!strcmp(ClassNames[k], TheClass)) break;
								}
								if(k==nClassNames+1)//New tag!
								{
									strcpy(ClassNames[k], TheClass);
									if(nClassNames<48) nClassNames ++;
								}
							}
							else k = 0;

							strcpy(FontNameInStyles[j][k], FontFamilyName);//Store the font family name.
						}
						gotFontFamilyAttribute = No; //Reset this
					}

					if(ch == '}') //Out of the block. Start to collect new style.
					{
						CollectingTagAndClass = Yes;
						CollectingFontFamilyName = No;
						nTagAndClass = 0;

						gotFontFamilyAttribute = No;//Reset this!
					}
				}
				else
				{
					if(iFontFamilyName < 28) FontFamilyName[iFontFamilyName ++] = ch;
				}
			}//if(CollectingFontFamilyName == Yes)
			else //CollectingFontFamilyName = No,  collect font-family attribute
			{
				if(ch == ':') //Got the font-family attribute
				{
					FontFamilyAttribute[iFontFamily] = '\0';
					CollectingFontFamilyName = Yes; //collect font-family name
					iFontFamily = 0;

					if(!strcmp(FontFamilyAttribute, "font-family")) gotFontFamilyAttribute = Yes;
				}
				else if(ch<=0x20 || ch == '\n'); // Ignore space
				else if(ch == '}') //Out of the block. Start to collect new style.
				{
					CollectingTagAndClass = Yes;
					CollectingFontFamilyName = No;
					nTagAndClass = 0;

					gotFontFamilyAttribute = No;//Reset this!
				}
				else
				{
					if(iFontFamily < 28) FontFamilyAttribute[iFontFamily ++] = ch;
				}
			}//End of else //CollectingFontFamilyName = No
		}//End of else //CollectingTagAndClass == No
	}//End of while()
	return EndOfFile;
}


//Collect the info from external style sheet file -->styleSheetFileName
int CollectExternalStyleSheetInfo()
{
	stylesheetFile = fopen(styleSheetFileName, "rb");//Open the style sheet file
	if (!stylesheetFile) return NoStyleSheetFile;

	CollectStyleSheetTagInfoFromExternalFile();

	fclose(stylesheetFile);

	return Yes;
}



//Locate the tag "<body>" by reading in and process the content in <head> </head>
int LocateBodyTag()   //20080229
{
	int ch, i, j;
	int ret;

	nClassNames = 0;
	nStyleTags = 0;

	for(i=0;  i<50; i++)//Initializing this!
	{
		for(j=0; j<50; j++) strcpy(FontNameInStyles[i][j], "");
	}

	while ((ch = getc(sourceFile)) != EOF)
	{
		if(ch == '<')
		{
			ret = ParseTag();

			if(ret == SimpleTag)
			{
				if(!strcmp(TagName, "body"))
				{
					WriteBackTag();

					//If there is style sheet file, read it!!
					if(strcmp(styleSheetFileName, "null") != 0) CollectExternalStyleSheetInfo();

					return BodyTagFound;
				}
				else if(!strcmp(TagName, "style"))
				{
					WriteBackTag();
					CollectStyleSheetTagInfo();
				}
				else WriteBackTag();
			}
			else if(ret == ClosingTagFound) WriteBackTag();
			else if(ret == TagWithAttributes)
			{
				if(!strcmp(TagName, "body"))
				{
					WriteBackTag();

					//If there is style sheet file, read it!!
					if(strcmp(styleSheetFileName, "null") != 0) CollectExternalStyleSheetInfo();

					return BodyTagFound;
				}
				else WriteBackTag();
			}
			else if(ret == DOCTYPETagFound) WriteBackDOCTYPETag();//20080229
			else if(ret == CommentTagFound) WriteBackCommentTag();
			else if(ret == EndOfFile) return ret;
			else ;//Ignore this tag.
		}
		else
		{
			if(TargetFileFormat == WebPage) fputc(ch, targetFile);
		}
	} //End of while()

	return EndOfFile;
}


//Check the attribute buffer to see if there is class attribute
//if Yes, put the class name into ClassName[] global
int IsThereClassInAttributeBuffer()
{
	int i;

	for(i=0; i<nAttributes; i++)
	{
		if(!strcmp(AttributeName[i], "class"))
		{
			strcpy(ClassName, AttributeContent[i]);
			return Yes;
		}
	}
	return No;
}


//get the font attributes in font tag <font ....>: face & size
//Return Yes if there is any font attribute in the tag, otherwise No
int FontAttributesOfFontTag()
{
	int i;
	int ret = No;

	if(strcmp(TagName, "font") != 0) return No;

	for(i=0; i<nAttributes; i++)
	{
		if(!strcmp(AttributeName[i], "face"))
		{
			strcpy(FontName, AttributeContent[i]);
			ret = Yes;
		}
		else if(!strcmp(AttributeName[i], "size"))
		{
			strcpy(FontSize, AttributeContent[i]);
			ret = Yes;
		}
		else ret = No;
	}
	return ret;
}


//Is there inline style sheet in current tag?
//return the index of style attribute in the attributes
//the buffer if yes, otherwise return -1.
int InlineStyleAppear()
{
	int i;

	for(i=0; i<nAttributes; i++)
	{
		if(!strcmp(AttributeName[i], "style"))
		{
			return i;
		}
	}
	return -1;
}


//Get the font attributes in inline style sheet.
int GetFontAttributesInInlineStyleSheet(int theIndex)
{
	int i=0, j=0;
	int ret = No;
	int WorkingOnAttributeName = Yes;
	char theAttribute[50];
	char theContent[50];

	while(AttributeContent[theIndex][i] != '\0' && i < 198)
	{
		if(WorkingOnAttributeName == Yes)
		{
			if(AttributeContent[theIndex][i]==0x20);
			else if(AttributeContent[theIndex][i]==':')//End of first term
			{
				WorkingOnAttributeName = No;
				theAttribute[j] = '\0';
				j = 0;//Reset this!
			}
			else theAttribute[j++] = AttributeContent[theIndex][i];

			i ++;
		}
		else //WorkingOnAttributeName == No --> WorkingOnAttributeContent == Yes
		{
			if((AttributeContent[theIndex][i]==0x20 ||
				AttributeContent[theIndex][i]=='\"' ||
				AttributeContent[theIndex][i]=='\'') && j==0);//Take off them
			else if(AttributeContent[theIndex][i]=='\"');//Take off the second '"'
			else if(AttributeContent[theIndex][i]==';' || AttributeContent[theIndex][i]=='\'')//End of the term
			{
				WorkingOnAttributeName = Yes;
				theContent[j] = '\0';
				j = 0;//Reset this!

				if(!strcmp(theAttribute, "font-size"))
				{
					strcpy(FontSize, theContent);
					ret = Yes;
				}
				else if(!strcmp(theAttribute, "font-family"))
				{
					strcpy(FontName, theContent);
					ret = Yes;
				}
				else ;
			}
			else theContent[j++] = AttributeContent[theIndex][i];

			i ++;
		}
	}//End of while()

	if(AttributeContent[theIndex][i] == '\0')//Clean up this!
	{
		theContent[j] = '\0';

		if(!strcmp(theAttribute, "font-size"))
		{
			strcpy(FontSize, theContent);
		}
		else if(!strcmp(theAttribute, "font-family"))
		{
			strcpy(FontName, theContent);
		}
		else ;
	}

	return ret;
}



//Look at the tag, see if the tag changes any font attributes
//Record the font attributes that is changed
int DoesTheTagChangeTextFontAttribute()
{
	int i;
	int ret = No;

	if(!strcmp(TagName,"font") && nAttributes > 0)//It's font tag. <font ....>
	{
		if(FontAttributesOfFontTag()==Yes)//Set face and size attributes of <font> tag
		{
			WayToChangeFontAttribute = 1;
			return Yes;//Process font tag. Record font attributes: face & size
		}
		else ret = No;
	}

	if((i=InlineStyleAppear()) != -1)//There is inline style in the tag.
	{
		if(GetFontAttributesInInlineStyleSheet(i) == No) ret = No;
		else
		{
			WayToChangeFontAttribute = 2;
			return Yes;
		}
	}

	if(IsStyleSheetTag(TagName) == Yes)//A style sheet tag found!!
	{
		if(IsThereClassInAttributeBuffer()==Yes)//style tag has a class
		{
			if(StoreStyleFontNameInFontName(TagName, ClassName)==Yes)
			{
				ClassName[0] = '\0';//Reset this!
				WayToChangeFontAttribute = 2;
				return Yes;//A style tag that has font has been found
			}
			else ret = No;
		}
		else //No class name is in this style
		{
			if(StoreStyleFontNameInFontName(TagName, "")==Yes)//No class name
			{
				WayToChangeFontAttribute = 2;
				return Yes;
			}
			else ret = No;
		}
	}
	else // if(IsStyleSheetTag(TagName) == No)//Not a style sheet tag --> see if it has a class.
	{
		if(IsThereClassInAttributeBuffer()==Yes)//style tag has a class
		{
			if(StoreStyleFontNameInFontName(TagName, ClassName)==Yes)
			{
				ClassName[0] = '\0';//Reset this!
				WayToChangeFontAttribute = 2;
				return Yes;//A style tag that has font has been found
			}
			else ret = No;
		}
		else //No class name is in this style --> not a font related tag!
		{
			ret = No;
		}
	}

	return ret;
}


//Modify currentWebFontInfo
void ChangeCurrentFontInfo()
{
	if(FontName[0] != '\0' || FontSize[0] != '\0')
	{
		if(PushWebFontinfoOnStack()!= PUSHED) return;

		strcpy(currentWebFontInfo.tagName, TagName);//Remeber the tag

		if(FontName[0] != '\0')
		{
			strcpy(currentWebFontInfo.fontName, FontName);
			if(SourceEncoding == ACIP || SourceEncoding == THDLWylie || SourceEncoding == Wylie || SourceEncoding == LCWylie)
			{	//Special for AW to pick up western fonts as Tibetan "font"
				if(IsAnyTibetanFont() == Yes) currentWebFontInfo.isTibetan = No;
				else currentWebFontInfo.isTibetan = Yes;
			}
			else
			{
				if(IsTibetanFontWebPage() == Yes) currentWebFontInfo.isTibetan = Yes;
				else currentWebFontInfo.isTibetan = No;
			}
		}
		if(FontSize[0] != '\0')
			strcpy(currentWebFontInfo.fontSize, FontSize);

		FontName[0] = '\0';//Reset these!
		FontSize[0] = '\0';
	}
}


void BufferCurrentTibetanFontTagInfo()
{
	int i;

	if(holdTibetanBuffer == Yes) return;//Keep the first tag in memory.

	strcpy(CurrentTibetanFontTagName, TagName);
	for(i=0; i<nAttributes; i++)
	{
		strcpy(CurrentTibetanFontTagAttributeName[i], AttributeName[i]);
		strcpy(CurrentTibetanFontTagAttributeContent[i], AttributeContent[i]);
		CurrentTibetanFontTagAttributeContent[i][198] = AttributeContent[i][198];
	}
	nCurrentTibetanFontTagAttributes = nAttributes;
}


//Check to see if current tag can cause a line break.
//Return Yes if yes. Otherwise no.
int LineBreakTag()
{
	if( !strcmp(TagName, "br") ||
		!strcmp(TagName, "p") ||
		!strcmp(TagName, "/p") ||
		!strcmp(TagName, "/div") )
		return Yes;
	else return No;
}



//Parse HTML web page file after body tag
int parseWEBPAGE()//20080229
{
	int ret, m, n = 0;
	int ch;
	char temp[50];
	int HighBits = 0;

	nTibetan = 0;
	nLatin = 0;
	nChinese = 0;
	nTibetanInAW = 0;
	stackIndex = 0;

	nFormatBuffer = 0;
	holdTibetanBuffer = No;

	//Set up default font info;
	strcpy(deffWebFontInfo.tagName, "deffTag");//set this for deff tag.
	strcpy(deffWebFontInfo.fontName, "Times New Roman");
	strcpy(deffWebFontInfo.fontSize, "20.0pt");
	deffWebFontInfo.isTibetan = No;

	strcpy(FontName, "Times New Roman");
	strcpy(FontSize, "20.0pt");

	//Set up current font info, it's default font
	currentWebFontInfo = deffWebFontInfo;//???????????????????????

	//Body-tag found!
	while ((ch = getc(sourceFile)) != EOF)
	{
		//Non-Unicode characters ---ASCII
		if(ch=='<' && HighBits==0)
		{
			if(nTibetan>0) TranslateAndWriteBack();
			if(nTibetanInAW>0) WriteBackTibetanInAW();
			if(nLatin>0) WriteBackLatin();
			if(nChinese>0) WriteBackChinese();

			ret = ParseTag();//Process a tag

			//Translate the line breaker
			if(LineBreakTag() == Yes && TargetFileFormat != WebPage)
			{
				if(nLatin >= SZBuffer) WriteBackLatin();
				LatinBuffer[nLatin++] = '\n';
			}

			if(ret == CommentTagFound) WriteBackCommentTag();
			else if(ret == TagWithAttributes) //TagWithAttributes
			{
				if(DoesTheTagChangeTextFontAttribute()==No) WriteBackTag();
				else//Font name is put in FontName[], font size is put in FontSize[]
				{
					ChangeCurrentFontInfo();
					if(currentWebFontInfo.isTibetan == Yes)
						BufferCurrentTibetanFontTagInfo();//Don't write back the tag
					else WriteBackTag();
				}
			}
			else if(!strcmp(TagName, "sub") || !strcmp(TagName, "/sub") ||
					!strcmp(TagName, "sup") || !strcmp(TagName, "/sup") ) ;//Take off them!
			else if(ret == ClosingTagFound)  // Such as </font>
			{
				WriteBackTag();
				PopWebFontinfoFromStack();
			}
			else if(ret == EndOfFile) return ret;
			else //Assume all are SimpleTags
			{
				if(IsStyleSheetTag(TagName)==No) WriteBackTag();
				else //It's a style sheet tag with a font attribute!!!
				{
					StoreStyleFontNameInFontName(TagName, "");//Get the FontName[]
					WayToChangeFontAttribute = 2;
					ChangeCurrentFontInfo();
					if(currentWebFontInfo.isTibetan == Yes)
						BufferCurrentTibetanFontTagInfo();
					else WriteBackTag();
				}
			}

			//For buffering format!
			if(ret == ClosingTagFound && holdTibetanBuffer == Yes)
			{
				strcpy(lastClosingTag, TagName);//Remember the closing tag.
			}
			else strcpy(lastClosingTag, "");//Reset it!
		}

		else if(ch==0x0d || ch=='\n')//New line, they are no use here, take off them by do nothing.
		{
			if((TargetFileFormat == WebPage || TargetFileFormat == RTF)
				&& nTibetan==0 && nLatin==0 && nChinese==0) fputc(ch, targetFile);//20081124
		}

		else //Text appears
		{
			//Take care of wide characters   GB2312 or CP936 //20080229
			if(HighBits == 0)
			{
				if(ch > 0x80) { HighBits = ch;  continue; }
			}
			else //get the wide chatacter
			{
				ch = (HighBits<<8) + ch;
				HighBits = 0;
			}

			//Work on Unicode characters and special characters.
			if(ch == '&' && HighBits==0)
			{
				if ((ch = getc(sourceFile)) == EOF) break;//Read in one more

				if(ch == '#')//Work on Unicode character.
				{
					m = 0;

	AnotherDigit:	if ((ch = getc(sourceFile)) == EOF) break;//Read in one more

					if(isdigit(ch))//12345
					{
						temp[m++] = ch;
						goto AnotherDigit;
					}
					else if(m==0)//Not a Unicode character
					{
						ungetc(ch, sourceFile);
						ungetc('#', sourceFile);
						ch = '&';
					}
					else //It's a Unicode character
					{
						temp[m] = '\0';
						if(ch != ';') ungetc(ch, sourceFile);
						ch = atoi(temp);
					}
				}
				else if(isalpha(ch))//abcdef
				{
					m = 0;

					do
					{
						temp[m++] = ch;
						if ((ch = getc(sourceFile)) == EOF) break;//Read in one more
					} while(isalpha(ch));

					temp[m] = '\0';

					if(TargetFileFormat != WebPage) ch = CharacterName2ItsValue(temp);
					else if(currentWebFontInfo.isTibetan == No) //TargetFileFormat == WebPage
					{
						if(nTibetan>0) TranslateAndWriteBack();
						if(nChinese>0) WriteBackChinese();
						if(nLatin>0) WriteBackLatin();

						fputc('&', targetFile);
						fputs(temp, targetFile);
						fputc(ch, targetFile);
						continue;
					}
					else //(currentWebFontInfo.isTibetan == Yes)
					{
						ch = CharacterName2ItsValue(temp);
					}
				}
				else //;',./-=":?
				{
					ungetc(ch, sourceFile);
					ch = '&';
				}
			}

			if(currentWebFontInfo.isTibetan == Yes) //Tibetan text
			{
				if(nTibetanInAW>0) WriteBackTibetanInAW();
				if(nLatin>0) WriteBackLatin();
				if(nChinese>0) WriteBackChinese();

				if(nTibetan >= SZBuffer)	//Prevent from out of boundry error.
					TranslateAndWriteBack();

				strcpy(TibetanBufferForWebPage[nTibetan].fontSize, currentWebFontInfo.fontSize);
				strcpy(TibetanBufferForWebPage[nTibetan].fontName, currentWebFontInfo.fontName);
				TibetanBufferForWebPage[nTibetan++].ch = ch;

				if(SourceEncoding==Wylie || SourceEncoding==THDLWylie || SourceEncoding==LCWylie || SourceEncoding==ACIP)
				{
					if(IsPunctuation(ch)==Yes) TranslateAndWriteBack();//For AW 2 Unicode. One sentence is encounted;
				}

				if(SourceEncoding == Unicode)//Only apply to Unicode 2 others
				{
					if(ch != 0xf0d)//Hold on the format!!
					{
						if(nFormatBuffer>0 && holdTibetanBuffer == No)//Format buffer is not empty, write back them
						{
							if(nTibetan>0) TranslateAndWriteBack();
							if(nLatin>0) WriteBackLatin();
							if(nChinese>0) WriteBackChinese();
							if(nFormatBuffer>0) WriteBackFormat();
						}
						holdTibetanBuffer = Yes; //Hold on!!
					}
					else holdTibetanBuffer = No;
				}
			}
			else if(ch>=0xf00 && ch<=0xfcf) //Tibetan Unicode
			{
				if(SourceEncoding == ACIP || SourceEncoding == THDLWylie || SourceEncoding == Wylie || SourceEncoding == LCWylie)
				{
					if(nTibetan>0) TranslateAndWriteBack();
					if(nLatin>0) WriteBackLatin();
					if(nChinese>0) WriteBackChinese();

					if(nTibetanInAW >= SZBuffer) WriteBackTibetanInAW();
					TibetanInAWBuffer[nTibetanInAW++] = ch;
				}
				else
				{
					if(nLatin>0) WriteBackLatin();
					if(nChinese>0) WriteBackChinese();
					if(nTibetanInAW>0) WriteBackTibetanInAW();

					if(nTibetan >= SZBuffer)	//Prevent from out of boundry error.
						TranslateAndWriteBack();

					TibetanBufferForWebPage[nTibetan ++].ch = ch;
				}

				if(SourceEncoding == Unicode)//Only apply to Unicode 2 others
				{
					if(ch != 0xf0d)//Hold on the format!!
					{
						if(nFormatBuffer>0 && holdTibetanBuffer == No)//Format buffer is not empty, write back them
						{
							if(nTibetan>0) TranslateAndWriteBack();
							if(nLatin>0) WriteBackLatin();
							if(nChinese>0) WriteBackChinese();
							if(nFormatBuffer>0) WriteBackFormat();
						}
						holdTibetanBuffer = Yes; //Hold on!!
					}
					else holdTibetanBuffer = No;
				}
			}
			else if(ch >= 0x00 && ch <= 0x7f)//Non-Tibetan text------ASCII  //20080229
			{
				holdTibetanBuffer = No;

				if(nTibetanInAW>0) WriteBackTibetanInAW();
				if(nChinese>0) WriteBackChinese();
				if(nTibetan>0) TranslateAndWriteBack();

				if(nLatin >= SZBuffer) WriteBackLatin();
				LatinBuffer[nLatin++] = ch;
			}
			else //Chinese
			{
				holdTibetanBuffer = No;

				if(nTibetan>0) TranslateAndWriteBack();
				if(nLatin>0) WriteBackLatin();
				if(nTibetanInAW>0) WriteBackTibetanInAW();

				if(nChinese >= SZBuffer) WriteBackChinese();
				ChineseBuffer[nChinese++] = ch;
			}

			if(TargetFileFormat == WebPage || TargetFileFormat == RTF)
			{	//For inserting line breakers   //20080229
				n ++;
				if(n>100){ n = 0; fputc('\n',targetFile); }//For inserting the line breaker.
			}

		} //End of  else //Text
	} //End of while()
	if(nTibetanInAW>0) WriteBackTibetanInAW();
//	if(nChinese>0) WriteBackChinese();
	if(nTibetan>0) TranslateAndWriteBack();
	if(nLatin>0) WriteBackLatin();

	return EndOfFile;
}

//------------------------------------RTF--------------------------
//on source fonts.
//Return YES if a source font ID is a Tibetan font
//Used when paring the rtf file in function parseRTF()
int IsTibetanFont(int fontID)
{
	int i;
	for (i=0;i<EncodingFontNumber[SourceEncoding]; i++)
	{
		if(sourceFontIDs[i] == fontID) return Yes;
	}
	for(i=0;i<nTibetanFontIDs;i++)//201004
	{
		if(TibetanFontIDs[i] == fontID) return Yes;
	}
	return No;
}

//Return YES if a source font ID is a western font
//Used when paring the rtf file in function parseRTF()
int IsWesternFont(int fontID)
{
	int i;
	for (i=0;i<nWestern; i++)
	{
		if(Western[i] == fontID) return Yes;
	}
	return No;
}



//Seperate Tibetan characters from RTF file
void AddCharacterToSourceTextBuffer(int character)
{
	if(nLatin>0) WriteBackLatin();
	if(nChinese>0) WriteBackChinese();
	if(nTibetanInAW>0) WriteBackTibetanInAW();

	if(nTibetan >= SZBuffer)	//Prevent from out of boundry error.
		TranslateAndWriteBack();

	TibetanBuffer[nTibetan].ch = character;
	TibetanBuffer[nTibetan].fontID = currentFontInfo.fontID;
	nTibetan ++;

	if(nFormatBuffer>0 && holdTibetanBuffer == No)
	{
		if(nTibetan>0) TranslateAndWriteBack();//Output Tibetan, then the format.
	}

	if(SourceEncoding==Wylie || SourceEncoding==THDLWylie || SourceEncoding==LCWylie || SourceEncoding==ACIP)
	{
		if(IsPunctuation(character)==Yes) TranslateAndWriteBack();//For AW 2 Unicode. One sentence is encounted;
	}
}


//Check if the file is RTF file, while keep writing
int IsThisRTFFile()
{
	int ch;

	fParam = fFalse;

	if ((ch = getc(sourceFile)) == EOF)
        return EmptyFile;

	if(ch!='{') return NotRtfFile;

	if ((ch = getc(sourceFile)) == EOF)
        return EndOfFile;

	if(ch!='\\') return NotRtfFile;

	if(CollectKeyword() != ControlWord) return NotRtfFile;

	if(strcmp(szKeyword, "rtf") != 0) return NotRtfFile;

	if(TargetFileFormat == RTF)
	{
		fputc('{', targetFile);
		fputc('\\', targetFile);
		fputs(szKeyword, targetFile);

		if(fParam)
		{
			fputs(szParameter, targetFile);
		}
	}

	return ThisIsRtfFile;
}


//Hunt deff, remeber it, while keep writing.
int GetDeff()
{
	int ch;

	FonttblPast = No;

	while ((ch = getc(sourceFile)) != EOF)
	{
		if(ch == '\\')
		{
			CollectKeyword();

			if(TargetFileFormat == RTF)
			{
				fputc('\\', targetFile);
				fputs(szKeyword, targetFile);

				if(fParam)
				{
					fputs(szParameter, targetFile);
				}
			}

			if(!strcmp(szKeyword, "deff"))
			{
				if(fParam)
				{
					deffFontInfo.fontID = param;
					deffFontInfo.fontSize = DeffFontSize;
					if(SourceEncoding == ACIP || SourceEncoding == Wylie || SourceEncoding == THDLWylie || SourceEncoding == LCWylie)
						deffFontInfo.isTibetan = Yes;
					else deffFontInfo.isTibetan = No;

					return GotTheDeff;
				}
				else return NoDeff;
			}
			else if(!strcmp(szKeyword, "fonttbl"))
			{
				FonttblPast = Yes;
				return ReachFontTbl;
			}
			else;
		}
		else
		{
			if(TargetFileFormat == RTF) fputc(ch, targetFile);
		}
	}

	return EndOfFile;
}



int onFonttbl()  //200806
{
	int nGroup = 0, n = 0, m = 0, nSourceFont = 0;
	int i, CollectingFontName = 0;
	int fontID, dalta = 1;
	int fontFamily;//1---Western Fonts; 0----others
	char fontBuffer[FontBufferSize];
	char ch;
	char fontName[100];
	int SourceIDs[200] = {-1}; //Temproly store the font IDs that can't be modified
						//Just for picking up the slots for target fonts
	int specialFontitbl = 0;//for something like this: {\fonttbl\f0\fnil\fcharset0 BZDBT;\f1\fnil\fcharset134 STXihei;}		//20090615

	nWestern = 0;
	Western[0] = 0;
	ChineseFontID[0] = 31;
	nTibetanFontIDs = 0; //201004
	//Locate the fonttbl-----------------------------------------
	if(FonttblPast == No)
	{
		while((ch = getc(sourceFile)) != EOF)
		{
			if(ch != '\\')
			{
				if(TargetFileFormat == RTF) putc(ch, targetFile);
			}
			else
			{
				if(CollectKeyword() == ControlWord)
				{
					//Write back.
					if(TargetFileFormat == RTF)
					{
						fputc('\\', targetFile);
						fputs(szKeyword, targetFile);
						if(fParam)
						{
							fputs(szParameter, targetFile);
						}
					}

					if(!strcmp(szKeyword, "fonttbl")) break;
					else continue;
				}
			}
		}
		if(ch == EOF) return EndOfFile;
	}

	//Start to collect one font-------------------------------------
	while((ch = getc(sourceFile)) != EOF)  //20090615
	{//Locate the first '{'
		if(ch == '{') break;
		else if(ch == '\\')
		{
			ungetc(ch, sourceFile);
			ch = '{';
			specialFontitbl = 1;
			break;
		}
		else
		{
			if(TargetFileFormat == RTF) fputc(ch, targetFile);
			continue;
		}
	}//20090615
	if(ch == EOF) return EndOfFile;

NEWFONT:	//Start to collect font info of a new font in fonttbl.

	fontFamily = 0;//Assume it's not western font
	nGroup ++;
	if(nGroup == 1) n = 0;
	for(;;){
		fontBuffer[n] = ch;
		n++;
		if(n==FontBufferSize-1) break;
		ch = getc(sourceFile);
		if(ch=='{') { nGroup++; continue; }
		else if(ch == EOF) return EndOfFile;
		else if(ch == '}')
		{
			nGroup --;
			if(nGroup==0) break;//Done collecting this font info {... . .. .}
			else continue;
		}
		else if(ch == ';') //20090615
		{
			if(specialFontitbl == 1)
			{
				nGroup = 0;
				fontBuffer[n++] = ch;
				ch = '}';
				break;
			}//Done collecting this font info like  \f0\fnil\fcharset0 BZDBT;
			else continue;
		}	//20090615
		else continue; //All other characters
	}
	fontBuffer[n] = ch;	// pick up '}'
	fontBuffer[n+1] = '\0'; //Finish collection of a font info

	//Start to parse fonttbl-----------------------------------------------------------
	//Parse the font info which was put in the buffer.
	//Put the properties of font in some buffers (globals)
	fontFamily = 0;
	CollectingFontName = 0;
	for(i=0; fontBuffer[i] != '\0'; i++)
	{
		if(fontBuffer[i]=='\\')
		{	//collect key word
			if(CollectKeywordFromFontBuffer(&(fontBuffer[i+1])) == EndOfBuffer) break;//TESTME  &(fontBuffer[i+1])
			if(!strcmp(szKeyword, "f")) {
				fontID = param;
			}
			else if(!strcmp(szKeyword, "froman") ||
				!strcmp(szKeyword, "fswiss")||
				!strcmp(szKeyword, "fmodern")||
				!strcmp(szKeyword, "fdecor")||
				!strcmp(szKeyword, "fscript")) fontFamily = 1;//Western font
			else ;	//other control words
		}
		else if(fontBuffer[i]==0x20) // It can be both boundry and part of font name.
		{
			if(fontBuffer[i+1]=='\\') continue;//skip this space.  20100205

			if(CollectingFontName==1) fontName[m++] = fontBuffer[i]; //This space is part of font name.
			else if(nGroup==1) CollectingFontName = 1; // Start to collect font name.
			else;
		}
		else if(fontBuffer[i]=='}')
		{
			nGroup --;
			if(nGroup==1)
			{
				m=0;
				CollectingFontName = 1;
			}
		}
		else if	(fontBuffer[i]==';')
		{
			if(CollectingFontName==1)
			{
				fontName[m] = '\0';
				m=0;
				break;
			}
		}
		else if	(fontBuffer[i]=='{')
		{
			nGroup ++;

			if(CollectingFontName==1)
			{
				fontName[m] = '\0';
				m=0;
				break;
			}
		}
		else
		{	//Regular characters
			if(CollectingFontName == 0) m = 0; //Not to collect, they are part of control words
			else fontName[m++] = fontBuffer[i];//Part of font name
		}
	}

	if(fontFamily != 1)//Double check the font to see if it's Latin font
	{
		if( !strcmp("Arial", fontName) ||
			!strcmp("Times New Roman", fontName) ||
			!strcmp("MS Arial Unicode", fontName))//ADD MORE FONT NAMES AT HERE!!!!!
			fontFamily = 1;
	}

//    fprintf(targetFile,"fontID1=%d\n",fontID);
	if(!strcmp(ChineseFontName, fontName))
	{
		ChineseFontID[0] = fontID;
	}

//    fprintf(targetFile,"fontID2=%d\n",fontID);
	if(SourceEncoding != ACIP && SourceEncoding != Wylie && SourceEncoding != THDLWylie && SourceEncoding != LCWylie)
	{
		if(nWestern < 100 && fontFamily == 1)//Collect western fonts
		{
			Western[nWestern++] = fontID;
		}

//    fprintf(targetFile,"fontID3=%d\n",fontID);
		//Is it a Tibetan font?
		for(i=BaseIndex[SourceEncoding]; i<BaseIndex[SourceEncoding]+EncodingFontNumber[SourceEncoding]; i++)
		{
			if(!strcmp(TibetanFontNames[i], fontName))
			{
				sourceFontIDs[i - BaseIndex[SourceEncoding]] = fontID; //Remember the source Tibetan font.
				TibetanFontIDs[nTibetanFontIDs++] = fontID;  //201004
				break;
			}
		}
		//If it's not a source font, write back this font.
		if(i==BaseIndex[SourceEncoding]+EncodingFontNumber[SourceEncoding]) //No match. It's not a Tibetan font.
		{
			if(TargetFileFormat == RTF) fputs(fontBuffer, targetFile); //Write back this font info.
			if(nSourceFont<200) SourceIDs[nSourceFont++] = fontID; //Collect source font IDs
		}
//    fprintf(targetFile,"fontID5=%d\n",fontID);
	}
	else //SourceEncoding == ACIP || SourceEncoding == Wylie
	{
		if(nWestern < 100 && fontFamily == 1)
		{
			//Western fonts are the source fonts!!!
			if(nSourceFont<100) sourceFontIDs[nWestern] = fontID;//Use nWestern to monitor the number of fonts
			Western[nWestern++] = fontID;
//    fprintf(targetFile,"fontID6=%d\n",fontID);
		}
		if(nSourceFont<200) SourceIDs[nSourceFont++] = fontID;//Remeber all current source font IDs.

		if(TargetFileFormat == RTF) fputs(fontBuffer, targetFile); //Write back this font info.
	}


	while((ch = getc(sourceFile)) != EOF)
	{
		if(ch == '{')
		{
			n=0;
			nGroup = 0;
			goto NEWFONT;
		}
		else if(ch == '\\') //20090615
		{
			ungetc(ch, sourceFile);
			ch = '{';
			specialFontitbl = 1;

			n=0;
			nGroup = 0;
			goto NEWFONT;
		} //20090615
//-----------------------------set up target fonts-----------------------------------
		else if(ch == '}')//The last '}' of fontinfo header
		{
			if(TargetFileFormat != RTF) return FonttblProcessed;

			if(ConversionDirection == OthersToUnicode) //For others to unicode conversion mode.
			{	//Write target font info
//					fprintf(targetFile, " font1 " ); //20181117 debuging
				if(SourceEncoding == ACIP || SourceEncoding == Wylie || SourceEncoding == THDLWylie || SourceEncoding == LCWylie)
				{
//					fprintf(targetFile, " font2 " ); //20181117 debuging
					for(i=0;i<nWestern; i++)//TESTME  FIXME
					{
						if((Western[i+1] - Western[i]) > 4)
							break;
						else continue;
					}
					//Write target font info
					fputs("{\\f", targetFile); // {\f
					fprintf(targetFile, "%d", Western[i]+3); //Output fontID represented as string
					fputc(0x20, targetFile);
					fputs(";}", targetFile);
					//Remeber the target font info here
					targetFontIDs[0] = Western[i]+3; //Remeber the target font
				}
				else  //SourceEncoding != ACIP && SourceEncoding != Wylie
				{
//					fprintf(targetFile, " font3 " ); //20181117 debuging
					fputs("{\\f", targetFile); // {\f
//					fprintf(targetFile, " SourceFontID=%d ", sourceFontIDs[0]); // before 20181117
//					fprintf(targetFile, " SourceFontID=%d ", Western[nWestern]); // 20181117 debuging
					fprintf(targetFile, "%d", fontID+1); // after 20181117 to fix Tibetan fontID=-1 causing rtf format issue
					fputc(0x20, targetFile);
//                    fprintf(targetFile,"TargetEncoding=%d",TargetEncoding); //add 20181201 for Unicode different name
                    if(TargetEncoding==0) // target encoding is one of the Unicode
                    {
//                        fprintf(targetFile,"Unicode1=%s\n",TibetanUnicodeFontName);
     					fputs(TibetanUnicodeFontName, targetFile);
                    }
                    else
                    {
//                        fprintf(targetFile,"Unicode2=%s\n",TibetanUnicodeFontName);
     					fputs(TibetanFontNames[BaseIndex[TargetEncoding]], targetFile);
//     					fputs(TibetanFontNames[BaseIndex[TargetEncoding]], targetFile);
					}
					fputs(";}", targetFile);
					//Remeber the target font info here
//					targetFontIDs[0] = sourceFontIDs[0]; //Remeber the single target font
					targetFontIDs[0] = fontID+1; // 20181117 debuging
				}
			}
			else if(ConversionDirection == UnicodeToOthers)//Unicode to others
			{	//Finding the rank of font IDs for target fonts
					//fprintf(targetFile, " font4 " ); //20181117 debuging
				for(i=1;i<nSourceFont; i++)//TESTME  FIXME
				{
					if((SourceIDs[i+1] - SourceIDs[i]) > EncodingFontNumber[TargetEncoding])
						break;
					else continue;
				}

				if(i==nSourceFont){ i--; dalta = EncodingFontNumber[TargetEncoding];}

				TargetFileBaseFontNumberRTF2RTF = SourceIDs[i] + dalta; //20090427

				for(n=0;n<EncodingFontNumber[TargetEncoding];n++)
				{	//Write target font info
					fputs("{\\f", targetFile); // {\f
					fprintf(targetFile, "%d", SourceIDs[i] + n + dalta);//Output fontID represented as string

					if(TargetEncoding == Bzd || TargetEncoding == Ty) fputs("\\fcharset134", targetFile);//20090427

					fputc(0x20, targetFile);
   					fputs(TibetanFontNames[BaseIndex[TargetEncoding]+n], targetFile);
					fputs(";}", targetFile);
					//Remeber the target font info here
					targetFontIDs[n] = SourceIDs[i] + n + dalta; //Remeber the target font
//                    fprintf(targetFile,"targetFontIDs[n]=\n",targetFontIDs[n]);
				}
			}
			else;
			//Write back '}'
			fputc(ch, targetFile);
			return FonttblProcessed;
		}
//---------------------------------------------------
		else //Other characters
		{	//Just Write back.
			if(TargetFileFormat == RTF) fputc(ch, targetFile);
		}
	}//End of while((ch = getc(sourceFile)) != EOF)

	return EndOfFile;
}



//Parse RTF file for non AW encoding files, seperate Tibetan characters from control words,
//transtlate them and write back to output file. From RTF to RTF.
int parseRTFforRTF2RTF()
{
	int cGroup = 0;
	int smartTag = 0; //20080827
	int ch, lastCh, tempCH;//20080229
	int retInfo, n = 0;
	int text = No; //Reach text?
	int higherBits = 0, lowerBits = 0, GBCodeHigher = 0; //20080229

	header = Yes;//Don't process control words in header  //20080229
	nTibetan = 0;
	nLatin = 0;
	nChinese = 0;
	nFormatBuffer = 0;
	holdTibetanBuffer = No;

	stackIndex = 0;
	//Set up current font info, it's default font
	currentFontInfo.fontID = deffFontInfo.fontID;
	currentFontInfo.fontSize = deffFontInfo.fontSize;
	currentFontInfo.isTibetan = deffFontInfo.isTibetan;

	if(TargetFileFormat == RTF && TargetEncoding == Unicode) fputs("\\uc0 \n", targetFile);//This is neccesary for Tibetan Unicode \uXXXX

	while((ch = getc(sourceFile)) != EOF)
	{
        //fprintf(targetFile,"nTibetan0=%d\n",nTibetan); //debuging
		n ++;
		if(GBCodeHigher != 0){ tempCH = ch; ch = 0xfff;} //20080229

		switch(ch)
		{
		case '\\':
			if(header == Yes)
			{
				if(TargetFileFormat == RTF)
					fputc(ch, targetFile);
				break;
			}
			GBCodeHigher = 0;//20080229

			retInfo = CollectKeyword();//Work on the RTF keywords.....

			if(retInfo != NonLatinCharacterCodeValue) //20090615
			{
				if(higherBits!=0) //20090615
				{
						if(currentFontInfo.isTibetan == Yes)
						{
							AddCharacterToSourceTextBuffer((int) higherBits);
						}
						else
						{
							if(nLatin >= SZBuffer) WriteBackLatin();
							LatinBuffer[nLatin++] = higherBits;
						}

						higherBits = 0;
				}//20090615
			}//20090615

			if(retInfo == ControlWord)	//It's control word.
			{
				if(!strcmp(szKeyword, "u"))
				{
					if(param<0) param = 65536 + param;

					if(currentFontInfo.isTibetan == Yes)//20090615
					{
						if(SourceEncoding == Unicode)//Only apply to Unicode 2 others
						{
							if(param != 0x0f0b && param != 0x0f0d)//Tibetan constants or vowels
							{
								if(nFormatBuffer>0 && holdTibetanBuffer == No)//Format buffer is not empty, write back them
								{
									if(nTibetan>0) TranslateAndWriteBack();
									if(nLatin>0) WriteBackLatin();
									if(nChinese>0) WriteBackChinese();
									if(nFormatBuffer>0) WriteBackFormat();
								}
								holdTibetanBuffer = Yes; //Hold on!!
							}
							else holdTibetanBuffer = No;//It's the last letter of a SYLLABLE. Don't need to hold on.
							//}    //20090302
							AddCharacterToSourceTextBuffer(param);//20090302
						}//20090302
						else//20090615   Others 2 Unicode
						{
							if(SourceEncoding == Ty || SourceEncoding == Bzd) continue;//ignore \u32379 in \u32379\'c0\'5d  \\20100205

							if(Unicode2GB[param] != 0 && SourceEncoding != NS)
							{
								param = Unicode2GB[param];//convert \uXXXXX to GB code
								AddCharacterToSourceTextBuffer(param);
							}
						}//20090615
					}//20090615
					else
					{
						holdTibetanBuffer = No;

						if(nTibetan>0) TranslateAndWriteBack();//Process Tibetan text first

						if(param >= 0x00 && param <= 0xff)//Latin
					 {
							if(nChinese>0) WriteBackChinese();
							if(nLatin >= SZBuffer) WriteBackLatin();
							LatinBuffer[nLatin++] = param;
						}
						else if(param>=0xf00 && param <=0xfff)//Tibetan
						{
							if(nChinese>0) WriteBackChinese();
							if(nLatin>0) WriteBackLatin();
							if(nTibetan >= SZBuffer) TranslateAndWriteBack();
							TibetanBuffer[nTibetan++].ch = param;
						}
						else //Chinese
						{
							if(nLatin>0) WriteBackLatin();
							if(nChinese >= SZBuffer) WriteBackChinese();
							ChineseBuffer[nChinese++] = param;
						}
					}
				}
				else if(!strcmp(szKeyword, "fs"))
				{
					if(TargetEncoding == ACIP || TargetEncoding == Wylie || TargetEncoding == THDLWylie || TargetEncoding == LCWylie)
					{
						param = 20;
						strcpy(szParameter, "20");
					}
                    else if(SourceEncoding == NewSambhota && TargetEncoding == Unicode && currentFontInfo.isTibetan==Yes)
                    {
                        param = param - 8; // 20181117 when convert from NewSambhota to Unicode, change font size 4 less to be the same.
                        sprintf(szParameter, "%d", param); 
                    }

					currentFontInfo.fontSize = param;
					if(nTibetan>0) TranslateAndWriteBack();
					if(nLatin>0) WriteBackLatin();
					if(nChinese>0) WriteBackChinese();

					if(holdTibetanBuffer == No) //Write back \fs24....
					{
						fputc(ch, targetFile);
						fputs(szKeyword, targetFile);
						if(fParam)
						{
							fputs(szParameter, targetFile);
						}
                        if(SourceEncoding == NewSambhota && TargetEncoding == Unicode)
                        {
                           if(currentFontInfo.isTibetan==Yes)
                           {
                              fprintf(targetFile,"\\dn4 "); // 20181119 adding for lower SambahotaUnicode
                              fprintf(targetFile,"\\sb100"); //20181118 added for adding space above Tibetan 
                           }
                           else
                              fprintf(targetFile,"\\dn0 "); // 20181119 adding for lower SambahotaUnicode
                        }
					}
				}
				else if(!strcmp(szKeyword, "f")) // 20181114 here it detect Tibetan font using IsTibetanFont(param)
				{
					currentFontInfo.fontID = param;
					if(nTibetan>0) TranslateAndWriteBack();
					if(nLatin>0) WriteBackLatin();
					if(nChinese>0) WriteBackChinese();

					if(!IsTibetanFont(param)) //skip Tibetan font info
											  //We do that when translate
					{
						currentFontInfo.isTibetan = No;
						if(holdTibetanBuffer == No)
						{
							fputc(ch, targetFile);
							fputs(szKeyword, targetFile);
							if(fParam)
							{
								fputs(szParameter, targetFile);
							}
						}
						else //holdTibetanBuffer == Yes
						{
							int nKeyword = 0;
							int nParameter = 0;

							FormatBuffer[nFormatBuffer++] = ch;
							while(szKeyword[nKeyword]!='\0') FormatBuffer[nFormatBuffer++] = szKeyword[nKeyword++];
							if(fParam)
							{
								while(szParameter[nParameter]!='\0') FormatBuffer[nFormatBuffer++] = szParameter[nParameter++];
							}
						}
					}
					else //20100307
					{
						currentFontInfo.isTibetan = Yes;

                     	fputs("\\f", targetFile);
                     	fprintf(targetFile, "%d", targetFontIDs[0]);
						if(EndOfFile == CheckSpace()) return EndOfFile;
					}
				}
				else if(!strcmp(szKeyword, "par"))// \par-- newline
				{
					if(nTibetanInAW>0) WriteBackTibetanInAW();
					if(nChinese>0) WriteBackChinese();
					if(nTibetan>0) TranslateAndWriteBack();
					if(nLatin>0) WriteBackLatin();
					if(holdTibetanBuffer == No)
					{
						fputc(ch, targetFile);//TargetFormat == RTF
						fputs(szKeyword, targetFile);
					}
				}
				else if(!strcmp(szKeyword, "filetbl") ||
					!strcmp(szKeyword, "stylesheet") ||
					!strcmp(szKeyword, "colortbl") ||
					!strcmp(szKeyword, "info") ||
					!strcmp(szKeyword, "revtbl"))//These control words should'n be processed, just copy them into new file
				{
					if(nTibetanInAW>0) WriteBackTibetanInAW();
					if(nChinese>0) WriteBackChinese();
					if(nTibetan>0) TranslateAndWriteBack();
					if(nLatin>0) WriteBackLatin();
					header = Yes; //RTF header! ???????????????????????????

					if(holdTibetanBuffer == No)
					{
						fputc(ch, targetFile);
						fputs(szKeyword, targetFile);
						if(fParam)
						{
							fputs(szParameter, targetFile);
						}
					}
				}
				else if(!strcmp(szKeyword, "hich") || // TAKE OFF THEM!!!
					!strcmp(szKeyword, "loch") ||
					!strcmp(szKeyword, "dbch") ||
					!strcmp(szKeyword, "up") ||
					(!strcmp(szKeyword, "uc") && (header == No))|| //20090302
					!strcmp(szKeyword, "af"))
				{
					if(nTibetanInAW>0) WriteBackTibetanInAW();
					if(nChinese>0) WriteBackChinese();
					if(nLatin>0) WriteBackLatin();
				}
				else if(!strcmp(szKeyword, "dn"))
				{
					if(nTibetanInAW>0) WriteBackTibetanInAW();
					if(nChinese>0) WriteBackChinese();
					if(nLatin>0) WriteBackLatin();

					if(ConversionDirection == OthersToUnicode);
					else if(TargetEncoding == ACIP || TargetEncoding == Wylie || TargetEncoding == THDLWylie || TargetEncoding == LCWylie);
					else
					{
						if(holdTibetanBuffer == No)
						{
							fputc(ch, targetFile);
							fputs(szKeyword, targetFile);
							if(fParam)
							{
								fputs(szParameter, targetFile);
							}
						}
					}
				}
				else if(!strcmp(szKeyword, "plain") || !strcmp(szKeyword, "pard"))// \plain //20090615
				{
					if(nTibetanInAW>0) WriteBackTibetanInAW();
					if(nChinese>0) WriteBackChinese();
					if(nTibetan>0) TranslateAndWriteBack();

					if(cGroup == 0) {text = Yes; header = No;} //200806 //We reach the main text.

					if(holdTibetanBuffer == No)
					{
						fputc(ch, targetFile);
						fputs(szKeyword, targetFile);
						if(fParam)
						{
							fputs(szParameter, targetFile);
						}
					}
					header = No; //20080229
				}
				else if(!strcmp(szKeyword, "bullet") ||  //20090302
					!strcmp(szKeyword, "endash") ||  //These control words are control symbols in fact.
					!strcmp(szKeyword, "emdash") ||  //They should be converted to their values, and put the value back to the file, then read it again.
					!strcmp(szKeyword, "ldblquote") ||
					!strcmp(szKeyword, "rdblquote") ||
					!strcmp(szKeyword, "lquote") ||
					!strcmp(szKeyword, "rquote"))
				{
					if(!strcmp(szKeyword, "bullet"))  ungetc(0x95, sourceFile);
					else if(!strcmp(szKeyword, "endash")) ungetc(0x96, sourceFile);
					else if(!strcmp(szKeyword, "emdash")) ungetc(0x97, sourceFile);
					else if(!strcmp(szKeyword, "ldblquote")) ungetc(0x93, sourceFile);
					else if(!strcmp(szKeyword, "rdblquote")) ungetc(0x94, sourceFile);
					else if(!strcmp(szKeyword, "lquote")) ungetc(0x91, sourceFile);
					else if(!strcmp(szKeyword, "rquote")) ungetc(0x92, sourceFile);
					else ;

					LastCh = lastCh;
				}//20090302
				else //default: other control words
				{
					if(nTibetan>0) TranslateAndWriteBack();
					if(nLatin>0) WriteBackLatin();
					if(nChinese>0) WriteBackChinese();
					//Write back.
					if(holdTibetanBuffer == No)
					{
						fputc(ch, targetFile);
						fputs(szKeyword, targetFile);
						if(fParam)
						{
							fputs(szParameter, targetFile);
						}
					}

					if(!strcmp(szKeyword, "xmlopen"))  smartTag = 1;// \xmlopen  20080827
				}

				if(holdTibetanBuffer == Yes) BufferControlWords(ch);

			}//End of if(retInfo == ControlWord)
			else if(retInfo == ControlSymbol)
			{
				if(currentFontInfo.isTibetan == Yes)
				{
                    if(szKeyword[0]=='\n'){  // 20181115 Tibetan control symbol will override 
                       fprintf(targetFile,"\\\n");
     				   AddCharacterToSourceTextBuffer((int) szKeyword[0]);
                    }
				}
				else //Non Tibetan characters
				{
					holdTibetanBuffer = No;

                    if(szKeyword[0]=='\n'){
                       fprintf(targetFile,"\\\n"); //20181122 adding return
                    }
					if(nTibetan>0) TranslateAndWriteBack();//Process Tibetan text first
					if(nChinese>0) WriteBackChinese();
					if(nLatin >= SZBuffer) WriteBackLatin();
					LatinBuffer[nLatin++] = szKeyword[0];
                    WriteBackLatin(); //20181111 
				}
			}
			else if(retInfo == NonLatinCharacterCodeValue) //   \'3f  etc.  //20080229
			{
				if((currentFontInfo.isTibetan == Yes)&&  //20090218
					(SourceEncoding == Jamyang || SourceEncoding == LTibetan || SourceEncoding == TCRCBodYig || SourceEncoding == OldSambhota || SourceEncoding == TM)) //20090218
				{
					ch = axtoi(GBCode);
					AddCharacterToSourceTextBuffer((int) ch);
				}  //20090218
				else if(!strcmp(GBCode, "3f") || !strcmp(GBCode, "3F")) ; //Take off \'3f. Do nothing!  //20090218
				else if(nGBCode != 2) ; //Take of the strange \'XXXXX. Do nothing.
				else // GB code apears.
				{
					if(higherBits == 0) higherBits = axtoi(GBCode);
					else
					{
						lowerBits = axtoi(GBCode);
						if(currentFontInfo.isTibetan == Yes)
						{
							AddCharacterToSourceTextBuffer((int) ((higherBits<<8) + lowerBits));//Bandrida or Tongyuan
						}
						else
						{
							if(nLatin>0) WriteBackLatin();
							if(nTibetan>0) TranslateAndWriteBack();
							if(nChinese >= SZBuffer) WriteBackChinese();
							ChineseBuffer[nChinese++] = (higherBits<<8) + lowerBits; //Chinese
						}
						higherBits = 0;
					}
				}
			}
			else if(retInfo == EndOfFile)
			{
				holdTibetanBuffer = No;
				if(nTibetan>0) TranslateAndWriteBack();
				if(nLatin>0) WriteBackLatin();
				if(nChinese>0) WriteBackChinese();
				return EndOfFile;
			}
			else//retInfo == SpecialControlWord
			{
				if(nTibetan>0) TranslateAndWriteBack();
				if(nLatin>0) WriteBackLatin();
				if(nChinese>0) WriteBackChinese();
				//Write back.
				if(holdTibetanBuffer == No)
				{
					fputc(ch, targetFile);
					fputs(szKeyword, targetFile);
					if(fParam)
					{
						fputs(szParameter, targetFile);
					}
				}
				else BufferControlWords(ch);
			}

			ch = LastCh;//Put back the last character that is read
			break;

		case '{':
			if(smartTag>0) smartTag++; //20080827

			if(higherBits!=0) //20090615
			{
					if(currentFontInfo.isTibetan == Yes)
					{
						AddCharacterToSourceTextBuffer((int) higherBits);
					}
					else
					{
						if(nLatin >= SZBuffer) WriteBackLatin();
						LatinBuffer[nLatin++] = higherBits;
					}

					higherBits = 0;
			}//20090615

			higherBits = 0; //20080229
			if(lastCh == '\\' && header == Yes)
			{
                        //fprintf(targetFile,"nTibetan33a=%d\n",nTibetan); //debuging
				fputc(ch, targetFile);
				break;
			}
			if(nTibetan>0) TranslateAndWriteBack();
			if(nLatin>0) WriteBackLatin();
			if(nChinese>0) WriteBackChinese();
			PushFontinfoOnStack();
			cGroup ++;
			if(holdTibetanBuffer == No)
			{
				if(TargetFileFormat == RTF)
				{	//For inserting line breakers
					if(n>70) //20080229
					{
						fputc('\n', targetFile);
						n = 0;
					}
				}

				fputc(ch, targetFile);
			}
			else FormatBuffer[nFormatBuffer++] = ch;
			break;

		case '}':
			if(smartTag>0) smartTag--; //20080827

			if(higherBits!=0) //20090615
			{
					if(currentFontInfo.isTibetan == Yes)
					{
						AddCharacterToSourceTextBuffer((int) higherBits);
					}
					else
					{
						if(nLatin >= SZBuffer) WriteBackLatin();
						LatinBuffer[nLatin++] = higherBits;
					}

					higherBits = 0;
			}//20090615

			higherBits = 0; //20080229
			if(lastCh == '\\' && header == Yes)
			{
				fputc(ch, targetFile);
				break;
			}
			if(nTibetan>0) TranslateAndWriteBack();
			if(nLatin>0) WriteBackLatin();
			if(nChinese>0) WriteBackChinese();
			if(cGroup<=0)
			{
				if(holdTibetanBuffer == No) fputc(ch, targetFile);
				else FormatBuffer[nFormatBuffer++] = ch;

				break; //Fix the bug of some RTF writers
			}
			PopFontinfoFromStack();
			cGroup --;
			if(holdTibetanBuffer == No) fputc(ch, targetFile);
			else FormatBuffer[nFormatBuffer++] = ch;

			if(header == Yes && cGroup <= 0) header = No;
			break;

		case 0x0d:
    		fputc("\n",targetFile);
		case 0x0a:	//Don't write back these chars. They are not part of the content of the file.
			if(currentFontInfo.isTibetan != Yes)
            {
    		    fputc(0x0a,targetFile); // same as \n
            }
			break;

		case 0xfff: //20080229    //20090218
		default://text:  characters //20080229

			if(higherBits!=0) //20090615
			{
					if(currentFontInfo.isTibetan == Yes)
					{
						AddCharacterToSourceTextBuffer((int) higherBits);
					}
					else
					{
						if(nLatin >= SZBuffer) WriteBackLatin();
						LatinBuffer[nLatin++] = higherBits;
					}

					higherBits = 0;
			}//20090615

			if(ch == 0xfff) ch = tempCH;   //20090218

			if(header == Yes || smartTag > 0) fputc(ch, targetFile);	//20080827
			else if(GBCodeHigher == 0 && ch >= 0x00 && ch <= 0x80)//Latin characters
			{
				if(currentFontInfo.isTibetan == Yes && ch > 0x20 && ch <= 0xff) //TSL Tibetan  //201010205
				{
					AddCharacterToSourceTextBuffer((int) ch);
				}
				else//Latin characters
				{
					if(nTibetan>0) TranslateAndWriteBack();//Process Tibetan text first
					if(nChinese>0) WriteBackChinese();
					if(nLatin >= SZBuffer) WriteBackLatin();
					LatinBuffer[nLatin++] = ch;
					WriteBackLatin(); // 20181110 solve a bug: print Latin after print control word
				}
			}
			else //Wide characters //20080229
			{
				holdTibetanBuffer = No;

				if((currentFontInfo.isTibetan == Yes)&&  //20090218
					(SourceEncoding == Jamyang || SourceEncoding == LTibetan || SourceEncoding == TCRCBodYig || SourceEncoding == OldSambhota || SourceEncoding == TM)) //20090218
				{
					AddCharacterToSourceTextBuffer((int) ch);
				}  //20090218
				//Wide characters   GB2312 or CP936 //20080229
				else if(GBCodeHigher == 0) GBCodeHigher = ch; //20090218
				else //get the wide chatacter
				{
					if(currentFontInfo.isTibetan == Yes)//BTN Tibetan wide characters
					{
						AddCharacterToSourceTextBuffer((int) ((GBCodeHigher<<8) + ch));
					}
					else //Chinese characters
					{
						if(nTibetan>0) TranslateAndWriteBack();//Process Tibetan text first
						if(nLatin>0) WriteBackLatin();
						if(nChinese >= SZBuffer) WriteBackChinese();
						ChineseBuffer[nChinese++] = (GBCodeHigher<<8) + ch;
					}
					GBCodeHigher = 0;
				}
			}
			higherBits = 0;//20090615

		}//End of switch()
		lastCh = ch; //Remember the previous character
	}//End of while()
	holdTibetanBuffer = No;
	if(nTibetan>0) TranslateAndWriteBack();
	if(nLatin>0) WriteBackLatin();
	if(nChinese>0) WriteBackChinese();

	return EndOfFile; //Conversion is completed.
}


//Parse RTF file with source encoding ACIP and Wylie, from RTF to RTF
int parseRTFforAWforRTF2RTF()
{
	int cGroup = 0;
	int smartTag = 0; //20080827
	int ch, lastCh;
	int retInfo, n = 0;
	int text = No; //Reach text?

	header = No;//Don't process control words in header

	nTibetan = 0;
	nLatin = 0;
	nChinese = 0;
	nTibetanInAW = 0;

	stackIndex = 0;
	//Set up current font info, it's default font
	currentFontInfo.fontID = deffFontInfo.fontID;
	currentFontInfo.fontSize = deffFontInfo.fontSize;
	currentFontInfo.isTibetan = deffFontInfo.isTibetan;

	if(TargetFileFormat == RTF && TargetEncoding == Unicode) fputs("\\uc0 ", targetFile);//This is neccesary	for Tibetan Unicode \uXXXX

	while((ch = getc(sourceFile)) != EOF)
	{
		if(TargetFileFormat == RTF)
		{	//For inserting line breakers
			n ++;
		}

		switch(ch)
		{
		case '\\':
			if(header == Yes)
			{
				if(TargetFileFormat == RTF)
					fputc(ch, targetFile);
				break;
			}
			retInfo = CollectKeyword();

			if(retInfo == ControlWord)	//It's control word.
			{
				if(!strcmp(szKeyword, "u"))
				{
					if(nTibetan>0) TranslateAndWriteBack();//Process Tibetan text first

					if(param<0) param = 65536 + param;

					if(param >= 0x00 && param <= 0xff)//Latin
					{
						if(nChinese>0) WriteBackChinese();
						if(nTibetanInAW>0) WriteBackTibetanInAW();

						if(nTibetan >= SZBuffer) TranslateAndWriteBack();
						TibetanBuffer[nTibetan++].ch = param;
					}
					else if(param>=0xf00 && param <=0xfff)//Tibetan
					{
						if(nChinese>0) WriteBackChinese();
						if(nTibetan>0) TranslateAndWriteBack();

						if(nTibetanInAW >= SZBuffer) WriteBackTibetanInAW();
						TibetanInAWBuffer[nTibetanInAW++] = param;
//							if(nTibetan >= 0) TranslateAndWriteBack(); // 20181113 test 
					}
					else //Chinese
					{
						if(nTibetanInAW>0) WriteBackTibetanInAW();
						if(nTibetan>0) TranslateAndWriteBack();

						if(nChinese >= SZBuffer) WriteBackChinese();
						ChineseBuffer[nChinese++] = param;
					}
				}
				else if(!strcmp(szKeyword, "fs"))
				{
					if(TargetEncoding == Unicode)
					{
						param = 36;
						strcpy(szParameter, "36");
					}

					currentFontInfo.fontSize = param;
					if(nTibetan>0) TranslateAndWriteBack();
					if(nChinese>0) WriteBackChinese();
					if(nTibetanInAW>0) WriteBackTibetanInAW();

					fputc(ch, targetFile);
					fputs(szKeyword, targetFile);
					if(fParam)
					{
						fputs(szParameter, targetFile);
					}
				}
				else if(!strcmp(szKeyword, "f"))
				{
                    //fprintf(targetFile,"in51\n"); //debuging
					currentFontInfo.fontID = param;
					if(nTibetan>0) TranslateAndWriteBack();
					if(nChinese>0) WriteBackChinese();
					if(nTibetanInAW>0) WriteBackTibetanInAW();

					if(!IsWesternFont(param)) //skip Tibetan font info
											  //We do that when translate
					{
						currentFontInfo.isTibetan = No;
					}
					else currentFontInfo.isTibetan = Yes;
				}
				else if(!strcmp(szKeyword, "par"))// \par-- newline
				{
                    //fprintf(targetFile,"in52\n"); //debuging
					if(nTibetanInAW>0) WriteBackTibetanInAW();
					if(nChinese>0) WriteBackChinese();
					if(nTibetan>0) TranslateAndWriteBack();

					fputc(ch, targetFile);//TargetFormat == RTF
					fputs(szKeyword, targetFile);
				}
				else if(!strcmp(szKeyword, "filetbl") ||
					!strcmp(szKeyword, "stylesheet") ||
					!strcmp(szKeyword, "colortbl") ||
					!strcmp(szKeyword, "listtable") ||
					!strcmp(szKeyword, "info") ||
					!strcmp(szKeyword, "revtbl"))//These control words should'n be processed, just copy them into new file
				{
					if(nTibetanInAW>0) WriteBackTibetanInAW();
					if(nChinese>0) WriteBackChinese();
					if(nTibetan>0) TranslateAndWriteBack();
					header = Yes;

					fputc(ch, targetFile);
					fputs(szKeyword, targetFile);
					if(fParam)
					{
						fputs(szParameter, targetFile);
					}
				}
				else if(!strcmp(szKeyword, "hich") || // TAKE OFF THEM!!!
					!strcmp(szKeyword, "loch") ||
					!strcmp(szKeyword, "dbch") ||
					!strcmp(szKeyword, "up") ||
					!strcmp(szKeyword, "af"))
				{
					if(nTibetanInAW>0) WriteBackTibetanInAW();
					if(nChinese>0) WriteBackChinese();
				}
				else if(!strcmp(szKeyword, "dn"))
				{
					if(nTibetanInAW>0) WriteBackTibetanInAW();
					if(nChinese>0) WriteBackChinese();

					if(ConversionDirection == OthersToUnicode);
					else if(TargetEncoding == ACIP || TargetEncoding == Wylie || TargetEncoding == THDLWylie || TargetEncoding == LCWylie);
					else
					{
						if(holdTibetanBuffer == No)
						{
							fputc(ch, targetFile);
							fputs(szKeyword, targetFile);
							if(fParam)
							{
								fputs(szParameter, targetFile);
							}
						}
					}
				}
				else if(!strcmp(szKeyword, "plain") || !strcmp(szKeyword, "pard"))// \plain //20090615
				{
					if(nTibetanInAW>0) WriteBackTibetanInAW();
					if(nChinese>0) WriteBackChinese();
					if(nTibetan>0) TranslateAndWriteBack();

					if(cGroup == 0){ text = Yes; header = No;} //200806//We reach the main text.

					fputc(ch, targetFile);
					fputs(szKeyword, targetFile);
					if(fParam)
					{
						fputs(szParameter, targetFile);
					}
				}
				else if(!strcmp(szKeyword, "bullet") ||  //20090302
					!strcmp(szKeyword, "endash") ||  //These control words are control symbols in fact.
					!strcmp(szKeyword, "emdash") ||  //They should be converted to their values, and put the value back to the file, then read it again.
					!strcmp(szKeyword, "ldblquote") ||
					!strcmp(szKeyword, "rdblquote") ||
					!strcmp(szKeyword, "lquote") ||
					!strcmp(szKeyword, "rquote"))
				{
					if(!strcmp(szKeyword, "bullet"))  ungetc(0x95, sourceFile);
					else if(!strcmp(szKeyword, "endash")) ungetc(0x96, sourceFile);
					else if(!strcmp(szKeyword, "emdash")) ungetc(0x97, sourceFile);
					else if(!strcmp(szKeyword, "ldblquote")) ungetc(0x93, sourceFile);
					else if(!strcmp(szKeyword, "rdblquote")) ungetc(0x94, sourceFile);
					else if(!strcmp(szKeyword, "lquote")) ungetc(0x91, sourceFile);
					else if(!strcmp(szKeyword, "rquote")) ungetc(0x92, sourceFile);
					else ;

					LastCh = lastCh;
				}//20090302
				else //default: other control words
				{
					if(nTibetan>0) TranslateAndWriteBack();
					if(nChinese>0) WriteBackChinese();
					if(nTibetanInAW>0) WriteBackTibetanInAW();
					//Write back.
					fputc(ch, targetFile);
					fputs(szKeyword, targetFile);
					if(fParam)
					{
						fputs(szParameter, targetFile);
					}

					if(!strcmp(szKeyword, "xmlopen"))  smartTag = 1;// \xmlopen  20080827
				}
			}//End of if(retInfo == ControlWord)
			else if(retInfo == ControlSymbol)
			{
				if(currentFontInfo.isTibetan == Yes)
				{
					AddCharacterToSourceTextBuffer((int) szKeyword[0]);
				}
				else	//Non Tibetan character///////////////////////
				{
					if(nTibetan>0) TranslateAndWriteBack();//Process Tibetan text first

					if(szKeyword[0] >= 0x00 && szKeyword[0] <= 0xff)//Latin
					{
						if(nChinese>0) WriteBackChinese();
						if(nLatin >= SZBuffer) WriteBackLatin();
						LatinBuffer[nLatin++] = szKeyword[0];
					}
					else if(szKeyword[0]>=0xf00 && szKeyword[0] <=0xfff)//Tibetan, It's likely impossible.
					{
						if(nChinese>0) WriteBackChinese();
						if(nLatin>0) WriteBackLatin();
						if(nTibetan >0) TranslateAndWriteBack();

						if(nTibetanInAW >= SZBuffer) WriteBackTibetanInAW();
						TibetanInAWBuffer[nTibetanInAW++] = szKeyword[0];
					}
					else //Chinese
					{
						if(nLatin>0) WriteBackLatin();
						if(nChinese >= SZBuffer) WriteBackChinese();
						ChineseBuffer[nChinese++] = szKeyword[0];
					}
				}
			}
			else if(retInfo == NonLatinCharacterCodeValue) //   \'3f  etc.
			{
				if(nChinese>0) WriteBackChinese();
				if(nLatin>0) WriteBackLatin();

				if(nTibetan>0) TranslateAndWriteBack();

				fputc(ch, targetFile);
				fputs(szKeyword, targetFile);
#ifdef Unix
				while(1) //For Unix: while(1)
#endif
#ifdef Windows
				while(true)
#endif
				{
					ch = getc(sourceFile);
					if(ch == EOF) return EndOfFile;
					else if (isalpha(ch) || isdigit(ch))
					{
						fputc(ch, targetFile);
					}
					else { ungetc(ch, sourceFile); break; }
				}
			}
			else if(retInfo == EndOfFile)
			{
				if(nTibetan>0) TranslateAndWriteBack();
				if(nLatin>0) WriteBackLatin();
				if(nChinese>0) WriteBackChinese();
				return EndOfFile;
			}
			else//retInfo == SpecialControlWord
			{
				if(nTibetan>0) TranslateAndWriteBack();
				if(nChinese>0) WriteBackChinese();
				if(nTibetanInAW>0) WriteBackTibetanInAW();
				//Write back.
				fputc(ch, targetFile);
				fputs(szKeyword, targetFile);
				if(fParam)
				{
					fputs(szParameter, targetFile);
				}
			}

			ch = LastCh;//Put back the last character that is read
			break;

		case '{':
			if(smartTag>0) smartTag++; //20080827

			if(lastCh == '\\' && header == Yes)
			{
				fputc(ch, targetFile);
				break;
			}
			if(nTibetan>0) TranslateAndWriteBack();
			if(nChinese>0) WriteBackChinese();
			if(nTibetanInAW>0) WriteBackTibetanInAW();

			PushFontinfoOnStack();
			//currentFontInfo = deffFontInfo;
			currentFontInfo.fontID = deffFontInfo.fontID;
			currentFontInfo.fontSize = deffFontInfo.fontSize;
			currentFontInfo.isTibetan = deffFontInfo.isTibetan;

			cGroup ++;

			if(TargetFileFormat == RTF)
			{	//For inserting line breakers
				if(n>50)
				{
					fputc('\n', targetFile);
					n = 0;
				}
			}

			fputc(ch, targetFile);
			break;

		case '}':
			if(smartTag>0) smartTag--; //20080827

			if(lastCh == '\\' && header == Yes)
			{
				fputc(ch, targetFile);
				break;
			}
			if(nTibetan>0) TranslateAndWriteBack();
			if(nChinese>0) WriteBackChinese();
			if(nTibetanInAW>0) WriteBackTibetanInAW();
			if(cGroup<=0)
			{
				fputc(ch, targetFile);
				break; //Fix the bug of some RTF writers
			}
			PopFontinfoFromStack();
			cGroup --;
			fputc(ch, targetFile);
			if(header == Yes && cGroup <= 0) header = No;
			break;

		case 0x0d:
            //fprintf(targetFile,"this is 0 0d"); //debuging
            //fputc(0x0d,targetFile);
		case 0x0a:	//Don't write back this char.
            //fprintf(targetFile,"this is 0 0a"); //debuging
            //fputc(0x0a,targetFile);
			break;

		default://text:  characters//////////////////
			if(header == Yes || smartTag > 0) fputc(ch, targetFile);	//20080827
			else if(currentFontInfo.isTibetan == Yes)
			{
				AddCharacterToSourceTextBuffer((int) ch);
			}
			else	//Non Tibetan character
			{
				if(nTibetan>0) TranslateAndWriteBack();//Process Tibetan text first
				fputc(ch, targetFile);
				if(TargetFileFormat == UnicodeTXT) fputc(0x00, targetFile);
			}
		}//End of switch()
		lastCh = ch;
	}//End of while()
	if(nTibetan>0) TranslateAndWriteBack();
	if(nTibetanInAW>0) WriteBackTibetanInAW();
	if(nLatin>0) WriteBackLatin();
	if(nChinese>0) WriteBackChinese();

	return EndOfFile; //Conversion is completed.
}



//Parse RTF file in non AW encodings, seperate Tibetan characters from control words,
//transtlate them and write back to output file. From RTF to TXT or Webpage
int parseRTFforRTF2TXTorWEBPAGE()
{
	int cGroup = 0;
	int smartTag = 0; //20080827
	int shpTag = 0; //20090218  for take off images
	int ch, tempCH; //20080229
	int retInfo, n = 0;
	int text = No; //Reach text?
	int annotation = No;
	int higherBits = 0, lowerBits = 0, GBCodeHigher = 0; //20080229

	header = Yes;//Don't process control words in header  //20080229
	nTibetan = 0;
	nLatin = 0;
	nChinese = 0;

	holdTibetanBuffer = No;

	stackIndex = 0;
	//Set up current font info, it's default font
	currentFontInfo.fontID = deffFontInfo.fontID;
	currentFontInfo.fontSize = deffFontInfo.fontSize;
	currentFontInfo.isTibetan = deffFontInfo.isTibetan;

	while((ch = getc(sourceFile)) != EOF)
	{
		if(TargetFileFormat == WebPage)
		{	//For inserting line breakers
			if(n>50)
			{
				fputc('\n', targetFile);
				n = 0;
			}
		}

		if(GBCodeHigher != 0){ tempCH = ch; ch = 0xfff;} //20080229

		switch(ch)
		{
		case '\\':

			GBCodeHigher = 0;//20080229

			retInfo = CollectKeyword();//Work on the RTF keywords.....

			if(retInfo == ControlWord)	//It's control word.
			{
				if(!strcmp(szKeyword, "u"))
				{
					if(TargetFileFormat == WebPage)
					{	//For inserting line breakers
						n ++;
					}

					if(text == No || header == Yes || annotation == Yes) break; //Take off them

					if(param<0) param = 65536 + param;

					if(currentFontInfo.isTibetan == Yes)
					{
//						if(param>=0xf40 && param<=0xfbc) holdTibetanBuffer = Yes; //Hold on!!
						if(param != 0x0f0b && param != 0xf0d) holdTibetanBuffer = Yes; //Hold on!!
						else holdTibetanBuffer = No;//It's the last letter of a RUN. Don't need to hold on.
						if(SourceEncoding == Ty || SourceEncoding == Bzd) continue;//ignore \u32379 in \u32379\'c0\'5d  \\20100205

						AddCharacterToSourceTextBuffer(param);
					}
					else//Non Tibetan character////////////////
					{
						if(nTibetan>0) TranslateAndWriteBack();//Process Tibetan text first

						if(param >= 0x00 && param <= 0xff)//Latin
						{
							if(nChinese>0) WriteBackChinese();
							if(nLatin >= SZBuffer) WriteBackLatin();
							LatinBuffer[nLatin++] = param;
						}
						else if(param>=0xf00 && param <=0xfff)//Tibetan
						{
							if(nChinese>0) WriteBackChinese();
							if(nLatin>0) WriteBackLatin();
							if(nTibetan >= SZBuffer) TranslateAndWriteBack();
							TibetanBuffer[nTibetan++].ch = param;
						}
						else //Chinese
						{
							if(nLatin>0) WriteBackLatin();
							if(nChinese >= SZBuffer) WriteBackChinese();
							ChineseBuffer[nChinese++] = param;
						}
					}
				}
				else if(!strcmp(szKeyword, "fs"))
				{
					currentFontInfo.fontSize = param;
					if(nTibetan>0) TranslateAndWriteBack();
					if(nLatin>0) WriteBackLatin();
					if(nChinese>0) WriteBackChinese();
				}
				else if(!strcmp(szKeyword, "f"))
				{
					currentFontInfo.fontID = param;
					if(nTibetan>0) TranslateAndWriteBack();
					if(nLatin>0) WriteBackLatin();
					if(nChinese>0) WriteBackChinese();

					if(!IsTibetanFont(param)) //skip Tibetan font info
											  //We do that when translate
					{
						currentFontInfo.isTibetan = No;
					}
					else currentFontInfo.isTibetan = Yes;
				}
				else if(!strcmp(szKeyword, "par") || !strcmp(szKeyword, "line"))// \par-- newline
				{
					if(text == No || header == Yes || annotation == Yes) break; //Take off them
					if(nTibetan>0) TranslateAndWriteBack();
					if(nTibetanInAW>0) WriteBackTibetanInAW();
					if(nChinese>0) WriteBackChinese();

					if(nLatin >= SZBuffer) WriteBackLatin();
					LatinBuffer[nLatin++] = '\n';
				}
				else if(!strcmp(szKeyword, "filetbl") ||
					!strcmp(szKeyword, "stylesheet") ||
					!strcmp(szKeyword, "colortbl") ||
					!strcmp(szKeyword, "info") ||
					!strcmp(szKeyword, "field") ||
					!strcmp(szKeyword, "footnote") ||
					!strcmp(szKeyword, "v") ||
					!strcmp(szKeyword, "annotation") ||
					!strcmp(szKeyword, "revtbl"))//These control words should'n be processed, just copy them into new file
				{
					if(nTibetan>0) TranslateAndWriteBack();
					if(nTibetanInAW>0) WriteBackTibetanInAW();
					if(nChinese>0) WriteBackChinese();
					if(nLatin>0) WriteBackLatin();

					header = Yes;	//Don't process the text
				}
				else if(!strcmp(szKeyword, "fldrslt"))//Text is followed after this keyword
				{
					if(nTibetan>0) TranslateAndWriteBack();
					if(nTibetanInAW>0) WriteBackTibetanInAW();
					if(nChinese>0) WriteBackChinese();

					header = No; //Process the text
				}
				else if(!strcmp(szKeyword, "atrfstart") || //20100205
					!strcmp(szKeyword, "atrfend") ||
					!strcmp(szKeyword, "atnid") ||
					!strcmp(szKeyword, "atnauthor") ||
					!strcmp(szKeyword, "atnref") ||
					!strcmp(szKeyword, "atntime") ||
					!strcmp(szKeyword, "lsdlocked") ||
					!strcmp(szKeyword, "lsdlockedexcept") ||
					!strcmp(szKeyword, "themedata") ||
					!strcmp(szKeyword, "colorschememapping") ||
					!strcmp(szKeyword, "datastore") ||
					!strcmp(szKeyword, "atnicn"))//These control words are "annotations"
				{
					if(nTibetan>0) TranslateAndWriteBack();
					if(nTibetanInAW>0) WriteBackTibetanInAW();
					if(nChinese>0) WriteBackChinese();
					annotation = Yes;
				}
				else if(!strcmp(szKeyword, "hich") || // TAKE OFF THEM!!!
					!strcmp(szKeyword, "loch") ||
					!strcmp(szKeyword, "dbch") ||
					!strcmp(szKeyword, "af"))
				{
					if(nTibetanInAW>0) WriteBackTibetanInAW();
					if(nChinese>0) WriteBackChinese();
				}
				else if(!strcmp(szKeyword, "plain") || !strcmp(szKeyword, "pard"))// \plain //20090615
				{
					if(cGroup == 0) {text = Yes; header = No; }//200806 //We reach the main text.
				}
				else if(!strcmp(szKeyword, "shp") || !strcmp(szKeyword, "shppict") || !strcmp(szKeyword, "nonshppict")) //image or picture appears   201004
				{
					if(nTibetan>0) TranslateAndWriteBack();
					if(nLatin>0) WriteBackLatin();
					if(nChinese>0) WriteBackChinese();

					shpTag = 1;
				}
				else if(!strcmp(szKeyword, "bullet") ||  //20090302
					!strcmp(szKeyword, "endash") ||  //These control words are control symbols in fact.
					!strcmp(szKeyword, "emdash") ||  //They should be converted to their values, and put the value back to the file, then read it again.
					!strcmp(szKeyword, "ldblquote") ||
					!strcmp(szKeyword, "rdblquote") ||
					!strcmp(szKeyword, "lquote") ||
					!strcmp(szKeyword, "rquote"))
				{
					if(!strcmp(szKeyword, "bullet"))  ungetc(0x95, sourceFile);
					else if(!strcmp(szKeyword, "endash")) ungetc(0x96, sourceFile);
					else if(!strcmp(szKeyword, "emdash")) ungetc(0x97, sourceFile);
					else if(!strcmp(szKeyword, "ldblquote")) ungetc(0x93, sourceFile);
					else if(!strcmp(szKeyword, "rdblquote")) ungetc(0x94, sourceFile);
					else if(!strcmp(szKeyword, "lquote")) ungetc(0x91, sourceFile);
					else if(!strcmp(szKeyword, "rquote")) ungetc(0x92, sourceFile);
					else ;
				}//20090302
				else //default: other control words
				{
					if(nTibetan>0) TranslateAndWriteBack();
					if(nLatin>0) WriteBackLatin();
					if(nChinese>0) WriteBackChinese();

					if(!strcmp(szKeyword, "xmlopen"))  smartTag = 1;// \xmlopen  20080827
				}
			}//End of if(retInfo == ControlWord)
			else if(retInfo == ControlSymbol)
			{
				if(text == No || header == Yes || annotation == Yes) break; //Take off them

				if(currentFontInfo.isTibetan == Yes)
				{
					AddCharacterToSourceTextBuffer((int) szKeyword[0]);
				}
				else	//Non Tibetan character
				{
					if(nTibetan>0) TranslateAndWriteBack();//Process Tibetan text first

					if(szKeyword[0] >= 0x00 && szKeyword[0] <= 0xff)//Latin
					{
						if(nChinese>0) WriteBackChinese();
						if(nLatin >= SZBuffer) WriteBackLatin();
						LatinBuffer[nLatin++] = szKeyword[0];
					}
					else if(szKeyword[0]>=0xf00 && szKeyword[0] <=0xfff)//Tibetan, It's likely impossible.
					{
						if(nChinese>0) WriteBackChinese();
						if(nLatin>0) WriteBackLatin();
						if(nTibetan >= SZBuffer) TranslateAndWriteBack();
						TibetanBuffer[nTibetan++].ch = szKeyword[0];
					}
					else //Chinese
					{
						if(nLatin>0) WriteBackLatin();
						if(nChinese >= SZBuffer) WriteBackChinese();
						ChineseBuffer[nChinese++] = szKeyword[0];
					}
				}
			}
			else if(retInfo == NonLatinCharacterCodeValue) //   \'3f  etc.//20080229
			{
				if(header == Yes || text == No || annotation == Yes) ; //Don't output to TXT	//200806
				else if((currentFontInfo.isTibetan == Yes)&&  //20090218
					(SourceEncoding == Jamyang || SourceEncoding == LTibetan || SourceEncoding == TCRCBodYig || SourceEncoding == OldSambhota || SourceEncoding == TM)) //20090218
				{
					ch = axtoi(GBCode);
					AddCharacterToSourceTextBuffer((int) ch);
				}  //20090218
				else if(!strcmp(GBCode, "3f") || !strcmp(GBCode, "3F")) ; //Take off \'3f. Do nothing!  //200806
				else if(nGBCode != 2) ; //Take of the strange \'XXXXX. Do nothing.
				else // GB code apears.
				{
					if(nLatin>0) WriteBackLatin();
					if(nTibetan>0) TranslateAndWriteBack();

					if(higherBits == 0) higherBits = axtoi(GBCode);
					else
					{
						lowerBits = axtoi(GBCode);
						if(nChinese >= SZBuffer) WriteBackChinese();

						if(currentFontInfo.isTibetan == Yes)//20080926
						{
							AddCharacterToSourceTextBuffer((higherBits<<8) + lowerBits);
						}
						else//20080926
						{
							ChineseBuffer[nChinese++] = (higherBits<<8) + lowerBits;
						}

						higherBits = 0;
					}

				}
			}
			else if(retInfo == EndOfFile)
			{
				holdTibetanBuffer = No;
				if(nTibetan>0) TranslateAndWriteBack();
				if(nLatin>0) WriteBackLatin();
				if(nChinese>0) WriteBackChinese();
				return EndOfFile;
			}
			else//retInfo == SpecialControlWord
			{
				if(nTibetan>0) TranslateAndWriteBack();
				if(nLatin>0) WriteBackLatin();
				if(nChinese>0) WriteBackChinese();
			}
			break;

		case '{':
			if(smartTag>0) smartTag++; //20080827

			if(shpTag>0) shpTag++; //20090218

			higherBits = 0; //20080229
			if(nTibetan>0) TranslateAndWriteBack();
			if(nLatin>0) WriteBackLatin();
			if(nChinese>0) WriteBackChinese();
			PushFontinfoOnStack();
			cGroup ++;
			break;

		case '}':
			if(smartTag>0) smartTag--; //20080827

			if(shpTag>0) shpTag--; //20090218

			higherBits = 0; //20080229
			if(annotation == Yes) annotation = No;

			if(nTibetan>0) TranslateAndWriteBack();
			if(nLatin>0) WriteBackLatin();
			if(nChinese>0) WriteBackChinese();

			if(cGroup<=0) break; //Fix the bug of some RTF writers

			PopFontinfoFromStack();
			cGroup --;

			if(header == Yes && cGroup <= 0) header = No;
			break;

		case 0x0d:
		case '\n':  //20080926
			break;
//20090325
		case 0xfff: //20080229    //20090218
		default://text:  characters //20080229
			higherBits = 0;
			if(ch == 0xfff) ch = tempCH;   //20090218

			if(header == Yes || text == No || annotation == Yes || smartTag > 0 || shpTag > 0) ; //Don't output to TXT	//20080827	//20090218
			else if(GBCodeHigher == 0 && ch >= 0x00 && ch <= 0x80)//Latin characters
			{
				if(currentFontInfo.isTibetan == Yes && ch > 0x20 && ch <= 0xff) //TSL Tibetan  //20100205
				{
					AddCharacterToSourceTextBuffer((int) ch);
				}
				else//Latin characters
				{
					if(nTibetan>0) TranslateAndWriteBack();//Process Tibetan text first
					if(nChinese>0) WriteBackChinese();
					if(nLatin >= SZBuffer) WriteBackLatin();
					LatinBuffer[nLatin++] = ch;
				}
			}
			else //Wide characters //20080229
			{
				holdTibetanBuffer = No;

				if((currentFontInfo.isTibetan == Yes)&&  //20090218
					(SourceEncoding == Jamyang || SourceEncoding == LTibetan || SourceEncoding == TCRCBodYig || SourceEncoding == OldSambhota || SourceEncoding == TM)) //20090218
				{
					AddCharacterToSourceTextBuffer((int) ch);
				}  //20090218
				//Wide characters   GB2312 or CP936 //20080229
				else if(GBCodeHigher == 0) GBCodeHigher = ch; //20090218
				else //get the wide chatacter
				{
					if(currentFontInfo.isTibetan == Yes)//BTN Tibetan wide characters
					{
						AddCharacterToSourceTextBuffer((int) ((GBCodeHigher<<8) + ch));
					}
					else //Chinese characters
					{
						if(nTibetan>0) TranslateAndWriteBack();//Process Tibetan text first
						if(nLatin>0) WriteBackLatin();
						if(nChinese >= SZBuffer) WriteBackChinese();
						ChineseBuffer[nChinese++] = (GBCodeHigher<<8) + ch;
					}
					GBCodeHigher = 0;
				}
			}
//20090325

		}//End of switch()
	}//End of while()
	holdTibetanBuffer = No;
	if(nTibetan>0) TranslateAndWriteBack();
	if(nLatin>0) WriteBackLatin();
	if(nChinese>0) WriteBackChinese();

	return EndOfFile; //Conversion is completed.
}



//Parse RTF file with source encoding ACIP and Wylie
//From RTF to TXT or Webpage
int parseRTFforAWforRTF2TXTorWEBPAGE()
{
	int cGroup = 0;
	int smartTag = 0; //20080827
	int shpTag = 0; //20090218  for take off images
	int ch;
	int retInfo, n = 0;
	int text = No;
	int annotation = No;

	header = No;//Don't process control words in header

	nTibetan = 0;
	nLatin = 0;
	nChinese = 0;
	nTibetanInAW = 0;

	stackIndex = 0;
	//Set up current font info, it's default font
	currentFontInfo.fontID = deffFontInfo.fontID;
	currentFontInfo.fontSize = deffFontInfo.fontSize;
	currentFontInfo.isTibetan = deffFontInfo.isTibetan;

	while((ch = getc(sourceFile)) != EOF)
	{
		switch(ch)
		{
		case '\\':

			retInfo = CollectKeyword();

			if(retInfo == ControlWord)	//It's control word.
			{
				if(!strcmp(szKeyword, "u"))
				{
					if(TargetFileFormat == WebPage)
					{	//For inserting line breakers
						n ++;
					}

					if(text == No || header == Yes || annotation == Yes)
					{
						if(TargetFileFormat != RTF) break; //Take off them
					}

					if(param<0) param = 65536 + param;

					if(nTibetan>0) TranslateAndWriteBack();//Process Tibetan text first

					if(param >= 0x00 && param <= 0xff)//Latin
					{
						if(nChinese>0) WriteBackChinese();
						if(nTibetanInAW>0) WriteBackTibetanInAW();

						if(nTibetan >= SZBuffer) TranslateAndWriteBack();
						TibetanBuffer[nTibetan++].ch = param;
					}
					else if(param>=0xf00 && param <=0xfff)//Tibetan
					{
						if(nChinese>0) WriteBackChinese();
						if(nTibetan>0) TranslateAndWriteBack();

						if(nTibetanInAW >= SZBuffer) WriteBackTibetanInAW();
						TibetanInAWBuffer[nTibetanInAW++] = param;
					}
					else //Chinese
					{
						if(nTibetanInAW>0) WriteBackTibetanInAW();
						if(nTibetan>0) TranslateAndWriteBack();

						if(nChinese >= SZBuffer) WriteBackChinese();
						ChineseBuffer[nChinese++] = param;
					}
				}
				else if(!strcmp(szKeyword, "fs"))
				{
					currentFontInfo.fontSize = param;
					if(nTibetan>0) TranslateAndWriteBack();
					if(nChinese>0) WriteBackChinese();
					if(nTibetanInAW>0) WriteBackTibetanInAW();
				}
				else if(!strcmp(szKeyword, "f"))
				{
					currentFontInfo.fontID = param;
					if(nTibetan>0) TranslateAndWriteBack();
					if(nChinese>0) WriteBackChinese();
					if(nTibetanInAW>0) WriteBackTibetanInAW();

					if(!IsWesternFont(param)) //skip Tibetan font info
											  //We do that when translate
					{
						currentFontInfo.isTibetan = No;
					}
					else currentFontInfo.isTibetan = Yes;
				}
				else if(!strcmp(szKeyword, "par") || !strcmp(szKeyword, "line"))// \par-- newline
				{
					if(text == No || header == Yes || annotation == Yes) break; //Take off them

					if(nTibetanInAW>0) WriteBackTibetanInAW();
					if(nChinese>0) WriteBackChinese();
					if(nTibetan>0) TranslateAndWriteBack();

					if(nLatin >= SZBuffer) WriteBackLatin();
					LatinBuffer[nLatin++] = '\n';
				}
				else if(!strcmp(szKeyword, "filetbl") ||
					!strcmp(szKeyword, "stylesheet") ||
					!strcmp(szKeyword, "colortbl") ||
					!strcmp(szKeyword, "info") ||
					!strcmp(szKeyword, "field") ||
					!strcmp(szKeyword, "footnote") ||
					!strcmp(szKeyword, "v") ||
					!strcmp(szKeyword, "annotation") ||
					!strcmp(szKeyword, "revtbl"))//These control words are headers
				{
					if(nTibetanInAW>0) WriteBackTibetanInAW();
					if(nChinese>0) WriteBackChinese();
					if(nTibetan>0) TranslateAndWriteBack();
					header = Yes;
				}
				else if(!strcmp(szKeyword, "fldrslt"))//Text is followed after this keyword
				{
					if(nTibetan>0) TranslateAndWriteBack();
					if(nTibetanInAW>0) WriteBackTibetanInAW();
					if(nChinese>0) WriteBackChinese();

					header = No; //we need to rocess the text in this domain
				}
				else if(!strcmp(szKeyword, "atrfstart") || //20100205
					!strcmp(szKeyword, "atrfend") ||
					!strcmp(szKeyword, "atnid") ||
					!strcmp(szKeyword, "atnauthor") ||
					!strcmp(szKeyword, "atnref") ||
					!strcmp(szKeyword, "atntime") ||
					!strcmp(szKeyword, "lsdlocked") ||
					!strcmp(szKeyword, "lsdlockedexcept") ||
					!strcmp(szKeyword, "themedata") ||
					!strcmp(szKeyword, "colorschememapping") ||
					!strcmp(szKeyword, "datastore") ||
					!strcmp(szKeyword, "atnicn"))//These control words are "annotations"
				{
					if(nTibetanInAW>0) WriteBackTibetanInAW();
					if(nChinese>0) WriteBackChinese();
					if(nTibetan>0) TranslateAndWriteBack();
					annotation = Yes;
				}
				else if(!strcmp(szKeyword, "plain") || !strcmp(szKeyword, "pard"))// \plain //20090615
				{
					if(cGroup == 0) {text = Yes; header = No;} //200806//We reach the main text.
				}
				else if(!strcmp(szKeyword, "shp") || !strcmp(szKeyword, "shppict") || !strcmp(szKeyword, "nonshppict")) //image or picture appears   201004
				{
					if(nTibetan>0) TranslateAndWriteBack();
					if(nLatin>0) WriteBackLatin();
					if(nChinese>0) WriteBackChinese();

					shpTag = 1;
				}
				else if(!strcmp(szKeyword, "bullet") ||  //20090302
					!strcmp(szKeyword, "endash") ||  //These control words are control symbols in fact.
					!strcmp(szKeyword, "emdash") ||  //They should be converted to their values, and put the value back to the file, then read it again.
					!strcmp(szKeyword, "ldblquote") ||
					!strcmp(szKeyword, "rdblquote") ||
					!strcmp(szKeyword, "lquote") ||
					!strcmp(szKeyword, "rquote"))
				{
					if(!strcmp(szKeyword, "bullet"))  ungetc(0x95, sourceFile);
					else if(!strcmp(szKeyword, "endash")) ungetc(0x96, sourceFile);
					else if(!strcmp(szKeyword, "emdash")) ungetc(0x97, sourceFile);
					else if(!strcmp(szKeyword, "ldblquote")) ungetc(0x93, sourceFile);
					else if(!strcmp(szKeyword, "rdblquote")) ungetc(0x94, sourceFile);
					else if(!strcmp(szKeyword, "lquote")) ungetc(0x91, sourceFile);
					else if(!strcmp(szKeyword, "rquote")) ungetc(0x92, sourceFile);
					else ;
				}//20090302
				else //default: other control words
				{
					if(nTibetan>0) TranslateAndWriteBack();
					if(nChinese>0) WriteBackChinese();
					if(nTibetanInAW>0) WriteBackTibetanInAW();

					if(!strcmp(szKeyword, "xmlopen"))  smartTag = 1;// \xmlopen  20080827
				}
			}//End of if(retInfo == ControlWord)
			else if(retInfo == ControlSymbol)
			{
				if(text == No || header == Yes || annotation == Yes) break; //Take off them

				if(currentFontInfo.isTibetan == Yes)
				{
					AddCharacterToSourceTextBuffer((int) szKeyword[0]);
				}
				else	//Non Tibetan character///////////////////////
				{
					if(nChinese>0) WriteBackChinese();
					if(nTibetanInAW>0) WriteBackTibetanInAW();

					if(nTibetan >= SZBuffer) TranslateAndWriteBack();
					TibetanBuffer[nTibetan++].ch = szKeyword[0];
				}
			}
			else if(retInfo == EndOfFile)
			{
				if(nTibetan>0) TranslateAndWriteBack();
				if(nLatin>0) WriteBackLatin();
				if(nChinese>0) WriteBackChinese();
				return EndOfFile;
			}
			else//retInfo == SpecialControlWord
			{
				if(nTibetan>0) TranslateAndWriteBack();
				if(nChinese>0) WriteBackChinese();
				if(nTibetanInAW>0) WriteBackTibetanInAW();
			}
			break;

		case '{':
			if(smartTag>0) smartTag++; //20080827

			if(shpTag>0) shpTag++; //20090218

			if(nTibetan>0) TranslateAndWriteBack();
			if(nChinese>0) WriteBackChinese();
			if(nTibetanInAW>0) WriteBackTibetanInAW();

			PushFontinfoOnStack();
			//currentFontInfo = deffFontInfo;
			currentFontInfo.fontID = deffFontInfo.fontID;
			currentFontInfo.fontSize = deffFontInfo.fontSize;
			currentFontInfo.isTibetan = deffFontInfo.isTibetan;

			cGroup ++;

			if(TargetFileFormat == WebPage)
			{	//For inserting line breakers
				if(n>100)
				{
					fputc('\n', targetFile);
					n = 0;
				}
			}

			break;

		case '}':
			if(smartTag>0) smartTag--; //20080827

			if(shpTag>0) shpTag--; //20090218

			if(annotation == Yes) annotation = No;
			if(nTibetan>0) TranslateAndWriteBack();
			if(nChinese>0) WriteBackChinese();
			if(nTibetanInAW>0) WriteBackTibetanInAW();
			if(cGroup<=0) break; //Fix the bug of some RTF writers

			PopFontinfoFromStack();
			cGroup --;
			if(header == Yes && cGroup <= 0) header = No;
			break;

		case '\n':
		case 0x0d:
//		case 0x0a:
			if(text == Yes && TargetFileFormat == WebPage) fputc('\n', targetFile);
			break;

		default://text:  characters//////////////////
			if(TargetFileFormat == WebPage)
			{	//For inserting line breakers
				n ++;
			}

			if(header == Yes || text == No || annotation == Yes || smartTag > 0 || shpTag > 0) ;  //Take off it. //20080827  //20090218
			else if(currentFontInfo.isTibetan == Yes)
			{
				AddCharacterToSourceTextBuffer((int) ch);
			}
			else	//Non Tibetan character
			{
				if(nTibetan>0) TranslateAndWriteBack();//Process Tibetan text first
				fputc(ch, targetFile);
				if(TargetFileFormat == UnicodeTXT) fputc(0x00, targetFile);
			}
		}//End of switch()
	}//End of while()
	if(nTibetan>0) TranslateAndWriteBack();
	if(nTibetanInAW>0) WriteBackTibetanInAW();
	if(nLatin>0) WriteBackLatin();
	if(nChinese>0) WriteBackChinese();

	return EndOfFile; //Conversion is completed.
}


//Parse RTF file for source encodings of Unicode, TSLs
int parseRTF()
{
	if(TargetFileFormat == RTF) return parseRTFforRTF2RTF();
	else return parseRTFforRTF2TXTorWEBPAGE();
}


//Parse RTF file with source encoding ACIP and Wylie,
int parseRTFforAW()
{
	if(TargetFileFormat == RTF) return parseRTFforAWforRTF2RTF();
	else return parseRTFforAWforRTF2TXTorWEBPAGE();
}


//---------------------AW Tibetan Syllable Utilities------------------------------------


//To examine a component to see if it is a vowel. Return true if the component is vowel.
int IsTibetanVowel(char *thecomponent)
{
	if(SourceEncoding == ACIP)
	{
		if( !strcmp(thecomponent, "A") ||
			!strcmp(thecomponent, "I") ||
			!strcmp(thecomponent, "U") ||
			!strcmp(thecomponent, "E") ||
			!strcmp(thecomponent, "O") )
			return Yes;
		else return No;
	}
	else //if(SourceEncoding == Wylie, or THDLWylie, LCWylie)
	{
		if( !strcmp(thecomponent, "a") ||
			!strcmp(thecomponent, "i") ||
			!strcmp(thecomponent, "u") ||
			!strcmp(thecomponent, "e") ||
			!strcmp(thecomponent, "o") )
			return Yes;
		else return No;
	}
}


//To examine a component, see if it is a prefix. Return true if the component is prefix.
int IsPrefix(char *thecomponent)
{
	if(SourceEncoding == ACIP)
	{
		if( !strcmp(thecomponent, "G") ||
			!strcmp(thecomponent, "D") ||
			!strcmp(thecomponent, "B") ||
			!strcmp(thecomponent, "M") ||
			!strcmp(thecomponent, "'") )
			return Yes;
		else return No;
	}

	else //Wylie, THDLWylie,  LCWylie
	{
		if( !strcmp(thecomponent, "g") ||
			!strcmp(thecomponent, "d") ||
			!strcmp(thecomponent, "b") ||
			!strcmp(thecomponent, "m") ||
			!strcmp(thecomponent, "'") )
			return Yes;
		else return No;
	}

}


//To examine a component, see if it is a base letter. Return Yes if it is.
int IsBaseLetter(char *thecomponent)
{
	if(SourceEncoding == LCWylie)
	{
		if( !strcmp(thecomponent, "k") ||
			!strcmp(thecomponent, "kh") ||
			!strcmp(thecomponent, "g") ||
			!strcmp(thecomponent, "鏽") ||
			!strcmp(thecomponent, "c") ||
			!strcmp(thecomponent, "ch") ||
			!strcmp(thecomponent, "j") ||
			!strcmp(thecomponent, "鋘") ||
			!strcmp(thecomponent, "t") ||
			!strcmp(thecomponent, "th") ||
			!strcmp(thecomponent, "d") ||
			!strcmp(thecomponent, "n") ||
			!strcmp(thecomponent, "p") ||
			!strcmp(thecomponent, "ph") ||
			!strcmp(thecomponent, "b") ||
			!strcmp(thecomponent, "m") ||
			!strcmp(thecomponent, "ts") ||
			!strcmp(thecomponent, "tsh") ||
			!strcmp(thecomponent, "dz") ||
			!strcmp(thecomponent, "w") ||
			!strcmp(thecomponent, "鈠") ||
			!strcmp(thecomponent, "z") ||
			!strcmp(thecomponent, "'") ||
			!strcmp(thecomponent, "y") ||
			!strcmp(thecomponent, "") ||	//Special for making "g.ya" different from gya.
			!strcmp(thecomponent, "r") ||
			!strcmp(thecomponent, "l") ||
			!strcmp(thecomponent, "鈙") ||
			!strcmp(thecomponent, "s") ||
			!strcmp(thecomponent, "h") ||
			!strcmp(thecomponent, "a") )
			return Yes;
		else return No;
	}

	else if(SourceEncoding == ACIP) //20080620
	{
		 if( !strcmp(thecomponent, "K") ||
			!strcmp(thecomponent, "KH") ||
			!strcmp(thecomponent, "G") ||
			!strcmp(thecomponent, "NG") ||
			!strcmp(thecomponent, "C") ||
			!strcmp(thecomponent, "CH") ||
			!strcmp(thecomponent, "J") ||
			!strcmp(thecomponent, "NY") ||
			!strcmp(thecomponent, "T") ||
			!strcmp(thecomponent, "TH") ||
			!strcmp(thecomponent, "D") ||
			!strcmp(thecomponent, "N") ||
			!strcmp(thecomponent, "P") ||
			!strcmp(thecomponent, "PH") ||
			!strcmp(thecomponent, "B") ||
			!strcmp(thecomponent, "M") ||
			!strcmp(thecomponent, "TZ") ||
			!strcmp(thecomponent, "TS") ||
			!strcmp(thecomponent, "DZ") ||
			!strcmp(thecomponent, "W") ||
			!strcmp(thecomponent, "ZH") ||
			!strcmp(thecomponent, "Z") ||
			!strcmp(thecomponent, "'") ||
			!strcmp(thecomponent, "Y") ||
			!strcmp(thecomponent, "-Y") ||	//Special for making "g.ya" different from gya.
			!strcmp(thecomponent, "R") ||
			!strcmp(thecomponent, "L") ||
			!strcmp(thecomponent, "SH") ||
			!strcmp(thecomponent, "S") ||
			!strcmp(thecomponent, "H") ||
			!strcmp(thecomponent, "A") )
			return Yes;
		else return No;
	}

	else //THDLWylie, Wylie
	{
		 if( !strcmp(thecomponent, "k") ||
			!strcmp(thecomponent, "kh") ||
			!strcmp(thecomponent, "g") ||
			!strcmp(thecomponent, "ng") ||
			!strcmp(thecomponent, "c") ||
			!strcmp(thecomponent, "ch") ||
			!strcmp(thecomponent, "j") ||
			!strcmp(thecomponent, "ny") ||
			!strcmp(thecomponent, "t") ||
			!strcmp(thecomponent, "th") ||
			!strcmp(thecomponent, "d") ||
			!strcmp(thecomponent, "n") ||
			!strcmp(thecomponent, "p") ||
			!strcmp(thecomponent, "ph") ||
			!strcmp(thecomponent, "b") ||
			!strcmp(thecomponent, "m") ||
			!strcmp(thecomponent, "ts") ||
			!strcmp(thecomponent, "tsh") ||
			!strcmp(thecomponent, "dz") ||
			!strcmp(thecomponent, "w") ||
			!strcmp(thecomponent, "zh") ||
			!strcmp(thecomponent, "z") ||
			!strcmp(thecomponent, "'") ||
			!strcmp(thecomponent, "y") ||
			!strcmp(thecomponent, ".y") ||	//Special for making "g.ya" different from gya.
			!strcmp(thecomponent, "r") ||
			!strcmp(thecomponent, "l") ||
			!strcmp(thecomponent, "sh") ||
			!strcmp(thecomponent, "s") ||
			!strcmp(thecomponent, "h") ||
			!strcmp(thecomponent, "a") )
			return Yes;
		else return No;
	}
}





//To examine a component,  see if it is a supperscript. Return Yes if it is, otherwise No.
int IsSuperscript(char *thecomponent)
{
	if(SourceEncoding == ACIP)
	{
		if( !strcmp(thecomponent, "R") ||
			!strcmp(thecomponent, "L") ||
			!strcmp(thecomponent, "S") )
			return Yes;
		else return No;
	}
	else //Wylie, THDLWylie, LCWylie
	{
		if( !strcmp(thecomponent, "r") ||
			!strcmp(thecomponent, "l") ||
			!strcmp(thecomponent, "s") )
			return Yes;
		else return No;
	}
}


//To examine a component,  see if it is a subscript. Return Yes if it is, otherwise No.
int IsSubscript(char *thecomponent)
{
	if(SourceEncoding == ACIP)
	{
		if( !strcmp(thecomponent, "W") ||
			!strcmp(thecomponent, "Y") ||
			!strcmp(thecomponent, "R") ||
			!strcmp(thecomponent, "L") )
			return Yes;
		else return No;
	}
	else //(SourceEncoding == Wylie, THDLWylie, LCWylie)
	{
		if( !strcmp(thecomponent, "w") ||
			!strcmp(thecomponent, "y") ||
			!strcmp(thecomponent, "r") ||
			!strcmp(thecomponent, "l") )
			return Yes;
		else return No;
	}
}

//To examine a component,  see if it is a suffix. Return Yes if it is, otherwise No.
int IsSuffix(char *thecomponent)
{
	if(SourceEncoding == ACIP)
	{
		if( !strcmp(thecomponent, "G") ||
			!strcmp(thecomponent, "NG") ||
			!strcmp(thecomponent, "D") ||
			!strcmp(thecomponent, "N") ||
			!strcmp(thecomponent, "B") ||
			!strcmp(thecomponent, "M") ||
			!strcmp(thecomponent, "'") ||
			!strcmp(thecomponent, "R") ||
			!strcmp(thecomponent, "L") ||
			!strcmp(thecomponent, "S") )
			return Yes;
		else return No;
	}
	else if(SourceEncoding == LCWylie)
	{
		if( !strcmp(thecomponent, "g") ||
			!strcmp(thecomponent, "鏽") ||
			!strcmp(thecomponent, "d") ||
			!strcmp(thecomponent, "n") ||
			!strcmp(thecomponent, "b") ||
			!strcmp(thecomponent, "m") ||
			!strcmp(thecomponent, "'") ||
			!strcmp(thecomponent, "r") ||
			!strcmp(thecomponent, "l") ||
			!strcmp(thecomponent, "s") )
			return Yes;
		else return No;
	}
	else //(SourceEncoding == Wylie, THDLWylie)
	{
		if( !strcmp(thecomponent, "g") ||
			!strcmp(thecomponent, "ng") ||
			!strcmp(thecomponent, "d") ||
			!strcmp(thecomponent, "n") ||
			!strcmp(thecomponent, "b") ||
			!strcmp(thecomponent, "m") ||
			!strcmp(thecomponent, "'") ||
			!strcmp(thecomponent, "r") ||
			!strcmp(thecomponent, "l") ||
			!strcmp(thecomponent, "s") )
			return Yes;
		else return No;
	}
}


//To examine a component,  see if it is a the second suffix. Return Yes if it is, otherwise No.
int IsSecondSuffix(char *thecomponent)
{
	if(SourceEncoding == ACIP)
	{
		if(!strcmp(thecomponent, "S"))
			return Yes;
		else return No;
	}
	else
	{
		if(!strcmp(thecomponent, "s"))
			return Yes;
		else return No;
	}
}


//To examine a component,  see if it is a Sanskrit consonant. Return Yes if it is, otherwise No.
int IsSanskritConsonant(char *thecomponent)
{
	if(SourceEncoding == ACIP)
	{
		if( !strcmp(thecomponent, "t") ||
			!strcmp(thecomponent, "th") ||
			!strcmp(thecomponent, "d") ||
			!strcmp(thecomponent, "n") ||
			!strcmp(thecomponent, "sh") ||
			!strcmp(thecomponent, "m") ||
			!strcmp(thecomponent, "`") ||
			!strcmp(thecomponent, "f") ||
			!strcmp(thecomponent, "v"))
			return Yes;
		else return No;
	}
	else if(SourceEncoding == LCWylie)
	{
		if( !strcmp(thecomponent, "騮") ||
			!strcmp(thecomponent, "騮h") ||
			!strcmp(thecomponent, "騞") ||
			!strcmp(thecomponent, "騞h") ||
			!strcmp(thecomponent, "騨") ||
			!strcmp(thecomponent, "騭") ||
			!strcmp(thecomponent, "騧") ||
			!strcmp(thecomponent, "騢") ||
			!strcmp(thecomponent, "f") ||
			!strcmp(thecomponent, "v"))
			return Yes;
		else return No;
	}
	else
	{
		if( !strcmp(thecomponent, "T") ||
			!strcmp(thecomponent, "Th") ||
			!strcmp(thecomponent, "D") ||
			!strcmp(thecomponent, "N") ||
			!strcmp(thecomponent, "Sh") ||
			!strcmp(thecomponent, "M") ||
			!strcmp(thecomponent, "`") ||
			!strcmp(thecomponent, "f") ||
			!strcmp(thecomponent, "v"))
			return Yes;
		else return No;
	}
}


//To examine a component,  see if it is a Sanskrit vowel. Return Yes if it is, otherwise No.
int IsSanskritVowel(char *thecomponent)
{
	char thevowel[2] = {0xf4, '\0' };

	if(SourceEncoding == ACIP)
	{
		if( !strcmp(thecomponent, "a") ||
			!strcmp(thecomponent, "'i")||
			!strcmp(thecomponent, "EE")||
			!strcmp(thecomponent, "OO"))
			return Yes;
		else return No;
	}
	else if(SourceEncoding == LCWylie)
	{
		if( !strcmp(thecomponent, "錫") ||
			!strcmp(thecomponent, "錳") ||
			!strcmp(thecomponent, "鍀") ||
			!strcmp(thecomponent, thevowel )||
			!strcmp(thecomponent, "弭")||
			!strcmp(thecomponent, "ai")||
			!strcmp(thecomponent, "au"))
			return Yes;
		else return No;
	}
	else //Wylie, THDLWylie
	{
		if( !strcmp(thecomponent, "A") ||
			!strcmp(thecomponent, "I") ||
			!strcmp(thecomponent, "U") ||
			!strcmp(thecomponent, "-i")||
			!strcmp(thecomponent, "-I")||
			!strcmp(thecomponent, "ai")||
			!strcmp(thecomponent, "au"))
			return Yes;
		else return No;
	}
}


//To examine a component,  see if it is a Sanskrit symbole. Return Yes if it is, otherwise No.
int IsSanskritSpecialSymbol(char *thecomponent)
{
	if( !strcmp(thecomponent, "+") )
		return Yes;
	else return No;
}


//To examine a component,  see if it is a Tibetan symbol. Return Yes if it is, otherwise No.
int IsThisTibetanSymbol(char *thecomponent)
{
	if( IsTibetanVowel(thecomponent) ||
		IsBaseLetter(thecomponent) )
		return Yes;
	else return No;
}


//To examine a component,  see if it is a Sanskrit symbol. Return Yes if it is, otherwise No.
int IsThisSanskritSymbol(char *thecomponent)
{
	if( IsSanskritVowel(thecomponent) ||
		IsSanskritConsonant( thecomponent) ||
		IsSanskritSpecialSymbol(thecomponent) )
		return Yes;
	else return No;
}


//To examine a component,  see if it is a Tibetan symbol. Return Yes if it is, otherwise No.
int IsThisTibetanOrSanskritSymbole(char *thecomponent)
{
	if( IsThisTibetanSymbol(thecomponent) ||
		IsThisSanskritSymbol(thecomponent) )
		return Yes;
	else return No;
}


//To examine a component, see if it is a Tibetan or Sanskrit symbol. Return Yes if it is, otherwise No.
int IsThisTibetanOrSanskritVowel(char *thecomponent)
{
	if( IsSanskritVowel( thecomponent) ||
		IsTibetanVowel( thecomponent) )
		return Yes;
	else return No;
}


//To examine a pair of components, see if one of them is a prefix and the other one is
//a base letter that can follow the prefix. Return Yes if it is, otherwise No.
int PrefixBaseletterMatch(char *prefix, char *baseletter)
{
	char c;
	c = *prefix;

	if(SourceEncoding == ACIP)
	{
		switch(c){
		case 'G':
		if( !strcmp(baseletter, "C") ||
			!strcmp(baseletter, "NY") ||
			!strcmp(baseletter, "T") ||
			!strcmp(baseletter, "D") ||
			!strcmp(baseletter, "N") ||
			!strcmp(baseletter, "TZ") ||		//20080620
			!strcmp(baseletter, "ZH") ||
			!strcmp(baseletter, "Z") ||
			!strcmp(baseletter, "SH") ||
			!strcmp(baseletter, "S") ||
			!strcmp(baseletter, "-Y") )
			return Yes;
		else return No;

		case 'D':
		if( !strcmp(baseletter, "K") ||
			!strcmp(baseletter, "P") ||
			!strcmp(baseletter, "G") ||
			!strcmp(baseletter, "B") ||
			!strcmp(baseletter, "NG") ||
			!strcmp(baseletter, "M") )
			return Yes;
		else return No;

		case 'B':
		if( !strcmp(baseletter, "C") ||
			!strcmp(baseletter, "NY") ||
			!strcmp(baseletter, "NG") ||  //20080620
			!strcmp(baseletter, "N") ||
			!strcmp(baseletter, "G") ||
			!strcmp(baseletter, "T") ||
			!strcmp(baseletter, "D") ||
			!strcmp(baseletter, "TZ") ||	//20080620
			!strcmp(baseletter, "ZH") ||
			!strcmp(baseletter, "Z") ||
			!strcmp(baseletter, "SH") ||
			!strcmp(baseletter, "S") ||
			!strcmp(baseletter, "K") )
			return Yes;
		else return No;

		case 'M':
		if( !strcmp(baseletter, "KH") ||
			!strcmp(baseletter, "CH") ||
			!strcmp(baseletter, "TH") ||
			!strcmp(baseletter, "TS") ||  //20080620
			!strcmp(baseletter, "G") ||
			!strcmp(baseletter, "J") ||
			!strcmp(baseletter, "D") ||
			!strcmp(baseletter, "DZ") ||
			!strcmp(baseletter, "NG") ||
			!strcmp(baseletter, "NY") ||
			!strcmp(baseletter, "N") )
			return Yes;
		else return No;

		case '\'':
		if( !strcmp(baseletter, "KH") ||
			!strcmp(baseletter, "CH") ||
			!strcmp(baseletter, "TH") ||
			!strcmp(baseletter, "PH") ||
			!strcmp(baseletter, "TS") ||  //20080620
			!strcmp(baseletter, "G") ||
			!strcmp(baseletter, "J") ||
			!strcmp(baseletter, "D") ||
			!strcmp(baseletter, "B") ||
			!strcmp(baseletter, "DZ") )
			return Yes;
		else return No;
		}
		return No;
	}

	else if(SourceEncoding == LCWylie)//LCWylie
	{
		switch(c){
		case 'g':
		if( !strcmp(baseletter, "c") ||
			!strcmp(baseletter, "鋘") ||
			!strcmp(baseletter, "鏽") || //20080620
			!strcmp(baseletter, "t") ||
			!strcmp(baseletter, "d") ||
			!strcmp(baseletter, "n") ||
			!strcmp(baseletter, "ts") ||
			!strcmp(baseletter, "鈠") ||
			!strcmp(baseletter, "z") ||
			!strcmp(baseletter, "鈙") ||
			!strcmp(baseletter, "s") ||
			!strcmp(baseletter, "") )
			return Yes;
		else return No;

		case 'd':
		if( !strcmp(baseletter, "k") ||
			!strcmp(baseletter, "p") ||
			!strcmp(baseletter, "g") ||
			!strcmp(baseletter, "b") ||
			!strcmp(baseletter, "鏽") ||
			!strcmp(baseletter, "m") )
			return Yes;
		else return No;

		case 'b':
		if( !strcmp(baseletter, "c") ||
			!strcmp(baseletter, "鋘") ||
			!strcmp(baseletter, "g") ||
			!strcmp(baseletter, "t") ||
			!strcmp(baseletter, "d") ||
			!strcmp(baseletter, "n") ||
			!strcmp(baseletter, "ts") ||
			!strcmp(baseletter, "鈠") ||
			!strcmp(baseletter, "z") ||
			!strcmp(baseletter, "鈙") ||
			!strcmp(baseletter, "s") ||
			!strcmp(baseletter, "k") )
			return Yes;
		else return No;

		case 'm':
		if( !strcmp(baseletter, "kh") ||
			!strcmp(baseletter, "ch") ||
			!strcmp(baseletter, "th") ||
			!strcmp(baseletter, "tsh") ||
			!strcmp(baseletter, "g") ||
			!strcmp(baseletter, "j") ||
			!strcmp(baseletter, "d") ||
			!strcmp(baseletter, "dz") ||
			!strcmp(baseletter, "ng") ||
			!strcmp(baseletter, "ny") ||
			!strcmp(baseletter, "n") )
			return Yes;
		else return No;

		case '\'':
		if( !strcmp(baseletter, "kh") ||
			!strcmp(baseletter, "ch") ||
			!strcmp(baseletter, "th") ||
			!strcmp(baseletter, "ph") ||
			!strcmp(baseletter, "tsh") ||
			!strcmp(baseletter, "g") ||
			!strcmp(baseletter, "j") ||
			!strcmp(baseletter, "d") ||
			!strcmp(baseletter, "b") ||
			!strcmp(baseletter, "dz") )
			return Yes;
		else return No;
		}
		return No;
	}

	else //Wylie, THDLWylie
	{
		switch(c){
		case 'g':
		if( !strcmp(baseletter, "c") ||
			!strcmp(baseletter, "ny") ||
			!strcmp(baseletter, "t") ||
			!strcmp(baseletter, "d") ||
			!strcmp(baseletter, "n") ||
			!strcmp(baseletter, "ts") ||
			!strcmp(baseletter, "zh") ||
			!strcmp(baseletter, "z") ||
			!strcmp(baseletter, "sh") ||
			!strcmp(baseletter, "s") ||
			!strcmp(baseletter, ".y") )
			return Yes;
		else return No;

		case 'd':
		if( !strcmp(baseletter, "k") ||
			!strcmp(baseletter, "p") ||
			!strcmp(baseletter, "g") ||
			!strcmp(baseletter, "b") ||
			!strcmp(baseletter, "ng") ||
			!strcmp(baseletter, "m") )
			return Yes;
		else return No;

		case 'b':
		if( !strcmp(baseletter, "c") ||
			!strcmp(baseletter, "ny") || //New added!!
			!strcmp(baseletter, "ng") ||  //20080620
			!strcmp(baseletter, "g") ||
			!strcmp(baseletter, "t") ||
			!strcmp(baseletter, "d") ||
			!strcmp(baseletter, "n") || //New added!!
			!strcmp(baseletter, "ts") ||
			!strcmp(baseletter, "zh") ||
			!strcmp(baseletter, "z") ||
			!strcmp(baseletter, "sh") ||
			!strcmp(baseletter, "s") ||
			!strcmp(baseletter, "k") )
			return Yes;
		else return No;

		case 'm':
		if( !strcmp(baseletter, "kh") ||
			!strcmp(baseletter, "ch") ||
			!strcmp(baseletter, "th") ||
			!strcmp(baseletter, "tsh") ||
			!strcmp(baseletter, "g") ||
			!strcmp(baseletter, "j") ||
			!strcmp(baseletter, "d") ||
			!strcmp(baseletter, "dz") ||
			!strcmp(baseletter, "ng") ||
			!strcmp(baseletter, "ny") ||
			!strcmp(baseletter, "n") )
			return Yes;
		else return No;

		case '\'':
		if( !strcmp(baseletter, "kh") ||
			!strcmp(baseletter, "ch") ||
			!strcmp(baseletter, "th") ||
			!strcmp(baseletter, "ph") ||
			!strcmp(baseletter, "tsh") ||
			!strcmp(baseletter, "g") ||
			!strcmp(baseletter, "j") ||
			!strcmp(baseletter, "d") ||
			!strcmp(baseletter, "b") ||
			!strcmp(baseletter, "dz") )
			return Yes;
		else return No;
		}
		return No;
	}
}


//To examine a pair of components, see if one of them is a subscript and the other one is
//a base letter that can be followed by the subscript. Return Yes if it is, otherwise No.
int BaseletterSubscriptMatch(char *baseletter, char *subscript)
{
	char c;
	c = *subscript;

	if(SourceEncoding == ACIP)
	{
		switch(c){
		case 'Y':
		if( !strcmp(baseletter, "K") ||
			!strcmp(baseletter, "KH") ||
			!strcmp(baseletter, "G") ||
			!strcmp(baseletter, "P") ||
			!strcmp(baseletter, "PH") ||
			!strcmp(baseletter, "B") ||
			!strcmp(baseletter, "M") )
			return Yes;
		else return No;

		case 'R':
		if( !strcmp(baseletter, "K") ||
			!strcmp(baseletter, "T") ||
			!strcmp(baseletter, "P") ||
			!strcmp(baseletter, "KH") ||
			!strcmp(baseletter, "PH") ||
			!strcmp(baseletter, "G") ||
			!strcmp(baseletter, "D") ||
			!strcmp(baseletter, "B") ||
			!strcmp(baseletter, "H") ||
			!strcmp(baseletter, "M") ||
			!strcmp(baseletter, "S"))
			return Yes;
		else return No;


		case 'L':
		if( !strcmp(baseletter, "K") ||
			!strcmp(baseletter, "G") ||
			!strcmp(baseletter, "B") ||
			!strcmp(baseletter, "R") ||
			!strcmp(baseletter, "S") ||
			!strcmp(baseletter, "Z") )
			return Yes;
		else return No;

		case 'W':
		if( !strcmp(baseletter, "K") ||
			!strcmp(baseletter, "KH") ||
			!strcmp(baseletter, "G") ||
			!strcmp(baseletter, "NY") ||
			!strcmp(baseletter, "D") ||
			!strcmp(baseletter, "CH") ||
			!strcmp(baseletter, "ZH") ||
			!strcmp(baseletter, "Z") ||
			!strcmp(baseletter, "R") ||
			!strcmp(baseletter, "L") ||
			!strcmp(baseletter, "SH")||
			!strcmp(baseletter, "S") ||
			!strcmp(baseletter, "H") )
			return Yes;
		else return No;
		}
		return No;
	}
	else if(SourceEncoding == LCWylie)//LCWylie
	{
		switch(c){
		case 'y':
		if( !strcmp(baseletter, "k") ||
			!strcmp(baseletter, "kh") ||
			!strcmp(baseletter, "g") ||
			!strcmp(baseletter, "p") ||
			!strcmp(baseletter, "ph") ||
			!strcmp(baseletter, "b") ||
			!strcmp(baseletter, "m") )
			return Yes;
		else return No;

		case 'r':
		if( !strcmp(baseletter, "k") ||
			!strcmp(baseletter, "t") ||
			!strcmp(baseletter, "p") ||
			!strcmp(baseletter, "kh") ||
			!strcmp(baseletter, "ph") ||
			!strcmp(baseletter, "g") ||
			!strcmp(baseletter, "d") ||
			!strcmp(baseletter, "b") ||
			!strcmp(baseletter, "h") ||
			!strcmp(baseletter, "m") ||
			!strcmp(baseletter, "s"))
			return Yes;
		else return No;


		case 'l':
		if( !strcmp(baseletter, "k") ||
			!strcmp(baseletter, "g") ||
			!strcmp(baseletter, "b") ||
			!strcmp(baseletter, "r") ||
			!strcmp(baseletter, "s") ||
			!strcmp(baseletter, "z") )
			return Yes;
		else return No;

		case 'w':
		if( !strcmp(baseletter, "k") ||
			!strcmp(baseletter, "kh") ||
			!strcmp(baseletter, "g") ||
			!strcmp(baseletter, "鋘") ||
			!strcmp(baseletter, "d") ||
			!strcmp(baseletter, "ch") ||
			!strcmp(baseletter, "鈠") ||
			!strcmp(baseletter, "z") ||
			!strcmp(baseletter, "r") ||
			!strcmp(baseletter, "l") ||
			!strcmp(baseletter, "鈙")||
			!strcmp(baseletter, "s") ||
			!strcmp(baseletter, "h") )
			return Yes;
		else return No;

		}
		return No;
	}
	else //Wylie, THDLWylie
	{
		switch(c){
		case 'y':
		if( !strcmp(baseletter, "k") ||
			!strcmp(baseletter, "kh") ||
			!strcmp(baseletter, "g") ||
			!strcmp(baseletter, "p") ||
			!strcmp(baseletter, "ph") ||
			!strcmp(baseletter, "b") ||
			!strcmp(baseletter, "m") )
			return Yes;
		else return No;

		case 'r':
		if( !strcmp(baseletter, "k") ||
			!strcmp(baseletter, "t") ||
			!strcmp(baseletter, "p") ||
			!strcmp(baseletter, "kh") ||
			!strcmp(baseletter, "ph") ||
			!strcmp(baseletter, "g") ||
			!strcmp(baseletter, "d") ||
			!strcmp(baseletter, "b") ||
			!strcmp(baseletter, "h") ||
			!strcmp(baseletter, "m") ||
			!strcmp(baseletter, "s"))
			return Yes;
		else return No;


		case 'l':
		if( !strcmp(baseletter, "k") ||
			!strcmp(baseletter, "g") ||
			!strcmp(baseletter, "b") ||
			!strcmp(baseletter, "r") ||
			!strcmp(baseletter, "s") ||
			!strcmp(baseletter, "z") )
			return Yes;
		else return No;

		case 'w':
		if( !strcmp(baseletter, "k") ||
			!strcmp(baseletter, "kh") ||
			!strcmp(baseletter, "g") ||
			!strcmp(baseletter, "ny") ||
			!strcmp(baseletter, "d") ||
			!strcmp(baseletter, "ch") ||
			!strcmp(baseletter, "zh") ||
			!strcmp(baseletter, "z") ||
			!strcmp(baseletter, "r") ||
			!strcmp(baseletter, "l") ||
			!strcmp(baseletter, "sh")||
			!strcmp(baseletter, "s") ||
			!strcmp(baseletter, "h") )
			return Yes;
		else return No;

		}
		return No;
	}

}


//To examine a pair of components, see if one of them is a superscript and the other one is
//a base letter that can follow the superscript. Return Yes if it is, otherwise No.
int SuperscriptBaseletterMatch(char *superscript, char *baseletter)
{
	char c;
	c = *superscript;

	if(SourceEncoding == ACIP)
	{
		switch(c){
		case 'R':
		if( !strcmp(baseletter, "K") ||
			!strcmp(baseletter, "T") ||
			!strcmp(baseletter, "TZ") ||		//20080620
			!strcmp(baseletter, "G") ||
			!strcmp(baseletter, "J") ||
			!strcmp(baseletter, "D") ||
			!strcmp(baseletter, "B") ||
			!strcmp(baseletter, "DZ") ||
			!strcmp(baseletter, "NG") ||
			!strcmp(baseletter, "NY") ||
			!strcmp(baseletter, "N") ||
			!strcmp(baseletter, "M") )
			return Yes;
		else return No;

		case 'L':
		if( !strcmp(baseletter, "K") ||
			!strcmp(baseletter, "C") ||
			!strcmp(baseletter, "T") ||
			!strcmp(baseletter, "P") ||
			!strcmp(baseletter, "G") ||
			!strcmp(baseletter, "J") ||
			!strcmp(baseletter, "D") ||
			!strcmp(baseletter, "B") ||
			!strcmp(baseletter, "NG") ||
			!strcmp(baseletter, "H") )
			return Yes;
		else return No;

		case 'S':
		if( !strcmp(baseletter, "K") ||
			!strcmp(baseletter, "T") ||
			!strcmp(baseletter, "P") ||
			!strcmp(baseletter, "TZ") ||  //20080620
			!strcmp(baseletter, "G") ||
			!strcmp(baseletter, "D") ||
			!strcmp(baseletter, "B") ||
			!strcmp(baseletter, "NG") ||
			!strcmp(baseletter, "NY") ||
			!strcmp(baseletter, "N") ||
			!strcmp(baseletter, "M"))
			return Yes;
		else return No;

		}
		return No;
	}
	else if(SourceEncoding == LCWylie)
	{
		switch(c){
		case 'r':
		if( !strcmp(baseletter, "k") ||
			!strcmp(baseletter, "t") ||
			!strcmp(baseletter, "ts") ||
			!strcmp(baseletter, "g") ||
			!strcmp(baseletter, "j") ||
			!strcmp(baseletter, "d") ||
			!strcmp(baseletter, "b") ||
			!strcmp(baseletter, "dz") ||
			!strcmp(baseletter, "鏽") ||
			!strcmp(baseletter, "鋘") ||
			!strcmp(baseletter, "n") ||
			!strcmp(baseletter, "m") )
			return Yes;
		else return No;

		case 'l':
		if( !strcmp(baseletter, "k") ||
			!strcmp(baseletter, "c") ||
			!strcmp(baseletter, "t") ||
			!strcmp(baseletter, "p") ||
			!strcmp(baseletter, "g") ||
			!strcmp(baseletter, "j") ||
			!strcmp(baseletter, "d") ||
			!strcmp(baseletter, "b") ||
			!strcmp(baseletter, "鏽") ||
			!strcmp(baseletter, "h") )
			return Yes;
		else return No;

		case 's':
		if( !strcmp(baseletter, "k") ||
			!strcmp(baseletter, "t") ||
			!strcmp(baseletter, "p") ||
			!strcmp(baseletter, "ts") ||
			!strcmp(baseletter, "g") ||
			!strcmp(baseletter, "d") ||
			!strcmp(baseletter, "b") ||
			!strcmp(baseletter, "鏽") ||
			!strcmp(baseletter, "鋘") ||
			!strcmp(baseletter, "n") ||
			!strcmp(baseletter, "m"))
			return Yes;
		else return No;

		}
		return No;
	}
	else //Wylie, THDLWylie
	{
		switch(c){
		case 'r':
		if( !strcmp(baseletter, "k") ||
			!strcmp(baseletter, "t") ||
			!strcmp(baseletter, "ts") ||
			!strcmp(baseletter, "g") ||
			!strcmp(baseletter, "j") ||
			!strcmp(baseletter, "d") ||
			!strcmp(baseletter, "b") ||
			!strcmp(baseletter, "dz") ||
			!strcmp(baseletter, "ng") ||
			!strcmp(baseletter, "ny") ||
			!strcmp(baseletter, "n") ||
			!strcmp(baseletter, "m") )
			return Yes;
		else return No;

		case 'l':
		if( !strcmp(baseletter, "k") ||
			!strcmp(baseletter, "c") ||
			!strcmp(baseletter, "t") ||
			!strcmp(baseletter, "p") ||
			!strcmp(baseletter, "g") ||
			!strcmp(baseletter, "j") ||
			!strcmp(baseletter, "d") ||
			!strcmp(baseletter, "b") ||
			!strcmp(baseletter, "ng") ||
			!strcmp(baseletter, "h") )
			return Yes;
		else return No;

		case 's':
		if( !strcmp(baseletter, "k") ||
			!strcmp(baseletter, "t") ||
			!strcmp(baseletter, "p") ||
			!strcmp(baseletter, "ts") ||
			!strcmp(baseletter, "g") ||
			!strcmp(baseletter, "d") ||
			!strcmp(baseletter, "b") ||
			!strcmp(baseletter, "ng") ||
			!strcmp(baseletter, "ny") ||
			!strcmp(baseletter, "n") ||
			!strcmp(baseletter, "m"))
			return Yes;
		else return No;

		}
		return No;
	}
}


//declare the functions
int IsUnicodeBaseLetter(int);
int IsThisUnicodeVowel(int);


//Write back a Unicode
int WriteBackUnicode(int theUnicode)
{
	int ch1, ch2;

	if(TargetFileFormat == TXT) //200802
	{
		fputs( "Check Converter.h at Sector001\n", targetFile);
		return 0;
	}

	if(TargetFileFormat == UnicodeTXT)
	{
		ch1 = theUnicode >> 8;
		ch2 = theUnicode & 0xff;
		fputc( ch2, targetFile);
		fputc( ch1, targetFile);
	}
	else if(TargetFileFormat == RTF)
	{
		fputs( "\\u", targetFile);
		fprintf(targetFile, "%d", theUnicode);
	}
	else //(TargetFileFormat == WebPage)
	{
		fputs( "&#", targetFile);
		fprintf(targetFile, "%d", theUnicode);
	}

	return 1;
}


//Write back a Unicode
int WriteBackWideCharacter(char *theChar)  //20090427
{
	char *p;
	char ch1, ch2;
	char ch[10];
	int l;

	p = theChar;

	ch1 = *p;
	p++;
	ch2 = *p;

//l = (ch1<<8) + ch2;

	if(TargetFileFormat == UnicodeTXT)
	{
		fputc( ch2, targetFile);
		fputc( ch1, targetFile);
	}
	else if(TargetFileFormat == RTF)
	{
		if(ch1==0x20)
		{
			fputc( ch1, targetFile);
			return 1;
		}

		l = ch1;
		_itoa(l&0xff, ch, 16);
		fputs("\\\'", targetFile);
		fputs(ch, targetFile);

		l = ch2;
		_itoa(l&0xff, ch, 16);
		fputs("\\\'", targetFile);
		fputs(ch, targetFile);
	}
	else //(TargetFileFormat == WebPage) || (TargetFileFormat == TXT)
	{
		fputs(theChar, targetFile);
	}

	return 1;
}


int MyHashFunction(int character)
{
	if(character>=0x20 && character < 0x7f) return character;
	else if(character == 0xa7) return 0x10;
	else if(character == 0xe2) return 0x11;
	else if(character == 0xe4) return 0x12;
	else if(character == 0xe5) return 0x13;
	else if(character == 0xe7) return 0x14;
	else if(character == 0xef) return 0x15;
	else if(character == 0xf2) return 0x16;
	else if(character == 0xf4) return 0x17;
	else if(character == 0xf6) return 0x18;
	else return 0x19;
}


//Write back a run which has nCharater "Tibetan characters".    Character----a sequence of letters
int WriteBackRun(char *pRun, int nCharacter)
{
	int i;
	unsigned theUnicode;
	int *p;
	int n;
	char *thisRun;

	thisRun = pRun;

	if(nCharacter == 0) return No;

	//Output first character of the RUN
	n = *(thisRun + 4); //Number of letters in the first "character"
	//Look at the Hash table
	if( n == 1)
		p = pAW2Unicode + MyHashFunction((int)(*thisRun));
	else if( n == 2)
		p = pAW2Unicode + MyHashFunction((int)(*thisRun))*130 + MyHashFunction((int)(*(thisRun+1)));
	else if( n == 3)
		p = pAW2Unicode + MyHashFunction((int)(*thisRun))*130*130 + MyHashFunction((int)(*(thisRun+1)))*130 + MyHashFunction((int)(*(thisRun+1+1)));
	else
		return No;

	theUnicode = *p;

	if(IsThisUnicodeVowel(theUnicode)==Yes)//First character is a vowel, this means the constant is 'a'.
		WriteBackUnicode(0x0f68);//Write back 'a'--0x0f68 first!

	WriteBackUnicode(theUnicode);

	//Output rest of characters, include vowel!!
	for(i=1; i<nCharacter; i++)
	{
		thisRun += 5;

		if(IsThisTibetanOrSanskritVowel(thisRun) && (*thisRun == 'a' || *thisRun == 'A'))//vowel a, no need to output
			break;

		n = *(thisRun + 4);//Pick up the number of letters in the "character"
		//Look at the Hash table
		if( n == 1)
			p = pAW2Unicode + MyHashFunction((int)(*thisRun));
		else if( n == 2)
			p = pAW2Unicode + MyHashFunction((int)(*thisRun))*130 + MyHashFunction((int)(*(thisRun+1)));
		else if( n == 3)
			p = pAW2Unicode + MyHashFunction((int)(*thisRun))*130*130 + MyHashFunction((int)(*(thisRun+1)))*130 + MyHashFunction((int)(*(thisRun+1+1)));
		else
			return No;

		theUnicode = *p;

		if(IsUnicodeBaseLetter(theUnicode)) //20080620
		{
			if(i==nCharacter-2 && theUnicode == 0x0f60) theUnicode = 0x0f71; //Pick up the right achung.
			else theUnicode = theUnicode + 0x50; //change to combining letter...This is the key step!!!
		}

		WriteBackUnicode(theUnicode);
	}

	return Yes; //Well done
}



//Analysize the AW Tibetan Syllable, and translate it into Unicode sequence and write back
int TranslateAWTibetanSyllableToUnicodeAndWriteBack(char *TibetanSyllable, int nTibetanSyllable)  //20080620
{
	int i=0, m=0, n=0; //nCharacter--index for Wylie character,   TheSyllable[nCharacter][5]; //m < 5

	int IsTibetanSyllable = Yes; //0----Not,   1----Yes

	int nCharacter = 0, nConstant = 0, nVowel = 0;

	char thisCharacter[5];
	char TheSyllable[50][5];  //TheSyllable[nCharacter][letters];

	char Run[20][20][5]; //Run[nRun][nRunCharacter][5]
	int nRun = 1;
	int nRunCharacter = 0;
	int RunCharacter[20]; //Number of characters in each run.  RunCharacter[nRun]

	//Cut the Syllable into sequence of Wylie characters
	for( i=0; i<nTibetanSyllable; i++)
	{
		thisCharacter[m++] = TibetanSyllable[i];
		thisCharacter[m] = '\0';

		if(nTibetanSyllable == 1) //Those special letters, include \u0f0b
		{
			strcpy(TheSyllable[nCharacter++], thisCharacter);
			TheSyllable[nCharacter-1][4] = m; //remeber the length of a "character"
			m = 0;
			n = 0;
		}
		else if(m==1 && thisCharacter[0] == '+') //'+' appears, take special process
		{
			strcpy(TheSyllable[nCharacter++], thisCharacter);
			m = 0;
			n = 0;
		}
		else if ( IsThisTibetanOrSanskritSymbole(thisCharacter) )
		{
			n = m; //n--the size of the string that is a "Wylie character"

			if(m==3)
			{
				strcpy(TheSyllable[nCharacter++], thisCharacter);
				TheSyllable[nCharacter-1][4] = n; //remeber the length of a "character"
				m = 0;
				n = 0;
			}
			else if(i == nTibetanSyllable - 1) //It's the last character, get out from here
			{
				strcpy(TheSyllable[nCharacter++], thisCharacter);
				TheSyllable[nCharacter-1][4] = n; //remeber the length of a "character"
				break;
			}
			else continue; //See one more
		}
		else
		{
			if(m==3) //string of three letters is not legal "Wylie character"
			{
				if(n==0) //No substring is a legal "Wylie character"
				{
					IsTibetanSyllable = No;
					break;
				}
				else
				{
					thisCharacter[n] = '\0';
					strcpy(TheSyllable[nCharacter++], thisCharacter);
					TheSyllable[nCharacter-1][4] = n; //remeber the length of a "character"
					i -= m-n;

					m = 0;
					n = 0;
				}
			}
			else if(i == nTibetanSyllable - 1) //last character, get out from here
			{
				if(n==0) //No substring is a legal "Wylie character"
				{
					IsTibetanSyllable = No;
					break;
				}
				else
				{
					thisCharacter[n] = '\0';
					strcpy(TheSyllable[nCharacter++], thisCharacter);
					TheSyllable[nCharacter-1][4] = n; //remeber the length of a "character"
					i -= m-n;

					m = 0;
					n = 0;
				}
			}
			else continue; //Check next character
		}
	}
	if(IsTibetanSyllable == No) return No;


	//Generate Runs from TheSyllable[nCharacter][letters];
	RunCharacter[0] = Tibetan; //Use RunCharacter[0] as the flag to see if the first run is Sanskirt
	nRun = 1; //Starts 1, not 0
	for(i=0; i<nCharacter; i++)
	{
		if(IsThisSanskritSymbol(TheSyllable[i]))
		{
			RunCharacter[0] = Sanskrit;
		}

		if(IsThisTibetanOrSanskritVowel(TheSyllable[i]))//It's vowel; --- Run Boundary
		{
			strcpy(Run[nRun][nRunCharacter++], TheSyllable[i]);//Index starts 0, not 1, ...
			Run[nRun][nRunCharacter-1][4] = TheSyllable[i][4]; //Remember the length of the character
			RunCharacter[nRun] = nRunCharacter; //Index starts 1, not 0, ...
			nRunCharacter = 0; //New run
			nRun ++;
			nVowel ++; //Remeber the number of vowl
		}
		else if(i+1 == nCharacter)//Last "character" of the syllable
		{
			strcpy(Run[nRun][nRunCharacter++], TheSyllable[i]);//Index starts 0, not 1, ...
			Run[nRun][nRunCharacter-1][4] = TheSyllable[i][4]; //Remember the length of the character
			RunCharacter[nRun] = nRunCharacter; //Index starts 1, not 0, ...
			nRunCharacter = 0; //New run
			nRun ++;
		}
		else if(TheSyllable[i][0] == '+')//Ignore the character '+'
		{
			RunCharacter[0] = Sanskrit;
		}
		else
		{
			if(IsThisSanskritSymbol(TheSyllable[i]))
			{
				RunCharacter[0] = Sanskrit;
			}
			strcpy(Run[nRun][nRunCharacter++], TheSyllable[i]);
			Run[nRun][nRunCharacter-1][4] = TheSyllable[i][4]; //Remember the length of the character
		}
	}
	nRun --;

	//Translate the syllable into Unicode sequence
	if(RunCharacter[0] == Sanskrit) //one is Sanskrit, so all are Sanskrit runs
	{
		for(i=1; i<nRun+1; i++)
		{
			WriteBackRun((char *)Run[i], RunCharacter[i]);
		}
	}
	else if(nVowel != 1)  //nVowel ==0  || nVowel >=2: not a Tibetan syllable
	{
		if(nVowel == 2 && nRun == 2 && Run[2][0][0] == '\'')//nVowel == 2
		{
			if((SourceEncoding == ACIP && (Run[2][1][0] == 'I'||Run[2][1][0] == 'U'||Run[2][1][0] == 'E'||Run[2][1][0] == 'O')) ||
			   (SourceEncoding != ACIP && (Run[2][1][0] == 'i'||Run[2][1][0] == 'u'||Run[2][1][0] == 'e'||Run[2][1][0] == 'o')))
			{
				//Work on first run
				nConstant = RunCharacter[1] - 1; //Number of constants in first run

				switch(nConstant)
				{
				case 1://Single-Character-Run syllable
					WriteBackRun((char *)Run[1], RunCharacter[1]);
					break;

				case 2://First run with two charaters
					if(BaseletterSubscriptMatch(Run[1][0], Run[1][1]) || SuperscriptBaseletterMatch(Run[1][0], Run[1][1]))
					{
						WriteBackRun((char *)Run[1], RunCharacter[1]);
					}
					else if(PrefixBaseletterMatch(Run[1][0], Run[1][1]))//There are two runs, not one.
					{
						WriteBackRun((char *)Run[1], 1);
						WriteBackRun((char *)Run[1][1], RunCharacter[1]-1);
					}
					else WriteBackRun((char *)Run[1], RunCharacter[1]);
					break;

				case 3://There are three characters in the first run
					if(SuperscriptBaseletterMatch(Run[1][0], Run[1][1]) &&
						BaseletterSubscriptMatch(Run[1][1], Run[1][2])) //One Run
					{
						WriteBackRun((char *)Run[1], RunCharacter[1]);
					}
					else if(SuperscriptBaseletterMatch(Run[1][1], Run[1][2]) &&
							PrefixBaseletterMatch(Run[1][0], Run[1][2]))
					{
						WriteBackRun((char *)Run[1], 1);
						WriteBackRun((char *)Run[1][1], RunCharacter[1]-1);
					}
					else if(PrefixBaseletterMatch(Run[1][0], Run[1][1]) &&
							BaseletterSubscriptMatch(Run[1][1], Run[1][2]))
					{
						WriteBackRun((char *)Run[1], 1);
						WriteBackRun((char *)Run[1][1], RunCharacter[1]-1);
					}
					else WriteBackRun((char *)Run[1], RunCharacter[1]);
					break;

				case 4://Four characters in the first run
					if(PrefixBaseletterMatch(Run[1][0], Run[1][2]) &&
						BaseletterSubscriptMatch(Run[1][2], Run[1][3]) &&
						SuperscriptBaseletterMatch(Run[1][1], Run[1][2]))
					{
						WriteBackRun((char *)Run[1], 1);
						WriteBackRun((char *)Run[1][1], RunCharacter[1]-1);
					}
					else WriteBackRun((char *)Run[1], RunCharacter[1]);//20040310
					break;

				default://No constant or there are more than 4 characters in the first run
					WriteBackRun((char *)Run[1], RunCharacter[1]);
				}
				//Work on second run
				WriteBackRun((char *)Run[2], RunCharacter[2]);
			}
			else //Syllable is not ended by 'i  or  'I
			{
				for(i=1; i<nRun+1; i++)
				{
					WriteBackRun((char *)Run[i], RunCharacter[i]);
				}
			}
		}
		else //nVowel == 0  || nVowel > 2
		{
			for(i=1; i<nRun+1; i++)
			{
				WriteBackRun((char *)Run[i], RunCharacter[i]);
			}
		}
	}
	else //(RunCharacter[0] == Tibetan)
	{
		//Work on first run
		nConstant = RunCharacter[1] - 1; //Number of constants in first run

		switch(nConstant)
		{
		case 1://Single-Character-Run syllable
			WriteBackRun((char *)Run[1], RunCharacter[1]);
			break;

		case 2://First run with two charaters
			if(BaseletterSubscriptMatch(Run[1][0], Run[1][1]) || SuperscriptBaseletterMatch(Run[1][0], Run[1][1]))
			{
				WriteBackRun((char *)Run[1], RunCharacter[1]);
			}
			else if(PrefixBaseletterMatch(Run[1][0], Run[1][1]))//There are two runs, not one.
			{
				WriteBackRun((char *)Run[1], 1);
				WriteBackRun((char *)Run[1][1], RunCharacter[1]-1);
			}
			else WriteBackRun((char *)Run[1], RunCharacter[1]);
			break;

		case 3://There are three characters in the first run
			if(SuperscriptBaseletterMatch(Run[1][0], Run[1][1]) &&
				BaseletterSubscriptMatch(Run[1][1], Run[1][2])) //One Run
			{
				WriteBackRun((char *)Run[1], RunCharacter[1]);
			}
			else if(SuperscriptBaseletterMatch(Run[1][1], Run[1][2]) &&
				    PrefixBaseletterMatch(Run[1][0], Run[1][2]))
			{
				WriteBackRun((char *)Run[1], 1);
				WriteBackRun((char *)Run[1][1], RunCharacter[1]-1);
			}
			else if(PrefixBaseletterMatch(Run[1][0], Run[1][1]) &&
				    BaseletterSubscriptMatch(Run[1][1], Run[1][2]))
			{
				WriteBackRun((char *)Run[1], 1);
				WriteBackRun((char *)Run[1][1], RunCharacter[1]-1);
			}
			else WriteBackRun((char *)Run[1], RunCharacter[1]);
			break;

		case 4://Four characters in the first run
			if(PrefixBaseletterMatch(Run[1][0], Run[1][2]) &&
				BaseletterSubscriptMatch(Run[1][2], Run[1][3]) &&
				SuperscriptBaseletterMatch(Run[1][1], Run[1][2]))
			{
				WriteBackRun((char *)Run[1], 1);
				WriteBackRun((char *)Run[1][1], RunCharacter[1]-1);
			}
			else WriteBackRun((char *)Run[1], RunCharacter[1]);//20040310
			break;

		default://No constant or there are more than 4 characters in the first run
			WriteBackRun((char *)Run[1], RunCharacter[1]);
		}

		//Work on second run
		if(nRun == 2)
		{
			if(nVowel == 1)
			{
				nConstant = RunCharacter[2];
				if(nConstant == 1 && IsSuffix(Run[2][0]))//Only one suffix
				{
					WriteBackRun((char *)Run[2], 1);
				}
				else if(nConstant == 2 && IsSuffix(Run[2][0]) && IsSecondSuffix(Run[2][1]))//Two suffixes
				{
					WriteBackRun((char *)Run[2][0], 1);
					WriteBackRun((char *)Run[2][1], 1);
				}
			}
			else
			{
				WriteBackRun((char *)Run[2], RunCharacter[2]);
			}
		}
		else if(nRun>2)
		{
			for(i=1; i<nRun+1; i++)
			{
				WriteBackRun((char *)Run[i], RunCharacter[i]);
			}
		}
		else ;
	}

	return Translated;
}



//---------------Unicode Tibetan Syllable Utilities------------------------------------

//To examine a component to see if it is a vowel. Return true if the component is vowel.
int IsUnicodeTibetanVowel(int thecomponent)
{

	if( (thecomponent==0xf72) ||
		(thecomponent==0xf74) ||
		(thecomponent==0xf7a) ||
		(thecomponent==0xf7c) )
		return Yes;
	else return No;
}


//To examine a component, see if it is a prefix. Return true if the component is prefix.
int IsUnicodePrefix(int thecomponent)
{

	if( (thecomponent==0xf42) ||
		(thecomponent==0xf51) ||
		(thecomponent==0xf56) ||
		(thecomponent==0xf58) ||
		(thecomponent==0xf60) )
		return Yes;
	else return No;

}


//To examine a component, see if it is a base (non-combining) letter. Return Yes if it is.
int IsUnicodeBaseLetter(int thecomponent)
{
	if( (thecomponent>=0xf40) && (thecomponent<=0xf6a))
		return Yes;
	else return No;
}


//To examine a component, see if it is a constant. Return Yes if it is.
int IsTibetanUnicodeConstant(int thecomponent)
{
	if( ((thecomponent>=0xf40) && (thecomponent<=0xf6a)) ||
		((thecomponent>=0xf90) && (thecomponent<=0xfbc)) ||
		thecomponent == 0xf71 )	return Yes;
	else return No;
}


//To examine a component, see if it is a base (non-combining) letter. Return Yes if it is.
int IsUnicodeOtherBaseLetter(int thecomponent)
{
	if( ((thecomponent>=0xf88) && (thecomponent<=0xf8b)) ||
		thecomponent == 0xf85 )
		return Yes;
	else return No;
}



//To examine a component, see if it is combining letter. Return Yes if it is.
int IsUnicodeCombiningCharacter(int thecomponent)
{
	if(thecomponent>0xf70 && thecomponent<0xfbd && !IsUnicodeOtherBaseLetter(thecomponent))
		return Yes;
	else return No;
}


//To examine a component, see if it is a base letter. Return Yes if it is.
int IsUnicodeCombiningLetter(int thecomponent)
{
	if( ((thecomponent>=0xf90) && (thecomponent<=0xfbc)) || thecomponent == 0xf71 )
		return Yes;
	else return No;
}



int IsUnicodeMark(int thecomponent)
{
	if( (thecomponent==0xf7e) ||
		(thecomponent==0xf7f) ||
		(thecomponent==0xf82) ||
		(thecomponent==0xf83) ||
		(thecomponent==0xf84) ||
		(thecomponent==0xf86) ||
		(thecomponent==0xf87) ||
		(thecomponent==0xf39) ||
		(thecomponent==0xf37) ||
		(thecomponent==0xf35))
		return Yes;
	else return No;
}


//To examine a component,  see if it is a supperscript. Return Yes if it is, otherwise No.
int IsUnicodeSuperscript(int thecomponent)
{
	if( (thecomponent==0xf62) ||
		(thecomponent==0xf6a) ||
		(thecomponent==0xf63) ||
		(thecomponent==0xf66) )
		return Yes;
	else return No;
}


//To examine a component,  see if it is a subscript. Return Yes if it is, otherwise No.
int IsUnicodeSubscript(int thecomponent)
{
	if( (thecomponent==0xfad) ||
		(thecomponent==0xfb1) ||
		(thecomponent==0xfb2) ||
		(thecomponent==0xfb3) )
		return Yes;
	else return No;
}


//To examine a component,  see if it is a suffix. Return Yes if it is, otherwise No.
int IsUnicodeSuffix(int thecomponent)
{
	if( (thecomponent==0xf42) ||
		(thecomponent==0xf44) ||
		(thecomponent==0xf51) ||
		(thecomponent==0xf53) ||
		(thecomponent==0xf56) ||
		(thecomponent==0xf58) ||
		(thecomponent==0xf60) ||
		(thecomponent==0xf62) ||
		(thecomponent==0xf63) ||
		(thecomponent==0xf66) )
		return Yes;
	else return No;
}


//To examine a component,  see if it is a the second suffix. Return Yes if it is, otherwise No.
int IsUnicodeSecondSuffix(int thecomponent)
{
	if(thecomponent==0xf66)
		return Yes;
	else return No;
}


//To examine a component,  see if it is a Sanskrit consonant. Return Yes if it is, otherwise No.
int IsUnicodeSanskritConsonant(int thecomponent)
{
	if( (thecomponent==0xf43) ||
		(thecomponent==0xf4a) ||
		(thecomponent==0xf4b) ||
		(thecomponent==0xf4c) ||
		(thecomponent==0xf4d) ||
		(thecomponent==0xf4e) ||
		(thecomponent==0xf52) ||
		(thecomponent==0xf57) ||
		(thecomponent==0xf5c) ||
		(thecomponent==0xf65) ||
		(thecomponent==0xf69))
		return Yes;
	else return No;
}


//To examine a component,  see if it is a Sanskrit vowel. Return Yes if it is, otherwise No.
int IsUnicodeSanskritVowel(int thecomponent)
{
	if( (thecomponent>0xf71) &&
		(thecomponent<0xf82) &&
		(thecomponent!=0xf7e) &&
		(thecomponent!=0xf7f) &&
		(thecomponent!=0xf72) &&
		(thecomponent!=0xf74) &&
		(thecomponent!=0xf7a) &&
		(thecomponent!=0xf7c) )
		return Yes;
	else return No;
}


//To examine a component,  see if it is a Sanskrit symbole. Return Yes if it is, otherwise No.
int IsUnicodeSanskritSpecialSymbol(int thecomponent)
{
	if( ((thecomponent>0xf81) &&
		(thecomponent<0xf8b)) ||
		(thecomponent==0xf7e) ||
		(thecomponent==0xf7f) )
		return Yes;
	else return No;
}


//To examine a component,  see if it is a Tibetan symbol. Return Yes if it is, otherwise No.
int IsThisUnicodeTibetanSymbol(int thecomponent)
{
	if((IsUnicodeTibetanVowel(thecomponent) ||
		IsUnicodeBaseLetter(thecomponent) ||
		IsUnicodeCombiningLetter(thecomponent) ) && (
		!IsUnicodeSanskritConsonant(thecomponent) &&
		!IsUnicodeSanskritVowel(thecomponent))
	)
		return Yes;
	else return No;
}


//To examine a component,  see if it is a Sanskrit symbol. Return Yes if it is, otherwise No.
int IsThisUnicodeSanskritSymbol(int thecomponent)
{
	if( IsUnicodeSanskritVowel(thecomponent) ||
		IsUnicodeSanskritConsonant(thecomponent) ||
		IsUnicodeSanskritSpecialSymbol(thecomponent))
		return Yes;
	else return No;
}


//To examine a component,  see if it is a Tibetan symbol. Return Yes if it is, otherwise No.
int IsThisUnicodeTibetanOrSanskritSymbol(int thecomponent)
{
	if( IsThisUnicodeTibetanSymbol(thecomponent) ||
		IsThisUnicodeSanskritSymbol(thecomponent) )
		return Yes;
	else return No;
}


//To examine a component, see if it is a Tibetan or Sanskrit symbol. Return Yes if it is, otherwise No.
int IsThisUnicodeVowel(int thecomponent)
{
	if(IsUnicodeSanskritVowel(thecomponent) ||
		IsUnicodeTibetanVowel(thecomponent))
		return Yes;
	else return No;
}


//To examine a pair of components, see if one of them is a prefix and the other one is
//a base letter that can follow the prefix. Return Yes if it is, otherwise No.
int UnicodePrefixBaseletterMatch(int prefix, int baseletter)
{
	if(baseletter>=0xf90) baseletter = baseletter - 0x50;
	if(baseletter==0xfba) baseletter = 0xf5d;
	if(baseletter==0xfbb) baseletter = 0xf61;
	if(baseletter==0xfbc) baseletter = 0xf62;

	switch(prefix){
	case 0xf42:
	if( (baseletter==0xf45) ||
		(baseletter==0xf49) ||
		(baseletter==0xf4f) ||
		(baseletter==0xf51) ||
		(baseletter==0xf53) ||
		(baseletter==0xf59) ||
		(baseletter==0xf5e) ||
		(baseletter==0xf5f) ||
		(baseletter==0xf64) ||
		(baseletter==0xf66) ||
		(baseletter==0xf61) )
		return Yes;
	else return No;

	case 0xf51:
	if( (baseletter==0xf40) ||
		(baseletter==0xf54) ||
		(baseletter==0xf42) ||
		(baseletter==0xf56) ||
		(baseletter==0xf44) ||
		(baseletter==0xf58) )
		return Yes;
	else return No;

	case 0xf56:
	if( (baseletter==0xf45) ||
		(baseletter==0xf42) ||
		(baseletter==0xf49) ||
		(baseletter==0xf4f) ||
		(baseletter==0xf51) ||
		(baseletter==0xf53) ||
		(baseletter==0xf59) ||
		(baseletter==0xf5e) ||
		(baseletter==0xf5f) ||
		(baseletter==0xf64) ||
		(baseletter==0xf66) ||
		(baseletter==0xf40) )
		return Yes;
	else return No;

	case 0xf58:
	if( (baseletter==0xf41) ||
		(baseletter==0xf46) ||
		(baseletter==0xf50) ||
		(baseletter==0xf5a) ||
		(baseletter==0xf42) ||
		(baseletter==0xf47) ||
		(baseletter==0xf51) ||
		(baseletter==0xf5b) ||
		(baseletter==0xf44) ||
		(baseletter==0xf49) ||
		(baseletter==0xf53) )
		return Yes;
	else return No;

	case 0xf60:
	if( (baseletter==0xf41) ||
		(baseletter==0xf46) ||
		(baseletter==0xf50) ||
		(baseletter==0xf55) ||
		(baseletter==0xf5a) ||
		(baseletter==0xf42) ||
		(baseletter==0xf47) ||
		(baseletter==0xf51) ||
		(baseletter==0xf56) ||
		(baseletter==0xf5b) )
		return Yes;
	else return No;

	}
	return No;
}


//To examine a pair of components, see if one of them is a subscript and the other one is
//a base letter that can be followed by the subscript. Return Yes if it is, otherwise No.
int UnicodeBaseletterSubscriptMatch(int baseletter, int subscript)
{
	if(baseletter>=0xf90) baseletter = baseletter - 0x50;
	if(baseletter==0xfba) baseletter = 0xf5d;
	if(baseletter==0xfbb) baseletter = 0xf61;
	if(baseletter==0xfbc) baseletter = 0xf62;

	switch(subscript){

	case 0xfb1:
	if( (baseletter==0xf40) ||
		(baseletter==0xf41) ||
		(baseletter==0xf42) ||
		(baseletter==0xf54) ||
		(baseletter==0xf55) ||
		(baseletter==0xf56) ||
		(baseletter==0xf58) )
		return Yes;
	else return No;

	case 0xfb2:
	if( (baseletter==0xf40) ||
		(baseletter==0xf4f) ||
		(baseletter==0xf54) ||
		(baseletter==0xf41) ||
		(baseletter==0xf55) ||
		(baseletter==0xf42) ||
		(baseletter==0xf51) ||
		(baseletter==0xf56) ||
		(baseletter==0xf67) ||
		(baseletter==0xf58) ||
		(baseletter==0xf66))
		return Yes;
	else return No;


	case 0xfb3:
	if( (baseletter==0xf40) ||
		(baseletter==0xf42) ||
		(baseletter==0xf56) ||
		(baseletter==0xf62) ||
		(baseletter==0xf66) ||
		(baseletter==0xf5f) )
		return Yes;
	else return No;

	case 0xfad:
	if( (baseletter==0xf40) ||
		(baseletter==0xf41) ||
		(baseletter==0xf42) ||
		(baseletter==0xf49) ||
		(baseletter==0xf51) ||
		(baseletter==0xf46) ||
		(baseletter==0xf5e) ||
		(baseletter==0xf5f) ||
		(baseletter==0xf62) ||
		(baseletter==0xf63) ||
		(baseletter==0xf64) ||
		(baseletter==0xf66) ||
		(baseletter==0xf67) )
		return Yes;
	else return No;

	}
	return No;
}


//To examine a pair of components, see if one of them is a superscript and the other one is
//a base letter that can follow the superscript. Return Yes if it is, otherwise No.
int UnicodeSuperscriptBaseletterMatch(int superscript, int baseletter)
{
	switch(superscript){

	case 0xf62:
	case 0xf6a:
	if( (baseletter==0xf90) ||
		(baseletter==0xf9f) ||
		(baseletter==0xfa9) ||
		(baseletter==0xf92) ||
		(baseletter==0xf97) ||
		(baseletter==0xfa1) ||
		(baseletter==0xfa6) ||
		(baseletter==0xfab) ||
		(baseletter==0xf94) ||
		(baseletter==0xf99) ||
		(baseletter==0xfa3) ||
		(baseletter==0xfa8) )
		return Yes;
	else return No;

	case 0xf63:
	if( (baseletter==0xf90) ||
		(baseletter==0xf95) ||
		(baseletter==0xf9f) ||
		(baseletter==0xfa4) ||
		(baseletter==0xf92) ||
		(baseletter==0xf97) ||
		(baseletter==0xfa1) ||
		(baseletter==0xfa6) ||
		(baseletter==0xf94) ||
		(baseletter==0xfb7) )
		return Yes;
	else return No;

	case 0xf66:
	if( (baseletter==0xf90) ||
		(baseletter==0xf9f) ||
		(baseletter==0xfa4) ||
		(baseletter==0xfa9) ||
		(baseletter==0xf92) ||
		(baseletter==0xfa1) ||
		(baseletter==0xfa6) ||
		(baseletter==0xf94) ||
		(baseletter==0xf99) ||
		(baseletter==0xfa3) ||
		(baseletter==0xfa8))
		return Yes;
	else return No;

	}
	return No;
}



//Return the base letter of a run, and info about which script
int ItsBaseLetter(int *ThisRun, int nLetter, int *Script)
{
	int i;
	int nCombiningLetter = 0, nVowel = 0, nMark = 0;
	int *run;
	int BaseLetter;

	run = ThisRun;

	//Count the components of this run
	for(i=0; i<nLetter; i++)
	{
		ThisRun ++;
		if(IsUnicodeCombiningLetter(*ThisRun)) nCombiningLetter ++;
		else if(IsThisUnicodeVowel(*ThisRun)) nVowel ++;
		else if(IsUnicodeMark(*ThisRun)) nMark ++;
		else ; //Base letter
	}

	switch(nCombiningLetter)
	{
	case 0:
		BaseLetter = run[1]; *Script = Tibetan;
		break;

	case 1:
		if(UnicodeBaseletterSubscriptMatch(run[1], run[2]))
		{
			*Script = Tibetan;
			BaseLetter = run[1];
		}
		else if(UnicodeSuperscriptBaseletterMatch(run[1], run[2]))
		{
			*Script = Tibetan;
			BaseLetter = run[2];
		}
		else//Sanskrit
		{
			*Script = Sanskrit;
			BaseLetter = 0;
		}
		break;

	case 2:
		if(UnicodeSuperscriptBaseletterMatch(run[1], run[2]) &&
		   UnicodeBaseletterSubscriptMatch(run[2], run[3])) // Tibetan
		{
			*Script = Tibetan;
			BaseLetter = run[2];
		}
		else
		{
			*Script = Sanskrit;
			BaseLetter = 0;
		}
		break;

	default: //has more than 3 consotants, all are treated as Sanskrit
			*Script = Sanskrit;
			BaseLetter = 0;
	}

	return BaseLetter;
}


//Write back a string to traget file, take care the TargetFileFormat
int WriteBackString(char *outputString)
{
	char *theString;

	theString = outputString;

	if(TargetFileFormat == TXT)
		fputs(theString, targetFile);

	else if(TargetFileFormat == UnicodeTXT)
	{
		while(*theString != '\0')
		{
			fputc(*theString, targetFile);
			fputc(0x00, targetFile);
			theString++;
		}
	}

	else if(TargetFileFormat == RTF)
	{
		while(*theString != '\0')
		{
			if(*theString=='\\' || *theString=='{' || *theString=='}')
				fputc('\\', targetFile);
			fputc(*theString, targetFile);
			theString++;
		}
	}

	else //TargetFileFormat == WebPage
	{
		while(*theString != '\0')
		{
			if(*theString==34 || *theString==38 || *theString==39 || *theString==60 || *theString==62 || *theString > 159)
			{
				fputs("&#", targetFile);
				fprintf(targetFile, "%d", (int)*theString);
				fputc(';', targetFile);
			}
			else fputc(*theString, targetFile);
			theString ++;
		}
	}

	return 1;
}



//Write back a string to traget file, take care the TargetFileFormat
int WriteBackCharacter(char Character)
{
	if(TargetFileFormat == TXT)
		fputc(Character, targetFile);

	else if(TargetFileFormat == UnicodeTXT)
	{
		fputc(Character, targetFile);
		fputc(0x00, targetFile);
	}

	else if(TargetFileFormat == RTF)
	{
		if(Character=='\\' || Character=='{' || Character=='}')
				fputc('\\', targetFile);
			fputc(Character, targetFile);
	}

	else //TargetFileFormat == WebPage
	{
		if(Character==34 || Character==38 || Character==39 || Character==60 || Character==62 || Character > 159)
		{
			fputs("&#", targetFile);
			fprintf(targetFile, "%d", (int)Character);
			fputc(';', targetFile);
		}
		else fputc(Character, targetFile);
	}

	return 1;
}



//Output a run
int OutputOneRun(int *ThisRun, int nLetter, int whichWylie)
{
	int i;
	int nCombiningLetter = 0, nVowel = 0, nMark = 0;
	int *run;
	int aChungAppear = 0;
	char *vowel;

	run = ThisRun;

	//Count the components of this run
	for(i=0; i<nLetter; i++)
	{
		ThisRun ++;
		if(IsUnicodeCombiningLetter(*ThisRun)) nCombiningLetter ++;
		else if(IsThisUnicodeVowel(*ThisRun)) nVowel ++;
		else if(IsUnicodeMark(*ThisRun)) nMark ++;
		else ; //Base letter
	}

	if(nCombiningLetter>=1 && (run[nCombiningLetter+1]==0x0f71 || run[nCombiningLetter+1]==0x0fb0))
		aChungAppear = Yes;

	//Output base letter and combining letter if any
	switch(nCombiningLetter)
	{
	case 0:
		if(run[1] != 0x0f68) //Do not write back 'a' or 'A' for 0x0f68
			WriteBackString(pUnicodeToAW + (run[1]-0xf00)*4*5 + (whichWylie - 1)*5);
		break;

	case 1:
		if(run[1] != 0x0f68) WriteBackString(pUnicodeToAW + (run[1]-0xf00)*4*5 + (whichWylie - 1)*5);
		if(!UnicodeBaseletterSubscriptMatch(run[1], run[2]) &&
		   !UnicodeSuperscriptBaseletterMatch(run[1], run[2])) // Sanskrit, Output '+' between
		{
			if(aChungAppear == No) WriteBackCharacter('+');
			else if(TargetEncoding == ACIP || TargetEncoding == Wylie) WriteBackCharacter('+');
			else;
		}

		if(aChungAppear == No) WriteBackString(pUnicodeToAW + (run[2]-0xf00)*4*5 + (whichWylie - 1)*5);
		else //aChungAppear = Yes
		{
			if(TargetEncoding == ACIP || TargetEncoding == Wylie) WriteBackString(pUnicodeToAW + (run[2]-0xf00)*4*5 + (whichWylie - 1)*5);
			else if(TargetEncoding == THDLWylie);
			else if(TargetEncoding == LCWylie) WriteBackCharacter((char)0xe5);
			else;
		}
		break;

	case 2:
		if(run[1] != 0x0f68) WriteBackString(pUnicodeToAW + (run[1]-0xf00)*4*5 + (whichWylie - 1)*5);

		if(run[1] == 0x0f42 && run[2] == 0x0fb2 && run[3] == 0x0fad)//For slob grwa "school"
		{
			WriteBackString(pUnicodeToAW + (run[2]-0xf00)*4*5 + (whichWylie - 1)*5);
			WriteBackString(pUnicodeToAW + (run[3]-0xf00)*4*5 + (whichWylie - 1)*5);
		}
		else if(!UnicodeSuperscriptBaseletterMatch(run[1], run[2]) ||
		   !UnicodeBaseletterSubscriptMatch(run[2], run[3])) // Sanskrit, Output '+' between
		{
			WriteBackCharacter('+');
			WriteBackString(pUnicodeToAW + (run[2]-0xf00)*4*5 + (whichWylie - 1)*5);

			//WriteBackCharacter('+');
			if(aChungAppear == No) WriteBackCharacter('+');
			else if(TargetEncoding == ACIP || TargetEncoding == Wylie) WriteBackCharacter('+');
			else;

			//WriteBackString(pUnicodeToAW + (run[3]-0xf00)*4*5 + (whichWylie - 1)*5);
			if(aChungAppear == No) WriteBackString(pUnicodeToAW + (run[3]-0xf00)*4*5 + (whichWylie - 1)*5);
			else //aChungAppear = Yes
			{
				if(TargetEncoding == ACIP || TargetEncoding == Wylie) WriteBackString(pUnicodeToAW + (run[3]-0xf00)*4*5 + (whichWylie - 1)*5);
				else if(TargetEncoding == THDLWylie);
				else if(TargetEncoding == LCWylie) WriteBackCharacter((char)0xe5);
				else;
			}
		}
		else
		{
			WriteBackString(pUnicodeToAW + (run[2]-0xf00)*4*5 + (whichWylie - 1)*5);
			WriteBackString(pUnicodeToAW + (run[3]-0xf00)*4*5 + (whichWylie - 1)*5);
		}
		break;

	default: //has more than 3 consotants, all are treated as Sanskrit
		for(i=0; i<nCombiningLetter ; i++)
		{
			WriteBackString(pUnicodeToAW + (run[i+1]-0xf00)*4*5 + (whichWylie - 1)*5);
			WriteBackCharacter('+');
		}
		WriteBackString(pUnicodeToAW + (run[nCombiningLetter+1]-0xf00)*4*5 + (whichWylie - 1)*5);
	}

	//Output 'a'   'A'
	if(nVowel == 0)
	{
		if(aChungAppear!=Yes)
		{
			if(TargetEncoding == ACIP) WriteBackCharacter('A');
			else WriteBackCharacter('a');
		}
		else //aChungAppear==Yes
		{
			if(TargetEncoding == ACIP) WriteBackCharacter('a');
			else WriteBackCharacter('A');
		}
	}

	//Output vowel and mark
	for(i=nCombiningLetter+1; i<nLetter ; i++)
	{
		if(aChungAppear==Yes && IsThisUnicodeVowel(run[i+1]) && TargetEncoding == THDLWylie)
		{
			vowel = pUnicodeToAW + (run[i+1]-0xf00)*4*5 + (whichWylie - 1)*5;
			vowel[0] = vowel[0] - 0x20;//Change to upercase.
			WriteBackString(vowel);
		}
		else WriteBackString(pUnicodeToAW + (run[i+1]-0xf00)*4*5 + (whichWylie - 1)*5);
	}

	return nCombiningLetter;
}



//Analysize the Unicode Tibetan Syllable, and translate it into AW sequence
int TranslateUnicodeTibetanSyllableToAWandWriteBack(int *tibetanSyllable, int nTibetanSyllable)
{
	int whichWylie;// 1--ACIP,  2--Wylie, 3--THDLWylie  4---LCWylie
	int Script, BaseLetter;
	int *TibetanSyllable;
	int Run[10][20];
	int i, j, k;
	int runNumber = 0;
	int nRun;
	char A7 = 0xa7;

	TibetanSyllable = tibetanSyllable;

	if(TargetEncoding == ACIP) whichWylie = 1;
	else if(TargetEncoding == Wylie) whichWylie = 2;
	else if(TargetEncoding == THDLWylie) whichWylie = 3;
	else if(TargetEncoding == LCWylie) whichWylie = 4;
	else whichWylie = 2; //default----Wylie;


	//For taking off those leading combining letters
	while (nTibetanSyllable!=0)
	{
		if(IsUnicodeCombiningCharacter(TibetanSyllable[0]))
		{
			nTibetanSyllable --;//Adjust the number of letters in the syllable
			for(i=0; i<nTibetanSyllable; i++) TibetanSyllable[i] = TibetanSyllable[i+1]; //Move on one point
		}
		else break;
	}
	if(nTibetanSyllable==0) return Translated;//The syllable does't have a non-combining letter. We don't transfer this syllable.


	//Seperate the syllable into runs
	for(j=0, k=1, i=0; i<nTibetanSyllable; i++)
	{
		if(TibetanSyllable[i] <= 0x0f6a || TibetanSyllable[i] == 0x0f85 ||
		  (TibetanSyllable[i] >= 0x0f88 && TibetanSyllable[i] <= 0x0f8b)) //Non-combining letters!!!!!!
		{
			Run[j][0] = k - 1; //Remember the number of letters of previous run

			j ++;	if(j>9) j = 9; //Protect the array boundry
			k = 1;
		}
		Run[j][k++] = TibetanSyllable[i]; if(k>18) k = 18;//Protect the array boundry
	}
	Run[j][0] = k - 1; // Remember number of letter of the last run
	nRun = j; //Remember number of run of the syllable


	//Analysize the syllable and output the translated result
	if(nRun == nTibetanSyllable)//All are non-combining letters, there is no combining letter in the syllable
	{
		if(nTibetanSyllable == 1)//one-letter-syllable
		{
			if(Run[1][1] != 0x0f68) WriteBackString(pUnicodeToAW + (Run[1][1]-0xf00)*4*5 + (whichWylie - 1)*5);
			if(Run[1][1] >= 0xf40 && Run[1][1] <= 0xf6a)//Base letters
			{
				if(TargetEncoding == ACIP) WriteBackCharacter('A');
				else WriteBackCharacter('a');
			}
		}
		else if(nTibetanSyllable == 2)
		{
			if(IsUnicodeSuffix(Run[2][1])) //Regular Tibetan
			{
				if(Run[1][1] != 0x0f68) WriteBackString(pUnicodeToAW + (Run[1][1]-0xf00)*4*5 + (whichWylie - 1)*5);
				if(Run[1][1] >= 0xf40 && Run[1][1] <= 0xf6a)//Base letters
				{
					if(TargetEncoding == ACIP) WriteBackCharacter('A');
					else WriteBackCharacter('a');
				}
				WriteBackString(pUnicodeToAW + (Run[2][1]-0xf00)*4*5 + (whichWylie - 1)*5);
			}
			else //just two letters
			{
				for(i=0; i<nRun ; i++)
				{
					if(Run[i+1][1] != 0x0f68) WriteBackString(pUnicodeToAW + (Run[i+1][1]-0xf00)*4*5 + (whichWylie - 1)*5);
					if(Run[i+1][1] >= 0xf40 && Run[i+1][1] <= 0xf6a)//Base letters
					{
						if(TargetEncoding == ACIP) WriteBackCharacter('A');
						else WriteBackCharacter('a');
					}
				}
			}
		}
		else if(nTibetanSyllable == 3)
		{
			if(IsUnicodeSuffix(Run[3][1]) && UnicodePrefixBaseletterMatch(Run[1][1], Run[2][1]))
			{
				if(Run[1][1] != 0x0f68) WriteBackString(pUnicodeToAW + (Run[1][1]-0xf00)*4*5 + (whichWylie - 1)*5);
				if(Run[1][1] == 0xf42 && Run[2][1] == 0xf61) //gyang --to-- g.yang
				{
					if(TargetEncoding==ACIP) WriteBackCharacter('-');
					else if(TargetEncoding==LCWylie) WriteBackCharacter(A7); //A7 = 0xa7
					else WriteBackCharacter('.');
				}
				WriteBackString(pUnicodeToAW + (Run[2][1]-0xf00)*4*5 + (whichWylie - 1)*5);
				if(Run[2][1] >= 0xf40 && Run[2][1] <= 0xf6a)//Base letters
				{
					if(TargetEncoding == ACIP) WriteBackCharacter('A');
					else WriteBackCharacter('a');
				}
				WriteBackString(pUnicodeToAW + (Run[3][1]-0xf00)*4*5 + (whichWylie - 1)*5);
			}
			else if(Run[2][1]==0xf60 && (Run[3][1]==0xf44 || Run[3][1]==0xf58)) //la'ng; ya'ng; ...
			{
				WriteBackString(pUnicodeToAW + (Run[1][1]-0xf00)*4*5 + (whichWylie - 1)*5);
				if(Run[1][1] >= 0xf40 && Run[1][1] <= 0xf6a)//Base letters
				{
					if(TargetEncoding == ACIP) WriteBackCharacter('A');
					else WriteBackCharacter('a');
				}
				WriteBackString(pUnicodeToAW + (Run[2][1]-0xf00)*4*5 + (whichWylie - 1)*5);
				WriteBackString(pUnicodeToAW + (Run[3][1]-0xf00)*4*5 + (whichWylie - 1)*5);
			}
			else if(IsUnicodeSuffix(Run[2][1]) && IsUnicodeSecondSuffix(Run[3][1])) //first one is base letter
			{
				WriteBackString(pUnicodeToAW + (Run[1][1]-0xf00)*4*5 + (whichWylie - 1)*5);
				if(Run[1][1] >= 0xf40 && Run[1][1] <= 0xf6a)//Base letters
				{
					if(TargetEncoding == ACIP) WriteBackCharacter('A');
					else WriteBackCharacter('a');
				}
				WriteBackString(pUnicodeToAW + (Run[2][1]-0xf00)*4*5 + (whichWylie - 1)*5);
				WriteBackString(pUnicodeToAW + (Run[3][1]-0xf00)*4*5 + (whichWylie - 1)*5);
			}
			else //Not Tibetan
			{
				for(i=0; i<nRun ; i++)
				{
					if(Run[i+1][1] != 0x0f68) WriteBackString(pUnicodeToAW + (Run[i+1][1]-0xf00)*4*5 + (whichWylie - 1)*5);
					if(Run[i+1][1] >= 0xf40 && Run[i+1][1] <= 0xf6a)//Base letters
					{
						if(TargetEncoding == ACIP) WriteBackCharacter('A');
						else WriteBackCharacter('a');
					}
				}
			}
		}
		else if(nTibetanSyllable == 4)
		{
			if(IsUnicodeSuffix(Run[3][1]) && IsUnicodeSecondSuffix(Run[4][1]) &&
				UnicodePrefixBaseletterMatch(Run[1][1], Run[2][1]) )//It's Tibetan syllable
			{
				if(Run[1][1] != 0x0f68) WriteBackString(pUnicodeToAW + (Run[1][1]-0xf00)*4*5 + (whichWylie - 1)*5);
				if(Run[1][1] == 0xf42 && Run[2][1] == 0xf61) //gyang --to-- g.yang
				{
					if(TargetEncoding==ACIP) WriteBackCharacter('-');
					else if(TargetEncoding==LCWylie) WriteBackCharacter(A7);
					else WriteBackCharacter('.');
				}
				WriteBackString(pUnicodeToAW + (Run[2][1]-0xf00)*4*5 + (whichWylie - 1)*5);
				if(Run[2][1] >= 0xf40 && Run[2][1] <= 0xf6a)//Base letters
				{
					if(TargetEncoding == ACIP) WriteBackCharacter('A');
					else WriteBackCharacter('a');
				}
				WriteBackString(pUnicodeToAW + (Run[3][1]-0xf00)*4*5 + (whichWylie - 1)*5);
				WriteBackString(pUnicodeToAW + (Run[4][1]-0xf00)*4*5 + (whichWylie - 1)*5);
			}
			else//Not Tibetan
			{
				for(i=0; i<nRun ; i++)
				{
					if(Run[i+1][1] != 0x0f68) WriteBackString(pUnicodeToAW + (Run[i+1][1]-0xf00)*4*5 + (whichWylie - 1)*5);
					if(Run[i+1][1] >= 0xf40 && Run[i+1][1] <= 0xf6a)//Base letters
					{
						if(TargetEncoding == ACIP) WriteBackCharacter('A');
						else WriteBackCharacter('a');
					}
				}
			}
		}
		else //nTibetanSyllable > 4   ----Not Tibetan
		{
			for(i=0; i<nRun ; i++)
			{
				if(Run[i+1][1] != 0x0f68) WriteBackString(pUnicodeToAW + (Run[i+1][1]-0xf00)*4*5 + (whichWylie - 1)*5);
				if(Run[i+1][1] >= 0xf40 && Run[i+1][1] <= 0xf6a)//Base letters
				{
					if(TargetEncoding == ACIP) WriteBackCharacter('A');
					else WriteBackCharacter('a');
				}
			}
		}
	} //End of if(nRun == nTibetanSyllable)

	else //at least one run has more than one letters
	{
		if(Run[1][0]==1)//only one letter is in first run, there must be at least the second RUN
		{
			if(Run[2][0] == 1)//at least two prefix, Not Tibetan
			{
				for(i=0; i<nRun; i++)
				{
					OutputOneRun(Run[i+1], Run[i+1][0], whichWylie);
				}
			}
			else//Run[2][0] > 1 --- Base letter is at second run
			{
				BaseLetter = ItsBaseLetter(Run[2], Run[2][0], &Script);

				if(Script == Tibetan && UnicodePrefixBaseletterMatch(Run[1][1], BaseLetter)) //So far, it's Tibetan
				{
					if(nRun == 2)//Tibetan
					{
						WriteBackString(pUnicodeToAW + (Run[1][1]-0xf00)*4*5 + (whichWylie - 1)*5);
						if(Run[1][1] == 0xf42 && BaseLetter == 0xf61) //gyang --to-- g.yang
						{
							if(TargetEncoding==ACIP) WriteBackCharacter('-');
							else if(TargetEncoding==LCWylie) WriteBackCharacter(A7);
							else WriteBackCharacter('.');
						}
						OutputOneRun(Run[2], Run[2][0], whichWylie);
					}
					else if(Run[3][0] == 1 && nRun == 3)
					{
						if(IsUnicodeSuffix(Run[3][1]))//Tibetan
						{
							WriteBackString(pUnicodeToAW + (Run[1][1]-0xf00)*4*5 + (whichWylie - 1)*5);
							if(Run[1][1] == 0xf42 && BaseLetter == 0xf61) //gyang --to-- g.yang
							{
								if(TargetEncoding==ACIP) WriteBackCharacter('-');
								else if(TargetEncoding==LCWylie) WriteBackCharacter(A7);
								else WriteBackCharacter('.');
							}
							OutputOneRun(Run[2], Run[2][0], whichWylie);
							WriteBackString(pUnicodeToAW + (Run[3][1]-0xf00)*4*5 + (whichWylie - 1)*5);
						}
						else //Sanskrit
						{
							for(i=0; i<nRun; i++)
								OutputOneRun(Run[i+1], Run[i+1][0], whichWylie);
						}
					}
					else if(Run[3][0] == 1 && Run[4][0] == 1 && nRun == 4) //4 runs
					{
						if(IsUnicodeSuffix(Run[3][1]) && IsUnicodeSecondSuffix(Run[4][1])) //Tibetan
						{
							WriteBackString(pUnicodeToAW + (Run[1][1]-0xf00)*4*5 + (whichWylie - 1)*5);
							if(Run[1][1] == 0xf42 && BaseLetter == 0xf61) //gyang --to-- g.yang
							{
								if(TargetEncoding==ACIP) WriteBackCharacter('-');
								else if(TargetEncoding==LCWylie) WriteBackCharacter(A7);
								else WriteBackCharacter('.');
							}
							OutputOneRun(Run[2], Run[2][0], whichWylie);
							WriteBackString(pUnicodeToAW + (Run[3][1]-0xf00)*4*5 + (whichWylie - 1)*5);
							WriteBackString(pUnicodeToAW + (Run[4][1]-0xf00)*4*5 + (whichWylie - 1)*5);
						}
						else //Sanskrit
						{
							for(i=0; i<nRun; i++)
								OutputOneRun(Run[i+1], Run[i+1][0], whichWylie);
						}
					}
					else //Sanskrit
					{
						for(i=0; i<nRun; i++)
							OutputOneRun(Run[i+1], Run[i+1][0], whichWylie);
					}
				}//End of if(Script == Tibetan && UnicodePrefixBaseletterMatch(Run[1][1], BaseLetter))
				else //Sanskrit
				{
					for(i=0; i<nRun; i++)
						OutputOneRun(Run[i+1], Run[i+1][0], whichWylie);
				}
			}//End of  else//Base letter is at second run
		}//End of  if(Run[1][0]==1)//only one letter is in first run, is prefix?
		else	//(Run[1][0]>1)//Base letter is assumed in first run
		{
			OutputOneRun(Run[1], Run[1][0], whichWylie);

			if(nRun == 1); //Do nothing. The run has already output.
			else if(Run[2][0] == 1 && nRun == 2)
			{
				if(IsUnicodeSuffix(Run[2][1]))//Tibetan
					WriteBackString(pUnicodeToAW + (Run[2][1]-0xf00)*4*5 + (whichWylie - 1)*5);
				else OutputOneRun(Run[2], Run[2][0], whichWylie);
			}
			else if(Run[2][0] == 1 && Run[3][0] == 1 && nRun == 3)
			{
				if(IsUnicodeSuffix(Run[2][1]) && IsUnicodeSecondSuffix(Run[3][1]))//Tibetan
				{
					WriteBackString(pUnicodeToAW + (Run[2][1]-0xf00)*4*5 + (whichWylie - 1)*5);
					WriteBackString(pUnicodeToAW + (Run[3][1]-0xf00)*4*5 + (whichWylie - 1)*5);
				}
				else
				{
					OutputOneRun(Run[2], Run[2][0], whichWylie);
					OutputOneRun(Run[3], Run[3][0], whichWylie);
				}
			}
			else //More than two base letters
			{
				for(i=1; i<nRun; i++)
					OutputOneRun(Run[i+1], Run[i+1][0], whichWylie);
			}
		}//else	//(Run[1][0]>1)//Base letter is assumed in first run
	} //else //at least one run has more than one letters

	return Translated;
}
//-----------------------------------------------------------------------




//////////////////////////////////////////////////////
//													//
//	Routines of Translaters 						//
//													//
//////////////////////////////////////////////////////

//-----------------------------------------------------------------------
//Translation routines for Unicode to AW
int TranslateUnicode2AWAndWriteBack()
{
	int i, l;
	int TibetanSyllable[50];
	int nTibetanSyllable = 0;

	if(nTibetan<=0) return TextBufferEmpty;

	//Setup formating....
	if(TargetFileFormat == RTF)
	{
		fputs("\\f", targetFile);
		fprintf(targetFile, "%d", targetFontIDs[0]);
		fputc(0x20, targetFile);
	}
	else if(TargetFileFormat == WebPage && SourceFileFormat != WebPage)
	{
		fputs("<font face=\"", targetFile);
		fputs(TibetanFontNames[BaseIndex[TargetEncoding]], targetFile);
		fputs("\" size=5>", targetFile);
	}
	else if(TargetFileFormat == WebPage && SourceFileFormat == WebPage)
	{
		WriteBackTibetanFontTag();
	}
	else; //TXT and UnicodeTXT

	//Start to translating....
	for(i=0; i<nTibetan; i++)
	{
		if(SourceFileFormat == WebPage)
			l = TibetanBufferForWebPage[i].ch; //Work on this buffer
		else
			l = TibetanBuffer[i].ch; //Work on this buffer

		if(l==0xf0c) l = 0xf0b;//20080229

		if(l>0xfbc || l<0xf00) continue; //Take off non-Tibetan characters!!!

		if(l>=0xf40 && l<0xfbd && nTibetanSyllable < 50) TibetanSyllable[nTibetanSyllable ++] = l;
		else if(l<0xf40 && l>=0xf00 ) //Other Tibetan letters
		{
			if(nTibetanSyllable > 0)
			{
				if(nTibetanSyllable > 2 && TibetanSyllable[nTibetanSyllable-1] == 0xf72 && TibetanSyllable[nTibetanSyllable-2] == 0xf60)//ABC'i
				{
					TranslateUnicodeTibetanSyllableToAWandWriteBack(TibetanSyllable, nTibetanSyllable-2);
					TibetanSyllable[0] = 0xf60;
					TibetanSyllable[1] = 0xf72;
					TranslateUnicodeTibetanSyllableToAWandWriteBack(TibetanSyllable, 2);
				}
				else TranslateUnicodeTibetanSyllableToAWandWriteBack(TibetanSyllable, nTibetanSyllable);
			}
			nTibetanSyllable = 0;

			TibetanSyllable[0] = l; //Process the boundry
			TranslateUnicodeTibetanSyllableToAWandWriteBack(TibetanSyllable, 1);
		}
		else //Latin, Chinese, ....
		{
			if(nTibetanSyllable > 0)
			{
				if(nTibetanSyllable > 2 && TibetanSyllable[nTibetanSyllable-1] == 0xf72 && TibetanSyllable[nTibetanSyllable-2] == 0xf60)//ABC'i
				{
					TranslateUnicodeTibetanSyllableToAWandWriteBack(TibetanSyllable, nTibetanSyllable-2);
					TibetanSyllable[0] = 0xf60;
					TibetanSyllable[1] = 0xf72;
					TranslateUnicodeTibetanSyllableToAWandWriteBack(TibetanSyllable, 2);
				}
				else TranslateUnicodeTibetanSyllableToAWandWriteBack(TibetanSyllable, nTibetanSyllable);
			}

			nTibetanSyllable = 0;
			fputc(0x20, targetFile);
		}

	}//End of for()

	if(nTibetanSyllable > 0) //one syllable left, process it
	{
		if(nTibetanSyllable > 2 && TibetanSyllable[nTibetanSyllable-1] == 0xf72 && TibetanSyllable[nTibetanSyllable-2] == 0xf60)//ABC'i
		{
			TranslateUnicodeTibetanSyllableToAWandWriteBack(TibetanSyllable, nTibetanSyllable-2);
			TibetanSyllable[0] = 0xf60;
			TibetanSyllable[1] = 0xf72;
			TranslateUnicodeTibetanSyllableToAWandWriteBack(TibetanSyllable, 2);
		}
		else TranslateUnicodeTibetanSyllableToAWandWriteBack(TibetanSyllable, nTibetanSyllable);
	}

	//Setup formating at the end side....
	if(TargetFileFormat == WebPage && SourceFileFormat != WebPage)
	{
		fputs("</font>", targetFile);
	}

	nTibetan = 0;
	return TranslationDone;
}


int TranslateUniRtf2AWrtfAndWriteBack()
{
	return TranslateUnicode2AWAndWriteBack();
}


int TranslateUniRtf2AWWebpageAndWriteBack()
{
	return TranslateUnicode2AWAndWriteBack();
}


//From Unicode to BTN  //2007
int TranslateUnicode2BTNAndWriteBack()
{
	int i, l, previousUnicode;
	int j=0;

	if(nTibetan<=0) return TextBufferEmpty;

	if(TargetFileFormat == RTF)  //20090427
	{
			fputs("\\f", targetFile);
			if(SourceFileFormat != RTF) fprintf(targetFile, "%d", BaseNumber);
			else fprintf(targetFile, "%d", TargetFileBaseFontNumberRTF2RTF);//SourceFileFormat == RTF
			fputc(0x20, targetFile);

			fputs("\\fs28 ", targetFile);
	}
	if(TargetFileFormat == WebPage)  //20090427
	{
			fputs("<font face=\"", targetFile);
			fputs(TibetanFontNames[BaseIndex[TargetEncoding]], targetFile);
			fputs("\">", targetFile);
			fputs("<font size=4>", targetFile);
	}

	currentNode = TableHead; //Initialize currentNode
	strcpy(currentNode->data.FzHg, " ");

	for(i=0; i<nTibetan; i++)
	{
		if(SourceFileFormat == WebPage)
			l = TibetanBufferForWebPage[i].ch; //Work on this buffer
		else //SourceFileFormat == RTF,   TXT,  UnicodeTXT
			l = TibetanBuffer[i].ch; //Work on this buffer

		if(l==0xf0c) l = 0xf0b;//20080229

		if(l<0xf00 || l>=0xfd0) continue; //Take off non-Tibetan characters!!!

		if(l>=0xf00 && l<0xfd0) //Tibetan
		{
			if(l>=0xf40 && l<=0xfbc)	//Central characters
			{
				if(currentNode->NextNode[l-0xf40] == NULL)
				{	//Illegal Tibetan Unicode sequence appears
					//Ignore the last character of the sequence
					//which is current character, leave it as the beginer
					//of the next sequence. Output the previous sequence if any
					if(j>0)
					{
						if(j==1 && TargetEncoding == NS) WriteBackUnicode(previousUnicode);
						else WriteBackWideCharacter(currentNode->data.FzHg);

						j = 0;
						currentNode = TableHead; //reset currentNode
						i--; //2008
						continue;
					}
					else //j==0; The non-combining letter
					{
						if(TargetEncoding == NS) WriteBackUnicode(l);
					}
				}
				else
				{
					currentNode = currentNode->NextNode[l-0xf40];
					j++;//Current string is Tibetan string
				}
				previousUnicode = l;
			}
			else //Other Tibetan characters
			{
				if(j==1 && TargetEncoding == NS) WriteBackUnicode(previousUnicode);
				else if(j>0) WriteBackWideCharacter(currentNode->data.FzHg);
				else;

				if(TargetEncoding == NS) WriteBackUnicode(l);
				else WriteBackWideCharacter(otherCells[l-0xf00].FzHg);
				j = 0;
				currentNode = TableHead; //reset currentNode

				previousUnicode = l;
			}
		}
	}

	if(j==1 && TargetEncoding == NS) WriteBackUnicode(previousUnicode);
	else if(j>0) WriteBackWideCharacter(currentNode->data.FzHg);
	else;

	if(TargetFileFormat == WebPage)
	{
		fputs("</font>", targetFile); //this is for <font size=>
		fputs("</font>", targetFile);
	}

	nTibetan = 0;
	return TranslationDone;
}


//From Unicode RTF to TSL RTF
int TranslateUnicode2TSLAndWriteBack()
{
	int i, l, baseLetter = -1, currentLetter = -1, lastLetter = -1, preFileNumber = -1, currentFileNumber = -1;
	int j=0, k, nStack=0, nLastStack=0;  //20090302
	int forceToTranslate = No;
	unsigned char preChar = 0, currentChar = 0;

	int Level[SZBuffer + 1] = {0};
	int level = 0;
	int RUN[10]; int nRun = 0;

	if(nTibetan<=0) return TextBufferEmpty;

	if(TargetFileFormat == WebPage)
		fputs("<font size=4>", targetFile);

	if(SourceFileFormat != RTF && TargetFileFormat == RTF) fputs("\\fs28 ", targetFile);

	currentNode = TableHead; //Initialize currentNode
	currentNode->data.FileNumber = 0;
	currentNode->data.theChar = 0x20;

	if(TargetEncoding==LTibetan) //Get the level of letters in a RUN
	{
		for(i=0; i<nTibetan; i++)
		{
			if(SourceFileFormat == WebPage)
				l = TibetanBufferForWebPage[i].ch; //Work on this buffer
			else //SourceFileFormat == RTF,   TXT,  UnicodeTXT
				l = TibetanBuffer[i].ch; //Work on this buffer

			if(l==0xf0c) l = 0xf0b;//20080229

			if((l>=0xf40 && l<=0xf6a) || l==0xf85 || (l>=0xf88 && l<=0xf8a)) //Header of a RUN
			{
				if(nRun!=0)//There is a RUN
				{
					currentNode = TableHead;
					for(k=0; k<nRun; k++)
					{
						if(currentNode->NextNode[RUN[k]-0xf40] == NULL)
						{
							if(k==0) break;//A strange RUN with unmapable header
							level ++;	//Found one character
							currentNode = TableHead;
							if(currentNode->NextNode[RUN[k]-0xf40] == NULL) break;
							else k --;//Do it again on this same letter
						}
						else
						{
							currentNode = currentNode->NextNode[RUN[k]-0xf40];
						}
					}
					if(k==nRun) level++; //Clean up the last stack

					for(k=0; k<nRun; k++)
					{
						Level[i-1-k] = level;
					}
				}
				if(nRun==3 && RUN[0]==0xf62 && (RUN[1]==0xf90 || RUN[1]==0xf92 || RUN[1]==0xfa1 || RUN[1]==0xf9e) && RUN[2]==0xfb1)
				{
					Level[i-3] = 1;
					Level[i-2] = 1;
					Level[i-1] = 3;
				}
				if(level>=3)
				{
					Level[i-nRun] = 2;
					Level[i-nRun+1] = 2;
				}
				nRun = 0;
				level = 0;
				RUN[nRun++] = l;
			}
			else if((l>=0xf90 && l<=0xfbc) || l==0xf71)//combining letters
			{
				RUN[nRun++] = l;
			}
			else //terminater of a RUN  e.g. vowels
			{
				if(nRun!=0)//There is a RUN
				{
					currentNode = TableHead;
					for(k=0; k<nRun; k++)
					{
						if(currentNode->NextNode[RUN[k]-0xf40] == NULL)
						{
							if(k==0) break;//A strange RUN with unmapable header
							level ++;
							currentNode = TableHead;
							if(currentNode->NextNode[RUN[k]-0xf40] == NULL) break;
							else k --;//Do it again on this same letter
						}
						else
						{
							currentNode = currentNode->NextNode[RUN[k]-0xf40];
						}
					}
					if(k==nRun) level++; //Clean up the last stack

					for(k=0; k<nRun; k++)
					{
						Level[i-1-k] = level;
					}

				}
				if(l==0xf74)//This is for Shabju
				{
					if(nRun == 1 && //Those long leg letter with vowel Shabju
						(RUN[0]==0xf40 || RUN[0]==0xf42 || RUN[0]==0xf49 || RUN[0]==0xf4f || RUN[0]==0xf50 ||
						 RUN[0]==0xf51 || RUN[0]==0xf53 || RUN[0]==0xf5d || RUN[0]==0xf5e || RUN[0]==0xf64 ||
						 RUN[0]==0xf67 )) Level[i] = level + 1;
					else if(nRun == 2 && level == 1) Level[i] = level + 1;//Those long leg letter with super script and vowel Shabju
					else Level[i] = level;
				}
				else if((l==0xf72||l==0xf7a||l==0xf7c) && (RUN[0]==0xf59||RUN[0]==0xf5a||RUN[0]==0xf5b)) Level[i] = 1;
				else ;

				if(level>=3)
				{
					Level[i-nRun] = 2;
					Level[i-nRun+1] = 2;
				}

				nRun = 0;
				level = 0;
			}
		}
	}

	currentNode = TableHead; //Initialize currentNode
	for(i=0; i<nTibetan; i++)
	{
		if(SourceFileFormat == WebPage)
			l = TibetanBufferForWebPage[i].ch; //Work on this buffer
		else //SourceFileFormat == RTF,   TXT,  UnicodeTXT
			l = TibetanBuffer[i].ch; //Work on this buffer

		if(l<0xf00 || l>=0xfd0) continue; //Take off non-Tibetan characters!!!

LOOP3:
		if(l>=0xf40 && l<=0xfbc)	//Central characters
		{
			if(currentNode->NextNode[l-0xf40] == NULL || forceToTranslate == Yes)
			{	//New run arrives. Translate the previous run by
				//Ignoring the last character of the sequence
				//which is current character, leave it as the beginner
				//of the next run. Output the previous run if any
				if(forceToTranslate == Yes) forceToTranslate = No;

				if(j==1) //Previous run is needed to be translated
				{
					currentFileNumber = currentNode->data.FileNumber;//Actually it's of lastLetter
					currentChar = currentNode->data.theChar;

					if(TargetEncoding==LTibetan) //Special for LTibetan
					{
						currentFileNumber = currentNode->data.level[Level[i-1]].FileNumber;
						currentChar = currentNode->data.level[Level[i-1]].theChar;
						if(currentChar==0x20)
						{
							if(Level[i-1]>0)
							{
								currentFileNumber = currentNode->data.level[Level[i-1]-1].FileNumber;
								currentChar = currentNode->data.level[Level[i-1]-1].theChar;
							}
							else if(Level[i-1]<5)
							{
								currentFileNumber = currentNode->data.level[Level[i-1]+1].FileNumber;
								currentChar = currentNode->data.level[Level[i-1]+1].theChar;
							}
							else ;//Keep that original one
						}
					}

					PickUpRightVowelForTSL(currentLetter, lastLetter, baseLetter, preFileNumber, preChar, &currentFileNumber, &currentChar, nLastStack);	//20090302

					if(currentFileNumber < 0 || currentChar < 1)//If something goes wrong, just correct these as before!
					{
						currentFileNumber = currentNode->data.FileNumber;//Actually it's of lastLetter
						currentChar = currentNode->data.theChar;
					}

					if(currentChar==0x20 && nStack==2 && TargetEncoding==TCRCBodYig)
					{	//Special for TCRC Bod-Yig.
						WriteBackFileNumberToTSLFile(preFileNumber, currentFileNumber);
						if(preChar != 0) WriteBackCharacterToTSLFile(preChar);
					}

					if(TargetEncoding==TCRCBodYig)
						CombinationForTCRC (lastLetter, currentLetter, &preChar, &currentChar);

					if(preChar != 0) WriteBackCharacterToTSLFile(preChar);
					preChar = currentChar;

					WriteBackFileNumberToTSLFile(preFileNumber, currentFileNumber);
					preFileNumber = currentFileNumber;

					j = 0; //Currentlly no run needed to be translated
					nLastStack = nStack;//Remember the # of letters on last stack  //20090302
					nStack = 0;//Recount the # of letters on each stack
					currentNode = TableHead; //reset currentNode

					goto LOOP3;
				}
				else continue;//A strange letter, ignore it.
			}
			else
			{
				currentNode = currentNode->NextNode[l-0xf40];
				j=1;//Current run is needed to be translated
				nStack ++;//Count the # of letters on each stack!
			}
		}
		else //Other Tibetan characters
		{
			if(j==1)//There is a run needed to be translated
			{
				currentFileNumber = currentNode->data.FileNumber;//Actually it's of lastLetter
				currentChar = currentNode->data.theChar;

				if(TargetEncoding==LTibetan) //Special for LTibetan
				{
					currentFileNumber = currentNode->data.level[Level[i-1]].FileNumber;
					currentChar = currentNode->data.level[Level[i-1]].theChar;

					if(currentChar==0x20)
					{
						if(Level[i-1]>0)
						{
							currentFileNumber = currentNode->data.level[Level[i-1]-1].FileNumber;
							currentChar = currentNode->data.level[Level[i-1]-1].theChar;
						}
						else if(Level[i-1]<5)
						{
							currentFileNumber = currentNode->data.level[Level[i-1]+1].FileNumber;
							currentChar = currentNode->data.level[Level[i-1]+1].theChar;
						}
						else ;//Keep that original one
					}
				}

				PickUpRightVowelForTSL(currentLetter, lastLetter, baseLetter, preFileNumber, preChar, &currentFileNumber, &currentChar, nLastStack);	//20090302

				if(currentFileNumber < 0 || currentChar < 1)//If something goes wrong, just correct these as before!
				{
					currentFileNumber = currentNode->data.FileNumber;//Actually it's of lastLetter
					currentChar = currentNode->data.theChar;
				}

				if(currentChar==0x20 && nStack==2 && TargetEncoding==TCRCBodYig)
				{	//Special for TCRC Bod-Yig.
					WriteBackFileNumberToTSLFile(preFileNumber, currentFileNumber);
					if(preChar != 0) WriteBackCharacterToTSLFile(preChar);
				}

				if(TargetEncoding==TCRCBodYig)
					CombinationForTCRC (lastLetter, currentLetter, &preChar, &currentChar);

				if(preChar != 0) WriteBackCharacterToTSLFile(preChar);

				WriteBackFileNumberToTSLFile(preFileNumber, currentFileNumber);
				preFileNumber = currentFileNumber;

				WriteBackCharacterToTSLFile(currentChar);
			}

			currentFileNumber = (int)otherCells[l-0xf00].FileNumber;
			currentChar = otherCells[l-0xf00].theChar;

			WriteBackFileNumberToTSLFile(preFileNumber, currentFileNumber);
			preFileNumber = currentFileNumber;

			WriteBackCharacterToTSLFile(currentChar);
			preChar = 0;//No pre char here, it's already output.

			j = 0;
			nLastStack = 1;//Remember the # of letters on last stack  //20090302
			nStack = 0;//Recount the # of letters on each stack
			currentNode = TableHead; //reset currentNode
		}

		//Remeber them
		lastLetter = currentLetter;
		currentLetter = l;
		if(l >= 0xf40 && l <= 0xf6a) baseLetter = l;

	}//End of for()

	//There is still something to be cleaned
	if(j==1)//Central character left
	{
		currentFileNumber = currentNode->data.FileNumber;//Actually it's of lastLetter
		currentChar = currentNode->data.theChar;

		if(TargetEncoding==LTibetan) //Special for LTibetan
		{
			currentFileNumber = currentNode->data.level[Level[i-1]].FileNumber;
			currentChar = currentNode->data.level[Level[i-1]].theChar;
			if(currentChar==0x20)
			{
				if(Level[i-1]>0)
				{
					currentFileNumber = currentNode->data.level[Level[i-1]-1].FileNumber;
					currentChar = currentNode->data.level[Level[i-1]-1].theChar;
				}
				else if(Level[i-1]<5)
				{
					currentFileNumber = currentNode->data.level[Level[i-1]+1].FileNumber;
					currentChar = currentNode->data.level[Level[i-1]+1].theChar;
				}
				else ;//Keep that original one
			}
		}

		PickUpRightVowelForTSL(currentLetter, lastLetter, baseLetter, preFileNumber, preChar, &currentFileNumber, &currentChar, nLastStack);	//20090302

		if(currentFileNumber < 0 || currentChar < 1)//If something goes wrong, just correct these as before!
		{
			currentFileNumber = currentNode->data.FileNumber;//Actually it's of lastLetter
			currentChar = currentNode->data.theChar;
		}

		if(currentChar==0x20 && nStack==2 && TargetEncoding==TCRCBodYig)
		{	//Special for TCRC Bod-Yig.
			WriteBackFileNumberToTSLFile(preFileNumber, currentFileNumber);
			if(preChar != 0) WriteBackCharacterToTSLFile(preChar);
		}

		if(TargetEncoding==TCRCBodYig)
			CombinationForTCRC (lastLetter, currentLetter, &preChar, &currentChar);

		if(preChar != 0) WriteBackCharacterToTSLFile(preChar);

		WriteBackFileNumberToTSLFile(preFileNumber, currentFileNumber);
		preFileNumber = currentFileNumber;

		WriteBackCharacterToTSLFile(currentChar);
	}

	if(TargetFileFormat == WebPage)
	{
		fputs("</font>", targetFile); //this is for <font size=>
	}

	nTibetan = 0;
	return TranslationDone;
}


//From Unicode RTF to TSL WebPage
int TranslateUniRtf2TSLWebpageAndWriteBack()
{
	return TranslateUnicode2TSLAndWriteBack();
}


//From Unicode RTF to TSL RTF
int TranslateUniRtf2TSLrtfAndWriteBack()
{
	return TranslateUnicode2TSLAndWriteBack();
}


int TranslateUniRtf2BTNrtfAndWriteBack() //Bzd, Ty, NS //2007
{
	return TranslateUnicode2BTNAndWriteBack();
}


int TranslateUniRtf2BTNtxtAndWriteBack() //Bzd, Ty, NS //2007
{
	return TranslateUnicode2BTNAndWriteBack();
}


int TranslateUniRtf2BTNUniTxtAndWriteBack() // NS //2007
{
	return TranslateUnicode2BTNAndWriteBack();
}


int TranslateUniRtf2BTNWebpageAndWriteBack() //Bzd, Ty, NS //2007
{
	return TranslateUnicode2BTNAndWriteBack();
}


int TranslateUniRtf2AWtxtAndWriteBack()
{
	return TranslateUnicode2AWAndWriteBack();
}


//From Unicode RTF to FZ or Hg
int TranslateUniRtf2FzHgAndWriteBack()
{
	int i, l;

	int j=0;
	if(nTibetan<=0) return TextBufferEmpty;

	currentNode = TableHead; //Initialize currentNode
	strcpy(currentNode->data.FzHg, " ");


	for(i=0; i<nTibetan; i++)
	{
		l = TibetanBuffer[i].ch;

		if(l==0xf0c) l = 0xf0b;//20080229

		if(l>=0xf00 && l<0xfd0) //Tibetan
		{
LOOP4:
			if(l>=0xf40 && l<=0xfbc)	//Central characters
			{
				if(currentNode->NextNode[l-0xf40] == NULL)
				{	//Illegal Tibetan Unicode sequence appears
					//Ignore the last character of the sequence
					//which is current character, leave it as the beginer
					//of the next sequence. Output the previous sequence if any
					if(j==1)
					{
						fputs(currentNode->data.FzHg, targetFile);
						j = 0;
						currentNode = TableHead; //reset currentNode
						goto LOOP4;
					}
					else //j==0; The non-combining letter
					{
						continue;
					}
				}
				else
				{
					currentNode = currentNode->NextNode[l-0xf40];
					j=1;//Current string is Tibetan string
				}
			}
			else //Other Tibetan characters
			{
				if(j==1) fputs(currentNode->data.FzHg, targetFile);
				fputs(otherCells[l-0xf00].FzHg, targetFile);
				j = 0;
				currentNode = TableHead; //reset currentNode
			}
		}
	}

	if(j==1) fputs(currentNode->data.FzHg, targetFile);

	nTibetan = 0;
	return TranslationDone;
}


int TranslateUniRtf2AWUniTxtAndWriteBack()
{
	return TranslateUnicode2AWAndWriteBack();
}


//---------------------------------Unicode TXT to others--------------


int TranslateUniTxt2AWrtfAndWriteBack()
{
	return TranslateUnicode2AWAndWriteBack();
}


int TranslateUniTxt2AWWebpageAndWriteBack()
{
	return TranslateUnicode2AWAndWriteBack();
}


//Translate Unicode TXT to AW
int TranslateUniTxt2AWtxtAndWriteBack()
{
	return TranslateUnicode2AWAndWriteBack();
}


int TranslateUniTxt2AWUniTxtAndWriteBack()
{
	return TranslateUnicode2AWAndWriteBack();
}


//Unicode TXT to TSL RTF
int TranslateUniTxt2TSLrtfAndWriteBack()
{
	return TranslateUnicode2TSLAndWriteBack();
}


int TranslateUniTxt2TSLWebpageAndWriteBack()
{
	return TranslateUnicode2TSLAndWriteBack();
}


int TranslateUniTxt2BTNrtfAndWriteBack() //Bzd, Ty, NS //2007
{
	return TranslateUnicode2BTNAndWriteBack();
}


int TranslateUniTxt2BTNWebpageAndWriteBack() //Bzd, Ty, NS //2007
{
	return TranslateUnicode2BTNAndWriteBack();
}


//From Unicode TXT to Fz, Hg, Bzd, Ty, NS  //2007
int TranslateUniTxt2FzHgAndWriteBack()
{
	int j=0, l;
	int ch1, ch2;

	//Start to work on text of the file
	j=0;
	currentNode = TableHead; //Initialize currentNode
	strcpy(currentNode->data.FzHg, " ");

#ifdef Unix		//20090427
	while( ((ch2 = getc(sourceFile)) != EOF)
		&& ((ch1 = getc(sourceFile)) != EOF))
	{
		l = (ch1<<8) + ch2;
#endif
#ifdef Windows
	while( (l = getwc(sourceFile)) != WEOF) //20081124
	{
#endif			//20090427
#ifdef Mac		//20180304
	while( ((ch2 = getc(sourceFile)) != EOF)
		&& ((ch1 = getc(sourceFile)) != EOF))
	{
		l = (ch1<<8) + ch2;
#endif

		if(l==0xf0c) l = 0xf0b;//20080229

		if(l>=0xf00 && l<0xfd0) //Tibetan
		{
LOOP2:
			if(l>=0xf40 && l<=0xfbc)	//Central characters
			{
				if(currentNode->NextNode[l-0xf40] == NULL)
				{	//Illegal Tibetan Unicode sequence appears
					//Ignore the last character of the sequence
					//which is current character, leave it as the beginer
					//of the next sequence. Output the previous sequence if any
					if(j==1)
					{
						fputs(currentNode->data.FzHg, targetFile);
						j = 0;
						currentNode = TableHead; //reset currentNode
						goto LOOP2;
					}
					else continue;//j==0--non-combining letter
				}
				else
				{
					currentNode = currentNode->NextNode[l-0xf40];
					j=1;//Current string is Tibetan string
				}
			}
			else //Other Tibetan characters
			{
				if(j==1) fputs(currentNode->data.FzHg, targetFile);
				fputs(otherCells[l-0xf00].FzHg, targetFile);
				j = 0;
				currentNode = TableHead; //reset currentNode
			}
		}

		else //Chinese, ASCII or other languages
		{
			if(j==1) //Prevoius string is Tibetan string, translate it.
			{
				fputs(currentNode->data.FzHg, targetFile);
				j = 0;
				currentNode = TableHead; //reset currentNode
			}

			if(l>=0 && l<=0xff)//ASCII character
			{
				if(TargetFileFormat == UnicodeTXT) fputc(0x00,targetFile);
//				fputc(ch2,targetFile);   //20081124
				fputc(l,targetFile);   //20081124
			}
			else //Chinese or other languages  //GB2312
			{
//				l = Unicode2GB[l];    //20080926
				if(Unicode2GB[l] != 0) l = Unicode2GB[l];//20080926
				fputc((l>>8), targetFile);
				fputc((l & 0xff), targetFile);
			}
		}
	}//end of while()

	if(j==1) fputs(currentNode->data.FzHg, targetFile);

	nTibetan = 0;
	return EndOfFile;
}


int TranslateUniTxt2BTNtxtAndWriteBack() //Bzd, Ty, NS //2007
{
	return TranslateUnicode2BTNAndWriteBack();
}


int TranslateUniTxt2BTNUniTxtAndWriteBack() //only for NS //2007
{
	return TranslateUnicode2BTNAndWriteBack();
}


//------------------------------------------Unicode Webpage to others--------------

int TranslateUniWebpage2AWrtfAndWriteBack()
{
	return TranslateUnicode2AWAndWriteBack();
}


int TranslateUniWebpage2AWWebpageAndWriteBack()
{
	return TranslateUnicode2AWAndWriteBack();
}


int TranslateUniWebpage2TSLWebpageAndWriteBack()
{
	return TranslateUnicode2TSLAndWriteBack();
}


int TranslateUniWebpage2TSLrtfAndWriteBack()
{
	return TranslateUnicode2TSLAndWriteBack();
}


int TranslateUniWebpage2AWtxtAndWriteBack()
{
	return TranslateUnicode2AWAndWriteBack();
}


int TranslateUniWebpage2FzHgAndWriteBack()
{
	int i, l;

	int j=0;

	if(nTibetan<=0) return TextBufferEmpty;

	currentNode = TableHead; //Initialize currentNode
	strcpy(currentNode->data.FzHg, " ");

	for(i=0; i<nTibetan; i++)
	{
		l = TibetanBufferForWebPage[i].ch;

		if(l==0xf0c) l = 0xf0b;//20080229

		if(l>=0xf00 && l<0xfd0) //Tibetan
		{
LOOP4:
			if(l>=0xf40 && l<=0xfbc)	//Central characters
			{
				if(currentNode->NextNode[l-0xf40] == NULL)
				{	//Illegal Tibetan Unicode sequence appears
					//Ignore the last character of the sequence
					//which is current character, leave it as the beginer
					//of the next sequence. Output the previous sequence if any
					if(j==1)
					{
						fputs(currentNode->data.FzHg, targetFile);
						j = 0;
						currentNode = TableHead; //reset currentNode
						goto LOOP4;
					}
					else //j==0; The non-combining letter
					{
						continue;
					}
				}
				else
				{
					currentNode = currentNode->NextNode[l-0xf40];
					j=1;//Current string is Tibetan string
				}
			}
			else //Other Tibetan characters
			{
				if(j==1) fputs(currentNode->data.FzHg, targetFile);
				fputs(otherCells[l-0xf00].FzHg, targetFile);
				j = 0;
				currentNode = TableHead; //reset currentNode
			}
		}
	}

	if(j==1) fputs(currentNode->data.FzHg, targetFile);

	nTibetan = 0;
	return TranslationDone;
}



int TranslateUniWebpage2AWUniTxtAndWriteBack()
{
	return TranslateUnicode2AWAndWriteBack();
}


int TranslateUniWebpage2BTNrtfAndWriteBack() //Bzd, Ty, NS //2007
{
	return TranslateUnicode2BTNAndWriteBack();
}


int TranslateUniWebpage2BTNtxtAndWriteBack() //Bzd, Ty, NS //2007
{
	return TranslateUnicode2BTNAndWriteBack();
}


int TranslateUniWebpage2BTNUniTxtAndWriteBack() //NS //2007
{
	return TranslateUnicode2BTNAndWriteBack();
}


int TranslateUniWebpage2BTNWebpageAndWriteBack() //Bzd, Ty, NS //2007
{
	return TranslateUnicode2BTNAndWriteBack();
}

//------------------------------------AW to Unicode---------------------------------
int IsThisTibetanSyllable(char *thisString)
{
	int i;
	char * pTheSyllableList;

	i = strlen(thisString); //Number of characters in the syllable.

	if(i > 3 && thisString[i-2] == '\'')
	{
		if( (SourceEncoding != ACIP && thisString[i-1] == 'i') ||
			(SourceEncoding == ACIP && thisString[i-1] == 'I') )
		{
			i = i - 2; //Ignore 'i  or  'I at the end of the syllable.
		}
	}

	if(i == 1)//One character syllable
	{
		if(*(pTSLookupTable + MyHashFunction(thisString[0])*130*130) != NULL) return Yes; //It's a Tibetan syllable
		else return No;
	}
	else if(i==2)//Two character syllable
	{
		if(*(pTSLookupTable + MyHashFunction(thisString[0])*130*130 + MyHashFunction(thisString[1])*130) != NULL) return Yes;
		else return No;
	}
	else //i>=3. It has three or more characters. Check the syllable in the range in the table.
	{
		pTheSyllableList = *(pTSLookupTable + MyHashFunction(thisString[0])*130*130 + MyHashFunction(thisString[1])*130 + MyHashFunction(thisString[2]));

		if(pTheSyllableList == NULL) return No;
		else
		{
			while(*(pTheSyllableList) == thisString[0] &&
				  *(pTheSyllableList+1) == thisString[1] &&
				  *(pTheSyllableList+2) == thisString[2])//In the same type of syllables list that have same first three characters
			{
				if(strcmp(thisString, pTheSyllableList)==0) return Yes;
				else pTheSyllableList = pTheSyllableList + 20;//Next syllable in the table
			}
			return No; //Out of the same type of syllables list. It's not a Tibetan syllable.
		}
	}
}


//For AW to Unicode
int IsSingleCharacterSyllable(char theCharacter)
{
	if(theCharacter == 'a' ||
	   theCharacter == 'A' ||
	   theCharacter == 'e' ||
	   theCharacter == 'E' ||
	   theCharacter == 'i' ||
	   theCharacter == 'I' ||
	   theCharacter == 'u' ||
	   theCharacter == 'U' ||
	   theCharacter == 'o' ||
	   theCharacter == 'O' )
	   return Yes;
	else return No;
}



//Latin translateration (Latin character sequence) to Unicode code point
//Separate Tibetan syllables from English words. Do not translate English words into
//Tibetan Unicode sequences.
int TranslateAW2UnicodeAndWriteBack()
{
	int i, j, ch, lastCh = 0, nextCh = 0;
	char A7 = 0xa7;

	//-----------------------
	char Syllables[500][50];
	int nSyllableInBuffer = 0;
	int nCharcaterPerSyllable = 0;
	float nTibetanSyllables = 0;
	float nEnglishWords = 0;
	float rate; //rate = nTibetanSyllables/(nTibetanSyllables + nEnglishWords);
	//-----------------------

	int previousCharacterIsTibetanLetter = No;

	if(nTibetan<=0) return TextBufferEmpty;

	nLatin = 0;

//---------------------------------------------------------------
	//Generating syllables from the characters in the buffer
	for(i=0; i<nTibetan; i++) //All are Latin characters
	{
		if(SourceFileFormat == WebPage)
			ch = TibetanBufferForWebPage[i].ch; //Work on this buffer
		else
			ch = TibetanBuffer[i].ch; //Otherwise work on this buffer

		if(((SourceEncoding == THDLWylie && ch == '/') || ch == ',') && i>=2) // Insert tseg between nga and shad    //20080620
		{
			if(SourceFileFormat == WebPage)
			{
				if((TibetanBufferForWebPage[i-1].ch == 'g' && TibetanBufferForWebPage[i-2].ch == 'n') || //"ng"
					(TibetanBufferForWebPage[i-1].ch == 'G' && TibetanBufferForWebPage[i-2].ch == 'N'))
				{
					for(j=nTibetan;j>=i;j--) TibetanBufferForWebPage[j+1].ch = TibetanBufferForWebPage[j].ch;
					nTibetan++;//increase the number of characters
					TibetanBufferForWebPage[i].ch = 0x20; //Insert tseg
					ch = 0x20;
				}
			}
			else
			{
				if((TibetanBuffer[i-1].ch == 'g' && TibetanBuffer[i-2].ch == 'n') ||
					(TibetanBuffer[i-1].ch == 'G' && TibetanBuffer[i-2].ch == 'N'))
				{
					for(j=nTibetan;j>=i;j--) TibetanBuffer[j+1].ch = TibetanBuffer[j].ch;
					nTibetan++;//increase the number of characters
					TibetanBuffer[i].ch = 0x20; //Insert tseg
					ch = 0x20;
				}
			}
		}

		if( nCharcaterPerSyllable < 48 && ( isalpha(ch) || ch=='\'' || ch=='+'
			|| ch==0xa7 || ch==0xe2 || ch==0xe4 || ch==0xe5 || ch==0xe7 || ch==0xef
			|| ch==0xf2 || ch==0xf4 || ch==0xf6 ) )
		{
			Syllables[nSyllableInBuffer][nCharcaterPerSyllable++] = ch;
		}
		else if((ch=='.' || ch==A7 || ch=='-') && nCharcaterPerSyllable < 48)
		{
			if(SourceFileFormat == WebPage)
				nextCh = TibetanBufferForWebPage[i+1].ch;
			else
				nextCh = TibetanBuffer[i+1].ch;

			if((lastCh=='g' || lastCh=='G') && (nextCh=='y' || nextCh=='Y'))//special character for g.yang
			{
				Syllables[nSyllableInBuffer][nCharcaterPerSyllable++] = ch;
			}
			else //It's English punctuation.
			{
				if(nCharcaterPerSyllable > 0) //It's a syllable.
				{
					//Remeber the number of characters in previous syllable.
					//Keep this number in the last slot of the array, which is [49]
					Syllables[nSyllableInBuffer][49] = nCharcaterPerSyllable;

					Syllables[nSyllableInBuffer][nCharcaterPerSyllable] = '\0'; //End the string of the syllable

					nSyllableInBuffer++;//Point to this character
					Syllables[nSyllableInBuffer][0] = ch;
					Syllables[nSyllableInBuffer][49] = 1;

					Syllables[nSyllableInBuffer][1] = '\0'; //End the string of the syllable

					nSyllableInBuffer++;//Point to next syllable
					nCharcaterPerSyllable = 0;//Reset the counter
				}
				else //Current character is a single character
				{
					Syllables[nSyllableInBuffer][0] = ch;
					Syllables[nSyllableInBuffer][49] = 1;

					Syllables[nSyllableInBuffer][1] = '\0'; //End the string of the syllable

					nSyllableInBuffer++;//Point to next syllable
					nCharcaterPerSyllable = 0;//Reset the counter
				}
			}
		}
		else //Space and other non-alpha characters, as a delimiter
		{
			if(nCharcaterPerSyllable > 0) //It's a syllable.
			{
				//Remeber the number of characters in previous syllable.
				Syllables[nSyllableInBuffer][49] = nCharcaterPerSyllable;

				Syllables[nSyllableInBuffer][nCharcaterPerSyllable] = '\0'; //End the string of the syllable

				nSyllableInBuffer++;//Point to this character
				Syllables[nSyllableInBuffer][0] = ch;
				Syllables[nSyllableInBuffer][49] = 1;

				Syllables[nSyllableInBuffer][1] = '\0'; //End the string of the syllable

				nSyllableInBuffer++;//Point to next syllable
				nCharcaterPerSyllable = 0;//Reset the counter
			}
			else //Current character is a single character
			{
				Syllables[nSyllableInBuffer][0] = ch;
				Syllables[nSyllableInBuffer][49] = 1;

				Syllables[nSyllableInBuffer][1] = '\0'; //End the string of the syllable

				nSyllableInBuffer++;//Point to next syllable
				nCharcaterPerSyllable = 0;//Reset the counter
			}
		}
		lastCh = ch;
	}
	//Clean up -- Take care the last syllable
	if(nCharcaterPerSyllable > 0) //It's a syllable.
	{	//Remeber the number of characters in previous syllable.
		Syllables[nSyllableInBuffer][49] = nCharcaterPerSyllable;
		Syllables[nSyllableInBuffer][nCharcaterPerSyllable] = '\0'; //End the string of the syllable
		nSyllableInBuffer ++;//Increase this number!!!!
	}


	//Separate Tibetan syllables from English text
	//by marking a flag in slot of Syllable[][48]. Tibetan---Tibetan syllable; Latin---English text
	for(i=0; i<nSyllableInBuffer; i++)
	{
		if(Syllables[i][49]==1)
		{	//There is no single letter Tibetan syllable, except 'a' or 'A'
			Syllables[i][48] = Latin;
		}
		else
		{
			if(IsThisTibetanSyllable(Syllables[i])==Yes) { Syllables[i][48] = Tibetan; nTibetanSyllables ++;}
			else { Syllables[i][48] = Latin; nEnglishWords ++;}
		}
	}

	if((nTibetanSyllables + nEnglishWords)<=0) rate = 0;//For safety!!
	else rate = nTibetanSyllables/(nTibetanSyllables + nEnglishWords);

	if(rate > 0.60)//More than 60% number of strings in the buffer are Tibetan syllables. //20080620
	{	//Assume all strings in the buffer are Tibetan syllables.
		//Reconsider single character items, especially spaces.
		for(i=0; i<nSyllableInBuffer; i++)
		{
			if(Syllables[i][49]!=1)
			{
				if(Syllables[i][48] == Latin) Syllables[i][48] = Tibetan;//Assume all are Tibetan syllables.
			}
		}
	}
	else if(rate < 0.10)//Whole sentence is more likely a English sentence.
	{
		for(i=0; i<nSyllableInBuffer; i++)
		{
			if(Syllables[i][49]!=1)
			{
				if(Syllables[i][48] == Tibetan) Syllables[i][48] = Latin;//Assume all are English words.
			}
		}
	}
	else//It's mixing sentence.
	{	//Change English word that is between two Tibetan syllables into Tibetan syllable.
		for(i=0; i<nSyllableInBuffer; i++)
		{
			if(i==0 && nSyllableInBuffer>=3)//The first string
			{
				if(Syllables[i][49]!=1 && Syllables[i][48] == Latin)//First string is not Tibetan
				{
					if(Syllables[i+2][48] == Tibetan)//Next string is Tibetan (skip the space!)
						Syllables[i][48] = Tibetan;//Then, assume this string is Tibetan.
				}
			}
			else if(i==nSyllableInBuffer-1 && nSyllableInBuffer>=3)//The last string.
			{
				if(Syllables[i][49]!=1 && Syllables[i][48] == Latin)//First string is not Tibetan
				{
					if(Syllables[i-2][48] == Tibetan)//Last string is Tibetan
						Syllables[i][48] = Tibetan;//Then, assume this string is Tibetan.
				}
			}
			else//Other strings
			{
				if(Syllables[i][49]!=1 && Syllables[i][48] == Latin)//This string is not Tibetan.
				{
					if(Syllables[i-2][48] == Tibetan && Syllables[i+2][48] == Tibetan)//Strings at both sides are Tibetan syllables.
						Syllables[i][48] = Tibetan;//Then, assume this string is Tibetan.
				}
			}
		}
	}
	//Work on the single characters, especially spaces, digits and 'a', 'A', ...
	if(nSyllableInBuffer>=3 && Syllables[0][49]==1 &&
		IsSingleCharacterSyllable(Syllables[0][0]) &&
		(Syllables[1][0]==0x20 || Syllables[1][0]=='-') &&
		Syllables[2][48] == Tibetan)
		Syllables[0][48] = Tibetan;//A or a at the beginning of a sentence.

	if(Syllables[0][49]==1 && (Syllables[0][0]==',' || Syllables[0][0]=='/'))
		Syllables[0][48] = Tibetan;//"," or "/" at the beginning of a sentence.

	if((Syllables[1][49]==1 && (Syllables[1][0]==',' || Syllables[1][0]=='/'))
		&& (Syllables[0][49]==1 && Syllables[0][0]==0x20))
		Syllables[1][48] = Tibetan;//"," or "/" is next to the beginning of a sentence.


	for(i=1; i<nSyllableInBuffer; i++)
	{
		if(Syllables[i][49]==1 && Syllables[i-1][48]==Tibetan)//single characters
		{
			if(Syllables[i-1][49]!=1)//A Tibetan syllable followed by a single character
			{
				if(SourceEncoding==LCWylie)//Special cases for LCWylie
				{
					if(Syllables[i][0]=='-') Syllables[i][0] = 0x20; //Replace '-' with space;
				}

				if(isdigit(Syllables[i][0]) || Syllables[i][0]== 0x20)
				{
					Syllables[i][48] = Tibetan;
				}
			}
			else //Syllables[i-1][49]==1--Two single characters, one after another.
			{
				if(isdigit(Syllables[i][0])) Syllables[i][48] = Tibetan;
				else if(IsSingleCharacterSyllable(Syllables[i][0]) && Syllables[i+1][0] == 0x20)
				{
					Syllables[i][48] = Tibetan;
				}
				else if(IsSingleCharacterSyllable(Syllables[i-1][0]) && Syllables[i][0] == 0x20)
				{
					Syllables[i][48] = Tibetan;
				}
				else;
			}

			if(Syllables[i][0]==',' || Syllables[i][0]=='/')//Tibetan punctuations
				Syllables[i][48] = Tibetan;
		}

		if(Syllables[i][49]==1 && (Syllables[i][0]==',' || Syllables[i][0]=='/'))//Current character is "," or "/"
		{
			if(Syllables[i-1][49]==1 && (Syllables[i-1][0]==',' || Syllables[i-1][0]=='/'
				|| Syllables[i-1][0]==0x20))//"," or "/" following a space or "," or "/"

				Syllables[i][48] = Tibetan;//Tibetan shag "|"
		}
	}

	if(nSyllableInBuffer>=2 && Syllables[nSyllableInBuffer-1][49]==1
		&& (Syllables[nSyllableInBuffer-1][0]==',' || Syllables[nSyllableInBuffer-1][0]=='/')
		&& Syllables[nSyllableInBuffer-2][0] == 0x20)
		Syllables[nSyllableInBuffer-1][48] = Tibetan;//At the end of a sentence.

	//Special cases in LCWylie-----replace uppercase letters to lowercase letters
	if(SourceEncoding==LCWylie)
	{
		for(i=0; i<nSyllableInBuffer; i++)
		{
			if(Syllables[i][49]!=1)
			{
				for(j=0; j<Syllables[i][49]; j++)
				{
					if(Syllables[i][j]>=0x41 && Syllables[i][j]<=0x5a)//uppercase
					{
						Syllables[i][j] = Syllables[i][j] + 0x20;//change to lowercase
					}
				}
			}
		}
	}


	//Write back Latin strings; Translate and write back Tibetan syllables.
	for(i=0; i<nSyllableInBuffer; i++)
	{
		if(Syllables[i][48]==Latin)//It's Latin word.
		{
			if(previousCharacterIsTibetanLetter==Yes)
			{
				//Close the formating....
				if(TargetFileFormat == WebPage && SourceFileFormat != WebPage)
				{
					fputs("</font>", targetFile);
				}
			}

			for(j=0; j<Syllables[i][49]; j++) LatinBuffer[nLatin++] = Syllables[i][j];
			previousCharacterIsTibetanLetter = No;
		}
		else //It's Tibetan syllable.
		{
			if(nLatin>0) WriteBackLatin();

			if(previousCharacterIsTibetanLetter==No)
			{
				//Setup formating....
				if(TargetFileFormat == RTF && SourceFileFormat != RTF)
				{
					fputs("\\fs28", targetFile);
					fputs("\\f", targetFile);
					fputs("30", targetFile);
					fputc(0x20, targetFile);
				}
				else if(TargetFileFormat == RTF && SourceFileFormat == RTF)
				{
					fputs("\\f", targetFile);
					fprintf(targetFile, "%d", targetFontIDs[0]);
					fputc(0x20, targetFile);
				}
				else if(TargetFileFormat == WebPage && SourceFileFormat != WebPage)
				{
					fputs("<font face=\"", targetFile);
					fputs(TibetanFontNames[BaseIndex[TargetEncoding]], targetFile);
					fputs("\" size=5>", targetFile);
				}
				else if(TargetFileFormat == WebPage && SourceFileFormat == WebPage)
				{
					WriteBackTibetanFontTag();
				}
				else; //UnicodeTXT, do nothing
			}

			if(Syllables[i][49]>0) TranslateAWTibetanSyllableToUnicodeAndWriteBack(Syllables[i], Syllables[i][49]);
			previousCharacterIsTibetanLetter = Yes;
		}
	}
	if(nLatin>0) WriteBackLatin();//Clean up the last Latin string!!
	if(previousCharacterIsTibetanLetter==Yes)
	{
		//Close the formating....
		if(TargetFileFormat == WebPage && SourceFileFormat != WebPage)
		{
			fputs("</font>", targetFile);
		}
	}

	nTibetan = 0;
	return TranslationDone;
}



int TranslateAWrtf2UniRtfAndWriteBack()
{
	return TranslateAW2UnicodeAndWriteBack();
}


int TranslateAWrtf2UniTxtAndWriteBack()
{
	return TranslateAW2UnicodeAndWriteBack();
}



int TranslateAWrtf2UniWebpageAndWriteBack()
{
	return TranslateAW2UnicodeAndWriteBack();
}



int TranslateAWtxt2UniRtfAndWriteBack()
{
	return TranslateAW2UnicodeAndWriteBack();
}


int TranslateAWtxt2UniTxtAndWriteBack()
{
	return TranslateAW2UnicodeAndWriteBack();
}



int TranslateAWtxt2UniWebpageAndWriteBack()
{
	return TranslateAW2UnicodeAndWriteBack();
}


int TranslateAWUniTxt2UniRtfAndWriteBack()
{
	return TranslateAW2UnicodeAndWriteBack();
}



int TranslateAWUniTxt2UniTxtAndWriteBack()
{
	return TranslateAW2UnicodeAndWriteBack();
}


int TranslateAWUniTxt2UniWebpageAndWriteBack()
{
	return TranslateAW2UnicodeAndWriteBack();
}



int TranslateAWWebpage2UniRtfAndWriteBack()
{
	return TranslateAW2UnicodeAndWriteBack();
}



int TranslateAWWebpage2UniTxtAndWriteBack()
{
	return TranslateAW2UnicodeAndWriteBack();
}



int TranslateAWWebpage2UniWebpageAndWriteBack()
{
	return TranslateAW2UnicodeAndWriteBack();
}



//------------------------TSL to Unicode --------------------------
int TranslateTSLrtf2UniRtfAndWriteBack()
{
	int i, l, fontFileID, n, m;
	char  *thepoint;
	char temp[50];

	if(nTibetan<=0) return TextBufferEmpty;

//   	fputs("{", targetFile); // 20181119 adding for lower SambahotaUnicode
    	//printf("targetFontID=%d", targetFontIDs[0]); //20181114 added for debug
//   	fputs("\\f", targetFile);
//   	fprintf(targetFile, "%d", targetFontIDs[0]);
//        if(SourceEncoding == NewSambhota && TargetEncoding == Unicode) 
//        {
//    fprintf(targetFile,"\\sb100"); //20181118 added for adding space above Tibetan 
//    fprintf(targetFile,"\\dn4 "); // 20181119 adding for lower SambahotaUnicode
//        }

	for(i=0; i<nTibetan; i++)
	{
		m = TibetanBuffer[i].ch;

		if(m < 0x21 || m > 0xff) continue;

		fontFileID = SourceFontFileID(TibetanBuffer[i].fontID);

		//This is for Dedris vs Ededris, and Sam vs Esam
		if((SourceEncoding == NewSambhota) && fontFileID > 29)
			fontFileID = fontFileID - 30;
		if((SourceEncoding == OldSambhota) && fontFileID > 2)
			fontFileID = fontFileID - 3;
		//Font file size is different
		if(SourceEncoding == TMW || SourceEncoding == NewSambhota)
			l = fontFileID * 94 + (m - 0x21);
		else if(SourceEncoding == Jamyang) l = fontFileID * 223 + (m - 0x21);  //20090218
		else l = fontFileID * 222 + (m - 0x21);

		thepoint=pTable+l*30;

		n=0;
		while(*thepoint != '\0')
		{
			temp[n++] = *thepoint++;
			if(n==4)
			{
				temp[n] = '\0';
				fputs("\\u",targetFile);
				fputs(temp,targetFile);
				fputs(" ",targetFile); //20100307
				n=0;
			}
		}
	}
    fprintf(targetFile,"\n");
	nTibetan = 0;
	return TranslationDone;
}



///From TSL rtf to Unicode TXT
int TranslateTSLrtf2UniTxtAndWriteBack()
{
	int i, l, fontFileID, n, m;
	char  *thepoint;
	char temp[50];

	if(nTibetan<=0) return TextBufferEmpty;


	for(i=0; i<nTibetan; i++)
	{
		m = TibetanBuffer[i].ch;

		if(m < 0x21 || m > 0xff) continue;

		fontFileID = SourceFontFileID(TibetanBuffer[i].fontID);

		//This is for Dedris vs Ededris, and Sam vs Esam
		if((SourceEncoding == NewSambhota) && fontFileID > 29)
			fontFileID = fontFileID - 30;
		if((SourceEncoding == OldSambhota) && fontFileID > 2)
			fontFileID = fontFileID - 3;
		//Font file size is different
		if(SourceEncoding == TMW || SourceEncoding == NewSambhota)
			l = fontFileID * 94 + (m - 0x21);
		else if(SourceEncoding == Jamyang) l = fontFileID * 223 + (m - 0x21);  //20090218
		else l = fontFileID * 222 + (m - 0x21);

		thepoint=pTable+l*30;

		n=0;
		while(*thepoint != '\0')
		{
			temp[n++] = *thepoint++;
			if(n==4)
			{
				temp[n] = '\0';
				l = atoi(temp);
				fputc((char)(l&0xff),targetFile);
				fputc((char)(l>>8),targetFile);
				n=0;
			}
		}
	}

	nTibetan = 0;
	return TranslationDone;
}



int TranslateTSLrtf2UniWebpageAndWriteBack()
{
	int i, l, fontFileID, n, m;
	char  *thepoint;
	char temp[50];

	if(nTibetan<=0) return TextBufferEmpty;

	if(TargetFileFormat == WebPage)
	{
		fputs("<font face=\"", targetFile);
		fputs(TibetanFontNames[BaseIndex[TargetEncoding]], targetFile);
		fputs("\" size=5>", targetFile);
	}

	for(i=0; i<nTibetan; i++)
	{
		m = TibetanBuffer[i].ch;

		if(m < 0x21 || m > 0xff) continue;

		fontFileID = SourceFontFileID(TibetanBuffer[i].fontID);

		//This is for Dedris vs Ededris, and Sam vs Esam
		if((SourceEncoding == NewSambhota) && fontFileID > 29)
			fontFileID = fontFileID - 30;
		if((SourceEncoding == OldSambhota) && fontFileID > 2)
			fontFileID = fontFileID - 3;
		//Font file size is different
		if(SourceEncoding == TMW || SourceEncoding == NewSambhota)
			l = fontFileID * 94 + (m - 0x21);
		else if(SourceEncoding == Jamyang) l = fontFileID * 223 + (m - 0x21);  //20090218
		else l = fontFileID * 222 + (m - 0x21);

		thepoint=pTable+l*30;

		n=0;
		while(*thepoint != '\0')
		{
			temp[n++] = *thepoint++;
			if(n==4)
			{
				temp[n] = '\0';
				fputs("&#",targetFile);
				fputs(temp,targetFile);
				fputc(';',targetFile);
				n=0;
			}
		}
	}
	//Close the formating....
	if(TargetFileFormat == WebPage)
	{
		fputs("</font>", targetFile);
	}

	nTibetan = 0;
	return TranslationDone;
}


int TranslateTSLWebpage2UniWebpageAndWriteBack()
{
	int i, l, fontFileID, n, m;
	char  *thepoint;
	char temp[50];

	if(nTibetan<=0) return TextBufferEmpty;

	if(TargetFileFormat == WebPage)
	{
		WriteBackTibetanFontTag();//Only on font --- Unicode
	}

	for(i=0; i<nTibetan; i++)
	{
		m = TibetanBufferForWebPage[i].ch;

		if(m < 0x21 || m > 0xff) continue;

		fontFileID = SourceFontFileIDforWebPage(TibetanBufferForWebPage[i].fontName);

		if(fontFileID == -1) continue;//Wrong font name appears

		//This is for Dedris vs Ededris, and Sam vs Esam
		if((SourceEncoding == NewSambhota) && fontFileID > 29)
			fontFileID = fontFileID - 30;
		if((SourceEncoding == OldSambhota) && fontFileID > 2)
			fontFileID = fontFileID - 3;
		//Font file size is different
		if(SourceEncoding == TMW || SourceEncoding == NewSambhota)
			l = fontFileID * 94 + (m - 0x21);
		else if(SourceEncoding == Jamyang) l = fontFileID * 223 + (m - 0x21);  //20090218
		else l = fontFileID * 222 + (m - 0x21);

		thepoint=pTable+l*30;

		n=0;
		while(*thepoint != '\0')
		{
			temp[n++] = *thepoint++;
			if(n==4)
			{
				temp[n] = '\0';
				fputs("&#",targetFile);
				fputs(temp,targetFile);
				fputc(';',targetFile);
				n=0;
			}
		}
	}

	nTibetan = 0;
	return TranslationDone;
}



int TranslateTSLWebpage2UniRtfAndWriteBack()
{
	int i, l, fontFileID, n, m;
	char  *thepoint;
	char temp[50];

	if(nTibetan<=0) return TextBufferEmpty;

	fputs("\\fs28", targetFile);
	fputs("\\f", targetFile);
	fputs("30", targetFile);//Tibetan Unicode font name's ID is set as 30 in TSLWebPage2UniRtf()

	for(i=0; i<nTibetan; i++)
	{
		m = TibetanBufferForWebPage[i].ch;

		if(m < 0x21 || m > 0xff) continue;

		fontFileID = SourceFontFileIDforWebPage(TibetanBufferForWebPage[i].fontName);

		if(fontFileID == -1) continue;//Wrong font name appears

		//This is for Dedris vs Ededris, and Sam vs Esam
		if((SourceEncoding == NewSambhota) && fontFileID > 29)
			fontFileID = fontFileID - 30;
		if((SourceEncoding == OldSambhota) && fontFileID > 2)
			fontFileID = fontFileID - 3;
		//Font file size is different
		if(SourceEncoding == TMW || SourceEncoding == NewSambhota)
			l = fontFileID * 94 + (m - 0x21);
		else if(SourceEncoding == Jamyang) l = fontFileID * 223 + (m - 0x21);  //20090218
		else l = fontFileID * 222 + (m - 0x21);

		thepoint=pTable+l*30;

		n=0;
		while(*thepoint != '\0')
		{
			temp[n++] = *thepoint++;
			if(n==4)
			{
				temp[n] = '\0';
				fputs("\\u",targetFile);
				fputs(temp,targetFile);
				fputs(" ",targetFile);//20100307
				n=0;
			}
		}
	}

	nTibetan = 0;
	return TranslationDone;
}



int TranslateTSLWebpage2UniTxtAndWriteBack()
{
	int i, l, fontFileID, n, m;
	char  *thepoint;
	char temp[50];

	if(nTibetan<=0) return TextBufferEmpty;

	for(i=0; i<nTibetan; i++)
	{
		m = TibetanBufferForWebPage[i].ch;

		if(m < 0x21 || m > 0xff) continue;

		fontFileID = SourceFontFileIDforWebPage(TibetanBufferForWebPage[i].fontName);

		if(fontFileID == -1) continue;//Wrong font name appears

		//This is for Dedris vs Ededris, and Sam vs Esam
		if((SourceEncoding == NewSambhota) && fontFileID > 29)
			fontFileID = fontFileID - 30;
		if((SourceEncoding == OldSambhota) && fontFileID > 2)
			fontFileID = fontFileID - 3;
		//Font file size is different
		if(SourceEncoding == TMW || SourceEncoding == NewSambhota)
			l = fontFileID * 94 + (m - 0x21);
		else if(SourceEncoding == Jamyang) l = fontFileID * 223 + (m - 0x21);  //20090218
		else l = fontFileID * 222 + (m - 0x21);

		thepoint=pTable+l*30;

		n=0;
		while(*thepoint != '\0')
		{
			temp[n++] = *thepoint++;
			if(n==4)
			{
				temp[n] = '\0';
				l = atoi(temp);
				fputc((char)(l&0xff),targetFile);
				fputc((char)(l>>8),targetFile);
				n=0;
			}
		}
	}

	nTibetan = 0;
	return TranslationDone;
}


//--------------------------- BTN to Unicode --------------------//2007

int TranslateBTNtxt2UniRtfAndWriteBack()
{
	int ch, thecode;
	int l, i, n;
	char *thepoint;
	char temp[50];

	if(nTibetan<=0) return TextBufferEmpty;

	fputs("\\fs28", targetFile);
	fputs("\\f30", targetFile);
	fputc(0x20, targetFile);

	for(i=0; i<nTibetan; i++)
	{
		l = TibetanBuffer[i].ch;

		ch = l & 0xff;
		thecode = l>>8;

//20090325
		if(SourceEncoding == NS && thecode >= 0xf3 && thecode <= 0xf9)
			l = 256*((int)thecode-0xF3)+((int)ch-0x00);
		else if(SourceEncoding == Ty && thecode > 0x80 && thecode < 0xf6 && ch >= 0x40)
			l = 192*((int)thecode-0x81)+((int)ch-0x40);
		else if(SourceEncoding == Bzd && thecode > 0xa5 && thecode < 0xe1 && ch >= 0xa1)
			l = 95*((int)thecode-0xa6)+((int)ch-0xa1); //SourceEncoding == Bzd
		else //Out of the range of Tibetan code  points --->> not a Tibetan --->> Chinese
			l = -1; //Use it as an indicater here!

		if(l != -1) thepoint = pTable + l * 30; //Set the right place for the Unicode points of this code.

		if(l == -1 || *thepoint == '\0') //Check the table and found it's Chinese
		{
			l = ((int)thecode << 8) + (int)ch;
			if(l<=0xff)
			{
				LatinBuffer[nLatin++] = l;
				WriteBackLatin();
			}
			else
			{
				if(nChinese >= SZBuffer) WriteBackChinese();
				ChineseBuffer[nChinese++] = l;
				WriteBackChinese();
			}
			continue;
		}
//20090325
		//Look up the mapping table and translate.
		n=0;
		while(*thepoint != '\0')
		{
			temp[n++] = *thepoint++;
			if(n==4)
			{
				temp[n] = '\0';
				fputs("\\u",targetFile);
				fputs(temp,targetFile);
				fputs(" ",targetFile); //20100307
				n=0;
			}
		}
	}
	nTibetan = 0;

	return EndOfFile;
}


//BTNtxt to Unicode TXT
int TranslateBTNtxt2UniTxtAndWriteBack()
{
	int ch, thecode;
	int l, i, n;
	char *thepoint;
	char temp[50];

	if(nTibetan<=0) return TextBufferEmpty;

	for(i=0; i<nTibetan; i++)
	{
		l = TibetanBuffer[i].ch;

		ch = l & 0xff;
		thecode = l>>8;

//20090325
		if(SourceEncoding == NS && thecode >= 0xf3 && thecode <= 0xf9)
			l = 256*((int)thecode-0xF3)+((int)ch-0x00);
		else if(SourceEncoding == Ty && thecode > 0x80 && thecode < 0xf6 && ch >= 0x40)
			l = 192*((int)thecode-0x81)+((int)ch-0x40);
		else if(SourceEncoding == Bzd && thecode > 0xa5 && thecode < 0xe1 && ch >= 0xa1)
			l = 95*((int)thecode-0xa6)+((int)ch-0xa1); //SourceEncoding == Bzd
		else //Out of the range of Tibetan code  points --->> not a Tibetan --->> Chinese
			l = -1; //Use it as an indicater here!

		if(l != -1) thepoint = pTable + l * 30; //Set the right place for the Unicode points of this code.

		if(l == -1 || *thepoint == '\0') //Check the table and found it's Chinese
		{
			l = ((int)thecode << 8) + (int)ch;
			if(l<=0xff)
			{
				LatinBuffer[nLatin++] = l;
				WriteBackLatin();
			}
			else
			{
				if(nChinese >= SZBuffer) WriteBackChinese();
				ChineseBuffer[nChinese++] = l;
				WriteBackChinese();
			}
			continue;
		}
//20090325

		n=0;
		while(*thepoint != '\0')
		{
			temp[n++] = *thepoint++;
			if(n==4)
			{
				temp[n] = '\0';
				l = atoi(temp);
				fputc((char)(l&0xff),targetFile);
				fputc((char)(l>>8),targetFile);
				n=0;
			}
		}
	}
	nTibetan = 0;

	return EndOfFile;
}


//BTNtxt to Unicode Webpage
int TranslateBTNtxt2UniWebpageAndWriteBack()
{
	int ch, thecode;
	int l, i, n;
	char *thepoint;
	char temp[50];

	if(nTibetan<=0) return TextBufferEmpty;

	if(TargetFileFormat == WebPage)
	{
		fputs("<font face=\"", targetFile);
		fputs(TibetanFontNames[BaseIndex[TargetEncoding]], targetFile);
		fputs("\" size=5>", targetFile);
	}

	for(i=0; i<nTibetan; i++)
	{
		l = TibetanBuffer[i].ch;

		ch = l & 0xff;
		thecode = l>>8;

/*  //20090325
		if(SourceEncoding == NS) l = 256*((int)thecode-0xF3)+((int)ch-0x00);
		else if(SourceEncoding == Ty) l = 192*((int)thecode-0x81)+((int)ch-0x40);
		else l = 95*((int)thecode-0xa6)+((int)ch-0xa1); //SourceEncoding == Bzd

		thepoint=pTable+l*30;
*/  //20090325

//20090325
		if(SourceEncoding == NS && thecode >= 0xf3 && thecode <= 0xf9)
			l = 256*((int)thecode-0xF3)+((int)ch-0x00);
		else if(SourceEncoding == Ty && thecode > 0x80 && thecode < 0xf6 && ch >= 0x40)
			l = 192*((int)thecode-0x81)+((int)ch-0x40);
		else if(SourceEncoding == Bzd && thecode > 0xa5 && thecode < 0xe1 && ch >= 0xa1)
			l = 95*((int)thecode-0xa6)+((int)ch-0xa1); //SourceEncoding == Bzd
		else //Out of the range of Tibetan code  points --->> not a Tibetan --->> Chinese
			l = -1; //Use it as an indicater here!

		if(l != -1) thepoint = pTable + l * 30; //Set the right place for the Unicode points of this code.

		if(l == -1 || *thepoint == '\0') //Check the table and found it's Chinese
		{
			l = ((int)thecode << 8) + (int)ch;
			if(l<=0xff)
			{
				LatinBuffer[nLatin++] = l;
				WriteBackLatin();
			}
			else
			{
				if(nChinese >= SZBuffer) WriteBackChinese();
				ChineseBuffer[nChinese++] = l;
				WriteBackChinese();
			}
			continue;
		}
//20090325
		//Look up the mapping table and translate.
		n=0;
		while(*thepoint != '\0')
		{
			temp[n++] = *thepoint++;
			if(n==4)
			{
				temp[n] = '\0';
				fputs("&#",targetFile);
				fputs(temp,targetFile);
				fputc(';',targetFile);
				n=0;
			}
		}
	}
	//Close the formating....
	if(TargetFileFormat == WebPage)
	{
		fputs("</font>", targetFile);
	}

	nTibetan = 0;

	return EndOfFile;
}


int TranslateBTNrtf2UniRtfAndWriteBack()
{
	int i, l, ch, thecode, n;
	char  *thepoint;
	char temp[50];

	if(nTibetan<=0) return TextBufferEmpty;

	fputs("\\f", targetFile);
	fprintf(targetFile, "%d", targetFontIDs[0]);

	for(i=0; i<nTibetan; i++)
	{
		l = TibetanBuffer[i].ch;

		ch = l & 0xff;
		thecode = l>>8;
/*  //20090325
		if(SourceEncoding == NS) l = 256*((int)thecode-0xF3)+((int)ch-0x00);
		else if(SourceEncoding == Ty) l = 192*((int)thecode-0x81)+((int)ch-0x40);
		else l = 95*((int)thecode-0xa6)+((int)ch-0xa1); //SourceEncoding == Bzd

		thepoint=pTable+l*30;
*/  //20090325

//20090325
		if(SourceEncoding == NS && thecode >= 0xf3 && thecode <= 0xf9)
			l = 256*((int)thecode-0xF3)+((int)ch-0x00);
		else if(SourceEncoding == Ty && thecode > 0x80 && thecode < 0xf6 && ch >= 0x40)
			l = 192*((int)thecode-0x81)+((int)ch-0x40);
		else if(SourceEncoding == Bzd && thecode > 0xa5 && thecode < 0xe1 && ch >= 0xa1)
			l = 95*((int)thecode-0xa6)+((int)ch-0xa1); //SourceEncoding == Bzd
		else //Out of the range of Tibetan code  points --->> not a Tibetan --->> Chinese
			l = -1; //Use it as an indicater here!

		if(l != -1) thepoint = pTable + l * 30; //Set the right place for the Unicode points of this code.

		if(l == -1 || *thepoint == '\0') //Check the table and found it's Chinese
		{
			l = ((int)thecode << 8) + (int)ch;
			if(l<=0xff)
			{
				LatinBuffer[nLatin++] = l;
				WriteBackLatin();
			}
			else
			{
				if(nChinese >= SZBuffer) WriteBackChinese();
				ChineseBuffer[nChinese++] = l;
				WriteBackChinese();
			}
			continue;
		}
//20090325

		n=0;
		while(*thepoint != '\0')
		{
			temp[n++] = *thepoint++;
			if(n==4)
			{
				temp[n] = '\0';
				fputs("\\u",targetFile);
				fputs(temp,targetFile);
				fputs(" ",targetFile);//20100307
				n=0;
			}
		}
	}

	nTibetan = 0;
	return TranslationDone;
}



///From TSL rtf to Unicode TXT
int TranslateBTNrtf2UniTxtAndWriteBack()
{
	int i, l, ch, thecode, n;
	char  *thepoint;
	char temp[50];

	if(nTibetan<=0) return TextBufferEmpty;

	for(i=0; i<nTibetan; i++)
	{
		l = TibetanBuffer[i].ch;

		ch = l & 0xff;
		thecode = l>>8;
/*  //20090325
		if(SourceEncoding == NS) l = 256*((int)thecode-0xF3)+((int)ch-0x00);
		else if(SourceEncoding == Ty) l = 192*((int)thecode-0x81)+((int)ch-0x40);
		else l = 95*((int)thecode-0xa6)+((int)ch-0xa1); //SourceEncoding == Bzd

		thepoint=pTable+l*30;
*/  //20090325

//20090325
		if(SourceEncoding == NS && thecode >= 0xf3 && thecode <= 0xf9)
			l = 256*((int)thecode-0xF3)+((int)ch-0x00);
		else if(SourceEncoding == Ty && thecode > 0x80 && thecode < 0xf6 && ch >= 0x40)
			l = 192*((int)thecode-0x81)+((int)ch-0x40);
		else if(SourceEncoding == Bzd && thecode > 0xa5 && thecode < 0xe1 && ch >= 0xa1)
			l = 95*((int)thecode-0xa6)+((int)ch-0xa1); //SourceEncoding == Bzd
		else //Out of the range of Tibetan code  points --->> not a Tibetan --->> Chinese
			l = -1; //Use it as an indicater here!

		if(l != -1) thepoint = pTable + l * 30; //Set the right place for the Unicode points of this code.

		if(l == -1 || *thepoint == '\0') //Check the table and found it's Chinese
		{
			l = ((int)thecode << 8) + (int)ch;
			if(l<=0xff)
			{
				LatinBuffer[nLatin++] = l;
				WriteBackLatin();
			}
			else
			{
				if(nChinese >= SZBuffer) WriteBackChinese();
				ChineseBuffer[nChinese++] = l;
				WriteBackChinese();
			}
			continue;
		}
//20090325

		n=0;
		while(*thepoint != '\0')
		{
			temp[n++] = *thepoint++;
			if(n==4)
			{
				temp[n] = '\0';
				l = atoi(temp);
				fputc((char)(l&0xff),targetFile);
				fputc((char)(l>>8),targetFile);
				n=0;
			}
		}
	}

	nTibetan = 0;
	return TranslationDone;
}


int TranslateBTNrtf2UniWebpageAndWriteBack()
{
	int i, l, ch, thecode, n;
	char  *thepoint;
	char temp[50];

	if(nTibetan<=0) return TextBufferEmpty;

	if(TargetFileFormat == WebPage)
	{
		fputs("<font face=\"", targetFile);
		fputs(TibetanFontNames[BaseIndex[TargetEncoding]], targetFile);
		fputs("\" size=5>", targetFile);
	}

	for(i=0; i<nTibetan; i++)
	{
		l = TibetanBuffer[i].ch;

		ch = l & 0xff;
		thecode = l>>8;

/*  //20090325
		if(SourceEncoding == NS) l = 256*((int)thecode-0xF3)+((int)ch-0x00);
		else if(SourceEncoding == Ty) l = 192*((int)thecode-0x81)+((int)ch-0x40);
		else l = 95*((int)thecode-0xa6)+((int)ch-0xa1); //SourceEncoding == Bzd

		thepoint=pTable+l*30;
*/  //20090325

//20090325
		if(SourceEncoding == NS && thecode >= 0xf3 && thecode <= 0xf9)
			l = 256*((int)thecode-0xF3)+((int)ch-0x00);
		else if(SourceEncoding == Ty && thecode > 0x80 && thecode < 0xf6 && ch >= 0x40)
			l = 192*((int)thecode-0x81)+((int)ch-0x40);
		else if(SourceEncoding == Bzd && thecode > 0xa5 && thecode < 0xe1 && ch >= 0xa1)
			l = 95*((int)thecode-0xa6)+((int)ch-0xa1); //SourceEncoding == Bzd
		else //Out of the range of Tibetan code  points --->> not a Tibetan --->> Chinese
			l = -1; //Use it as an indicater here!

		if(l != -1) thepoint = pTable + l * 30; //Set the right place for the Unicode points of this code.

		if(l == -1 || *thepoint == '\0') //Check the table and found it's Chinese
		{
			l = ((int)thecode << 8) + (int)ch;
			if(l<=0xff)
			{
				LatinBuffer[nLatin++] = l;
				WriteBackLatin();
			}
			else
			{
				if(nChinese >= SZBuffer) WriteBackChinese();
				ChineseBuffer[nChinese++] = l;
				WriteBackChinese();
			}
			continue;
		}
//20090325

		n=0;
		while(*thepoint != '\0')
		{
			temp[n++] = *thepoint++;
			if(n==4)
			{
				temp[n] = '\0';
				fputs("&#",targetFile);
				fputs(temp,targetFile);
				fputc(';',targetFile);
				n=0;
			}
		}
	}
	//Close the formating....
	if(TargetFileFormat == WebPage)
	{
		fputs("</font>", targetFile);
	}

	nTibetan = 0;
	return TranslationDone;
}


int TranslateBTNWebpage2UniWebpageAndWriteBack()
{
	int i, l, ch, thecode, n;
	char  *thepoint;
	char temp[50];

	if(nTibetan<=0) return TextBufferEmpty;

	if(TargetFileFormat == WebPage)
	{
		WriteBackTibetanFontTag();//Only on font --- Unicode
	}

	for(i=0; i<nTibetan; i++)
	{
		l = TibetanBufferForWebPage[i].ch;

		ch = l & 0xff;
		thecode = l>>8;

/*  //20090325
		if(SourceEncoding == NS) l = 256*((int)thecode-0xF3)+((int)ch-0x00);
		else if(SourceEncoding == Ty) l = 192*((int)thecode-0x81)+((int)ch-0x40);
		else l = 95*((int)thecode-0xa6)+((int)ch-0xa1); //SourceEncoding == Bzd

		thepoint=pTable+l*30;
*/  //20090325

//20090325
		if(SourceEncoding == NS && thecode >= 0xf3 && thecode <= 0xf9)
			l = 256*((int)thecode-0xF3)+((int)ch-0x00);
		else if(SourceEncoding == Ty && thecode > 0x80 && thecode < 0xf6 && ch >= 0x40)
			l = 192*((int)thecode-0x81)+((int)ch-0x40);
		else if(SourceEncoding == Bzd && thecode > 0xa5 && thecode < 0xe1 && ch >= 0xa1)
			l = 95*((int)thecode-0xa6)+((int)ch-0xa1); //SourceEncoding == Bzd
		else //Out of the range of Tibetan code  points --->> not a Tibetan --->> Chinese
			l = -1; //Use it as an indicater here!

		if(l != -1) thepoint = pTable + l * 30; //Set the right place for the Unicode points of this code.

		if(l == -1 || *thepoint == '\0') //Check the table and found it's Chinese
		{
			l = ((int)thecode << 8) + (int)ch;
			if(l<=0xff)
			{
				LatinBuffer[nLatin++] = l;
				WriteBackLatin();
			}
			else
			{
				if(nChinese >= SZBuffer) WriteBackChinese();
				ChineseBuffer[nChinese++] = l;
				WriteBackChinese();
			}
			continue;
		}
//20090325

		n=0;
		while(*thepoint != '\0')
		{
			temp[n++] = *thepoint++;
			if(n==4)
			{
				temp[n] = '\0';
				fputs("&#",targetFile);
				fputs(temp,targetFile);
				fputc(';',targetFile);
				n=0;
			}
		}
	}

	nTibetan = 0;
	return TranslationDone;
}


int TranslateBTNWebpage2UniRtfAndWriteBack()
{
	int i, l, ch, thecode, n;
	char  *thepoint;
	char temp[50];

	if(nTibetan<=0) return TextBufferEmpty;

	fputs("\\fs28", targetFile);
	fputs("\\f", targetFile);
	fputs("30", targetFile);//Tibetan Unicode font name's ID is set as 30 in TSLWebPage2UniRtf()

	for(i=0; i<nTibetan; i++)
	{
		l = TibetanBufferForWebPage[i].ch;

		ch = l & 0xff;
		thecode = l>>8;

/*  //20090325
		if(SourceEncoding == NS) l = 256*((int)thecode-0xF3)+((int)ch-0x00);
		else if(SourceEncoding == Ty) l = 192*((int)thecode-0x81)+((int)ch-0x40);
		else l = 95*((int)thecode-0xa6)+((int)ch-0xa1); //SourceEncoding == Bzd

		thepoint=pTable+l*30;
*/  //20090325

//20090325
		if(SourceEncoding == NS && thecode >= 0xf3 && thecode <= 0xf9)
			l = 256*((int)thecode-0xF3)+((int)ch-0x00);
		else if(SourceEncoding == Ty && thecode > 0x80 && thecode < 0xf6 && ch >= 0x40)
			l = 192*((int)thecode-0x81)+((int)ch-0x40);
		else if(SourceEncoding == Bzd && thecode > 0xa5 && thecode < 0xe1 && ch >= 0xa1)
			l = 95*((int)thecode-0xa6)+((int)ch-0xa1); //SourceEncoding == Bzd
		else //Out of the range of Tibetan code  points --->> not a Tibetan --->> Chinese
			l = -1; //Use it as an indicater here!

		if(l != -1) thepoint = pTable + l * 30; //Set the right place for the Unicode points of this code.

		if(l == -1 || *thepoint == '\0') //Check the table and found it's Chinese
		{
			l = ((int)thecode << 8) + (int)ch;
			if(l<=0xff)
			{
				LatinBuffer[nLatin++] = l;
				WriteBackLatin();
			}
			else
			{
				if(nChinese >= SZBuffer) WriteBackChinese();
				ChineseBuffer[nChinese++] = l;
				WriteBackChinese();
			}
			continue;
		}
//20090325

		n=0;
		while(*thepoint != '\0')
		{
			temp[n++] = *thepoint++;
			if(n==4)
			{
				temp[n] = '\0';
				fputs("\\u",targetFile);
				fputs(temp,targetFile);
				fputs(" ",targetFile);//20100307
				n=0;
			}
		}
	}

	nTibetan = 0;
	return TranslationDone;
}


int TranslateBTNWebpage2UniTxtAndWriteBack()
{
	int i, l, ch, thecode, n;
	char  *thepoint;
	char temp[50];

	if(nTibetan<=0) return TextBufferEmpty;

	for(i=0; i<nTibetan; i++)
	{
		l = TibetanBufferForWebPage[i].ch;

		ch = l & 0xff;
		thecode = l>>8;

/*  //20090325
		if(SourceEncoding == NS) l = 256*((int)thecode-0xF3)+((int)ch-0x00);
		else if(SourceEncoding == Ty) l = 192*((int)thecode-0x81)+((int)ch-0x40);
		else l = 95*((int)thecode-0xa6)+((int)ch-0xa1); //SourceEncoding == Bzd

		thepoint=pTable+l*30;
*/  //20090325

//20090325
		if(SourceEncoding == NS && thecode >= 0xf3 && thecode <= 0xf9)
			l = 256*((int)thecode-0xF3)+((int)ch-0x00);
		else if(SourceEncoding == Ty && thecode > 0x80 && thecode < 0xf6 && ch >= 0x40)
			l = 192*((int)thecode-0x81)+((int)ch-0x40);
		else if(SourceEncoding == Bzd && thecode > 0xa5 && thecode < 0xe1 && ch >= 0xa1)
			l = 95*((int)thecode-0xa6)+((int)ch-0xa1); //SourceEncoding == Bzd
		else //Out of the range of Tibetan code  points --->> not a Tibetan --->> Chinese
			l = -1; //Use it as an indicater here!

		if(l != -1) thepoint = pTable + l * 30; //Set the right place for the Unicode points of this code.

		if(l == -1 || *thepoint == '\0') //Check the table and found it's Chinese
		{
			l = ((int)thecode << 8) + (int)ch;
			if(l<=0xff)
			{
				LatinBuffer[nLatin++] = l;
				WriteBackLatin();
			}
			else
			{
				if(nChinese >= SZBuffer) WriteBackChinese();
				ChineseBuffer[nChinese++] = l;
				WriteBackChinese();
			}
			continue;
		}
//20090325

		n=0;
		while(*thepoint != '\0')
		{
			temp[n++] = *thepoint++;
			if(n==4)
			{
				temp[n] = '\0';
				l = atoi(temp);
				fputc((char)(l&0xff),targetFile);
				fputc((char)(l>>8),targetFile);
				n=0;
			}
		}
	}

	nTibetan = 0;
	return TranslationDone;
}


//------------------------------------ NSUniTxt to Unicode --------------------//2007

int TranslateBTNUniTxt2UniRtfAndWriteBack()
{
	int ch, thecode;
	int l, i, n;
	char *thepoint;
	char temp[50];

	if(nTibetan<=0) return TextBufferEmpty;

	fputs("\\fs28", targetFile);
	fputs("\\f30", targetFile);
	fputc(0x20, targetFile);

	for(i=0; i<nTibetan; i++)
	{
		l = TibetanBuffer[i].ch;

		if(l>=0x0f00 && l<=0x0fff) //Uicode Tibetan codepoints
		{
			fputs("\\u",targetFile);
			fprintf(targetFile, "%d", l);
			fputs(" ",targetFile);//20100307
			continue;
		}

		ch = l & 0xff;
		thecode = l>>8;

/*  //20090325
		if(SourceEncoding == NS) l = 256*((int)thecode-0xF3)+((int)ch-0x00);
		else if(SourceEncoding == Ty) l = 192*((int)thecode-0x81)+((int)ch-0x40);
		else l = 95*((int)thecode-0xa6)+((int)ch-0xa1); //SourceEncoding == Bzd

		thepoint=pTable+l*30;
*/  //20090325

//20090325
		if(SourceEncoding == NS && thecode >= 0xf3 && thecode <= 0xf9)
			l = 256*((int)thecode-0xF3)+((int)ch-0x00);
		else if(SourceEncoding == Ty && thecode > 0x80 && thecode < 0xf6 && ch >= 0x40)
			l = 192*((int)thecode-0x81)+((int)ch-0x40);
		else if(SourceEncoding == Bzd && thecode > 0xa5 && thecode < 0xe1 && ch >= 0xa1)
			l = 95*((int)thecode-0xa6)+((int)ch-0xa1); //SourceEncoding == Bzd
		else //Out of the range of Tibetan code  points --->> not a Tibetan --->> Chinese
			l = -1; //Use it as an indicater here!

		if(l != -1) thepoint = pTable + l * 30; //Set the right place for the Unicode points of this code.

		if(l == -1 || *thepoint == '\0') //Check the table and found it's Chinese
		{
			l = ((int)thecode << 8) + (int)ch;
			if(l<=0xff)
			{
				LatinBuffer[nLatin++] = l;
				WriteBackLatin();
			}
			else
			{
				if(nChinese >= SZBuffer) WriteBackChinese();
				ChineseBuffer[nChinese++] = l;
				WriteBackChinese();
			}
			continue;
		}
//20090325
		//Look up the mapping table and translate.
		n=0;
		while(*thepoint != '\0')
		{
			temp[n++] = *thepoint++;
			if(n==4)
			{
				temp[n] = '\0';
				fputs("\\u",targetFile);
				fputs(temp,targetFile);
				fputs(" ",targetFile);//20100307
				n=0;
			}
		}
	}
	nTibetan = 0;

	return EndOfFile;
}


//NSUniTxt to Unicode TXT
int TranslateBTNUniTxt2UniTxtAndWriteBack()
{
	int ch, thecode;
	int l, i, n;
	char *thepoint;
	char temp[50];

	if(nTibetan<=0) return TextBufferEmpty;

	for(i=0; i<nTibetan; i++)
	{
		l = TibetanBuffer[i].ch;

		if(l>=0x0f00 && l<=0x0fff) //Uicode Tibetan codepoints
		{
			fputc((char)(l&0xff),targetFile);
			fputc((char)(l>>8),targetFile);
			continue;
		}

		ch = l & 0xff;
		thecode = l>>8;

/*  //20090325
		if(SourceEncoding == NS) l = 256*((int)thecode-0xF3)+((int)ch-0x00);
		else if(SourceEncoding == Ty) l = 192*((int)thecode-0x81)+((int)ch-0x40);
		else l = 95*((int)thecode-0xa6)+((int)ch-0xa1); //SourceEncoding == Bzd

		thepoint=pTable+l*30;
*/  //20090325

//20090325
		if(SourceEncoding == NS && thecode >= 0xf3 && thecode <= 0xf9)
			l = 256*((int)thecode-0xF3)+((int)ch-0x00);
		else if(SourceEncoding == Ty && thecode > 0x80 && thecode < 0xf6 && ch >= 0x40)
			l = 192*((int)thecode-0x81)+((int)ch-0x40);
		else if(SourceEncoding == Bzd && thecode > 0xa5 && thecode < 0xe1 && ch >= 0xa1)
			l = 95*((int)thecode-0xa6)+((int)ch-0xa1); //SourceEncoding == Bzd
		else //Out of the range of Tibetan code  points --->> not a Tibetan --->> Chinese
			l = -1; //Use it as an indicater here!

		if(l != -1) thepoint = pTable + l * 30; //Set the right place for the Unicode points of this code.

		if(l == -1 || *thepoint == '\0') //Check the table and found it's Chinese
		{
			l = ((int)thecode << 8) + (int)ch;
			if(l<=0xff)
			{
				LatinBuffer[nLatin++] = l;
				WriteBackLatin();
			}
			else
			{
				if(nChinese >= SZBuffer) WriteBackChinese();
				ChineseBuffer[nChinese++] = l;
				WriteBackChinese();
			}
			continue;
		}
//20090325

		n=0;
		while(*thepoint != '\0')
		{
			temp[n++] = *thepoint++;
			if(n==4)
			{
				temp[n] = '\0';
				l = atoi(temp);
				fputc((char)(l&0xff),targetFile);
				fputc((char)(l>>8),targetFile);
				n=0;
			}
		}
	}
	nTibetan = 0;

	return EndOfFile;
}


//NSUniTxt to Unicode Webpage
int TranslateBTNUniTxt2UniWebpageAndWriteBack()
{
	int ch, thecode;
	int l, i, n;
	char *thepoint;
	char temp[50];

	if(nTibetan<=0) return TextBufferEmpty;

	if(TargetFileFormat == WebPage)
	{
		fputs("<font face=\"", targetFile);
		fputs(TibetanFontNames[BaseIndex[TargetEncoding]], targetFile);
		fputs("\" size=5>", targetFile);
	}

	for(i=0; i<nTibetan; i++)
	{
		l = TibetanBuffer[i].ch;

		if(l>=0x0f00 && l<=0x0fff) //Uicode Tibetan codepoints
		{
			fputs("&#",targetFile);
			fprintf(targetFile, "%d", l);
			fputc(';',targetFile);
			continue;
		}

		ch = l & 0xff;
		thecode = l>>8;

/*  //20090325
		if(SourceEncoding == NS) l = 256*((int)thecode-0xF3)+((int)ch-0x00);
		else if(SourceEncoding == Ty) l = 192*((int)thecode-0x81)+((int)ch-0x40);
		else l = 95*((int)thecode-0xa6)+((int)ch-0xa1); //SourceEncoding == Bzd

		thepoint=pTable+l*30;
*/  //20090325

//20090325
		if(SourceEncoding == NS && thecode >= 0xf3 && thecode <= 0xf9)
			l = 256*((int)thecode-0xF3)+((int)ch-0x00);
		else if(SourceEncoding == Ty && thecode > 0x80 && thecode < 0xf6 && ch >= 0x40)
			l = 192*((int)thecode-0x81)+((int)ch-0x40);
		else if(SourceEncoding == Bzd && thecode > 0xa5 && thecode < 0xe1 && ch >= 0xa1)
			l = 95*((int)thecode-0xa6)+((int)ch-0xa1); //SourceEncoding == Bzd
		else //Out of the range of Tibetan code  points --->> not a Tibetan --->> Chinese
			l = -1; //Use it as an indicater here!

		if(l != -1) thepoint = pTable + l * 30; //Set the right place for the Unicode points of this code.

		if(l == -1 || *thepoint == '\0') //Check the table and found it's Chinese
		{
			l = ((int)thecode << 8) + (int)ch;
			if(l<=0xff)
			{
				LatinBuffer[nLatin++] = l;
				WriteBackLatin();
			}
			else
			{
				if(nChinese >= SZBuffer) WriteBackChinese();
				ChineseBuffer[nChinese++] = l;
				WriteBackChinese();
			}
			continue;
		}
//20090325
		//Look up the mapping table and translate.
		n=0;
		while(*thepoint != '\0')
		{
			temp[n++] = *thepoint++;
			if(n==4)
			{
				temp[n] = '\0';
				fputs("&#",targetFile);
				fputs(temp,targetFile);
				fputc(';',targetFile);
				n=0;
			}
		}
	}
	//Close the formating....
	if(TargetFileFormat == WebPage)
	{
		fputs("</font>", targetFile);
	}

	nTibetan = 0;

	return EndOfFile;
}

//------------------------------------ FzHg to Unicode -----------------------

int TranslateFzHg2UniRtfAndWriteBack()
{
	int ch, thecode;
	int l, i, n;
	char *thepoint;
	char temp[50];

	if(nTibetan<=0) return TextBufferEmpty;

	fputs("\\fs28", targetFile);
	fputs("\\f30", targetFile);
	fputc(0x20, targetFile);

	for(i=0; i<nTibetan; i++)
	{
		l = TibetanBuffer[i].ch;

		ch = l & 0xff;
		thecode = l>>8;

		if(SourceEncoding == Hg)
		{
//			if(thecode==0xee && ch > 0x25) continue; //Out of the mapping table. 0xEE25 the bottom of the mapping table//20090615
			if(l>0xee25) continue; //Out of the mapping table. 0xEE25 the bottom of the mapping table//20090615
			l=94*(thecode-0xb0)+(ch-0x21);
		}
		else //SourceEncoding == Fz
		{
//			if(thecode==0xef && ch > 0x63) continue; //Out of the mapping table. 0xEF63 the bottom of the mapping table//20090615
			if(l>0xef6b) continue; //Out of the mapping table. 0xEF63 the bottom of the mapping table//20090615
			l=94*(thecode-0xc0)+(ch-0x21);
		}

		thepoint=pTable+l*30;
		//Look up the mapping table and translate.
		n=0;
		while(*thepoint != '\0')
		{
			temp[n++] = *thepoint++;
			if(n==4)
			{
				temp[n] = '\0';
				fputs("\\u",targetFile);
				fputs(temp,targetFile);
				fputs(" ",targetFile);//20100307
				n=0;
			}
		}
	}
	nTibetan = 0;

	return EndOfFile;
}



//FzHg to Unicode TXT
int TranslateFzHg2UniTxtAndWriteBack()
{
	int ch, thecode;
	int l, i, n;
	char *thepoint;
	char temp[50];

	if(nTibetan<=0) return TextBufferEmpty;

	for(i=0; i<nTibetan; i++)
	{
		l = TibetanBuffer[i].ch;

		ch = l & 0xff;
		thecode = l>>8;

		if(SourceEncoding == Hg)
		{
//			if(thecode==0xee && ch > 0x25) continue; //Out of the mapping table. 0xEE25 the bottom of the mapping table//20090615
			if(l>0xee25) continue; //Out of the mapping table. 0xEE25 the bottom of the mapping table//20090615
			l=94*(thecode-0xb0)+(ch-0x21);
		}
		else //SourceEncoding == Fz
		{
//			if(thecode==0xef && ch > 0x63) continue; //Out of the mapping table. 0xEF63 the bottom of the mapping table//20090615
			if(l>0xef6b) continue; //Out of the mapping table. 0xEF63 the bottom of the mapping table//20090615
			l=94*(thecode-0xc0)+(ch-0x21);
		}

		thepoint=pTable+l*30;

		n=0;
		while(*thepoint != '\0')
		{
			temp[n++] = *thepoint++;
			if(n==4)
			{
				temp[n] = '\0';
				l = atoi(temp);
				fputc((char)(l&0xff),targetFile);
				fputc((char)(l>>8),targetFile);
				n=0;
			}
		}
	}
	nTibetan = 0;

	return EndOfFile;
}


//FzHg to Unicode Webpage
int TranslateFzHg2UniWebpageAndWriteBack()
{
	int ch, thecode;
	int l, i, n;
	char *thepoint;
	char temp[50];

	if(nTibetan<=0) return TextBufferEmpty;

	if(TargetFileFormat == WebPage)
	{
		fputs("<font face=\"", targetFile);
		fputs(TibetanFontNames[BaseIndex[TargetEncoding]], targetFile);
		fputs("\" size=5>", targetFile);
	}

	for(i=0; i<nTibetan; i++)
	{
		l = TibetanBuffer[i].ch;

		ch = l & 0xff;
		thecode = l>>8;

		if(SourceEncoding == Hg)
		{
//			if(thecode==0xee && ch > 0x25) continue; //Out of the mapping table. 0xEE25 the bottom of the mapping table//20090615
			if(l>0xee25) continue; //Out of the mapping table. 0xEE25 the bottom of the mapping table//20090615
			l=94*(thecode-0xb0)+(ch-0x21);
		}
		else //SourceEncoding == Fz
		{
//			if(thecode==0xef && ch > 0x63) continue; //Out of the mapping table. 0xEF63 the bottom of the mapping table//20090615
			if(l>0xef6b) continue; //Out of the mapping table. 0xEF63 the bottom of the mapping table//20090615
			l=94*(thecode-0xc0)+(ch-0x21);
		}

		thepoint=pTable+l*30;
		//Look up the mapping table and translate.
		n=0;
		while(*thepoint != '\0')
		{
			temp[n++] = *thepoint++;
			if(n==4)
			{
				temp[n] = '\0';
				fputs("&#",targetFile);
				fputs(temp,targetFile);
				fputc(';',targetFile);
				n=0;
			}
		}
	}
	//Close the formating....
	if(TargetFileFormat == WebPage)
	{
		fputs("</font>", targetFile);
	}

	nTibetan = 0;

	return EndOfFile;
}



//----------------------------------------------------------------------------------------------------
//Translate the sequence of source text by looking up the right mapping table.
int TranslateAndWriteBack(void)	//ToUnicode(void)  //2007
{
	int ret;

	if(ConversionDirection == UnicodeToOthers)
	{
		if(holdTibetanBuffer == Yes)//Need to wait for next character
		{
			if(nTibetan < SZBuffer) return TranslationDone; //Wait for next character
			else holdTibetanBuffer = No; //nTibetan >= SZBuffer, go ahead to process Tibetan
		}

		if(SourceFileFormat == RTF) //SourceFileFormat == RTF
		{
			if(TargetFileFormat == RTF)
			{
				if(TargetEncoding == ACIP || TargetEncoding == Wylie || TargetEncoding == THDLWylie || TargetEncoding == LCWylie)
                {
					ret = TranslateUniRtf2AWrtfAndWriteBack();
                }
				else if(TargetEncoding == Bzd || TargetEncoding == Ty || TargetEncoding == NS )  //2007
                {
					ret = TranslateUniRtf2BTNrtfAndWriteBack();
                }
				else //TSLs
                {
					ret = TranslateUniRtf2TSLrtfAndWriteBack();
                }
			}
			else if(TargetFileFormat == WebPage)
			{
				if(TargetEncoding == ACIP || TargetEncoding == Wylie || TargetEncoding == THDLWylie || TargetEncoding == LCWylie)
					ret = TranslateUniRtf2AWWebpageAndWriteBack();
				else if(TargetEncoding == Bzd || TargetEncoding == Ty || TargetEncoding == NS )  //2007
					ret = TranslateUniRtf2BTNWebpageAndWriteBack();
				else //TSLs
					ret = TranslateUniRtf2TSLWebpageAndWriteBack();
			}
			else if(TargetFileFormat == TXT)
			{
				if(TargetEncoding == ACIP || TargetEncoding == Wylie || TargetEncoding == THDLWylie || TargetEncoding == LCWylie)
					ret = TranslateUniRtf2AWtxtAndWriteBack();
				else if(TargetEncoding == Bzd || TargetEncoding == Ty || TargetEncoding == NS )  //2007
					ret = TranslateUniRtf2BTNtxtAndWriteBack();
				else //Fz, Hg, Bzd, Ty, NS  //2007
					ret = TranslateUniRtf2FzHgAndWriteBack();
			}
			else //TarfetFileFormat == UnicodeTXT
			{
				if(TargetEncoding == NS )  //2007
					ret = TranslateUniRtf2BTNUniTxtAndWriteBack();//
				else
					ret = TranslateUniRtf2AWUniTxtAndWriteBack();//TargetEncoding == AW )
			}
		}
		else if(SourceFileFormat == WebPage) //SourceFileFormat == WebPage
		{
			if(TargetFileFormat == RTF)
			{
				if(TargetEncoding == ACIP || TargetEncoding == Wylie || TargetEncoding == THDLWylie || TargetEncoding == LCWylie)
					ret = TranslateUniWebpage2AWrtfAndWriteBack();
				else if(TargetEncoding == Bzd || TargetEncoding == Ty || TargetEncoding == NS )  //2007
					ret = TranslateUniWebpage2BTNrtfAndWriteBack();
				else //TSLs
					ret = TranslateUniWebpage2TSLrtfAndWriteBack();
			}
			else if(TargetFileFormat == WebPage)
			{
				if(TargetEncoding == ACIP || TargetEncoding == Wylie || TargetEncoding == THDLWylie || TargetEncoding == LCWylie)
					ret = TranslateUniWebpage2AWWebpageAndWriteBack();
				else if(TargetEncoding == Bzd || TargetEncoding == Ty || TargetEncoding == NS )  //2007
					ret = TranslateUniWebpage2BTNWebpageAndWriteBack();
				else //TSLs
					ret = TranslateUniWebpage2TSLWebpageAndWriteBack();
			}
			else if(TargetFileFormat == TXT)
			{
				if(TargetEncoding == ACIP || TargetEncoding == Wylie || TargetEncoding == THDLWylie || TargetEncoding == LCWylie)
					ret = TranslateUniWebpage2AWtxtAndWriteBack();
				else if(TargetEncoding == Bzd || TargetEncoding == Ty || TargetEncoding == NS )  //2007
					ret = TranslateUniWebpage2BTNtxtAndWriteBack();
				else //FzHg
					ret = TranslateUniWebpage2FzHgAndWriteBack();
			}
			else //TarfetFileFormat == UnicodeTXT
			{
				if(TargetEncoding == NS )  //2007
					ret = TranslateUniWebpage2BTNUniTxtAndWriteBack();//
				else
					ret = TranslateUniWebpage2AWUniTxtAndWriteBack();//This is only the possible translater
			}
		}
		else //SourceFileFormat == UnicodeTXT
		{
			if(TargetFileFormat == RTF)
			{
				if(TargetEncoding == ACIP || TargetEncoding == Wylie || TargetEncoding == THDLWylie || TargetEncoding == LCWylie)
					ret = TranslateUniTxt2AWrtfAndWriteBack();
				else if(TargetEncoding == Bzd || TargetEncoding == Ty || TargetEncoding == NS )  //2007
					ret = TranslateUniTxt2BTNrtfAndWriteBack();
				else //TSLs
					ret = TranslateUniTxt2TSLrtfAndWriteBack();
			}
			else if(TargetFileFormat == WebPage)
			{
				if(TargetEncoding == ACIP || TargetEncoding == Wylie || TargetEncoding == THDLWylie || TargetEncoding == LCWylie)
					ret = TranslateUniTxt2AWWebpageAndWriteBack();
				else if(TargetEncoding == Bzd || TargetEncoding == Ty || TargetEncoding == NS )  //2007
					ret = TranslateUniTxt2BTNWebpageAndWriteBack();
				else //TSLs
					ret = TranslateUniTxt2TSLWebpageAndWriteBack();
			}
			else if(TargetFileFormat == TXT)
			{
				if(TargetEncoding == ACIP || TargetEncoding == Wylie || TargetEncoding == THDLWylie || TargetEncoding == LCWylie)
					ret = TranslateUniTxt2AWtxtAndWriteBack();
				else if(TargetEncoding == Bzd || TargetEncoding == Ty || TargetEncoding == NS )  //2007
					ret = TranslateUniTxt2BTNtxtAndWriteBack();
				else //FzHg
					ret = TranslateUniTxt2FzHgAndWriteBack();
			}
			else //TargetFileFormat == UnicodeTXT
			{
				if(TargetEncoding == NS )  //2007
					ret = TranslateUniTxt2BTNUniTxtAndWriteBack();//
				else
					ret = TranslateUniTxt2AWUniTxtAndWriteBack();//This is only the possible translater
			}
		}

		if(nFormatBuffer>0) WriteBackFormat();//Write back any format left in the format buffer
	}
	else if(ConversionDirection == OthersToUnicode) //fixed 20181201
	{
		if(TargetFileFormat == RTF) //TargetFileFormat == RTF
		{
			if(SourceFileFormat == RTF)
			{
				if(SourceEncoding == ACIP || SourceEncoding == Wylie || SourceEncoding == THDLWylie || SourceEncoding == LCWylie)
                {
					ret = TranslateAWrtf2UniRtfAndWriteBack();
                }
				else if(SourceEncoding == Bzd || SourceEncoding == Ty || SourceEncoding == NS)  //2007
                {
					ret = TranslateBTNrtf2UniRtfAndWriteBack();
                }
				else //TSLs
                {
					ret = TranslateTSLrtf2UniRtfAndWriteBack();
                }
			}
			else if(SourceFileFormat == WebPage)
			{
				if(SourceEncoding == ACIP || SourceEncoding == Wylie || SourceEncoding == THDLWylie || SourceEncoding == LCWylie)
					ret = TranslateAWWebpage2UniRtfAndWriteBack();
				else if(SourceEncoding == Bzd || SourceEncoding == Ty || SourceEncoding == NS)  //2007
					ret = TranslateBTNWebpage2UniRtfAndWriteBack();
				else //TSLs
					ret = TranslateTSLWebpage2UniRtfAndWriteBack();
			}
			else if(SourceFileFormat == TXT)
			{
				if(SourceEncoding == ACIP || SourceEncoding == Wylie || SourceEncoding == THDLWylie || SourceEncoding == LCWylie)
					ret = TranslateAWtxt2UniRtfAndWriteBack();
				//else if(SourceEncoding == Bzd || SourceEncoding == Ty || SourceEncoding == NS)  //2007
				else if(SourceEncoding == Bzd || SourceEncoding == Ty)  //20080924
					ret = TranslateBTNtxt2UniRtfAndWriteBack();
				else //FzHg
					ret = TranslateFzHg2UniRtfAndWriteBack();
			}
			else //SourceFileFormat == UnicodeTXT
			{
				if(SourceEncoding == NS)
					ret = TranslateBTNUniTxt2UniRtfAndWriteBack();
				else //SourceEncoding == AW
					ret = TranslateAWUniTxt2UniRtfAndWriteBack();
			}
		}
		else if(TargetFileFormat == WebPage) //TargetFileFormat == WebPage
		{
			if(SourceFileFormat == RTF)
			{
				if(SourceEncoding == ACIP || SourceEncoding == Wylie || SourceEncoding == THDLWylie || SourceEncoding == LCWylie)
					ret = TranslateAWrtf2UniWebpageAndWriteBack();
				else if(SourceEncoding == Bzd || SourceEncoding == Ty || SourceEncoding == NS)  //2007
					ret = TranslateBTNrtf2UniWebpageAndWriteBack();
				else //TSLs
					ret = TranslateTSLrtf2UniWebpageAndWriteBack();
			}
			else if(SourceFileFormat == WebPage)
			{
				if(SourceEncoding == ACIP || SourceEncoding == Wylie || SourceEncoding == THDLWylie || SourceEncoding == LCWylie)
					ret = TranslateAWWebpage2UniWebpageAndWriteBack();
				else if(SourceEncoding == Bzd || SourceEncoding == Ty || SourceEncoding == NS)  //2007
					ret = TranslateBTNWebpage2UniWebpageAndWriteBack();
				else //TSLs
					ret = TranslateTSLWebpage2UniWebpageAndWriteBack();
			}
			else if(SourceFileFormat == TXT)
			{
				if(SourceEncoding == ACIP || SourceEncoding == Wylie || SourceEncoding == THDLWylie || SourceEncoding == LCWylie)
					ret = TranslateAWtxt2UniWebpageAndWriteBack();
				else if(SourceEncoding == Bzd || SourceEncoding == Ty || SourceEncoding == NS)  //2007
					ret = TranslateBTNtxt2UniWebpageAndWriteBack();
				else //FzHg
					ret = TranslateFzHg2UniWebpageAndWriteBack();
			}
			else //SourceFileFormat == UnicodeTXT
			{
				if(SourceEncoding == NS)
					ret = TranslateBTNUniTxt2UniWebpageAndWriteBack();
				else //SourceEncoding == AW
					ret = TranslateAWUniTxt2UniWebpageAndWriteBack();
			}
		}
		else //TargetFileFormat == UnicodeTXT
		{
			if(SourceFileFormat == RTF)
			{
				if(SourceEncoding == ACIP || SourceEncoding == Wylie || SourceEncoding == THDLWylie || SourceEncoding == LCWylie)
					ret = TranslateAWrtf2UniTxtAndWriteBack();
				else if(SourceEncoding == Bzd || SourceEncoding == Ty || SourceEncoding == NS)  //2007
					ret = TranslateBTNrtf2UniTxtAndWriteBack();
				else //TSLs
					ret = TranslateTSLrtf2UniTxtAndWriteBack();
			}
			else if(SourceFileFormat == WebPage)
			{
				if(SourceEncoding == ACIP || SourceEncoding == Wylie || SourceEncoding == THDLWylie || SourceEncoding == LCWylie)
					ret = TranslateAWWebpage2UniTxtAndWriteBack();
				else if(SourceEncoding == Bzd || SourceEncoding == Ty || SourceEncoding == NS)  //2007
					ret = TranslateBTNWebpage2UniTxtAndWriteBack();
				else //TSLs
					ret = TranslateTSLWebpage2UniTxtAndWriteBack();
			}
			else if(SourceFileFormat == TXT)
			{
				if(SourceEncoding == ACIP || SourceEncoding == Wylie || SourceEncoding == THDLWylie || SourceEncoding == LCWylie)
					ret = TranslateAWtxt2UniTxtAndWriteBack();
				else if(SourceEncoding == Bzd || SourceEncoding == Ty || SourceEncoding == NS)  //2007
					ret = TranslateBTNtxt2UniTxtAndWriteBack();
				else //FzHg
					ret = TranslateFzHg2UniTxtAndWriteBack();
			}
			else //SourceFileFormat == UnicodeTXT
			{
				if(SourceEncoding == NS)
					ret = TranslateBTNUniTxt2UniTxtAndWriteBack();
				else //SourceEncoding == AW
					ret = TranslateAWUniTxt2UniTxtAndWriteBack();
			}
		}
	}
	else return Exception;

	nTibetan = 0;
	return ret;
}
//-----------------------End Of Translaters------------------------------------




	//////////////////////////////////////////////////////
	//													//
	//		Table Loading Routines						//
	//													//
	//////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//Load the tables for converting from Unicode (both RTF and TXT) to others:
//Fz, Hg, TMW, TM, OldSambhota, NewSambhota, LTibetan
int LoadMappingTableUni2others(void)
{
	int  i, l, ret=0;
	int Majority = 1;//flag to monitor Unicode points
					//1---Central Tibetan Unicode characters
					//0---Non frequent Tibetan character
	int curIndex;
	char temp[50]; int nLevel = 0;

	char tempPath1[1000];

	strcpy(tempPath1, m_UTFCdirectory);
	strcat(tempPath1, Uni2othersMTFNames[TargetEncoding]);

	fTable = fopen(tempPath1, "rb");	//Open the table file //20081124
    if (!fTable) return NoTableFile;

	nAllocatedNodes = 0; //Initialize it. It's global.

	//Loading the table......
	currentNode = TableHead = (BTREE*) malloc(sizeof(BTREE)); //Initialize the table entry.
	if(currentNode == NULL) return NoMemory;

	for(i=0;i<126;i++) currentNode->NextNode[i] = NULL;	//Initialize them

	for(i=0;i<208;i++)//Initialize them
	{
		otherCells[i].theChar = 0x20;
		otherCells[i].FileNumber = -1;
		strcpy (otherCells[i].FzHg, " ");
	}

	while (fscanf(fTable, "%ld", &l)!=EOF)
	{
		if(l>=0xf00 && l<0xfd0) //Tibetan Unicode.
		{
			if(l>=0xf40 && l<=0xfbc) //Central characters
			{
				if(currentNode->NextNode[l-0xf40] == NULL)
				{
					currentNode->NextNode[l-0xf40] = (BTREE*)malloc(sizeof(BTREE));

					if(currentNode->NextNode[l-0xf40] != NULL)
					{
						for(i=0;i<126;i++) currentNode->NextNode[l-0xf40]->NextNode[i] = NULL;//Initialize them

						strcpy(currentNode->NextNode[l-0xf40]->data.FzHg, " ");
						currentNode->NextNode[l-0xf40]->data.FileNumber = 0;
						currentNode->NextNode[l-0xf40]->data.theChar = (char)0x20;

						if(TargetEncoding==LTibetan)//Initialization for LTibetan
						{
							for(i=0;i<5;i++)
							{
								currentNode->NextNode[l-0xf40]->data.level[i].FileNumber = 0;
								currentNode->NextNode[l-0xf40]->data.level[i].theChar = (char)0x20;
							}
						}

						AllocatedNodes[nAllocatedNodes++] = currentNode->NextNode[l-0xf40];//For future delocating
					}
					else //There is no sufficient memory
					{
						//Close the file
						fclose(fTable);
						//Release the memory;
						for(i=0; i<nAllocatedNodes; i++)
						{
							free(AllocatedNodes[i]);
						}
						return NoMemory;
					}
				}
				currentNode = currentNode->NextNode[l-0xf40];
				Majority = 1; //They are central characters
			}
			else //Those non frequent characters
			{
				curIndex = l-0xf00;
				Majority = 0;
			}
		}
		else //The data relates to the Unicode sequence.
		{
			if( TargetEncoding == Fz || TargetEncoding == Hg ||
				TargetEncoding == Bzd || TargetEncoding == NS || //2007
				TargetEncoding == Ty)//Uni to Fz, Hg, Bzd, NS, Ty  2007
			{
				temp[0] =  l>>8;	//get the ASCII code
				temp[1] =  l & 0xFF;
				temp[2] = '\0';

				if(Majority == 1) //The Unicode sequence is about central Tibetan characters
				{
					if(currentNode->data.FzHg[0] == 0x20) //No data already put on here
						strcpy( currentNode->data.FzHg, temp);//Put it on the node
				}
				else //The previous Unicode Tibetan code point is a non-frequent character
				{
					if(otherCells[curIndex].FzHg[0] == 0x20)
						strcpy(otherCells[curIndex].FzHg, temp); //Put the data in the store
				}
				currentNode = TableHead; //reset currentNode
			}
			else if(TargetEncoding == ACIP || TargetEncoding == Wylie || TargetEncoding == THDLWylie || TargetEncoding == LCWylie) //Uni to ACIP, Wylie
			{
				//Not here. It's in other function.
			}
			else if(TargetEncoding == LTibetan)
			{
				temp[0] =  l>>8;	//the char --- High 4 bits
				temp[1] =  (l>>4) & 0x0F;	//the level number --- Low 4 bits
				temp[2] =  l & 0x0F;//The file number --- Lowest 4 bits

				nLevel = (int) temp[1]; //Get the level

				if(Majority == 1) //Central Tibetan characters
				{
					if(currentNode->data.level[nLevel].theChar == 0x20) //No data has been put here before
					{
						currentNode->data.level[nLevel].theChar = temp[0];//Put it on the node
						currentNode->data.level[nLevel].FileNumber = (int) temp[2];//Put it on the node
					}
				}
				else //Non-frequent Tibetan characters
				{
					if(otherCells[curIndex].theChar == 0x20)
					{
						otherCells[curIndex].theChar = temp[0]; //Put the data in the store
						otherCells[curIndex].FileNumber = (int) temp[2];
					}
				}
				currentNode = TableHead; //reset currentNode
			}
			else	//Unicode to rest of encoding
			{
				temp[0] =  l>>8;	//the ASCII --- Higher bits
				temp[1] =  l & 0xFF;//The file number --- Lower bits

				if(Majority == 1) //Central Tibetan characters
				{
					if(currentNode->data.theChar == 0x20) //No data has been put here before
					{
						currentNode->data.theChar = temp[0];//Put it on the node
						currentNode->data.FileNumber = (int) temp[1];//Put it on the node
					}
				}
				else //Non-frequent Tibetan characters
				{
					if(otherCells[curIndex].theChar == 0x20)
					{
						otherCells[curIndex].theChar = temp[0]; //Put the data in the store
						otherCells[curIndex].FileNumber = (int) temp[1];
					}

					if(TargetEncoding == TMW && curIndex>0xf1f-0xf00 && curIndex<0xf2a-0xf00)
					{
						otherCells[curIndex].theChar = temp[0]; //Put the data in the store
						otherCells[curIndex].FileNumber = (int) temp[1];
					}
				}
				currentNode = TableHead; //reset currentNode
			}
		} //End of else //The data relates to the Unicode sequence.
	}
	//Close the file
	fclose(fTable);
	return MappingTableLoaded;
}


//Free the memory
int FreeUni2otherTableMemory(void)
{
	int i;
	//Release the memory;
	for(i=0; i<nAllocatedNodes; i++)
	{
		if(AllocatedNodes[i] != NULL)
			free(AllocatedNodes[i]);
	}
	return MemoryFreed;
}


//Load combination charts for TCRC
int LoadCombinationChartForTCRC(void)
{
	int i,j,n;
	char temp[200];
	char Data[100][20];
	int SecondIndex[50];
	int FirstIndex;

	char tempPath1[1000];
	strcpy(tempPath1, m_UTFCdirectory);
	strcat(tempPath1, "TCRCvowelChart.tbl");

	//Load TCRCvowelChat!
	fTable = fopen(tempPath1, "rb");	//Open the table file //20081124
    if (!fTable) return NoVowelTableFile;

	for(i=0; i<300; i++)//Initializing !!!
	{
		for(j=0;j<6;j++) TCRCvowelChart[i][j] = 0;
	}

	while(fgets(temp, 200, fTable)!=NULL)
	{
		j = 0;
		n = 0;

		for(i=0; temp[i] != '\n'; i++)
		{
			if(temp[i] != 0x09) Data[n][j++] = temp[i];
			else
			{
				Data[n][j] = '\0';
				j = 0;
				n++;
			}//End of else
		}//End of for(i=0; temp[i] != '\n'; i++)

		for(i=1; i<n; i++) TCRCvowelChart[atoi(Data[0])][i] = atoi(Data[i]);

	}//End of while(fgets(temp, 100, fTable)!=NULL)
	fclose(fTable);

	strcpy(tempPath1, m_UTFCdirectory);
	strcat(tempPath1, "TCRCconstantChart.tbl");

	//Load TCRCconstantChat!
	fTable = fopen(tempPath1, "rb");	//Open the table file //20081124
    if (!fTable) return NoVowelTableFile;

	for(i=0; i<50; i++)//Initializing !!!
	{
		for(j=0;j<50;j++)
		{
			TCRCconstantChart[i][j][0] = 0;
			TCRCconstantChart[i][j][1] = 0;
		}
	}

	if(fgets(temp, 200, fTable)!=NULL)//Get the second index
	{
		j = 0;
		n = 0;
		for(i=0; temp[i] != '\n'; i++)
		{
			if(temp[i] != 0x09) Data[n][j++] = temp[i];
			else
			{
				Data[n][j] = '\0';
				j = 0;
				n++;
			}//End of else
		}//End of for(i=0; temp[i] != '\n'; i++)
		for(i=1; i<n; i++) SecondIndex[i] = atoi(Data[i]);
	}
	else return NoVowelTableFile;

	while(fgets(temp, 200, fTable)!=NULL)
	{
		j = 0;
		n = 0;

		for(i=0; temp[i] != '\n'; i++)
		{
			if(temp[i] != 0x09 && temp[i] != ',') Data[n][j++] = temp[i];
			else
			{
				Data[n][j] = '\0';
				j = 0;
				n++;
			}//End of else
		}//End of for(i=0; temp[i] != '\n'; i++)

		FirstIndex = atoi(Data[0]) - 0xf40;

		for(i=1; i<n; i=i+2)
		{
			TCRCconstantChart[FirstIndex][SecondIndex[i/2 + 1]-0xf90][0] = atoi(Data[i]);
			TCRCconstantChart[FirstIndex][SecondIndex[i/2 + 1]-0xf90][1] = atoi(Data[i+1]);
		}
	}//End of while(fgets(temp, 100, fTable)!=NULL)
	fclose(fTable);

	return VowelTableLoaded;
}


//Load the tables for converting from TSL:  TMW, TM, OldSambhota, NewSambhota,
//LTibetan to Unicode
int LoadMappingTableOthers2Unicode(void)
{
	int ch, x, y, l, n=0, j=0, i, index;
	int TheUnicodes[10];
	char temp[100];
	char *p, *p2;//Tempral point;

	char tempPath1[1000];
	strcpy(tempPath1, m_UTFCdirectory);
	strcat(tempPath1, Others2UniMTFNames[SourceEncoding]);

	fTable = fopen(tempPath1, "rb");	//Open the table file //20081124
    if (!fTable) return NoTableFile;

	if(SourceEncoding == TMW)
	{
		//Allocating memory....
		pTable = (char*) malloc(30000);///!!!!!!!!!!!
	}
	else if(SourceEncoding == TM)
	{
		//Allocating memory....
		pTable = (char*)malloc(40000);//!!!!!!!!
	}
	else if(SourceEncoding == NewSambhota)
	{
		//Allocating memory....
		pTable = (char*)malloc(90000);//!!!!!!!!
	}
	else if(SourceEncoding == OldSambhota)
	{
		//Allocating memory....
		pTable =(char*) malloc(50000);//!!!!!!!!
	}
	else if(SourceEncoding == LTibetan)
	{
		//Allocating memory....
		pTable =(char*)malloc(40000);//!!!!!!!!
	}
	else if(SourceEncoding == Fz)
	{
		//Allocating memory....
		pTable = (char*)malloc(135000);//!!!!!!!!
	}
	else if(SourceEncoding == Hg)
	{
		//Allocating memory....
		pTable = (char*)malloc(176000);//!!!!!!!!
	}
	else if(SourceEncoding == TCRCBodYig)
	{
		//Allocating memory....
		pTable =(char*)malloc(40000);//!!!!!!!!
	}
	else if(SourceEncoding == Bzd) //2007
	{
		//Allocating memory....
		pTable = (char*)malloc(170000);//!!!!!!!!
		if(pTable != NULL)//Initialize the table
		{
			p = pTable;//Working on point p.
			for(i=0;i<170000;i++) *p++ = '\0';
		}
		p = pTable;//Working on point p.....
	}
	else if(SourceEncoding == Ty) //2007
	{
		//Allocating memory....
		pTable = (char*)malloc(680000);//!!!!!!!!
		if(pTable != NULL) //Initialize the table
		{
			p = pTable;//Working on point p.
			for(i=0;i<680000;i++) *p++ = '\0';
		}
	}
	else if(SourceEncoding == NS)//National Standard   //2007
	{
		//Allocating memory....
		pTable = (char*)malloc(48000);//!!!!!!!!
		if(pTable != NULL)//Initialize the table
		{
			p = pTable;//Working on point p.
			for(i=0;i<48000;i++) *p++ = '\0';
		}
	}
	else if(SourceEncoding == Jamyang)  //20080924
	{
		//Allocating memory....
		pTable =(char*)malloc(40000);//!!!!!!!!
	}
	else return WrongSourceEncodingOrFileFormat;

	if(pTable == NULL) return NoMemory;

	p = pTable;//Working on point p.....

	if(SourceEncoding == Bzd || SourceEncoding == Ty || SourceEncoding == NS) //2007
	{
		//Loading the table......
		while (fscanf(fTable, "%ld", &l)!=EOF)
		{
			if(l>=0xf00 && l<0xfd0) //Tibetan Unicode.
			{
				TheUnicodes[j++] = l;
			}
			else //The BTN code.
			{
				if (j>6) j = 6; //Keep the boundrary.

				p2 = p;
				for (i=0; i<j; i++) //Put the PREVIOUS Unicode points at p concatenatelly.
				{
					_itoa(TheUnicodes[i], temp, 10);
					strcpy(p2, temp);
					p2 = p2 + 4; //Move the pointer;
				}

				x = l & 0xff;
				y = l >> 8;

				if(SourceEncoding == NS) index = 256*(y-0xF3)+(x-0x00);
				else if(SourceEncoding == Ty) index = 192*(y-0x81)+(x-0x40);
				else index = 95*(y-0xa6)+(x-0xa1); //SourceEncoding == Bzd

				p = pTable + index * 30; //Set the right place for the Unicode points of this code.

				j = 0; //Reset the index for TheUnicodes[].
			}
		}
		//Specially for the LAST code point in the table file.
		if (j>6) j = 6; //Keep the boundrary.
		p2 = p;
		for (i=0; i<j; i++) //Put the previous Unicode points at p concatenatelly.
		{
			_itoa(TheUnicodes[i], temp, 10);
			strcpy(p2, temp);
			p2 = p2 + 4; //Move the pointer;
		}
	}
	else // For other Encodings such as Fz, Hg, LTibetan, ....
	{
		//Loading the table......
		while ((ch=fgetc(fTable))!=EOF)
		{
			if(ch!=0x20) temp[n++]=ch;//the boundry is 0x20
			else
			{
				temp[n] = '\0';
				n = 0;
				strcpy(p, temp);
				p = p + 30;//Max string length of a Unicode sequence of
							//a source code point is 14 bytes
			}
		}
	}

	fclose(fTable);
	return MappingTableLoaded;
}



//Free the memory
int FreeOthers2UniTableMemory(void)
{
	if(pTable != NULL)
		free(pTable);

	return MemoryFreed;
}



//Load the tables for picking up right vowels for TSL:  TMW, TM, OldSambhota, LTibetan
int LoadVowelsForTSLtable(void)
{
	int m, n = 0, i, j, gotCh;
	int theCh, fileNumber;
	int whichVowel = 0;
	char tempCh[10];
	char temp[100];
	char Data[10];
	int *p;//Tempral point;
	int *currentPoint;

	char tempPath1[1000];
	strcpy(tempPath1, m_UTFCdirectory);

	if(TargetEncoding == NewSambhota || TargetEncoding == LTibetan) //They don't need this step!!!
		return VowelTableLoaded;

	if(TargetEncoding == TCRCBodYig  || TargetEncoding == Jamyang) //20080924
		return VowelTableLoaded;

	strcat(tempPath1, VowelsForTSLtableNames[TargetEncoding]);

	fTable = fopen(tempPath1, "rb");	//Open the table file //20081124
    if (!fTable) return NoVowelTableFile;

	//Allocating memory....
	pVowel = (int*) malloc(sizeof(int)*11*256*6); //11*256*6
	p = pVowel;

	for(i=0; i<11*256*6; i++)
	{
		*p = -1;//Initialize the space, very important.
		p++;
	}

	if(pVowel == NULL) return NoMemory;

	while(fgets(temp, 100, fTable)!=NULL)
	{
		j = 0;
		gotCh = 0;
		whichVowel = 0;

		for(i=0; temp[i] != '\n'; i++)
		{
			if(temp[i] != 0x09) Data[j++] = temp[i];
			else
			{
				Data[j] = '\0';
				j = 0;

				if(gotCh==0) //the character
				{
					gotCh = 1;

					for(n=0, m=0; Data[n] != '\0' ;n++)
					{
						if(Data[n]!=',') tempCh[m++] = Data[n];
						else
						{
							tempCh[m] = '\0';
							m = 0;
							fileNumber = atoi(tempCh); //Got the file number
						}
					}
					tempCh[m] = '\0';
					m = 0;
					theCh = atoi(tempCh); //Got the character value

					p = pVowel;

					currentPoint = p + (fileNumber-1)*256*6 + theCh*6; //2004
				}
				else //the vowels
				{
					for(n=0, m=0; Data[n] != '\0' ;n++)
					{
						if(Data[n]!=',') tempCh[m++] = Data[n];
						else
						{
							tempCh[m] = '\0';
							m = 0;
							fileNumber = atoi(tempCh); //Got the file number
						}
					}
					if(m==0){ whichVowel ++; continue;}//Skip the empty space in the table

					whichVowel ++; //Count the vowels

					tempCh[m] = '\0';
					m = 0;
					theCh = atoi(tempCh);

					p = currentPoint;

					*(p + (whichVowel-1)*2) = fileNumber - 1; //Put the data on the memory, file number goes first, and should minus 1

					*(p + (whichVowel-1)*2 + 1) = theCh;
				}
			}//End of first else
		}//End of for(i=0; temp[i] != '\n'; i++)
	}//End of while(fgets(temp, 100, fTable)!=NULL)

	fclose(fTable);
	return VowelTableLoaded;
}



//Free the memory
int FreeVowelsForTSLtableMemory(void)
{
	if(pVowel != NULL)
		free(pVowel);

	return MemoryFreed;
}



//Load Unicode to AW tables
int LoadUnicode2AWtable(void)
{
	int i, j;
	int whichWylie = 0;
	int whichChar = 0;
	char temp[100];
	char Data[10];
	char *p;//Tempral point;

	char tempPath1[1000];

	strcpy(tempPath1, m_UTFCdirectory);
	strcat(tempPath1, "Unicode2AW.tbl");

	fTable = fopen(tempPath1, "rb");	//Open the table file //20081124
    if (!fTable) return NoUnicodeToAWTableFile;

	//Allocating memory....
	pUnicodeToAW = (char*) malloc(208*4*5);
	p = pUnicodeToAW;
	for(i=0; i<208*4*5; i++) *p++ = '\0';//Initialize the space, very important.

	if(pUnicodeToAW == NULL) return NoMemory;

	while(fgets(temp, 100, fTable)!=NULL)
	{
		j = 0;
		whichWylie = 0;

		for(i=0; temp[i] != '\n'; i++)
		{
			if(temp[i] != 0x09) Data[j++] = temp[i];
			else
			{
				Data[j] = '\0';
				j = 0;

				whichWylie ++; //Count Wylie: ACIP--1  Wylie---2  THDLWylie---3   LCWylie---4

				p = pUnicodeToAW;

				strcpy((p + whichChar*4*5 + (whichWylie - 1)*5), Data) ; //Put the data on the memory, file number goes first

			}//End of first else
		}//End of for(i=0; temp[i] != '\n'; i++)

		whichChar ++;

	}//End of while(fgets(temp, 100, fTable)!=NULL)

	fclose(fTable);
	return UnicodeToAWTableLoaded;
}



//Free the memory
int FreeUnicode2AWtableMemory(void)
{
	if(pUnicodeToAW != NULL)
		free(pUnicodeToAW);

	return MemoryFreed;
}



//Load Unicode to AW tables
int LoadAW2UnicodeTable(void)
{
	int i, j;
	int AWorUni = 0; //AW: 0     Uni: 1
	char temp[100];
	char AW[10];
	char TheUnicode[10];
	int *p;//Tempral point;
	char *pTibetanSyllableTemp;
	char **pTSLookupTableTemp;

	char tempPath1[1000];

	strcpy(tempPath1, m_UTFCdirectory);
	strcat(tempPath1, Others2UniMTFNames[SourceEncoding]);

	fTable = fopen(tempPath1, "rb");	//Open the table file //20081124
    if (!fTable) return NoAWToUnicodeTableFile;

	strcpy(tempPath1, m_UTFCdirectory);
	strcat(tempPath1, TibetanSyllableTableFileNames[SourceEncoding]);

	fTibetanSyllableTable = fopen(tempPath1, "rb");	//Open the table file //20081124
    if (!fTibetanSyllableTable) return NoTibetanSyllableTableFile;

	//Allocating memory for AW to Unicode table....
	pAW2Unicode = (int*) malloc(sizeof(int)*130*130*130);
	p = pAW2Unicode;
	for(i=0; i<130*130*130; i++) *p++ = 0;//Initialize the space, very important.

	if(pAW2Unicode == NULL) return NoMemory;

	while(fgets(temp, 100, fTable)!=NULL)
	{
		j = 0;
		AWorUni = 0;

		for(i=0; temp[i] != '\n'; i++)
		{
			if(temp[i] != 0x09)
			{
				if(AWorUni == 0) AW[j++] = temp[i];
				else TheUnicode[j++] = temp[i];
			}
			else
			{
				AW[j] = '\0';
				TheUnicode[j] = '\0';
				j = 0;

				AWorUni = 1;
			}//End of first else
		}//End of for(i=0; temp[i] != '\n'; i++)

		p = pAW2Unicode;

		if(AW[0] != '\0' && TheUnicode[0] != '\0')
		{
			i = strlen(AW);

			if(i == 1)
			{
				if(*(p + MyHashFunction(AW[0])) != 0) continue; //Other data has been already put there. Ignore this data.
				*(p + MyHashFunction(AW[0])) = axtoi(TheUnicode); //Put the data on the memory, file number goes first
			}
			else if(i==2)
			{
				if(*(p + MyHashFunction(AW[0])*130 + MyHashFunction(AW[1])) != 0) continue;//Other data has been already put there. Ignore this data.
				*(p + MyHashFunction(AW[0])*130 + MyHashFunction(AW[1])) = axtoi(TheUnicode);
			}
			else //i=3
			{
				if(*(p + MyHashFunction(AW[0])*130*130 + MyHashFunction(AW[1])*130 + MyHashFunction(AW[2])) != 0) continue;//Other data has been already put there. Ignore this data.
				*(p + MyHashFunction(AW[0])*130*130 + MyHashFunction(AW[1])*130 + MyHashFunction(AW[2])) = axtoi(TheUnicode);
			}
		}
		else continue;

	}//End of while(fgets(temp, 100, fTable)!=NULL)


	//Allocating memory for Tibetan syllable table....
	pTSLookupTable = (char **) malloc(sizeof(char*)*130*130*130);//For Lookup table.
	pTSLookupTableTemp = pTSLookupTable;
	for(i=0; i<130*130*130; i++) *pTSLookupTableTemp++ = NULL;//Initialize the space, very important.


	pTibetanSyllable = (char*) malloc(sizeof(char)*100000);//For Tibetan syllable table
	pTibetanSyllableTemp = pTibetanSyllable;

	if(pTSLookupTable == NULL || pTibetanSyllable == NULL) return NoMemory;

	while(fgets(temp, 100, fTibetanSyllableTable)!=NULL)
	{
		for(i=0; temp[i] != '\n'; i++) pTibetanSyllableTemp[i] = temp[i];
		pTibetanSyllableTemp[i] = '\0';
		temp[i] = '\0';

		pTSLookupTableTemp = pTSLookupTable;
		i = strlen(temp); //Number of characters in the syllable.
		if(i == 1)//One character syllable
		{
			if(*(pTSLookupTableTemp + MyHashFunction(temp[0])*130*130) == NULL) //It's the first syllable
				*(pTSLookupTableTemp + MyHashFunction(temp[0])*130*130) = pTibetanSyllableTemp;
		}
		else if(i==2)//Two character syllable
		{
			if(*(pTSLookupTableTemp + MyHashFunction(temp[0])*130*130 + MyHashFunction(temp[1])*130) == NULL)
				*(pTSLookupTableTemp + MyHashFunction(temp[0])*130*130 + MyHashFunction(temp[1])*130) = pTibetanSyllableTemp;
		}
		else //i>=3
		{
			if(*(pTSLookupTableTemp + MyHashFunction(temp[0])*130*130 + MyHashFunction(temp[1])*130 + MyHashFunction(temp[2])) == NULL)
				*(pTSLookupTableTemp + MyHashFunction(temp[0])*130*130 + MyHashFunction(temp[1])*130 + MyHashFunction(temp[2])) = pTibetanSyllableTemp;
		}

		pTibetanSyllableTemp = pTibetanSyllableTemp + 20;//For next syllable, move to next slot.

	}//End of while()

	fclose(fTable);
	fclose(fTibetanSyllableTable);
	return AWToUnicodeTableLoaded;
}

//Free the memory
int FreeAW2UnicodeTableMemory(void)
{
	if(pAW2Unicode != NULL)
		free(pAW2Unicode);

	if(pTibetanSyllable != NULL)
		free(pTibetanSyllable);

	if(pTSLookupTable != NULL)
		free(pTSLookupTable);

	return MemoryFreed;
}



//Closing files...  //20080918
int CleanUp(void)
{
	if (sourceFile != NULL) fclose(sourceFile);
	if (targetFile != NULL) fclose(targetFile);
	return 0;
}



//Output the messages
void PrintDebugInfo(int ret)
{
	printf("%s\n",DebugInfo[ret]);
}


//////////////////////////////////////////////////////
//													//
//	Routines of Different Conversion Direction		//
//													//
//////////////////////////////////////////////////////


//--------------------------UniRtf to others----------------------


//Converting from Unicode RTF files to RTF files of TMW, TM, Jamyang
//OldSambhota, NewSambhota, LTibetan
int UniRtf2TSLrtf(void)
{
	int ret;

	if(IsThisRTFFile() != ThisIsRtfFile) return NotRtfFile;

	ret = GetDeff();
	if(ret != GotTheDeff && ret != ReachFontTbl) return NoDeff;

	ret = onFonttbl();
	if( ret != FonttblProcessed) return ret;

	ret = LoadMappingTableUni2others();
	if( ret != MappingTableLoaded) return ret;

	ret = LoadVowelsForTSLtable();//Load the vowel table
	if( ret != VowelTableLoaded) return ret;

	if(TargetEncoding == TCRCBodYig)
	{
		ret = LoadCombinationChartForTCRC();//Load vowel table for TCRC
		if( ret != VowelTableLoaded) return ret;
	}

	ret = parseRTF();	//Call Translater in this function

	FreeUni2otherTableMemory();//Free the memory allocated

	FreeVowelsForTSLtableMemory();//Free the memory allocated

	if(ret != EndOfFile) return Exception;

	return ConversionDone;
}



int UniRtf2TSLWebpage(void)
{
	int ret;

	char HtmlHead[] = "<html><head></head><body>";//Very simple HTML file, only focus on TEXT
	char HtmlEnd[] = "</body></html>";

	if(IsThisRTFFile() != ThisIsRtfFile) return NotRtfFile;

	ret = GetDeff();
	if(ret != GotTheDeff && ret != ReachFontTbl) return NoDeff;

	ret = onFonttbl();
	if( ret != FonttblProcessed) return ret;

	ret = LoadMappingTableUni2others();
	if( ret != MappingTableLoaded) return ret;

	ret = LoadVowelsForTSLtable();//Load the vowel table
	if( ret != VowelTableLoaded) return ret;

	if(TargetEncoding == TCRCBodYig)
	{
		ret = LoadCombinationChartForTCRC();//Load vowel table for TCRC
		if( ret != VowelTableLoaded) return ret;
	}

	fputs(HtmlHead, targetFile);

	ret = parseRTF();	//Call Translater in this function

	fputs(HtmlEnd, targetFile);

	FreeUni2otherTableMemory();//Free the memory allocated

	FreeVowelsForTSLtableMemory();//Free the memory allocated

	if(ret != EndOfFile) return Exception;

	return ConversionDone;
}


//Unicode RTF to Fz Hg
int UniRtf2FzHg(void)
{
	int ret;

	if(IsThisRTFFile() != ThisIsRtfFile) return NotRtfFile;

	ret = GetDeff();
	if(ret != GotTheDeff && ret != ReachFontTbl) return NoDeff;

	ret = onFonttbl();
	if( ret != FonttblProcessed) return ret;

	ret = LoadMappingTableUni2others();
	if( ret != MappingTableLoaded) return ret;

	ret = parseRTF();	//Call Translater in this function

	FreeUni2otherTableMemory();//Free the memory allocated

	if(ret != EndOfFile) return Exception;

	return ConversionDone;
}

//Unicode RTF to NS UnicodeTXT
int UniRtf2BTNUniTxt(void)
{
	int ret;

	if(IsThisRTFFile() != ThisIsRtfFile) return NotRtfFile;

	ret = GetDeff();
	if(ret != GotTheDeff && ret != ReachFontTbl) return NoDeff;

	ret = onFonttbl();
	if( ret != FonttblProcessed) return ret;

	ret = LoadMappingTableUni2others();
	if( ret != MappingTableLoaded) return ret;

	fputc(0xFF, targetFile);//Add FFFE to the file, make it as Unicode txt file
	fputc(0xFE, targetFile);

	ret = parseRTF();	//Call Translater in this function

	FreeUni2otherTableMemory();//Free the memory allocated

	if(ret != EndOfFile) return Exception;

	return ConversionDone;
}

//Unicode RTF to BTN TXT   //2007
int UniRtf2BTNtxt(void)
{
	return UniRtf2FzHg();
}

//Unicode RTF to BTN rtf   //2007
int UniRtf2BTNrtf(void)
{
	int ret;

	if(IsThisRTFFile() != ThisIsRtfFile) return NotRtfFile;

	ret = GetDeff();
	if(ret != GotTheDeff && ret != ReachFontTbl) return NoDeff;

	ret = onFonttbl();
	if( ret != FonttblProcessed) return ret;

	ret = LoadMappingTableUni2others();
	if( ret != MappingTableLoaded) return ret;

	ret = parseRTF();	//Call Translater in this function

	FreeUni2otherTableMemory();//Free the memory allocated

	if(ret != EndOfFile) return Exception;

	return ConversionDone;
}

//Unicode RTF to BTN webpage   //2007
int UniRtf2BTNWebpage(void)
{
	int ret;

	char HtmlHead[] = "<html><head></head><body>";//Very simple HTML file, only focus on TEXT
	char HtmlEnd[] = "</body></html>";

	if(IsThisRTFFile() != ThisIsRtfFile) return NotRtfFile;

	ret = GetDeff();
	if(ret != GotTheDeff && ret != ReachFontTbl) return NoDeff;

	ret = onFonttbl();
	if( ret != FonttblProcessed) return ret;

	ret = LoadMappingTableUni2others();
	if( ret != MappingTableLoaded) return ret;

	fputs(HtmlHead, targetFile);

	ret = parseRTF();	//Call Translater in this function

	fputs(HtmlEnd, targetFile);

	FreeUni2otherTableMemory();//Free the memory allocated

	if(ret != EndOfFile) return Exception;

	return ConversionDone;
}

//Converting from Unicode RTF files to RTF files of ACIP and Wylie
int UniRtf2AWrtf(void)
{
	int ret;

	if(IsThisRTFFile() != ThisIsRtfFile) return NotRtfFile;

	ret = GetDeff();
	if(ret != GotTheDeff && ret != ReachFontTbl) return NoDeff;

	ret = onFonttbl();
	if( ret != FonttblProcessed) return ret;

	ret = LoadUnicode2AWtable();
	if( ret != UnicodeToAWTableLoaded) return ret;

	ret = parseRTF();	//Call Translater in this function

	FreeUnicode2AWtableMemory();//Free the memory allocated

	if(ret != EndOfFile) return Exception;

	return ConversionDone;
}



//Converting from Unicode RTF files to RTF files of ACIP and Wylie
int UniRtf2AWtxt(void)
{
	return UniRtf2AWrtf();
}


//Converting from Unicode RTF files to UnicodeTXT files of ACIP and Wylie
int UniRtf2AWUniTxt(void)
{
	int ret;

	if(IsThisRTFFile() != ThisIsRtfFile) return NotRtfFile;

	ret = GetDeff();
	if(ret != GotTheDeff && ret != ReachFontTbl) return NoDeff;

	ret = onFonttbl();
	if( ret != FonttblProcessed) return ret;

	ret = LoadUnicode2AWtable();
	if( ret != UnicodeToAWTableLoaded) return ret;

	fputc(0xFF, targetFile);//Add FFFE to the file, make it as Unicode txt file
	fputc(0xFE, targetFile);

	ret = parseRTF();	//Call Translater in this function

	FreeUnicode2AWtableMemory();//Free the memory allocated

	if(ret != EndOfFile) return Exception;

	return ConversionDone;
}



int UniRtf2AWWebpage(void)
{
	int ret;

	char HtmlHead[] = "<html><head></head><body>";//Very simple HTML file, only focus on TEXT
	char HtmlEnd[] = "</body></html>";

	if(IsThisRTFFile() != ThisIsRtfFile) return NotRtfFile;

	ret = GetDeff();
	if(ret != GotTheDeff && ret != ReachFontTbl) return NoDeff;

	ret = onFonttbl();
	if( ret != FonttblProcessed) return ret;

	ret = LoadUnicode2AWtable();
	if( ret != UnicodeToAWTableLoaded) return ret;

	fputs(HtmlHead, targetFile);

	ret = parseRTF();	//Call Translater in this function

	fputs(HtmlEnd, targetFile);

	FreeUnicode2AWtableMemory();//Free the memory allocated

	if(ret != EndOfFile) return Exception;

	return ConversionDone;
}



//-------------------------------------UniWebPage to others-----------------

//Converting from Unicode RTF files to RTF files of TMW, TM,Jamyang
//OldSambhota, NewSambhota, LTibetan
int UniWebpage2TSLrtf(void)
{
	int ret, i;

	char rtfHead[] = "{\\rtf1\\ansi\\ansicpg1252\\uc0\\deff0";
	char fonttbl[] = "{\\fonttbl{\\f0\\froman\\fcharset0\\fprq2 Times New Roman;}{\\f31 ";

	if(LocateBodyTag() != BodyTagFound) return EndOfFile;

	ret = LoadMappingTableUni2others();
	if( ret != MappingTableLoaded) return ret;

	ret = LoadVowelsForTSLtable();//Load the vowel table
	if( ret != VowelTableLoaded) return ret;

	if(TargetEncoding == TCRCBodYig)
	{
		ret = LoadCombinationChartForTCRC();//Load vowel table for TCRC
		if( ret != VowelTableLoaded) return ret;
	}

	//Converting........
	//SEt up the RTF head..
	fputs(rtfHead, targetFile);//This Unicode RTF file
	fputs(fonttbl, targetFile);
	fputs(ChineseFontName, targetFile);
	fputs(";}", targetFile);

	for(i=0; i< EncodingFontNumber[TargetEncoding]; i++)
	{
		fputs("{\\f", targetFile);
		fprintf(targetFile, "%d", BaseNumber+i);
		fputc(0x20, targetFile);

		fputs(TibetanFontNames[BaseIndex[TargetEncoding] + i], targetFile);
		fputs(";}", targetFile);
	}
	fputs("}\\plain", targetFile);

	ret = parseWEBPAGE();	//Call Translater in this function

	fputc('}', targetFile);//Close the file

	FreeUni2otherTableMemory();//Free the memory allocated

	FreeVowelsForTSLtableMemory();//Free the memory allocated

	if(ret != EndOfFile) return Exception;

	return ConversionDone;
}



int UniWebpage2TSLWebpage(void)
{
	int ret;

	if(LocateBodyTag() != BodyTagFound) return EndOfFile;

	ret = LoadMappingTableUni2others();
	if( ret != MappingTableLoaded) return ret;

	ret = LoadVowelsForTSLtable();//Load the vowel table
	if( ret != VowelTableLoaded) return ret;

	if(TargetEncoding == TCRCBodYig)
	{
		ret = LoadCombinationChartForTCRC();//Load vowel table for TCRC
		if( ret != VowelTableLoaded) return ret;
	}

	ret = parseWEBPAGE();	//Call Translater in this function

	FreeUni2otherTableMemory();//Free the memory allocated

	FreeVowelsForTSLtableMemory();//Free the memory allocated

	if(ret != EndOfFile) return Exception;

	return ConversionDone;
}


int UniWebpage2BTNrtf(void) //2007
{
	int ret, i;

	char rtfHead[] = "{\\rtf1\\ansi\\ansicpg1252\\uc0\\deff0";
	char fonttbl[] = "{\\fonttbl{\\f0\\froman\\fcharset0\\fprq2 Times New Roman;}{\\f31 ";

	if(LocateBodyTag() != BodyTagFound) return EndOfFile;

	ret = LoadMappingTableUni2others();
	if( ret != MappingTableLoaded) return ret;

	//Converting........
	//SEt up the RTF head..
	fputs(rtfHead, targetFile);//This Unicode RTF file
	fputs(fonttbl, targetFile);
	fputs(ChineseFontName, targetFile);
	fputs(";}", targetFile);

	for(i=0; i< EncodingFontNumber[TargetEncoding]; i++) //20090427
	{
		fputs("{\\f", targetFile);
		fprintf(targetFile, "%d", BaseNumber+i);

		fputs("\\fcharset134", targetFile);

		fputc(0x20, targetFile);

		fputs(TibetanFontNames[BaseIndex[TargetEncoding] + i], targetFile);
		fputs(";}", targetFile);
	}
	fputs("}\\plain", targetFile);

	ret = parseWEBPAGE();	//Call Translater in this function

	fputc('}', targetFile);//Close the file

	FreeUni2otherTableMemory();//Free the memory allocated

	if(ret != EndOfFile) return Exception;

	return ConversionDone;
}


int UniWebpage2BTNWebpage(void) //2007
{
	int ret;

	if(LocateBodyTag() != BodyTagFound) return EndOfFile;

	ret = LoadMappingTableUni2others();
	if( ret != MappingTableLoaded) return ret;

	ret = parseWEBPAGE();	//Call Translater in this function

	FreeUni2otherTableMemory();//Free the memory allocated

	if(ret != EndOfFile) return Exception;

	return ConversionDone;
}


int UniWebpage2BTNUniTxt(void) //2007
{
	int ret;

	if(LocateBodyTag() != BodyTagFound) return EndOfFile;

	ret = LoadMappingTableUni2others();
	if( ret != MappingTableLoaded) return ret;

	fputc(0xFF, targetFile);//Add FFFE to the file, make it as Unicode txt file
	fputc(0xFE, targetFile);

	ret = parseWEBPAGE();	//Call Translater in this function

	FreeUni2otherTableMemory();//Free the memory allocated

	if(ret != EndOfFile) return Exception;

	return ConversionDone;
}


//Unicode webpage to Fz Hg
int UniWebpage2FzHg()
{
	int ret;

	if(LocateBodyTag() != BodyTagFound) return EndOfFile;

	ret = LoadMappingTableUni2others();
	if( ret != MappingTableLoaded) return ret;

	ret = parseWEBPAGE();	//Call Translater in this function

	FreeUni2otherTableMemory();//Free the memory allocated

	if(ret != EndOfFile) return Exception;

	return ConversionDone;
}


//Unicode webpage to BTN txt //2007
int UniWebpage2BTNtxt()
{
	return UniWebpage2FzHg();
}


//Converting from Unicode webpage files to RTF files of ACIP and Wylie
int UniWebpage2AWrtf(void)
{
	int ret, i;

	char rtfHead[] = "{\\rtf1\\ansi\\ansicpg1252\\uc0\\deff0";
	char fonttbl[] = "{\\fonttbl{\\f0\\froman\\fcharset0\\fprq2 Times New Roman;}{\\f31 ";

	if(LocateBodyTag() != BodyTagFound) return EndOfFile;

	ret = LoadUnicode2AWtable();
	if( ret != UnicodeToAWTableLoaded) return ret;

	//Converting........
	//SEt up the RTF head..
	fputs(rtfHead, targetFile);//This Unicode RTF file
	fputs(fonttbl, targetFile);
	fputs(ChineseFontName, targetFile);
	fputs(";}", targetFile);

	for(i=0; i< EncodingFontNumber[TargetEncoding]; i++)
	{
		fputs("{\\f", targetFile);
		fprintf(targetFile, "%d", BaseNumber+i);
		fputc(0x20, targetFile);

		fputs(TibetanFontNames[BaseIndex[TargetEncoding] + i], targetFile);
		fputs(";}", targetFile);
	}
	fputs("}\\plain", targetFile);

	ret = parseWEBPAGE();	//Call Translater in this function

	fputc('}', targetFile);//Close the file

	FreeUnicode2AWtableMemory();//Free the memory allocated

	if(ret != EndOfFile) return Exception;

	return ConversionDone;
}



//Converting from Unicode webpage files to RTF files of ACIP and Wylie
int UniWebpage2AWtxt(void)
{
	int ret;

	if(LocateBodyTag() != BodyTagFound) return EndOfFile;

	ret = LoadUnicode2AWtable();
	if( ret != UnicodeToAWTableLoaded) return ret;

	ret = parseWEBPAGE();	//Call Translater in this function

	FreeUnicode2AWtableMemory();//Free the memory allocated

	if(ret != EndOfFile) return Exception;

	return ConversionDone;
}



//Converting from Unicode webpage files to UnicodeTXT files of ACIP and Wylie
int UniWebpage2AWUniTxt(void)
{
	int ret;

	if(LocateBodyTag() != BodyTagFound) return EndOfFile;

	ret = LoadUnicode2AWtable();
	if( ret != UnicodeToAWTableLoaded) return ret;

	fputc(0xFF, targetFile);//Add FFFE to the file, make it as Unicode txt file
	fputc(0xFE, targetFile);

	ret = parseWEBPAGE();	//Call Translater in this function

	FreeUnicode2AWtableMemory();//Free the memory allocated

	if(ret != EndOfFile) return Exception;

	return ConversionDone;
}



int UniWebpage2AWWebpage(void)
{
	int ret;

	if(LocateBodyTag() != BodyTagFound) return EndOfFile;

	ret = LoadUnicode2AWtable();
	if( ret != UnicodeToAWTableLoaded) return ret;

	ret = parseWEBPAGE();

	FreeUnicode2AWtableMemory();//Free the memory allocated

	if(ret != EndOfFile) return Exception;

	return ConversionDone;
}


//---------------------------------UniTxt to others---------------------
//Unicode TXT to AW
int UniTxt2AW()
{
	int j=0, l;
	int ch1, ch2;

	nTibetan = 0;
	nLatin = 0;
	nChinese = 0;

	//Start to translate.......
#ifdef Unix
	while( ((ch2 = getc(sourceFile)) != EOF) //20081124    //20090427
		&& ((ch1 = getc(sourceFile)) != EOF))
	{
		l = (ch1<<8) + ch2; //20081124
#endif
#ifdef Windows
	while( (l = getwc(sourceFile)) != WEOF) //20081124    //20090427
	{
#endif
#ifdef Mac // 20180304
	while( ((ch2 = getc(sourceFile)) != EOF) //20081124    //20090427
		&& ((ch1 = getc(sourceFile)) != EOF))
	{
		l = (ch1<<8) + ch2; //20081124
#endif

		if(l==0xf0c) l = 0xf0b;//20080229

		if(l>=0xf00 && l<=0xfd0) //Tibetan
		{
			if(nLatin>0) WriteBackLatin();
			if(nChinese>0) WriteBackChinese();

			if(nTibetan >= SZBuffer)
				TranslateUnicode2AWAndWriteBack();

			TibetanBuffer[nTibetan ++].ch = l;
		}
		else //Non-Tibetan letter appears, it's a boundry
		{
			if(nTibetan > 0)
				TranslateUnicode2AWAndWriteBack();

			if(l>=0 && l<=0xff)//ASCII character
			{
				if(nChinese>0) WriteBackChinese();

				if(nLatin >= SZBuffer) WriteBackLatin();
				LatinBuffer[nLatin++] = l;

				if((l == '\n' || l == 0x0d) && TargetFileFormat == WebPage) fputc(l, targetFile); //20081124
			}
			else //Chinese or other languages
			{
				if(nLatin>0) WriteBackLatin();

				if(nChinese >= SZBuffer) WriteBackChinese();
				ChineseBuffer[nChinese++] = l;
			}
		}
	}//end of while()
	if (nTibetan > 0) TranslateUnicode2AWAndWriteBack();
	if(nLatin>0) WriteBackLatin();
	if(nChinese>0) WriteBackChinese();

	return EndOfFile;
}


//Unicode TXT to ACIP and Wylie TXT
int UniTxt2AWtxt()
{
	int ret;

	//Loading mapping table.....
	ret = LoadUnicode2AWtable();
	if( ret != UnicodeToAWTableLoaded) return ret;

	//Converting......
	fgetc(sourceFile);//Take off FFFE from the Unicode plain text
	fgetc(sourceFile);

	ret = UniTxt2AW();

	FreeUnicode2AWtableMemory();//Free the memory allocated

	if(ret != EndOfFile) return Exception;

	return ConversionDone;
}



//Unicode TXT to ACIP and Wylie RTF
int UniTxt2AWrtf()
{
	int ret, i;

	char rtfHead[] = "{\\rtf1\\ansi\\ansicpg1252\\uc0\\deff0";
	char fonttbl[] = "{\\fonttbl{\\f0\\froman\\fcharset0\\fprq2 Times New Roman;}{\\f31 ";

	//Loading mapping table.....
	ret = LoadUnicode2AWtable();
	if( ret != UnicodeToAWTableLoaded) return ret;

	//Converting......
	fgetc(sourceFile);//Take off FFFE from the Unicode plain text
	fgetc(sourceFile);

	fputs(rtfHead, targetFile);//This Unicode RTF file
	fputs(fonttbl, targetFile);
	fputs(ChineseFontName, targetFile);
	fputs(";}", targetFile);

	for(i=0; i< EncodingFontNumber[TargetEncoding]; i++)
	{
		fputs("{\\f", targetFile);
		fprintf(targetFile, "%d", BaseNumber+i);
		fputc(0x20, targetFile);

		fputs(TibetanFontNames[BaseIndex[TargetEncoding] + i], targetFile);
		fputs(";}", targetFile);
	}
	fputs("}\\plain", targetFile);


	ret = UniTxt2AW();

	fputs("}", targetFile);

	FreeUnicode2AWtableMemory();//Free the memory allocated

	if(ret != EndOfFile) return Exception;

	return ConversionDone;
}


//Unicode TXT to ACIP and Wylie Webpage
int UniTxt2AWWebpage()
{
	int ret;

	char HtmlHead[] = "<html><head></head><body>";//Very simple HTML file, only focus on TEXT
	char HtmlEnd[] = "</body></html>";

	//Loading mapping table.....
	ret = LoadUnicode2AWtable();
	if( ret != UnicodeToAWTableLoaded) return ret;

	//Converting......
	fgetc(sourceFile);//Take off FFFE from the Unicode plain text
	fgetc(sourceFile);

	fputs(HtmlHead, targetFile);

	ret = UniTxt2AW();

	fputs(HtmlEnd, targetFile);

	FreeUnicode2AWtableMemory();//Free the memory allocated

	if(ret != EndOfFile) return Exception;

	return ConversionDone;
}

//Unicode TXT to ACIP and Wylie RTF
int UniTxt2AWUniTxt()
{
	int ret;
	int ch;

	//Loading mapping table.....
	ret = LoadUnicode2AWtable();
	if( ret != UnicodeToAWTableLoaded) return ret;

	//Converting......
	ch = fgetc(sourceFile);//Take off FFFE from the Unicode plain text, and put it back to taget UniTxt file
	fputc(ch, targetFile);
	ch = fgetc(sourceFile);
	fputc(ch, targetFile);

	ret = UniTxt2AW();

	FreeUnicode2AWtableMemory();//Free the memory allocated

	if(ret != EndOfFile) return Exception;

	return ConversionDone;
}



//From Unicode TXT to TSL
int UniTxt2TSL()
{
	int j=0, l;
	int ch1, ch2;

	nTibetan = 0;
	nLatin = 0;
	nChinese = 0;

	//Start to translate.......

#ifdef Unix		//20090427
	while( ((ch2 = getc(sourceFile)) != EOF)
		&& ((ch1 = getc(sourceFile)) != EOF))
	{
		l = (ch1<<8) + ch2;
#endif
#ifdef Windows
	while( (l = getwc(sourceFile)) != WEOF) //20081124
	{
#endif			//20090427
#ifdef Mac		//20180304
	while( ((ch2 = getc(sourceFile)) != EOF)
		&& ((ch1 = getc(sourceFile)) != EOF))
	{
		l = (ch1<<8) + ch2;
#endif

		if(l==0xf0c) l = 0xf0b;//20080229

		if(l>=0xf00 && l<0xfd0) //Tibetan
		{
			if(nLatin>0) WriteBackLatin();
			if(nChinese>0) WriteBackChinese();

			if(nTibetan == SZBuffer)
				TranslateUnicode2TSLAndWriteBack();

			TibetanBuffer[nTibetan ++].ch = l;
		}
		else //Non-Tibetan letter appears, it's a boundry
		{
			if(nTibetan > 0)
				TranslateUnicode2TSLAndWriteBack();

			if(l>=0 && l<=0xff)//ASCII character
			{
				if(nChinese>0) WriteBackChinese();

				if(nLatin >= SZBuffer) WriteBackLatin();
				LatinBuffer[nLatin++] = l;
				if((l == '\n' || l == 0x0d) && (TargetFileFormat == WebPage || TargetFileFormat == RTF)) fputc(l, targetFile);	//20081124	//20090325
			}
			else //Chinese or other languages
			{
				if(nLatin>0) WriteBackLatin();

				if(nChinese >= SZBuffer) WriteBackChinese();
				ChineseBuffer[nChinese++] = l;
			}
		}
	}//end of while()
	if(nTibetan > 0)TranslateUnicode2TSLAndWriteBack();
	if(nLatin>0) WriteBackLatin();
	if(nChinese>0) WriteBackChinese();

	return EndOfFile;

}


int UniTxt2TSLrtf()	//Unicode TXT to other RTF
{
	int ret, i;

	char rtfHead[] = "{\\rtf1\\ansi\\ansicpg1252\\uc0\\deff0";
	char fonttbl[] = "{\\fonttbl{\\f0\\froman\\fcharset0\\fprq2 Times New Roman;}{\\f31 ";

	//Loading mapping table.....
	ret = LoadMappingTableUni2others();
	if( ret != MappingTableLoaded) return ret;

	ret = LoadVowelsForTSLtable();//Load the vowel table
	if( ret != VowelTableLoaded) return ret;

	if(TargetEncoding == TCRCBodYig)
	{
		ret = LoadCombinationChartForTCRC();//Load vowel table for TCRC
		if( ret != VowelTableLoaded) return ret;
	}

	fputs(rtfHead, targetFile);//This Unicode RTF file
	fputs(fonttbl, targetFile);
	fputs(ChineseFontName, targetFile);
	fputs(";}", targetFile);

	for(i=0; i< EncodingFontNumber[TargetEncoding]; i++)
	{
		fputs("{\\f", targetFile);
		fprintf(targetFile, "%d", BaseNumber+i);
		fputc(0x20, targetFile);

		fputs(TibetanFontNames[BaseIndex[TargetEncoding] + i], targetFile);
		fputs(";}", targetFile);
	}
	fputs("}\\plain", targetFile);

	//Converting......
	fgetc(sourceFile);//Take off FFFE from the Unicode plain text,
	fgetc(sourceFile);

	ret = UniTxt2TSL();

	fputs("}", targetFile);

	FreeUni2otherTableMemory();//Free the memory allocated

	FreeVowelsForTSLtableMemory();//Free the memory allocated

	if(ret != EndOfFile) return Exception;

	return ConversionDone;
}



int UniTxt2TSLWebpage()	//Use the same routine
{
	int ret;

	char HtmlHead[] = "<html><head></head><body>";//Very simple HTML file, only focus on TEXT
	char HtmlEnd[] = "</body></html>";

	//Loading mapping table.....
	ret = LoadMappingTableUni2others();
	if( ret != MappingTableLoaded) return ret;

	ret = LoadVowelsForTSLtable();//Load the vowel table
	if( ret != VowelTableLoaded) return ret;

	if(TargetEncoding == TCRCBodYig)
	{
		ret = LoadCombinationChartForTCRC();//Load vowel table for TCRC
		if( ret != VowelTableLoaded) return ret;
	}

	//Converting......
	fgetc(sourceFile);//Take off FFFE from the Unicode plain text,
	fgetc(sourceFile);

	fputs(HtmlHead, targetFile);

	ret = UniTxt2TSL();

	fputs(HtmlEnd, targetFile);

	FreeUni2otherTableMemory();//Free the memory allocated

	FreeVowelsForTSLtableMemory();//Free the memory allocated

	if(ret != EndOfFile) return Exception;

	return ConversionDone;
}


//Unicode TXT to FzHg
int UniTxt2FzHg()
{
	int ret;

	//Loading mapping table.....
	ret = LoadMappingTableUni2others();
	if( ret != MappingTableLoaded) return ret;

	//Converting......
	fgetc(sourceFile);//Take off FFFE from the Unicode plain text
	fgetc(sourceFile);

	ret = TranslateAndWriteBack();//Take care to this///////////////////////////should do big job!

	FreeUni2otherTableMemory();//Free the memory allocated

	if(ret != EndOfFile) return Exception;

	return ConversionDone;
}


//From Unicode TXT to BTN  //2007
int UniTxt2BTN()
{
	int j=0, l;
	int ch1, ch2;

	nTibetan = 0;
	nLatin = 0;
	nChinese = 0;

	//Start to translate.......

#ifdef Unix		//20090427
	while( ((ch2 = getc(sourceFile)) != EOF)
		&& ((ch1 = getc(sourceFile)) != EOF))
	{
		l = (ch1<<8) + ch2;
#endif
#ifdef Windows
	while( (l = getwc(sourceFile)) != WEOF) //20081124
	{
#endif			//20090427
#ifdef Mac		//20180304
	while( ((ch2 = getc(sourceFile)) != EOF)
		&& ((ch1 = getc(sourceFile)) != EOF))
	{
		l = (ch1<<8) + ch2;
#endif

		if(l==0xf0c) l = 0xf0b;//20080229

		if(l>=0xf00 && l<0xfd0) //Tibetan
		{
			if(nLatin>0) WriteBackLatin();
			if(nChinese>0) WriteBackChinese();

			if(nTibetan == SZBuffer)
				TranslateAndWriteBack();

			TibetanBuffer[nTibetan ++].ch = l;
		}
		else //Non-Tibetan letter appears, it's a boundry
		{
			if(nTibetan > 0)
				TranslateAndWriteBack();

			if(l>=0 && l<=0xff)//ASCII character
			{
				if(nChinese>0) WriteBackChinese();

				if(nLatin >= SZBuffer) WriteBackLatin();
				LatinBuffer[nLatin++] = l;
				if((l == '\n' || l == 0x0d) && (TargetFileFormat == WebPage || TargetFileFormat == RTF)) fputc(l, targetFile);	//20081124	//20090325
			}
			else //Chinese or other languages
			{
				if(nLatin>0) WriteBackLatin();

				if(nChinese >= SZBuffer) WriteBackChinese();
				ChineseBuffer[nChinese++] = l;
			}
		}
	}//end of while()
	if(nTibetan>0) TranslateAndWriteBack();
	if(nLatin>0) WriteBackLatin();
	if(nChinese>0) WriteBackChinese();

	return EndOfFile;
}

//Unicode TXT to Bzd, Ty, NS TXT //2007
int UniTxt2BTNtxt()
{
	int ret;

	//Loading mapping table.....
	ret = LoadMappingTableUni2others();
	if( ret != MappingTableLoaded) return ret;

	//Converting......
	fgetc(sourceFile);//Take off FFFE from the Unicode plain text
	fgetc(sourceFile);

	ret = UniTxt2BTN();//Take care to this///////////////////////////should do big job!

	FreeUni2otherTableMemory();//Free the memory allocated

	if(ret != EndOfFile) return Exception;

	return ConversionDone;
}


//Unicode TXT to NS UniTXT //2007
int UniTxt2BTNUniTxt()
{
	int ret;
	int ch;

	//Loading mapping table.....
	ret = LoadMappingTableUni2others();
	if( ret != MappingTableLoaded) return ret;

	//Converting......
	ch = fgetc(sourceFile);//Take off FFFE from the Unicode plain text, and put it back to taget UniTxt file
	fputc(ch, targetFile);
	ch = fgetc(sourceFile);
	fputc(ch, targetFile);

	ret = UniTxt2BTN();//Take care to this///////////////////////////should do big job!

	FreeUni2otherTableMemory();//Free the memory allocated

	if(ret != EndOfFile) return Exception;

	return ConversionDone;
}


int UniTxt2BTNrtf()	//Unicode TXT to BTN RTF  //2007
{
	int ret, i;

	char rtfHead[] = "{\\rtf1\\ansi\\ansicpg1252\\uc0\\deff0";
	char fonttbl[] = "{\\fonttbl{\\f0\\froman\\fcharset0\\fprq2 Times New Roman;}{\\f31 ";

	//Loading mapping table.....
	ret = LoadMappingTableUni2others();
	if( ret != MappingTableLoaded) return ret;

	fputs(rtfHead, targetFile);//This Unicode RTF file
	fputs(fonttbl, targetFile);
	fputs(ChineseFontName, targetFile);
	fputs(";}", targetFile);

	for(i=0; i< EncodingFontNumber[TargetEncoding]; i++) //20090427
	{
		fputs("{\\f", targetFile);
		fprintf(targetFile, "%d", BaseNumber+i);

		fputs("\\fcharset134", targetFile);

		fputc(0x20, targetFile);

		fputs(TibetanFontNames[BaseIndex[TargetEncoding] + i], targetFile);
		fputs(";}", targetFile);
	}
	fputs("}\\plain", targetFile);

	//Converting......
	fgetc(sourceFile);//Take off FFFE from the Unicode plain text,
	fgetc(sourceFile);

	ret = UniTxt2BTN();

	fputs("}", targetFile);

	FreeUni2otherTableMemory();//Free the memory allocated

	if(ret != EndOfFile) return Exception;

	return ConversionDone;
}

int UniTxt2BTNWebpage()	//Use the same routine   //2007
{
	int ret;

	char HtmlHead[] = "<html><head></head><body>";//Very simple HTML file, only focus on TEXT
	char HtmlEnd[] = "</body></html>";

	//Loading mapping table.....
	ret = LoadMappingTableUni2others();
	if( ret != MappingTableLoaded) return ret;

	//Converting......
	fgetc(sourceFile);//Take off FFFE from the Unicode plain text,
	fgetc(sourceFile);

	fputs(HtmlHead, targetFile);

	ret = UniTxt2BTN();

	fputs(HtmlEnd, targetFile);

	FreeUni2otherTableMemory();//Free the memory allocated

	if(ret != EndOfFile) return Exception;

	return ConversionDone;
}


//-----------------------------------TSL RTF to Unicode------------------------------

//TSL to Unicode
int TSLrtf2UniRtf()
{
	int ret;

	if(IsThisRTFFile() != ThisIsRtfFile) return NotRtfFile;

	ret = GetDeff();
	if(ret != GotTheDeff && ret != ReachFontTbl) return NoDeff;
    //fprintf(targetFile,"head table1\n"); //debuging
	ret = onFonttbl();
	if( ret != FonttblProcessed) return ret;
    //fprintf(targetFile,"head table2\n"); //debuging

	ret = LoadMappingTableOthers2Unicode();
	if( ret != MappingTableLoaded) return ret;

	fputs("\\ApplyBrkRules ", targetFile);//20100307

	ret = parseRTF();

	FreeOthers2UniTableMemory();//Free the memory allocated

	if(ret != EndOfFile) return Exception;

	return ConversionDone;
}



//TSL to Unicode TXT
int TSLrtf2UniTxt()
{
	int ret;

	if(IsThisRTFFile() != ThisIsRtfFile) return NotRtfFile;

	ret = GetDeff();
	if(ret != GotTheDeff && ret != ReachFontTbl) return NoDeff;

	ret = onFonttbl();
	if( ret != FonttblProcessed) return ret;

	ret = LoadMappingTableOthers2Unicode();
	if( ret != MappingTableLoaded) return ret;

	fputc(0xFF, targetFile);//Add FFFE to the file
	fputc(0xFE, targetFile);

	ret = parseRTF();

	FreeOthers2UniTableMemory();//Free the memory allocated

	if(ret != EndOfFile) return Exception;

	return ConversionDone;
}




//TSL to Unicode Webpage
int TSLrtf2UniWebpage()
{

	int ret;

	char HtmlHead[] = "<html><head></head><body>";//Very simple HTML file, only focus on TEXT
	char HtmlEnd[] = "</body></html>";

	if(IsThisRTFFile() != ThisIsRtfFile) return NotRtfFile;

	ret = GetDeff();
	if(ret != GotTheDeff && ret != ReachFontTbl) return NoDeff;

	ret = onFonttbl();
	if( ret != FonttblProcessed) return ret;

	ret = LoadMappingTableOthers2Unicode();
	if( ret != MappingTableLoaded) return ret;

	fputs(HtmlHead, targetFile);

	ret = parseRTF();

	fputs(HtmlEnd, targetFile);

	FreeOthers2UniTableMemory();//Free the memory allocated

	if(ret != EndOfFile) return Exception;

	return ConversionDone;
}

//--------------------------------------TSL WebPage to Unicode-----------------

//TSL webpage to Unicode
int TSLWebpage2UniRtf()
{
	int ret;

	char rtfHead[] = "{\\rtf1\\ansi\\ansicpg1252\\uc0\\deff0";
	char fonttbl[] = "{\\fonttbl{\\f0\\froman\\fcharset0\\fprq2 Times New Roman;}{\\f31 ";

	//Converting........
	//SEt up the RTF head..
	fputs(rtfHead, targetFile);//This Unicode RTF file
	fputs(fonttbl, targetFile);
	fputs(ChineseFontName, targetFile);
	fputs(";}{\\f30\\fnil\\fcharset0\\fprq2 ", targetFile);

	fputs(TibetanUnicodeFontName, targetFile);
	fputs(";}}\\plain", targetFile);

	if(LocateBodyTag() != BodyTagFound) return EndOfFile;

	ret = LoadMappingTableOthers2Unicode();
	if( ret != MappingTableLoaded) return ret;

	ret = parseWEBPAGE();	//Call Translater in this function

	fputc('}', targetFile);//Close the file

	FreeOthers2UniTableMemory();//Free the memory allocated

	if(ret != EndOfFile) return Exception;

	return ConversionDone;
}



//TSL Webpageto Unicode TXT
int TSLWebpage2UniTxt()
{
	int ret;

	if(LocateBodyTag() != BodyTagFound) return EndOfFile;

	ret = LoadMappingTableOthers2Unicode();
	if( ret != MappingTableLoaded) return ret;

	fputc(0xFF, targetFile);//Add FFFE to the file
	fputc(0xFE, targetFile);

	ret = parseWEBPAGE();	//Call Translater in this function

	FreeOthers2UniTableMemory();//Free the memory allocated

	if(ret != EndOfFile) return Exception;

	return ConversionDone;

}



//TSL Webpage to Unicode Webpage
int TSLWebpage2UniWebpage()
{
	int ret;

	if(LocateBodyTag() != BodyTagFound) return EndOfFile;

	ret = LoadMappingTableOthers2Unicode();
	if( ret != MappingTableLoaded) return ret;

	ret = parseWEBPAGE();	//Call Translater in this function

	FreeOthers2UniTableMemory();//Free the memory allocated

	if(ret != EndOfFile) return Exception;

	return ConversionDone;
}


//-----------------------------FzHg to Unicode------------------------------------------
//FzHg to Unicode
int FzHg2Unicode()
{
	int ch, j=0, thecode, instruction = 0;
	int l;
	int HoldOnLatinBuffer = No;

	nTibetan = 0;
	nChinese = 0;
	nLatin = 0;

	while((ch = getc(sourceFile)) != EOF)
	{
		if((j==0)&&(ch<=0x7f))//Latin   //20090204
		{

if(nTibetan>0) TranslateAndWriteBack();//Take off this later!!!!!!!!!!!Beijing???????????????????????????????????????????

			if(nTibetan > 0 && nLatin == 0 && nChinese == 0 && (ch == 0x0d || ch == '\n'))//Tibetans followed by a linebreaker.
			{
				if(SourceEncoding == Hg)//Hg
				{
					if(TibetanBuffer[nTibetan-1].ch == 0xb670 || TibetanBuffer[nTibetan-1].ch == 0xb66f)//Delimters
						HoldOnLatinBuffer = No;
					else HoldOnLatinBuffer = Yes;
				}
				else //Fz
				{
					if(TibetanBuffer[nTibetan-1].ch == 0xc032 || TibetanBuffer[nTibetan-1].ch == 0xc031)//Delimters
						HoldOnLatinBuffer = No;
					else HoldOnLatinBuffer = Yes;
				}
			}
			else//Other Latin characters and ....
			{
				if(ch != 0x0a && HoldOnLatinBuffer == Yes) HoldOnLatinBuffer = No;//LB is followed by a Latin character
			}

			if(nChinese>0) WriteBackChinese();

			if(nTibetan>0 && HoldOnLatinBuffer == No) TranslateAndWriteBack();//Don't translate when the LatinBuffer is held on.

			if(nLatin >= SZBuffer) WriteBackLatin();
//			if(ch != 0x0a) LatinBuffer[nLatin++] = ch;//Take off 0x0a
			LatinBuffer[nLatin++] = ch;
			j = 0;

		}
		else if((j==0)&&(ch>0x7f)) { thecode=ch; j=1; continue; }   //20090204

		else if( (j==1)  && ( ((SourceEncoding == Fz) && (thecode>=0xc0) ) || ((SourceEncoding == Hg) && (thecode>=0xb0))) && (ch>0x20) && (ch<0x7f)  ) //Tibetan   //20090204  //20090918
		{
			if(thecode == 0xb7 && ch == 0x3c)
			{
				if(nChinese>0) WriteBackChinese();
				if(nTibetan>0) TranslateAndWriteBack();

				if(nLatin >= SZBuffer) WriteBackLatin();
				LatinBuffer[nLatin++] = 0x3c;
				if(nLatin >= SZBuffer) WriteBackLatin();
				LatinBuffer[nLatin++] = 0x3c;

				if(HoldOnLatinBuffer == Yes) HoldOnLatinBuffer = No;
			}
			else if(thecode == 0xb7 && ch == 0x3d)
			{
				if(nChinese>0) WriteBackChinese();
				if(nTibetan>0) TranslateAndWriteBack();

				if(nLatin >= SZBuffer) WriteBackLatin();
				LatinBuffer[nLatin++] = 0x3e;
				if(nLatin >= SZBuffer) WriteBackLatin();
				LatinBuffer[nLatin++] = 0x3e;

				if(HoldOnLatinBuffer == Yes) HoldOnLatinBuffer = No;
			}
			else
			{
				if(nLatin>0 && HoldOnLatinBuffer == No) WriteBackLatin();
				if(nChinese>0) WriteBackChinese();

				if(SourceEncoding == Fz) //20090615
				{
					if(thecode < 0xc0 ) { j = 0; continue;} //Out of the mapping table scope: 0xC021 to 0xEE25
				}

				l = (thecode << 8) + ch;

				if(nTibetan >= SZBuffer) TranslateAndWriteBack();
				TibetanBuffer[nTibetan ++].ch = l;
			}
			j = 0;

			if(nTibetan > 0 && HoldOnLatinBuffer == Yes)//Latin buffer is held
			{
				if(SourceEncoding == Hg)//Hg
				{
					if(TibetanBuffer[nTibetan-1].ch == 0xb670 || TibetanBuffer[nTibetan-1].ch == 0xb66f)//Delimters
					{
						TranslateAndWriteBack();
						HoldOnLatinBuffer = No;
						if(nLatin>0) WriteBackLatin();
					}
				}
				else //Fz
				{
					if(TibetanBuffer[nTibetan-1].ch == 0xc032 || TibetanBuffer[nTibetan-1].ch == 0xc031)//Delimters
					{
						TranslateAndWriteBack();
						HoldOnLatinBuffer = No;
						if(nLatin>0) WriteBackLatin();
					}
				}
			}
		}
		else //all are supposed to be chinese character,    /20090918
		{
			if(nTibetan>0) TranslateAndWriteBack();
			if(nLatin>0) WriteBackLatin();

			l = ((int)thecode << 8) + (int)ch;
			if(nChinese >= SZBuffer) WriteBackChinese();
			ChineseBuffer[nChinese++] = l;

			if(HoldOnLatinBuffer == Yes) HoldOnLatinBuffer = No;//Release the LatinBuffer
			j=0;
		}
	}//End of while()
	if(nChinese>0) WriteBackChinese();
	if(nTibetan>0) TranslateAndWriteBack();
	if(nLatin>0) WriteBackLatin();

	return EndOfFile;
}



//Converting from FzHg file to Unicode RTF format
int FzHg2UniRtf()//Fz Hg to Unicode RTF
{
	int ret;

	char rtfHead[] = "{\\rtf1\\ansi\\ansicpg1252\\uc0\\deff0";
	char fonttbl[] = "{\\fonttbl{\\f0\\froman\\fcharset0\\fprq2 Times New Roman;}{\\f31 ";

	//Load the table
	ret = LoadMappingTableOthers2Unicode();
	if( ret != MappingTableLoaded) return ret;

	//Set up the RTF head..
	fputs(rtfHead, targetFile);//This Unicode RTF file
	fputs(fonttbl, targetFile);
	fputs(ChineseFontName, targetFile);
	fputs(";}{\\f30\\fnil\\fcharset0\\fprq2 ", targetFile);

	fputs(TibetanUnicodeFontName, targetFile);
	fputs(";}}\\plain", targetFile);

	ret = FzHg2Unicode();		//TranslateFzHg2UniTxtAndWriteBack(); //Directly use this

	fputc('}', targetFile);//Close the file

	FreeOthers2UniTableMemory();//Free the memory allocated

	if(ret != EndOfFile) return Exception;

	return ConversionDone;
}



//Converting from FzHg file to Unicode Txt format
int FzHg2UniTxt()
{
	int ret;

	ret = LoadMappingTableOthers2Unicode();
	if( ret != MappingTableLoaded) return ret;

	//Converting...
	fputc(0xff, targetFile);//This Unicode plain text
	fputc(0xfe, targetFile);

	ret = FzHg2Unicode();

	FreeOthers2UniTableMemory();//Free the memory allocated

	if(ret != EndOfFile) return Exception;

	return ConversionDone;
}


//Converting from FzHg file to Unicode Webpage
int FzHg2UniWebpage()
{
	int ret;

	char HtmlHead[] = "<html><head></head><body>";//Very simple HTML file, only focus on TEXT
	char HtmlEnd[] = "</body></html>";

	ret = LoadMappingTableOthers2Unicode();
	if( ret != MappingTableLoaded) return ret;

	fputs(HtmlHead, targetFile);

	ret = FzHg2Unicode();

	fputs(HtmlEnd, targetFile);

	FreeOthers2UniTableMemory();//Free the memory allocated

	if(ret != EndOfFile) return Exception;

	return ConversionDone;
}


//--------------------------BTNtxt to Unicode----------------------//2007

//BTNtxt to Unicode
int BTNtxt2Unicode()
{
	int ch, j=0, thecode;
	int l, index = 0;
	char *p;

	nTibetan = 0;
	nChinese = 0;
	nLatin = 0;

	while((ch = getc(sourceFile)) != EOF)
	{
		if(j==0)
		{
			if(ch<=0x80)//Latin
			{
				if(nTibetan>0) TranslateAndWriteBack();

				if(nChinese>0) WriteBackChinese();

				if(nLatin >= SZBuffer) WriteBackLatin();
				LatinBuffer[nLatin++] = ch;
			}
			else //ch>0x80
			{
				thecode = ch;
				j=1;
			}
		}
		else //(j==1) Chinese, including those BTN code points (Tibetan code points)
		{
			j = 0;

			if(nLatin>0) WriteBackLatin();

			if(SourceEncoding == NS && thecode >= 0xf3 && thecode <= 0xf9)
				index = 256*((int)thecode-0xF3)+((int)ch-0x00);
			else if(SourceEncoding == Ty && thecode > 0x80 && thecode < 0xf6 && ch >= 0x40)
				index = 192*((int)thecode-0x81)+((int)ch-0x40);
			else if(SourceEncoding == Bzd && thecode > 0xa5 && thecode < 0xe1 && ch >= 0xa1)
				index = 95*((int)thecode-0xa6)+((int)ch-0xa1); //SourceEncoding == Bzd
			else //Out of the range of Tibetan code  points --->> not a Tibetan --->> Chinese
				index = -1; //Use it as an indicater here!

			if(index != -1) p = pTable + index * 30; //Set the right place for the Unicode points of this code.

			if(index == -1 || *p == '\0') //Check the table and found it's Chinese
			{
				if(nTibetan>0) TranslateAndWriteBack();

				l = ((int)thecode << 8) + (int)ch;
				if(nChinese >= SZBuffer) WriteBackChinese();
				ChineseBuffer[nChinese++] = l;
			}
			else //Tibetan
			{
				if(nChinese>0) WriteBackChinese();

				l = ((int)thecode << 8) + (int)ch;

				if(nTibetan >= SZBuffer) TranslateAndWriteBack();
				TibetanBuffer[nTibetan ++].ch = l;
			}
		}
	}//End of while()
	if(nChinese>0) WriteBackChinese();
	if(nTibetan>0) TranslateAndWriteBack();
	if(nLatin>0) WriteBackLatin();

	return EndOfFile;
}


//Converting from BTNtxt file to Unicode RTF format
int BTNtxt2UniRtf()//BTNtxt to Unicode RTF
{
	int ret;

	char rtfHead[] = "{\\rtf1\\ansi\\ansicpg1252\\uc0\\deff0";
	char fonttbl[] = "{\\fonttbl{\\f0\\froman\\fcharset0\\fprq2 Times New Roman;}{\\f31 ";

	//Load the table
	ret = LoadMappingTableOthers2Unicode();
	if( ret != MappingTableLoaded) return ret;

	//Set up the RTF head..
	fputs(rtfHead, targetFile);//This Unicode RTF file
	fputs(fonttbl, targetFile);
	fputs(ChineseFontName, targetFile);
	fputs(";}{\\f30\\fnil\\fcharset0\\fprq2 ", targetFile);

	fputs(TibetanUnicodeFontName, targetFile);
	fputs(";}}\\plain", targetFile);

	ret = BTNtxt2Unicode();		//TranslateBTNtxt2UniTxtAndWriteBack(); //Directly use this

	fputc('}', targetFile);//Close the file

	FreeOthers2UniTableMemory();//Free the memory allocated

	if(ret != EndOfFile) return Exception;

	return ConversionDone;
}


//Converting from BTNtxt file to Unicode Txt format
int BTNtxt2UniTxt()
{
	int ret;

	ret = LoadMappingTableOthers2Unicode();
	if( ret != MappingTableLoaded) return ret;

	//Converting...
	fputc(0xff, targetFile);//This Unicode plain text
	fputc(0xfe, targetFile);

	ret = BTNtxt2Unicode();

	FreeOthers2UniTableMemory();//Free the memory allocated

	if(ret != EndOfFile) return Exception;

	return ConversionDone;
}


//Converting from BTNtxt file to Unicode Webpage
int BTNtxt2UniWebpage()
{
	int ret;

	char HtmlHead[] = "<html><head></head><body>";//Very simple HTML file, only focus on TEXT
	char HtmlEnd[] = "</body></html>";

	ret = LoadMappingTableOthers2Unicode();
	if( ret != MappingTableLoaded) return ret;

	fputs(HtmlHead, targetFile);

	ret = BTNtxt2Unicode();

	fputs(HtmlEnd, targetFile);

	FreeOthers2UniTableMemory();//Free the memory allocated

	if(ret != EndOfFile) return Exception;

	return ConversionDone;
}


//-----------------------------NS UnicodeTXT to Unicode----------------------//2007

//BTNUniTxt to Unicode     Only for NS
int BTNUniTxt2Unicode()
{
	int ch1, ch2;
	int l;

	nTibetan = 0;
	nChinese = 0;
	nLatin = 0;

#ifdef Unix		//20090427
	while( ((ch2 = getc(sourceFile)) != EOF)
		&& ((ch1 = getc(sourceFile)) != EOF))
	{
		l = (ch1<<8) + ch2;
#endif
#ifdef Windows
	while( (l = getwc(sourceFile)) != WEOF) //20081124
	{
#endif			//20090427
#ifdef Mac		//20180304
	while( ((ch2 = getc(sourceFile)) != EOF)
		&& ((ch1 = getc(sourceFile)) != EOF))
	{
		l = (ch1<<8) + ch2;
#endif

		if(l<=0xff)//Latin
		{
			if(nTibetan>0) TranslateAndWriteBack();

			if(nChinese>0) WriteBackChinese();

			if(nLatin >= SZBuffer) WriteBackLatin();
			LatinBuffer[nLatin++] = l;
		}
		else if( (l>=0x0f00 && l<=0x0fff) || (l>=0xf300 && l<=0xf900))//Tibetan  ????
		{
			if(nLatin>0) WriteBackLatin();

			if(nChinese>0) WriteBackChinese();

			if(nTibetan >= SZBuffer) TranslateAndWriteBack();
			TibetanBuffer[nTibetan ++].ch = l;
		}
		else  //Chiness    ?????????
		{
			if(nLatin>0) WriteBackLatin();

			if(nTibetan>0) TranslateAndWriteBack();

			if(nChinese >= SZBuffer) WriteBackChinese();
			ChineseBuffer[nChinese++] = l;
		}
	}//End of while()
	if(nChinese>0) WriteBackChinese();
	if(nTibetan>0) TranslateAndWriteBack();
	if(nLatin>0) WriteBackLatin();

	return EndOfFile;
}

//Converting from BTNUniTxt file to Unicode Txt format
int BTNUniTxt2UniTxt()
{
	int ret, ch;

	ret = LoadMappingTableOthers2Unicode();
	if( ret != MappingTableLoaded) return ret;

	//Converting......
	ch = fgetc(sourceFile);//Take off FFFE from the Unicode plain text, and put it back to taget UniTxt file
	fputc(ch, targetFile);
	ch = fgetc(sourceFile);
	fputc(ch, targetFile);

	ret = BTNUniTxt2Unicode();

	FreeOthers2UniTableMemory();//Free the memory allocated

	if(ret != EndOfFile) return Exception;

	return ConversionDone;
}


int BTNUniTxt2UniRtf()//BTNtxt to Unicode RTF
{
	int ret;

	char rtfHead[] = "{\\rtf1\\ansi\\ansicpg1252\\uc0\\deff0";
	char fonttbl[] = "{\\fonttbl{\\f0\\froman\\fcharset0\\fprq2 Times New Roman;}{\\f31 ";

	//Load the table
	ret = LoadMappingTableOthers2Unicode();
	if( ret != MappingTableLoaded) return ret;

	if(getc(sourceFile) == EOF) return EndOfFile; //Take off FF
	if(getc(sourceFile) == EOF) return EndOfFile; //Take off FE

	//Set up the RTF head..
	fputs(rtfHead, targetFile);//This Unicode RTF file
	fputs(fonttbl, targetFile);
	fputs(ChineseFontName, targetFile);
	fputs(";}{\\f30\\fnil\\fcharset0\\fprq2 ", targetFile);

	fputs(TibetanUnicodeFontName, targetFile);
	fputs(";}}\\plain", targetFile);

	ret = BTNUniTxt2Unicode();		//TranslateBTNtxt2UniTxtAndWriteBack(); //Directly use this

	fputc('}', targetFile);//Close the file

	FreeOthers2UniTableMemory();//Free the memory allocated

	if(ret != EndOfFile) return Exception;

	return ConversionDone;
}


int BTNUniTxt2UniWebpage()//BTNtxt to Unicode RTF
{
	int ret;

	char HtmlHead[] = "<html><head></head><body>";//Very simple HTML file, only focus on TEXT
	char HtmlEnd[] = "</body></html>";

	ret = LoadMappingTableOthers2Unicode();
	if( ret != MappingTableLoaded) return ret;

	if(getc(sourceFile) == EOF) return EndOfFile; //Take off FF
	if(getc(sourceFile) == EOF) return EndOfFile; //Take off FE

	fputs(HtmlHead, targetFile);

	ret = BTNUniTxt2Unicode();

	fputs(HtmlEnd, targetFile);

	FreeOthers2UniTableMemory();//Free the memory allocated

	if(ret != EndOfFile) return Exception;

	return ConversionDone;
}

//-----------------------------BTNrtf to Unicode----------------------//2007

//Converting from BTNrtf file to Unicode TXT
int BTNrtf2UniTxt()
{
	return TSLrtf2UniTxt();
}


//Converting from BTNrtf file to Unicode RTF
int BTNrtf2UniRtf()
{
	return TSLrtf2UniRtf();
}


//Converting from BTNrtf file to Unicode Webpage
int BTNrtf2UniWebpage()
{
	return TSLrtf2UniWebpage();
}


//------------------------------BTNWebpage to Unicode----------------------//2007

//Converting from BTNWebpage file to Unicode TXT
int BTNWebpage2UniTxt()
{
	return TSLWebpage2UniTxt();
}


//Converting from BTNWebpage file to Unicode RTF
int BTNWebpage2UniRtf()
{
	return TSLWebpage2UniRtf();
}


//Converting from BTNWebpage file to Unicode Webpage
int BTNWebpage2UniWebpage()
{
	return TSLWebpage2UniWebpage();
}

//------------------------------AWrtf to Unicode-----------------------------

// ACIP and Wylie RTF to Unicode TXT
int AWrtf2UniTxt()
{
	int ret;

	if(IsThisRTFFile() != ThisIsRtfFile) return NotRtfFile;

	ret = GetDeff();
	if(ret != GotTheDeff && ret != ReachFontTbl) return NoDeff;

	ret = onFonttbl();
	if( ret != FonttblProcessed) return ret;

	ret = LoadAW2UnicodeTable();
	if( ret != AWToUnicodeTableLoaded) return ret;

	//Converting.....
	fputc(0xFF, targetFile);//Add FFFE to the file
	fputc(0xFE, targetFile);

	ret = parseRTFforAW();

	FreeAW2UnicodeTableMemory();

	if(ret != EndOfFile) return Exception;

	return ConversionDone;
}


// ACIP and Wylie RTF to Unicode RTF
int AWrtf2UniRtf()
{
	int ret;

	if(IsThisRTFFile() != ThisIsRtfFile) return NotRtfFile;

	ret = GetDeff();
	if(ret != GotTheDeff && ret != ReachFontTbl) return NoDeff;

	ret = onFonttbl();
	if( ret != FonttblProcessed) return ret;

	ret = LoadAW2UnicodeTable();
	if( ret != AWToUnicodeTableLoaded) return ret;

	ret = parseRTFforAW();

	FreeAW2UnicodeTableMemory();

	if(ret != EndOfFile) return Exception;

	return ConversionDone;
}



// ACIP and Wylie RTF to Unicode Webpage
int AWrtf2UniWebpage()
{
	int ret;

	char HtmlHead[] = "<html><head></head><body>";//Very simple HTML file, only focus on TEXT
	char HtmlEnd[] = "</body></html>";

	if(IsThisRTFFile() != ThisIsRtfFile) return NotRtfFile;

	ret = GetDeff();
	if(ret != GotTheDeff && ret != ReachFontTbl) return NoDeff;

	ret = onFonttbl();
	if( ret != FonttblProcessed) return ret;

	ret = LoadAW2UnicodeTable();
	if( ret != AWToUnicodeTableLoaded) return ret;

	fputs(HtmlHead, targetFile);

	ret = parseRTFforAW();

	fputs(HtmlEnd, targetFile);

	FreeAW2UnicodeTableMemory();

	if(ret != EndOfFile) return Exception;

	return ConversionDone;
}


//-------------------------------------------AWwebpage to Unicode-----------------------------
// ACIP and Wylie Webpage to Unicode RTF
int AWWebpage2UniRtf()
{
	int ret;

	char rtfHead[] = "{\\rtf1\\ansi\\ansicpg1252\\uc0\\deff0";
	char fonttbl[] = "{\\fonttbl{\\f0\\froman\\fcharset0\\fprq2 Times New Roman;}{\\f31 ";

	//Converting........
	//SEt up the RTF head..
	fputs(rtfHead, targetFile);//This Unicode RTF file
	fputs(fonttbl, targetFile);
	fputs(ChineseFontName, targetFile);
	fputs(";}{\\f30\\fnil\\fcharset0\\fprq2 ", targetFile);
	fputs(TibetanUnicodeFontName, targetFile);
	fputs(";}}\\plain", targetFile);


	if(LocateBodyTag() != BodyTagFound) return EndOfFile;

	ret = LoadAW2UnicodeTable();
	if( ret != AWToUnicodeTableLoaded) return ret;

	ret = parseWEBPAGE();	//Call Translater in this function

	fputc('}', targetFile);//Close the file

	FreeAW2UnicodeTableMemory();

	if(ret != EndOfFile) return Exception;

	return ConversionDone;
}



// ACIP and Wylie Webpage to Unicode TXT
int AWWebpage2UniTxt()
{
	int ret;

	if(LocateBodyTag() != BodyTagFound) return EndOfFile;

	ret = LoadAW2UnicodeTable();
	if( ret != AWToUnicodeTableLoaded) return ret;

	fputc(0xFF, targetFile);//Add FFFE to the file
	fputc(0xFE, targetFile);

	ret = parseWEBPAGE();	//Call Translater in this function

	FreeAW2UnicodeTableMemory();

	if(ret != EndOfFile) return Exception;

	return ConversionDone;

}



// ACIP and Wylie Webpage to Unicode Webpage
int AWWebpage2UniWebpage()
{
	int ret;

	if(LocateBodyTag() != BodyTagFound) return EndOfFile;

	ret = LoadAW2UnicodeTable();
	if( ret != AWToUnicodeTableLoaded) return ret;

	ret = parseWEBPAGE();	//Call Translater in this function

	FreeAW2UnicodeTableMemory();

	if(ret != EndOfFile) return Exception;

	return ConversionDone;
}


//-------------------------------------------AWtxt to Unicode-----------------------------
// ACIP and Wylie TXT to Unicode TXT
int AWtxt2UniTxt()		//20080620
{
	int ch, ret, lastCh = 0,j=0, thecode = 0, l;

	ret = LoadAW2UnicodeTable();
	if( ret != AWToUnicodeTableLoaded) return ret;

	//Converting.....
	fputc(0xFF, targetFile);//Add FFFE to the file
	fputc(0xFE, targetFile);

	nTibetan = 0;

	while((ch = getc(sourceFile)) != EOF)
	{
		if((j==0)&&(ch<0x7f))//Latin character --  possible AW letters
		{
//			if((ch>=0x20 && ch<=0x7e) || ch == 0xa7 || ch == 0xe2 || ch == 0xe4   ---------------this is for ALA-LC Wylie
//				|| ch == 0xe5 || ch == 0xe7 || ch == 0xef || ch == 0xf2
//				|| ch == 0xf4 || ch == 0xf6)//Printable characters, possible AW letters
			if(nChinese>0) WriteBackChinese();

			if(ch>=0x20 && ch<0x7f) //Printable characters, possible AW letters
			{
				if(nLatin>0) WriteBackLatin();
				if(nTibetan >= SZBuffer) TranslateAndWriteBack();
				TibetanBuffer[nTibetan++].ch = ch;//be carefull here!

				if(IsPunctuation(ch)==Yes) TranslateAndWriteBack();

				if(ch==0x20 && lastCh==0x20) TranslateAndWriteBack();
			}
			else //ch<0x20 || ch>0x7f,  can not be Wylie characters
			{
				if((ch=='\n') && (isalpha(lastCh) || lastCh == '\'')) TibetanBuffer[nTibetan++].ch = 0x20;
				if(nTibetan>0) TranslateAndWriteBack();

				if(nLatin >= SZBuffer) WriteBackLatin();
				LatinBuffer[nLatin++] = ch;
			}
		}
		else if((j==0)&&(ch>0x80)) //possible chinese character
		{
			if(nLatin>0) WriteBackLatin();
			if(nTibetan>0) TranslateAndWriteBack();

			thecode=ch; j=1; lastCh = ch;
			continue;
		}
		else if((j==1)&&(ch>0x40)) //Chinese
		{
			if(nLatin>0) WriteBackLatin();
			if(nTibetan>0) TranslateAndWriteBack();

			l = ((int)thecode << 8) + (int)ch;
			if(nChinese >= SZBuffer) WriteBackChinese();
			ChineseBuffer[nChinese++] = l;
			j = 0;
		}
		else//some strange characters
		{
			if(nLatin>0) WriteBackLatin();
			if(nTibetan > 0) TranslateAndWriteBack();
			if(nChinese > 0) WriteBackChinese();
			fputc((char)ch,targetFile);
			fputc(0x00,targetFile);
			j = 0;
		}

		lastCh = ch;
	}//end of while()
	if(nTibetan > 0) TranslateAndWriteBack();
	if(nLatin>0) WriteBackLatin();
	if(nChinese > 0) WriteBackChinese();

	FreeAW2UnicodeTableMemory();

	return ConversionDone;
}



// ACIP and Wylie TXT to Unicode Wepage
int AWtxt2UniWebpage()		//20080620
{
	int ch, ret, lastCh = 0, n = 0,j=0, thecode = 0, l;

	char HtmlHead[] = "<html><head></head><body>";//Very simple HTML file, only focus on TEXT
	char HtmlEnd[] = "</body></html>";

	ret = LoadAW2UnicodeTable();
	if( ret != AWToUnicodeTableLoaded) return ret;

	fputs(HtmlHead, targetFile);

	nTibetan = 0;
	nLatin = 0;

	while((ch = getc(sourceFile)) != EOF)
	{
		n ++; if(n>50){ n = 0; fputc('\n',targetFile); }//For inserting the line breaker.

		if((j==0)&&(ch<0x7f))//Latin character --  possible AW letters
		{
			if(nChinese>0) WriteBackChinese();

			if(ch>=0x20 && ch<0x7f) //Printable characters, possible AW letters
			{
				if(nLatin>0) WriteBackLatin();
				if(nTibetan >= SZBuffer) TranslateAndWriteBack();
				TibetanBuffer[nTibetan++].ch = ch;//be carefull here!

				if(IsPunctuation(ch)==Yes) TranslateAndWriteBack();

				if(ch==0x20 && lastCh==0x20) TranslateAndWriteBack();
			}
			else //ch<0x20 || ch>0x7f,  can not be Wylie characters
			{
				if((ch=='\n') && (isalpha(lastCh) || lastCh == '\'')) TibetanBuffer[nTibetan++].ch = 0x20;
				if(nTibetan>0) TranslateAndWriteBack();

				if(nLatin >= SZBuffer) WriteBackLatin();
				LatinBuffer[nLatin++] = ch;
			}
		}
		else if((j==0)&&(ch>0x80)) //possible chinese character
		{
			if(nLatin>0) WriteBackLatin();
			if(nTibetan>0) TranslateAndWriteBack();

			thecode=ch; j=1; lastCh = ch;
			continue;
		}
		else if((j==1)&&(ch>0x40)) //Chinese
		{
			if(nLatin>0) WriteBackLatin();
			if(nTibetan>0) TranslateAndWriteBack();

			l = ((int)thecode << 8) + (int)ch;
			if(nChinese >= SZBuffer) WriteBackChinese();
			ChineseBuffer[nChinese++] = l;
			j = 0;
		}
		else//some strange characters
		{
			if(nLatin>0) WriteBackLatin();
			if(nTibetan > 0) TranslateAndWriteBack();
			if(nChinese > 0) WriteBackChinese();
			fputc((char)ch,targetFile);
			j = 0;
		}

		lastCh = ch;
	}//end of while()
	if(nTibetan > 0) TranslateAndWriteBack();
	if(nLatin>0) WriteBackLatin();
	if(nChinese > 0) WriteBackChinese();

	fputs(HtmlEnd, targetFile);

	FreeAW2UnicodeTableMemory();

	return ConversionDone;
}



// ACIP and Wylie TXT to Unicode RTF
int AWtxt2UniRtf()		//20080620
{
	int ch, ret, lastCh = 0, n = 0,j=0, thecode = 0, l;

	char rtfHead[] = "{\\rtf1\\ansi\\ansicpg1252\\uc0\\deff0";
	char fonttbl[] = "{\\fonttbl{\\f0\\froman\\fcharset0\\fprq2 Times New Roman;}{\\f31 ";

	ret = LoadAW2UnicodeTable();
	if( ret != AWToUnicodeTableLoaded) return ret;

	//Set up the RTF head..
	fputs(rtfHead, targetFile);//This Unicode RTF file
	fputs(fonttbl, targetFile);
	fputs(ChineseFontName, targetFile);
	fputs(";}{\\f30\\fnil\\fcharset0\\fprq2 ", targetFile);
	fputs(TibetanUnicodeFontName, targetFile);
	fputs(";}}\\plain", targetFile);

	nTibetan = 0;
	nLatin = 0;

	while((ch = getc(sourceFile)) != EOF)
	{
		n ++; if(n>50){ n = 0; fputc('\n',targetFile); }//For inserting the line breaker.

		if((j==0)&&(ch<0x7f))//Latin character --  possible AW letters
		{
			if(nChinese>0) WriteBackChinese();

			if(ch>=0x20 && ch<0x7f) //Printable characters, possible AW letters
			{
				if(nLatin>0) WriteBackLatin();
				if(nTibetan >= SZBuffer) TranslateAndWriteBack();
				TibetanBuffer[nTibetan++].ch = ch;//be carefull here!

				if(IsPunctuation(ch)==Yes) TranslateAndWriteBack();

				if(ch==0x20 && lastCh==0x20) TranslateAndWriteBack();
			}
			else //ch<0x20 || ch>0x7f,  can not be Wylie characters
			{
				if((ch=='\n') && (isalpha(lastCh) || lastCh == '\'')) TibetanBuffer[nTibetan++].ch = 0x20;
				if(nTibetan>0) TranslateAndWriteBack();

				if(nLatin >= SZBuffer) WriteBackLatin();
				LatinBuffer[nLatin++] = ch;
			}
		}
		else if((j==0)&&(ch>0x80)) //possible chinese character
		{
			if(nLatin>0) WriteBackLatin();
			if(nTibetan>0) TranslateAndWriteBack();

			thecode=ch; j=1; lastCh = ch;
			continue;
		}
		else if((j==1)&&(ch>0x40)) //Chinese
		{
			if(nLatin>0) WriteBackLatin();
			if(nTibetan>0) TranslateAndWriteBack();

			l = ((int)thecode << 8) + (int)ch;
			if(nChinese >= SZBuffer) WriteBackChinese();
			ChineseBuffer[nChinese++] = l;
			j = 0;
		}
		else//some strange characters
		{
			if(nLatin>0) WriteBackLatin();
			if(nTibetan > 0) TranslateAndWriteBack();
			if(nChinese > 0) WriteBackChinese();
			fputc((char)ch,targetFile);
			j = 0;
		}

		lastCh = ch;
	}//end of while()
	if(nTibetan > 0) TranslateAndWriteBack();
	if(nLatin>0) WriteBackLatin();
	if(nChinese > 0) WriteBackChinese();

	fputc('}', targetFile);//Close the file

	FreeAW2UnicodeTableMemory();

	return ConversionDone;
}



//-------------------------------------------AWUniTxt to Unicode-----------------------------
//Convert ACIP and Wylie UnicodeTXT
// to Tibetan Unicode
int AWUniTxt2UniTxt()
{
	int ch1, ch2, ret, l;

	ret = LoadAW2UnicodeTable();
	if( ret != AWToUnicodeTableLoaded) return ret;

	//Transfer FFFE to target file
	ch1 = getc(sourceFile);
	fputc(ch1, targetFile);
	ch1 = getc(sourceFile);
	fputc(ch1, targetFile);

	nTibetan = 0;


#ifdef Unix		//20090427
	while( ((ch2 = getc(sourceFile)) != EOF)
		&& ((ch1 = getc(sourceFile)) != EOF))
	{
		l = (ch1<<8) + ch2;
#endif
#ifdef Windows
	while( (l = getwc(sourceFile)) != WEOF) //20081124
	{
#endif			//20090427
#ifdef Mac		//20180304
	while( ((ch2 = getc(sourceFile)) != EOF)
		&& ((ch1 = getc(sourceFile)) != EOF))
	{
		l = (ch1<<8) + ch2;
#endif

		if((l>=0x20 && l<=0x7e) || l == 0xa7 || l == 0xe2 || l == 0xe4 //20081124
			|| l == 0xe5 || l == 0xe7 || l == 0xef || l == 0xf2 || l == 0xf4 || l == 0xf6)//Printable characters, possible AW letters //20081124
		{
			if(nTibetan >= SZBuffer) TranslateAndWriteBack();
			TibetanBuffer[nTibetan++].ch = l;

			if(IsPunctuation(l)==Yes) TranslateAndWriteBack();
		}
		else
		{
			if(nTibetan > 0) TranslateAndWriteBack();
#ifdef Windows	//20090427
			fputwc(l, targetFile);//20081124
#endif
#ifdef Unix
			fputc(ch2, targetFile);//20081124
			fputc(ch1, targetFile);//20081124
#endif			//20090427
		}
	}//end of while()
	if(nTibetan > 0) TranslateAndWriteBack();

	FreeAW2UnicodeTableMemory();
	return ConversionDone;
}



//Convert ACIP and Wylie UnicodeTXT to Tibetan Unicode Webpage
int AWUniTxt2UniWebpage()
{
	int ch1, ch2, ret, l, preChIsEL, n = 0;

	char HtmlHead[] = "<html><head></head><body>";//Very simple HTML file, only focus on TEXT
	char HtmlEnd[] = "</body></html>";

	fputs(HtmlHead, targetFile);

	ret = LoadAW2UnicodeTable();
	if( ret != AWToUnicodeTableLoaded) return ret;

	//Take off FFFE from the Unicode plain text
	fgetc(sourceFile);
	fgetc(sourceFile);

	//Converting.....
	nTibetan = 0;
	nChinese = 0;
	nTibetanInAW = 0;


#ifdef Unix		//20090427
	while( ((ch2 = getc(sourceFile)) != EOF)
		&& ((ch1 = getc(sourceFile)) != EOF))
	{
		l = (ch1<<8) + ch2;
#endif
#ifdef Windows
	while( (l = getwc(sourceFile)) != WEOF) //20081124
	{
		ch2 = l & 0xff;  //20081124
		ch1 = l>>8;  //20081124
#endif			//20090427
#ifdef Mac		//20090427
	while( ((ch2 = getc(sourceFile)) != EOF)
		&& ((ch1 = getc(sourceFile)) != EOF))
	{
		l = (ch1<<8) + ch2;
#endif

		n ++; if(n>50){ n = 0; fputc('\n',targetFile); }//For inserting the line breaker.

		if(ch1==0) //ASCII
		{
			if(nChinese > 0) WriteBackChinese();

			if(IsPunctuation(ch2)==Yes)//Process the sentence
			{
				TibetanBuffer[nTibetan++].ch = ch2;
				if(nLatin>0) WriteBackLatin();
				if(nTibetan > 0) TranslateAndWriteBack();
				continue;
			}

			if(ch2=='\n' ||ch2==0x0d )
			{
				if(nTibetan > 0) TranslateAndWriteBack();
				if(preChIsEL == No) fputs("<br>",targetFile);

				preChIsEL = Yes;

				if(TargetFileFormat == WebPage) fputc(ch2, targetFile);
			}
			else if(ch1==0 && ((ch2>=0x20 && ch2<=0x7e) || ch2 == 0xa7 || ch2 == 0xe2 || ch2 == 0xe4
				|| ch2 == 0xe5 || ch2 == 0xe7 || ch2 == 0xef || ch2 == 0xf2
				|| ch2 == 0xf4 || ch2 == 0xf6))//Printable characters, possible AW letters
			{
				if(nTibetan >= SZBuffer) TranslateAndWriteBack();
				TibetanBuffer[nTibetan++].ch = ch2;

				preChIsEL = No;
			}
			else
			{
				if(nTibetan > 0) TranslateAndWriteBack();
				fputc((char)ch2,targetFile);

				preChIsEL = No;
			}
		}
		else if(ch1==0x0f)//Tibetan!!!!!!!!!!!!!!!!Be careful here
		{
			if(nTibetan > 0) TranslateAndWriteBack();//Latin!!!
			if(nChinese > 0) WriteBackChinese();

			l = (ch1 << 8) + ch2;
			if(nTibetanInAW >= SZBuffer) WriteBackTibetanInAW();
			TibetanInAWBuffer[nTibetanInAW++] = l;

			preChIsEL = No;
		}
		else //Chinese
		{
			if(nTibetan > 0) TranslateAndWriteBack();//Latin

			l = (ch1 << 8) + ch2;

			if(nChinese >= SZBuffer) WriteBackChinese();
			ChineseBuffer[nChinese++] = l;

			preChIsEL = No;
		}

	}//end of while()
	if(nTibetan > 0) TranslateAndWriteBack();
	if(nChinese > 0) WriteBackChinese();
	if(nTibetanInAW > 0) WriteBackTibetanInAW();

	fputs(HtmlEnd, targetFile);
	FreeAW2UnicodeTableMemory();

	return ConversionDone;
}



//Convert ACIP and Wylie UnicodeTXT to Tibetan Unicode RTF file
int AWUniTxt2UniRtf()
{
	int l, ch1, ch2, ret, preChIsEL = No, n = 0;

	char rtfHead[] = "{\\rtf1\\ansi\\ansicpg1252\\uc0\\deff0";
	char fonttbl[] = "{\\fonttbl{\\f0\\froman\\fcharset0\\fprq2 Times New Roman;}{\\f31 ";

	ret = LoadAW2UnicodeTable();
	if( ret != AWToUnicodeTableLoaded) return ret;

	//Set up the RTF head..
	fputs(rtfHead, targetFile);//This Unicode RTF file
	fputs(fonttbl, targetFile);
	fputs(ChineseFontName, targetFile);
	fputs(";}{\\f30\\fnil\\fcharset0\\fprq2 ", targetFile);
	fputs(TibetanUnicodeFontName, targetFile);
	fputs(";}}\\plain", targetFile);

	//Start converting ...
	nTibetan = 0;
	nChinese = 0;
	nTibetanInAW = 0;

	if(getc(sourceFile) == EOF) return EndOfFile; //Take off FF
	if(getc(sourceFile) == EOF) return EndOfFile; //Take off FE

#ifdef Unix		//20090427
	while( ((ch2 = getc(sourceFile)) != EOF)
		&& ((ch1 = getc(sourceFile)) != EOF))
	{
		l = (ch1<<8) + ch2;
#endif
#ifdef Windows
	while( (l = getwc(sourceFile)) != WEOF) //20081124
	{
		ch2 = l & 0xff;  //20081124
		ch1 = l>>8;  //20081124
#endif			//20090427
#ifdef Mac		//20180304
	while( ((ch2 = getc(sourceFile)) != EOF)
		&& ((ch1 = getc(sourceFile)) != EOF))
	{
		l = (ch1<<8) + ch2;
#endif

		n ++; if(n>50){ n = 0; fputc('\n',targetFile); }//For inserting the line breaker.

		if(ch1==0) //ASCII
		{
			if(nChinese > 0) WriteBackChinese();
			if(nTibetanInAW > 0) WriteBackTibetanInAW();

			if(ch2=='\n' || ch2==0x0d)
			{
				if(nTibetan > 0) TranslateAndWriteBack();
				if(preChIsEL == No) fputs("\\par",targetFile);

				preChIsEL = Yes;
			}
			else if((ch2>=0x20 && ch2<=0x7e) || ch2 == 0xa7 || ch2 == 0xe2 || ch2 == 0xe4
				|| ch2 == 0xe5 || ch2 == 0xe7 || ch2 == 0xef || ch2 == 0xf2
				|| ch2 == 0xf4 || ch2 == 0xf6)//Printable characters, possible AW letters
			{
				if(nTibetan >= SZBuffer) TranslateAndWriteBack();
				TibetanBuffer[nTibetan++].ch = ch2;

				if(IsPunctuation(ch2)==Yes) TranslateAndWriteBack();

				preChIsEL = No;
			}
			else //Other ASCII
			{
				if(nTibetan > 0) TranslateAndWriteBack();
				fputc((char)ch2,targetFile);

				preChIsEL = No;
			}
		}
		else if(ch1==0x0f)//Tibetan!!!!!!!!!!!!!!!!Be careful here
		{
			if(nTibetan > 0) TranslateAndWriteBack();//Latin!!!
			if(nChinese > 0) WriteBackChinese();

			l = (ch1 << 8) + ch2;

			if(nTibetanInAW >= SZBuffer) WriteBackTibetanInAW();
			TibetanInAWBuffer[nTibetanInAW++] = l;

			preChIsEL = No;
		}
		else //Chinese
		{
			if(nTibetan > 0) TranslateAndWriteBack();//Latin
			if(nTibetanInAW > 0) WriteBackTibetanInAW();

			l = (ch1 << 8) + ch2;

			if(nChinese >= SZBuffer) WriteBackChinese();
			ChineseBuffer[nChinese++] = l;

			preChIsEL = No;
		}
	}//end of while()
	if(nTibetan > 0) TranslateAndWriteBack();
	if(nChinese > 0) WriteBackChinese();
	if(nTibetanInAW > 0) WriteBackTibetanInAW();

	fputc('}', targetFile);//Close the file

	FreeAW2UnicodeTableMemory();
	return ConversionDone;
}
//-----------------end of conversion routines------------------------




//////////////////////////////////////
//									//
//									//
//	Initializing...........			//
//									//
//									//
//////////////////////////////////////

//Initialize the global variables for converting
//by taking input and output file names and conversion mode
//SourceEncoding, TargetEncoding, SourceFileFormat, TargetFileFormat
int
Initialization
(char *sourceFileName, char *targetFileName,
 char *sourceEncoding, char *targetEncoding, char *TFF)  //200802
{
	int i;
	int ch1, ch2, ch3, ch4, ch5;//200802
    char substring[20], *ret;

	ConversionDirection = -1;
	SourceEncoding = -1;
	TargetEncoding = -1;
	SourceFileType = -1;

    sourceFile = fopen(sourceFileName, "rb"); //20081124
    if (!sourceFile) return NoSourceFile;

    targetFile = fopen(targetFileName, "w+b"); //20081124
    if (!targetFile) {
    	printf ("DEBUG: failed to open target file: %s <br>\n", targetFileName);
    	return NoTargetFile;
    }
//  if targetEncoding contains Unicode, then need to get what kind of Unicode it is and assign it to UnicodeKind
//    puts("enter the sub string to find");
//    fgets(substring, sizeof(substring), stdin);
//    substring[strlen(substring)-1]='\0';
//
//    substring="Unicode";
//    ret=strstr(targetEncoding,substring); //20181130
//    if(strcmp((ret=strstr(targetEncoding,"Unicode")),"Unicode")) //20181130
//    if(targetEncoding=="Unicode") //20181130
    if(strcmp(targetEncoding,"Unicode")==0) //20181130
    {
        printf("testing strcmp\n");
        strcpy(TibetanUnicodeFontName,"Microsoft Himalaya");  // 20180304
    }
    else
    {
	    for (i=0;i<EncodingFontNumber[0]; i++) // check if it is Unicode fond name
    	{
    		if(!strcmp(TibetanFontNames[BaseIndex[0] + i], targetEncoding)) 
            {
                strcpy(TibetanUnicodeFontName,targetEncoding);  // 20180304
                targetEncoding="Unicode";
            }
    	}
    }
	for (i=0; i<Encodings; i++)
	{
		if(!strcmp(sourceEncoding, EncodingNames[i])) SourceEncoding = i;
		if(!strcmp(targetEncoding, EncodingNames[i])) TargetEncoding = i;
	}

	if(SourceEncoding == -1) return WrongSourceEncoding;
    if(TargetEncoding == -1) return WrongTargetEncoding;

	//Set up ConversionDirection
	if(SourceEncoding == Unicode && TargetEncoding != Unicode)
		ConversionDirection = UnicodeToOthers; //Unicode to others
	else if(SourceEncoding != Unicode && TargetEncoding == Unicode)
		ConversionDirection = OthersToUnicode; //Others to Unicode
	else if(SourceEncoding == TargetEncoding)
		ConversionDirection = EncodingToEncoding; //Same Encoding
	else ConversionDirection = OthersToOthers; //Others to others

	//Pick up the target file format
	if(!strcmp(TFF, "txt")) TargetFileFormat = TXT;
	else if(!strcmp(TFF, "rtf")) TargetFileFormat = RTF;
	else if(!strcmp(TFF, "UnicodeTXT")) TargetFileFormat = UnicodeTXT;
	else if(!strcmp(TFF, "webpage")) TargetFileFormat = WebPage;
	else return WrongTargetFileFormat;

	if(TargetEncoding == Unicode && TargetFileFormat == TXT)
		TargetFileFormat = UnicodeTXT;

	//Get the source file format infomation  //200802
	if((ch1 = getc(sourceFile)) == EOF) return EmptyFile;
	if((ch2 = getc(sourceFile)) == EOF) return EmptyFile;
	if((ch3 = getc(sourceFile)) == EOF) return EmptyFile;
	if((ch4 = getc(sourceFile)) == EOF) return EmptyFile;
	if((ch5 = getc(sourceFile)) == EOF) return EmptyFile;
	ungetc(ch5, sourceFile);//Put back the character
	ungetc(ch4, sourceFile);//Put back the character
	ungetc(ch3, sourceFile);//Put back the character
	ungetc(ch2, sourceFile);//Put back the character
    ungetc(ch1, sourceFile);//Put back the character

	if(ch1=='{' && ch2=='\\' && (ch3=='r'||ch3=='R') && (ch4=='t'||ch4=='T'))
		SourceFileFormat = RTF; //RTF file
	else if(ch1==0xff && ch2==0xfe)
	{
		SourceFileFormat = UnicodeTXT; //Unicode plain text
		if(ch3=='<' && ch4==0x00 && ((ch5=='!' || ch5=='h' || ch5=='H')))
		{
			SourceFileType = WebPage; //web pages with Unicode plain text file format
		}
	}
	else if(ch1==0xfe && ch2==0xff) //Unicode big endian file format
	{
		return UnicodeBigEndian;
	}
	else if(ch1==0xef && ch2==0xbb && ch3==0xbf) //UTF-8 file format
	{
		return UTF8;
	}
	else if(ch1=='<' && (ch2=='!' || ch2=='h' || ch2=='H'))
	{
		SourceFileFormat = WebPage; //plain text web pages
	}
	else //ANSI txt file format
	{
		i = 0; // As a flag
		while(ch1<=0x20) //Take off these characters <= 0x20
		{
			if((ch1 = getc(sourceFile)) == EOF) return EmptyFile;
			i = 1; //New read happend
		}
		if(i==1) //If we read new character, put back it!
			ungetc(ch1, sourceFile);//Put back the character

		SourceFileFormat = TXT; //regular TXT file
	}
	//Initialize these buffers
	for(i=0; i<MaxEncodingFontNumber; i++)
	{
		sourceFontIDs[i] = -1;
		targetFontIDs[i] = -1;
	}

	return Initialized;
}

//20080918
//The entry function
//It picks up the right conversion function for the conversion requirement
int EntryFunction(char *fInput, char *fOutput, char *sourceEncoding,
				  char *targetEncoding, char *TFF, char *StyleSheetFile)
{
	int ret;

	ret = Initialization(fInput, fOutput, sourceEncoding, targetEncoding, TFF);

	if(ret == NoSourceFile)
	{
		return ret;	//No need to clean!
	}

	if(ret == NoTargetFile)
	{
		fclose(sourceFile);
		return ret;
	}

	if(ret != Initialized)
	{
		CleanUp();
		return ret;
	}

	if(ConversionDirection == -1)
	{
		CleanUp();
		return UnknowConversionDirection;
	}

	//Files of certain encodings always should be RTF file ---- TSL
	if((SourceEncoding == LTibetan ||
		SourceEncoding == TMW ||
		SourceEncoding == TM ||
		SourceEncoding == TCRCBodYig ||
		SourceEncoding == NewSambhota ||
		SourceEncoding == Jamyang ||   //20080924
		SourceEncoding == OldSambhota) && (SourceFileFormat == TXT || SourceFileFormat == UnicodeTXT)){
			return WrongSourceEncodingOrFileFormat;
	}
	if((TargetEncoding == LTibetan ||
		TargetEncoding == TMW ||
		TargetEncoding == TM ||
		TargetEncoding == TCRCBodYig ||
		TargetEncoding == NewSambhota ||
		TargetEncoding == Jamyang ||   //20080924
		TargetEncoding == OldSambhota) && (TargetFileFormat == TXT || TargetFileFormat == UnicodeTXT)){
			return WrongTargetEncodingOrFileFormat;
	}
	//Files of certain encodings always should be TXT file---Fz Hg ----------------
	if((SourceEncoding == Fz || SourceEncoding == Hg) && SourceFileFormat != TXT){
		return WrongSourceEncodingOrFileFormat;
	}
	if((TargetEncoding == Fz || TargetEncoding == Hg) && TargetFileFormat != TXT){
		return WrongTargetEncodingOrFileFormat;
	}
	//Files of certain encodings always should be TXT file---Unicode--------------
	if(SourceEncoding == Unicode && SourceFileFormat == TXT){
		return WrongSourceEncodingOrFileFormat;
	}
	if(SourceEncoding == NS && SourceFileFormat == TXT){
		return WrongSourceEncodingOrFileFormat;
	}
	if(TargetEncoding == Unicode && TargetFileFormat == TXT){
		return WrongTargetEncodingOrFileFormat;
	}
	if(TargetEncoding == NS && TargetFileFormat == TXT){
		return WrongTargetEncodingOrFileFormat;
	}

	styleSheetFileName = StyleSheetFile;//Pass the style sheet file name to the global.

	if(CP936GBTablesBuilt == No) //Build the table //20080229
	{
		ret = BuildCp936UnicodeTables();//GB2312
		if(ret != CP936TableBuilt) return ret; //GB2312
	}

	//Active Converter
	//Pick up the right conversion function ( Converter )
	if(ConversionDirection == EncodingToEncoding) //Between same encodings.
	{
		if(SourceEncoding == Unicode)
		{
			if(SourceFileFormat == TargetFileFormat)
			{
				CleanUp();
				return SameEncoding;
			}
			else
			{
				CleanUp();
				return MSoffice;
			}
		}
		else if(SourceEncoding == Fz ||SourceEncoding == Hg)
		{
			CleanUp();
			return SameEncoding;
		}
		else //All other encodings
		{
			if(SourceFileFormat == TargetFileFormat)
			{
				CleanUp();
				return SameEncoding;
			}
			else
			{
				CleanUp();	//Close those opened files. Do every thing again.

				//Recusivelly call this function itself.
				ret = EntryFunction(fInput, TEMP_FILE, sourceEncoding, "Unicode", TFF, StyleSheetFile);
				if(ret == ConversionDone)
					ret = EntryFunction(TEMP_FILE, fOutput, "Unicode", targetEncoding, TFF, StyleSheetFile);

				remove(TEMP_FILE);//Delete the temproral file.

				return ret;
			}
		}
	}
	else if(ConversionDirection == OthersToUnicode) //Others to Unicode
	{
		if(SourceEncoding == Fz || SourceEncoding == Hg) //FzHg
		{
			if(TargetFileFormat == UnicodeTXT) ret = FzHg2UniTxt();
			else if(TargetFileFormat == WebPage) ret = FzHg2UniWebpage();
			else ret = FzHg2UniRtf();	//TargetFileFormat==RTF
		}
		else if(SourceEncoding == ACIP || SourceEncoding == Wylie || SourceEncoding == THDLWylie || SourceEncoding == LCWylie) //AW
		{
			if(SourceFileFormat == TXT)
			{
				if(TargetFileFormat == UnicodeTXT) ret = AWtxt2UniTxt();
				else if(TargetFileFormat == WebPage) ret = AWtxt2UniWebpage();
				else ret = AWtxt2UniRtf(); //TargetFileFormat == RTF
			}
			else if(SourceFileFormat == RTF)	//SourceFileFormat == RTF
			{
				if(TargetFileFormat == UnicodeTXT) ret = AWrtf2UniTxt();
				else if(TargetFileFormat == WebPage) ret = AWrtf2UniWebpage();
				else ret=AWrtf2UniRtf();	//TargetFileFormat == RTF
			}
			else if(SourceFileFormat == WebPage)	//SourceFileFormat == RTF
			{
				if(TargetFileFormat == UnicodeTXT) ret = AWWebpage2UniTxt();
				else if(TargetFileFormat == WebPage) ret = AWWebpage2UniWebpage();
				else ret = AWWebpage2UniRtf();	//TargetFileFormat == RTF
			}
			else //SourceFileFormat == UnicodeTXT
			{
				if(TargetFileFormat == UnicodeTXT) ret = AWUniTxt2UniTxt();
				else if(TargetFileFormat == WebPage) ret = AWUniTxt2UniWebpage();
				else ret=AWUniTxt2UniRtf();	//TargetFileFormat == RTF
			}
		}
		else if(SourceEncoding == Bzd || SourceEncoding == Ty || SourceEncoding == NS) //BTN  //2007
		{
			if(SourceFileFormat == RTF)
			{
				if(TargetFileFormat == UnicodeTXT) ret = BTNrtf2UniTxt();
				else if(TargetFileFormat == WebPage) ret = BTNrtf2UniWebpage();
				else ret = BTNrtf2UniRtf(); 	//TargetFileFormat==RTF
			}
			else if(SourceFileFormat == TXT)
			{
				if(TargetFileFormat == UnicodeTXT) ret = BTNtxt2UniTxt();
				else if(TargetFileFormat == WebPage) ret = BTNtxt2UniWebpage();
				else ret = BTNtxt2UniRtf(); 	//TargetFile
			}
			else if(SourceFileFormat == UnicodeTXT) //2008
			{
				if(TargetFileFormat == UnicodeTXT) ret = BTNUniTxt2UniTxt();
				else if(TargetFileFormat == WebPage) ret = BTNUniTxt2UniWebpage();
				else ret = BTNUniTxt2UniRtf(); 	//TargetFile
			}
			else //SourceFileFormat == WebPage
			{
				if(TargetFileFormat == UnicodeTXT) ret = BTNWebpage2UniTxt();
				else if(TargetFileFormat == WebPage) ret = BTNWebpage2UniWebpage();
				else ret = BTNWebpage2UniRtf(); 	//TargetFileFormat==RTF
			}
		}
		else	//TSL:  SourceEncoding:  LTibetan, OldSambhota, NewSambhota, TMW, TM, TCRCBodYig, Jamyang
		{		//SourceFileFormat always should be RTF

			if(SourceFileFormat == RTF)
			{
				if(TargetFileFormat == UnicodeTXT){ret = TSLrtf2UniTxt();}
				else if(TargetFileFormat == WebPage){ret = TSLrtf2UniWebpage();}
				else{ret = TSLrtf2UniRtf(); }//TargetFileFormat==RTF
			}
			else //SourceFileFormat == WebPage
			{
				if(TargetFileFormat == UnicodeTXT) ret = TSLWebpage2UniTxt();
				else if(TargetFileFormat == WebPage) ret = TSLWebpage2UniWebpage();
				else ret = TSLWebpage2UniRtf(); 	//TargetFileFormat==RTF
			}
		}

		CleanUp(); return ret;
	}
	else if(ConversionDirection == UnicodeToOthers) //Unicode to others
	{
		if(TargetEncoding == Fz || TargetEncoding == Hg)
		{
			if(SourceFileFormat == UnicodeTXT) ret = UniTxt2FzHg();
			else if(SourceFileFormat == WebPage) ret = UniWebpage2FzHg();
			else ret = UniRtf2FzHg();	//SourceFileFormat==RTF
		}
		else if(TargetEncoding == ACIP || TargetEncoding == Wylie || TargetEncoding == THDLWylie || TargetEncoding == LCWylie)
		{
			if(SourceFileFormat == UnicodeTXT)
			{
				if(TargetFileFormat == TXT) ret = UniTxt2AWtxt();
				else if(TargetFileFormat == UnicodeTXT) ret = UniTxt2AWUniTxt();
				else if(TargetFileFormat == WebPage) ret = UniTxt2AWWebpage();
				else ret = UniTxt2AWrtf(); //TargetFileFormat == RTF
			}
			else if(SourceFileFormat == WebPage)
			{
				if(TargetFileFormat == TXT) ret = UniWebpage2AWtxt();
				else if(TargetFileFormat == UnicodeTXT) ret = UniWebpage2AWUniTxt();
				else if(TargetFileFormat == WebPage) ret = UniWebpage2AWWebpage();
				else ret = UniWebpage2AWrtf(); //TargetFileFormat == RTF
			}
			else	//SourceFileFormat == RTF
			{
				if(TargetFileFormat == TXT) ret = UniRtf2AWtxt();
				else if(TargetFileFormat == UnicodeTXT) ret = UniRtf2AWUniTxt();
				else if(TargetFileFormat == WebPage) ret = UniRtf2AWWebpage();
				else ret = UniRtf2AWrtf();	//TargetFileFormat == RTF
			}
		}
		else if(TargetEncoding == Bzd || TargetEncoding == Ty || TargetEncoding == NS) //BTN  //2007
		{
			if(SourceFileFormat == UnicodeTXT)
			{
				if(TargetFileFormat == TXT) ret = UniTxt2BTNtxt();
				else if(TargetFileFormat == UnicodeTXT) ret = UniTxt2BTNUniTxt();
				else if(TargetFileFormat == WebPage) ret = UniTxt2BTNWebpage();
				else ret = UniTxt2BTNrtf(); //TargetFileFormat == RTF
			}
			else if(SourceFileFormat == WebPage)
			{
				if(TargetFileFormat == TXT) ret = UniWebpage2BTNtxt();
				else if(TargetFileFormat == UnicodeTXT) ret = UniWebpage2BTNUniTxt();
				else if(TargetFileFormat == WebPage) ret = UniWebpage2BTNWebpage();
				else ret = UniWebpage2BTNrtf(); //TargetFileFormat == RTF
			}
			else	//SourceFileFormat == RTF
			{
				if(TargetFileFormat == TXT) ret = UniRtf2BTNtxt();
				else if(TargetFileFormat == UnicodeTXT) ret = UniRtf2BTNUniTxt();
				else if(TargetFileFormat == WebPage) ret = UniRtf2BTNWebpage();
				else ret = UniRtf2BTNrtf();	//TargetFileFormat == RTF
			}
		}
		else	//TargetEncoding:  LTibetan, OldSambhota, NewSambhota, TMW, TM, TCRCBodYig, Jamyang
		{		//TargetFileFormat always should be RTF
			if(TargetFileFormat == RTF)
			{
				if(SourceFileFormat == UnicodeTXT) ret = UniTxt2TSLrtf();
				else if(SourceFileFormat == WebPage) ret = UniWebpage2TSLrtf();
				else ret = UniRtf2TSLrtf();	//TargetFileFormat==RTF
			}
			else //TargetFileFormat == WebPage
			{
				if(SourceFileFormat == UnicodeTXT) ret = UniTxt2TSLWebpage();
				else if(SourceFileFormat == WebPage) ret = UniWebpage2TSLWebpage();
				else ret = UniRtf2TSLWebpage();	//TargetFileFormat==RTF
			}
		}

		CleanUp(); return ret;
	}
	else // Others to others
	{
		CleanUp();	//Close those opened files. Do every thing again.

		//Recusivelly call this function itself.
		ret = EntryFunction(fInput, TEMP_FILE, sourceEncoding, "Unicode", TFF, StyleSheetFile);
		if(ret == ConversionDone)
			ret = EntryFunction(TEMP_FILE, fOutput, "Unicode", targetEncoding, TFF, StyleSheetFile);

		remove(TEMP_FILE);//Delete the temproral file.
		return ret;
	}

}



//////////////////////////////////////////////////////////////////////////////////


#ifdef Unix
//MAIN()
int main(int argc, char *argv[])
{
	int ret;

	if(argc != 7)
	{
		printf("Need six parameters on command line as following:\n\n");
		printf("UTFC sFile tFile sEncoding tEncoding tFileFormat ssFile\n\n");
		printf("sFile: Source file name, include the path to the location of the file;\n");
		printf("tFile: Target file name, include the path to the location of the file;\n");
		printf("sEncoding: Source file encoding;\n");
		printf("tEncoding: Target file encoding;\n");
		printf("tFileFormat: Target file format;\n");
		printf("ssFile: Style sheet file name of the source HTML file;\n");
		printf("\n");
		return 1;
	}

	m_UTFCdirectory[0] = '\0';//Path to the Converter.exe

	ret = EntryFunction(argv[1], argv[2], argv[3], argv[4], argv[5], argv[6]);
	PrintDebugInfo(ret);

	exit(ret);

	return 0;
}
#endif


#ifdef Windows
/*
//MAIN()
int main(int argc, char *argv[])
{
	int ret;

	if(argc != 7)
	{
		printf("Need six parameters on command line as following:\n\n");
		printf("UTFC sFile tFile sEncoding tEncoding tFileFormat ssFile\n\n");
		printf("sFile: Source file name, include the path to the location of the file;\n");
		printf("tFile: Target file name, include the path to the location of the file;\n");
		printf("sEncoding: Source file encoding;\n");
		printf("tEncoding: Target file encoding;\n");
		printf("tFileFormat: Target file format;\n");
		printf("ssFile: Style sheet file name of the source HTML file;\n");
		printf("\n");
		return 1;
	}

	ret = EntryFunction(argv[1], argv[2], argv[3], argv[4], argv[5], argv[6]);
	PrintDebugInfo(ret);

	exit(ret);

	return 0;
}
*/
#endif
