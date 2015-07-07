#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream.h>
#include <windows.h>
#include <fstream.h>



int GetIndex(int x, int y, int BytesPerRow, int Width, int BPP, char *Raster)
{
    int i_aux;

     	BytesPerRow = Width * BPP / 8;
	    BytesPerRow += (4-BytesPerRow%4) % 4;	// int alignment

        i_aux = int(char(Raster[y*BytesPerRow + x]));

        if( i_aux < 0 )
            i_aux = 256 + i_aux;

		return i_aux;
}

void ReadBMPPainting()
{
	char szFile[]="bitmap.bmp";

	BITMAPFILEHEADER bmfh;
	BITMAPINFOHEADER bmih;

		RGBQUAD * Palette;

	// Open file.
	ifstream bmpfile (szFile , ios::in | ios::binary);
	if (! bmpfile.is_open())cout<<"ERROR OPENING BMP FILE\n";		// Error opening file

	// Load bitmap fileheader & infoheader
	bmpfile.read ((char*)&bmfh,sizeof (BITMAPFILEHEADER));
	bmpfile.read ((char*)&bmih,sizeof (BITMAPINFOHEADER));

	// Check filetype signature
	if (bmfh.bfType!='MB')cout<<"FILE IS NOT A BMP FILE\n";		// File is not BMP

	// Assign some short variables:
	int BPP=bmih.biBitCount;
	int Width=bmih.biWidth;
	int Height= (bmih.biHeight>0) ? bmih.biHeight : -bmih.biHeight; // absoulte value
	int BytesPerRow = Width * BPP / 8;
	BytesPerRow += (4-BytesPerRow%4) % 4;	// int alignment

		BITMAPINFO * pbmi=(BITMAPINFO*) new char[sizeof(BITMAPINFOHEADER)+(1<<BPP)*sizeof(RGBQUAD)];
		Palette=(RGBQUAD*)((char*)pbmi+sizeof(BITMAPINFOHEADER));
		bmpfile.read ((char*)Palette,sizeof (RGBQUAD) * (1<<BPP));

	pbmi->bmiHeader=bmih;

	// Load Raster
	bmpfile.seekg (bmfh.bfOffBits,ios::beg);

	char * Raster= new char[BytesPerRow*Height];

	// (if height is positive the bmp is bottom-up, read it reversed)
	if (bmih.biHeight>0)
		for (int n=Height-1;n>=0;n--)
			bmpfile.read (Raster+BytesPerRow*n,BytesPerRow);
	else
		bmpfile.read (Raster,BytesPerRow*Height);
	bmpfile.close();


	// give some coordinates of the bmp
	int x=100,y=10,ix=0;

	// ix is the INDEX for the palette
	ix = GetIndex(x, y, BytesPerRow, Width, BPP, Raster);

	// Palette[INDEX].rgbBLABLA is the palette ;)
	cout << "palette index: " << ix << "\nblue: " << int (Palette[ix].rgbBlue) << "\ngreen: " << int (Palette[ix].rgbGreen) << "\nred: " << int (Palette[ix].rgbRed) << endl;
	
		delete pbmi;
	system("pause");
}


int main(int argc, char* argv[])
{

		ReadBMPPainting();

		return 0;
}
