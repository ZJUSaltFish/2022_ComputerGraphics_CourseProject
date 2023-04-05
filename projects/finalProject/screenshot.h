#pragma once
#include <iostream>
#include <ostream>
#include <fstream>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct BmpFileHeader
{
	unsigned short _fileType;//must be "BM" in ASCII
	unsigned long _fileSize;//size of file, in bytes
	unsigned short _fileReserved1;//must be 0
	unsigned short _fileReserved2;//must be 0
	unsigned long _fileOffsetBits;//offset from this struct to bitmap data. in bytes
};

struct BmpBitmapHeader
{
	unsigned long _infoSize;//size of this struct, in 
	long _bitmapWidth;
	long _bitmapHeight;
	unsigned short _bitmapPlanes;
	unsigned short _bitsPerPixel;
	unsigned long _bitmapComressionType;//0: no compresison; 1:RLE8; 2:RLE4
	unsigned long _bitmapSize;//size of image in byte
	long _pixelsPerMeterX;
	long _pixelsPerMeterY;
	unsigned long _numOfUsedColors;//actually used indices of color palette: if = 0: all
	unsigned long _numOfImportantColors;//important used indices of color palette. if = 0: all
};

struct RGBColor
{
	char B;//in WINDOWS bmp files, colors are stored in BGR but not RGB
	char G;
	char R;
};

void exportBmp(std::string _path, std::string _fileName, char* _colorBuffer, int _width, int _height)
{
	const int _colorBufferSize = _width * _height * 3*sizeof(char);
	
	BmpFileHeader _fileHeader;
	_fileHeader._fileType = 0x4D42; //0x4D = 'B', 0x42 = 'M'
	_fileHeader._fileSize = sizeof(BmpFileHeader) + sizeof(BmpBitmapHeader) + _colorBufferSize;
	_fileHeader._fileReserved1 = 0;
	_fileHeader._fileReserved2 = 0;
	_fileHeader._fileOffsetBits = sizeof(BmpFileHeader) + sizeof(BmpBitmapHeader);

	BmpBitmapHeader _bitmapHeader;
	_bitmapHeader._infoSize = sizeof(BmpBitmapHeader);
	_bitmapHeader._bitmapWidth = _width;
	_bitmapHeader._bitmapHeight = _height;
	_bitmapHeader._bitmapPlanes = 1;
	_bitmapHeader._bitsPerPixel = 24;//8 x 3
	_bitmapHeader._bitmapComressionType = 0;//no compression
	_bitmapHeader._bitmapSize = 0;
	_bitmapHeader._pixelsPerMeterX = 7500; //75ppc
	_bitmapHeader._pixelsPerMeterY = 7500;
	_bitmapHeader._numOfUsedColors = 0;//use all colors
	_bitmapHeader._numOfImportantColors = 0;//all important
	/*
	FILE* _fp = NULL;

	fopen_s(&_fp, &(_path + _fileName + ".bmp")[0], "wb");//create file
	if (_fp == NULL)std::cout << "ERROR: failed to create screenshot" << std::endl;
	else {
		std::cout << fwrite(&_fileHeader, sizeof(BmpFileHeader), 1, _fp) << std::endl;
		std::cout << fwrite(&_bitmapHeader, sizeof(BmpBitmapHeader), 1, _fp) << std::endl;
		//std::cout << fwrite(&_colorBuffer, sizeof(char), _width * _height, _fp) << std::endl;//some bugs here
		fclose(_fp);
		std::ofstream _outfile;
		_outfile.open(_path + _fileName + ".bmp", std::ios::out);
		_outfile<<	_colorBuffer;
		_outfile.close();
	}
	*/
	
	std::ofstream _outfile;
	_outfile.open(_path + _fileName + ".bmp", std::ios::binary );
	_outfile.write((char*)&_fileHeader, 14);
	_outfile.write((char*)&_bitmapHeader, _bitmapHeader._infoSize);
	_outfile.write((char*)&_colorBuffer, _width * _height * 3 );
	_outfile.close();
}