#ifndef _FOLIAGE__FILE
#define _FOLIAGE__FILE

#include <string>
#ifdef __PPC__
	#include <sysace_stdio.h>
	#include <ivga.h>
	#define FILE SYSACE_FILE
	#define fopen sysace_fopen
	#define fread sysace_fread
	#define fclose sysace_fclose
#else
	#include <cstdio>
#endif

namespace Foliage
{

	class File
	{
	public:
		File(const std::string filename)
		{
			#ifdef __PPC__
				_file = fopen(filename.c_str(), "r");
			#else
				std::string filename2 = "../game/resources/" + filename;
				_file = fopen(filename2.c_str(), "rb");
			#endif
			if (_file == NULL)
			{
				std::cout << "Could not open file " << filename << std::endl;
				exit(1);
			}
		}

		~File()
		{
			fclose(_file);
		}

		Uint32 read(Uint8* buffer, const Sint32 nBytes)
		{
			return (Uint32)fread(buffer, 1, nBytes, _file);
		}

		Uint16 readUint16()
		{
			Uint8 readBuffer[2];
			fread(readBuffer, 1, 2, _file);
			return ((readBuffer[1] << 8) | readBuffer[0]);
		}

		Uint32 readUint32LE()
		{
			Uint8 readBuffer[4];
			fread(readBuffer, 1, 4, _file);
			return ((((((readBuffer[3] << 8) | readBuffer[2]) << 8) | readBuffer[1]) << 8) | readBuffer[0]);
		}

		Uint32 readUint32BE()
		{
			Uint8 readBuffer[4];
			fread(readBuffer, 1, 4, _file);
			return ((((((readBuffer[0] << 8) | readBuffer[1]) << 8) | readBuffer[2]) << 8) | readBuffer[3]);
		}

	protected:
		FILE* _file;
	};
	
}

#endif //_FOLIAGE__FILE
