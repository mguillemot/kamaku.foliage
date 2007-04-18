#include <iostream>
#include "sound.hpp"

#ifdef __PPC__
	#include <sysace_stdio.h>
	#define FILE SYSACE_FILE
	#define fopen sysace_fopen
	#define fread sysace_fread
	#define fclose sysace_fclose
#else
	#include <cstdio>
#endif

inline Uint16 ReadUShort(FILE *fptr)
{
	Uint8 readBuffer[2];
	fread(readBuffer, 1, 2, fptr);
	return ((readBuffer[1] << 8) | readBuffer[0]);
}

inline Uint32 ReadUIntLil(FILE *fptr)
{
	Uint8 readBuffer[4];
	fread(readBuffer, 1, 4, fptr);
	return ((((((readBuffer[3] << 8) | readBuffer[2]) << 8) | readBuffer[1]) << 8) | readBuffer[0]);
}

inline Uint32 ReadUIntBig(FILE *fptr)
{
	Uint8 readBuffer[4];
	fread(readBuffer, 1, 4, fptr);
	return ((((((readBuffer[0] << 8) | readBuffer[1]) << 8) | readBuffer[2]) << 8) | readBuffer[3]);
}

struct RIFFCHUNK
{
   Uint32 chunkID;
   Uint32 chunkSize;
   Uint32 format;
};

struct FMTCHUNK 
{
   Uint32 subchunkID;
   Uint32 subchunkSize;
   Uint16 audioFormat;
   Uint16 numChannels;
   Uint32 sampleRate;
   Uint32 byteRate;
   Uint16 blockAlign;
   Uint16 bitsPerSample;
   Uint16 extraParamSize;
};

struct DATACHUNK 
{
   Uint32 subchunkID;
   Uint32 subchunkSize;
};

Foliage::Sound::Sound(const std::string filename)
	: _curSample(0)
{
	Uint8 readBuffer[2048];
	RIFFCHUNK riffchunk;
	FMTCHUNK fmtchunk;
	DATACHUNK datachunk;
	FILE *infile = fopen(filename.c_str(), "r");
	if (infile == NULL)
	{
		std::cout << "Couldn't open file " << filename << std::endl;
	}
	else
	{
		// Read the RIFF chunk
		riffchunk.chunkID = ReadUIntBig(infile);
		riffchunk.chunkSize = ReadUIntLil(infile);
		riffchunk.format = ReadUIntBig(infile);
		
		// Read the FMT chunk 
		fmtchunk.subchunkID = ReadUIntBig(infile);
		fmtchunk.subchunkSize = ReadUIntLil(infile);
		fmtchunk.audioFormat = ReadUShort(infile);
		fmtchunk.numChannels = ReadUShort(infile);
		fmtchunk.sampleRate = ReadUIntLil(infile);
		fmtchunk.byteRate = ReadUIntLil(infile);
		fmtchunk.blockAlign = ReadUShort(infile);
		fmtchunk.bitsPerSample = ReadUShort(infile);
		
		// Read the DATA chunk 
		datachunk.subchunkID = ReadUIntBig(infile);
		datachunk.subchunkSize = ReadUIntLil(infile);
		
		_samplesNb = datachunk.subchunkSize >> 2;
		if (_samplesNb % 256 != 0)
		{
			_samplesNb = ((_samplesNb >> 8) + 1) << 8; 
		}
		_samples = new Uint32[_samplesNb];
		if (_samples == NULL)
		{
			std::cout << "Not enough memory to load sound." << std::endl;
		}
		else
		{		
			Sint32 curSample = 0;
			Uint32 totalRead = 0;
			while (totalRead < datachunk.subchunkSize)
			{
				Sint32 numRead = Sint32(fread(readBuffer, 1, 2048, infile));
				totalRead += numRead;
				for (Sint32 i = 0; i < numRead; i += 4)
				{
					_samples[curSample++] = ((((((readBuffer[i+3] << 8) | readBuffer[i+2]) << 8) | readBuffer[i+1]) << 8) | readBuffer[i]);
				}
			}
			fclose(infile);
			while (curSample < _samplesNb)
			{
				_samples[curSample++] = 0;
			}
		}
	}
}

Foliage::Sound::~Sound()
{
	delete[] _samples;
}

Uint32 Foliage::Sound::getNextSample()
{
	if (_curSample < _samplesNb)
	{
		return _samples[_curSample++];
	}
	else
	{
		return 0;
	}
}

void Foliage::Sound::rewind()
{
	_curSample = 0;
}

Sint32 Foliage::Sound::getSamplesNb() const
{
	return _samplesNb;
}

bool Foliage::Sound::ended() const
{
	return (_curSample >= _samplesNb);
}
