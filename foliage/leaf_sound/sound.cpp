#include <iostream>
#include "sound.hpp"
#include "../leaf_io/file.hpp"

#ifdef __PPC__
	#include <sysace_stdio.h>
	#define FILE SYSACE_FILE
	#define fopen sysace_fopen
	#define fread sysace_fread
	#define fclose sysace_fclose
#else
	#include <cstdio>
#endif

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
	
	Foliage::File infile(filename);

	// Read the RIFF chunk
	riffchunk.chunkID = infile.readUint32BE();
	riffchunk.chunkSize = infile.readUint32LE();
	riffchunk.format = infile.readUint32BE();
	
	// Read the FMT chunk 
	fmtchunk.subchunkID = infile.readUint32BE();
	fmtchunk.subchunkSize = infile.readUint32LE();
	fmtchunk.audioFormat = infile.readUint16();
	fmtchunk.numChannels = infile.readUint16();
	fmtchunk.sampleRate = infile.readUint32LE();
	fmtchunk.byteRate = infile.readUint32LE();
	fmtchunk.blockAlign = infile.readUint16();
	fmtchunk.bitsPerSample = infile.readUint16();
	
	// Read the DATA chunk 
	datachunk.subchunkID = infile.readUint32BE();
	datachunk.subchunkSize = infile.readUint32LE();
	
	_samplesNb = datachunk.subchunkSize >> 2;
	if (_samplesNb % 256 != 0)
	{
		_samplesNb = ((_samplesNb >> 8) + 1) << 8; 
	}
	_samples = new Uint32[_samplesNb];
	if (_samples == NULL)
	{
		std::cout << "Not enough memory to load sound." << std::endl;
		exit(1);
	}
	else
	{		
		Sint32 curSample = 0;
		Uint32 totalRead = 0;
		while (totalRead < datachunk.subchunkSize)
		{
			Uint32 numRead = infile.read(readBuffer, 2048);
			totalRead += numRead;
			for (Uint32 i = 0; i < numRead; i += 4)
			{
				_samples[curSample++] = ((((((readBuffer[i+3] << 8) | readBuffer[i+2]) << 8) | readBuffer[i+1]) << 8) | readBuffer[i]);
			}
		}
		while (curSample < _samplesNb)
		{
			_samples[curSample++] = 0;
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

Sint32 Foliage::Sound::getCurrentSampleNb() const
{
	return _curSample;
}
