#include <iostream>
#include "foliage.hpp"

#ifdef __PPC__
	#include <xintc_l.h>
	#include <xexception_l.h>
	#include <xcache_l.h>

	void XCache_EnableWriteThrough(Xuint32 adr)
	{
		asm("mtdcwr 3");
		asm("sync");
		asm("isync");
	}
#endif

void Foliage::init()
{
	std::cout << std::endl << "Loading FOLIAGE system ";
	#ifdef __PPC__
		std::cout << "(for PPC405)..." << std::endl;
	#else
		std::cout << "(for x86)..." << std::endl;
	#endif

	#ifdef __PPC__
		XCache_EnableICache(0xC0000000);
		XCache_EnableDCache(0xC0000000);
		XCache_EnableWriteThrough(0xC0000000);
		flushDCache();
		std::cout << "FOLIAGE: processor caches enabled." << std::endl;
		
		XExc_Init();
		XExc_RegisterHandler(XEXC_ID_NON_CRITICAL_INT, (XExceptionHandler)XIntc_DeviceInterruptHandler, (void *)XPAR_OPB_INTC_0_DEVICE_ID);
		std::cout << "FOLIAGE: interrupt vectors initialized." << std::endl;
	#else
		if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
		{ 
			std::cout << "Could not initialize SDL." << std::endl;
			exit(1);
		}
		std::cout << "FOLIAGE: SDL initialized." << std::endl;
	#endif

	Foliage::FastMath::init();
	std::cout << "FOLIAGE: fast math tables initialized." << std::endl;

	std::cout << "LEAF_graphic:" << std::endl;
	Foliage::Screen::init();
	std::cout << "LEAF_io:" << std::endl;
	Foliage::InputManager::init();
	std::cout << "LEAF_sound:" << std::endl;
	Foliage::SoundManager::init();

	#ifdef __PPC__
		XIntc_mMasterEnable(XPAR_OPB_INTC_0_BASEADDR);
		XIntc_mEnableIntr(XPAR_OPB_INTC_0_BASEADDR, 
			XPAR_ISUPERDMA_0_ISUPERDMA_INTERRUPT_MASK
			| XPAR_IVGA_0_VGA_INTERRUPT_MASK
			| XPAR_PS2_PORTS_SYS_INTR1_MASK
			| XPAR_AUDIO_CODEC_INTERRUPT_MASK
			| XPAR_ISPRITE_0_ISPRITE_INTERRUPT_MASK);
		XExc_mEnableExceptions(XEXC_NON_CRITICAL);
		std::cout << "FOLIAGE: interrupts activated." << std::endl;
	#endif
	std::cout << "FOLIAGE: ready!" << std::endl << std::endl;
}

void Foliage::shutDown()
{
	std::cout << "FOLIAGE shut down!" << std::endl << std::endl;
}
