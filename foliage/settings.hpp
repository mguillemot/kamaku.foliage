#ifndef _FOLIAGE__SETTINGS
#define _FOLIAGE__SETTINGS

#define SCREEN_WIDTH    320
#define SCREEN_HEIGHT   240
#define SCREEN_DIAGONAL 400
#define SCREEN_FPS      60

// undef for normal "yoko" mode
#define SCREEN_VERTICAL 

// undef for normal 15 kHz mode
//#define SCREEN_VGA

// whether to store surfaces data in colomns rather then in rows
#ifdef __PPC__
	#ifdef SCREEN_VERTICAL
		#define REARRANGE_SURFACES
	#endif
#endif

#endif //_FOLIAGE__SETTINGS
