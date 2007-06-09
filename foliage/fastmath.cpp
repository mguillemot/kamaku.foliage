#include <iostream>
#include <cmath>
#include "basic_types.hpp"
#include "fastmath.hpp"

#ifdef __PPC__
	#include <xparameters.h>
	Foliage::Fixed *Foliage::FastMath::atan_t0 = (Foliage::Fixed *)(XPAR_PLB_BRAM_IF_CNTLR_1_BASEADDR + 1024 * sizeof(Foliage::Fixed) * 0);
	Foliage::Fixed *Foliage::FastMath::atan_t1 = (Foliage::Fixed *)(XPAR_PLB_BRAM_IF_CNTLR_1_BASEADDR + 1024 * sizeof(Foliage::Fixed) * 1);
	Foliage::Fixed *Foliage::FastMath::atan_t2 = (Foliage::Fixed *)(XPAR_PLB_BRAM_IF_CNTLR_1_BASEADDR + 1024 * sizeof(Foliage::Fixed) * 2);
	Foliage::Fixed *Foliage::FastMath::atan_t3 = (Foliage::Fixed *)(XPAR_PLB_BRAM_IF_CNTLR_1_BASEADDR + 1024 * sizeof(Foliage::Fixed) * 3);
	Foliage::Fixed *Foliage::FastMath::cos_t = (Foliage::Fixed *)(XPAR_PLB_BRAM_IF_CNTLR_1_BASEADDR + 1024 * sizeof(Foliage::Fixed) * 4);
#else
	Foliage::Fixed *Foliage::FastMath::atan_t0 = new Foliage::Fixed[1024];
	Foliage::Fixed *Foliage::FastMath::atan_t1 = new Foliage::Fixed[1024];
	Foliage::Fixed *Foliage::FastMath::atan_t2 = new Foliage::Fixed[1024];
	Foliage::Fixed *Foliage::FastMath::atan_t3 = new Foliage::Fixed[1024];
	Foliage::Fixed *Foliage::FastMath::cos_t = new Foliage::Fixed[1024];
#endif

Foliage::Fixed Foliage::FastMath::cos(const Foliage::Fixed x)
{
	if (x < Sint16(0))
	{
		return cos(x.opposite());
	}	
	if (x > F_TWOPI)
	{
		return cos(x - F_TWOPI);
	}
	if (x > F_PI)
	{
		return cos(x - F_PI).opposite();
	}
	if (x > F_PI_2)
	{
		return cos(F_PI - x).opposite();
	}
	Fixed x2(x);
	x2 *= F_2000_OVER_PI;
	Sint16 i = Sint16(x2);
	return Foliage::FastMath::cos_t[i];
}

Foliage::Fixed Foliage::FastMath::sin(const Foliage::Fixed x)
{
	return Foliage::FastMath::cos(F_PI_2 - x);
}

Foliage::Fixed Foliage::FastMath::atan(const Foliage::Fixed x)
{
	if (x < Sint16(0))
	{
		return atan(x.opposite()).opposite();
	}
	if (x < F_0_DOT_1)
	{
		Sint32 i = int(x * F_10000);
		return Foliage::FastMath::atan_t0[i];
	}
	else if (x < F_1)
	{
		Sint32 i = int(x * F_1000);
		return Foliage::FastMath::atan_t1[i];
	}
	else if (x < F_10)
	{
		Sint32 i = int(x * F_100);
		return Foliage::FastMath::atan_t2[i];
	}
	else if (x < F_100)
	{
		Sint32 i = int(x * F_10);
		return Foliage::FastMath::atan_t3[i];
	}
	else
	{
		return F_PI_2;
	}
}

Foliage::Direction Foliage::FastMath::direction(const Sint16 dx, const Sint16 dy)
{
	return direction(Foliage::Fixed(dx), Foliage::Fixed(dy));
}

Foliage::Direction Foliage::FastMath::direction(const Foliage::Fixed dx, const Foliage::Fixed dy)
{
	if (dx == F_0)
	{
		if (dy > F_0)
		{
			return DIR_S;
		}
		else
		{
			return DIR_N;
		}
	}
	else if (dx > F_0)
	{
		const Foliage::Fixed tt = dy.opposite() / dx;
		if (tt <= F_TAN_7_PI_OVER_16.opposite())
		{
			return Foliage::Direction(12);
		}
		else if (tt <= F_TAN_5_PI_OVER_16.opposite())
		{
			return Foliage::Direction(13);
		}
		else if (tt <= F_TAN_3_PI_OVER_16.opposite())
		{
			return Foliage::Direction(14);
		}
		else if (tt <= F_TAN_PI_OVER_16.opposite())
		{
			return Foliage::Direction(15);
		}
		else if (tt <= F_TAN_PI_OVER_16)
		{
			return Foliage::Direction(0);
		}
		else if (tt <= F_TAN_3_PI_OVER_16)
		{
			return Foliage::Direction(1);
		}
		else if (tt <= F_TAN_5_PI_OVER_16)
		{
			return Foliage::Direction(2);
		}
		else if (tt <= F_TAN_7_PI_OVER_16)
		{
			return Foliage::Direction(3);
		}
		else
		{
			return Foliage::Direction(4);
		}
	}
	else
	{
		const Foliage::Fixed tt = dy / dx;
		if (tt <= F_TAN_7_PI_OVER_16.opposite())
		{
			return Foliage::Direction(12);
		}
		else if (tt <= F_TAN_5_PI_OVER_16.opposite())
		{
			return Foliage::Direction(11);
		}
		else if (tt <= F_TAN_3_PI_OVER_16.opposite())
		{
			return Foliage::Direction(10);
		}
		else if (tt <= F_TAN_PI_OVER_16.opposite())
		{
			return Foliage::Direction(9);
		}
		else if (tt <= F_TAN_PI_OVER_16)
		{
			return Foliage::Direction(8);
		}
		else if (tt <= F_TAN_3_PI_OVER_16)
		{
			return Foliage::Direction(7);
		}
		else if (tt <= F_TAN_5_PI_OVER_16)
		{
			return Foliage::Direction(6);
		}
		else if (tt <= F_TAN_7_PI_OVER_16)
		{
			return Foliage::Direction(5);
		}
		else
		{
			return Foliage::Direction(4);
		}
	}
}

void Foliage::FastMath::init()
{
	for (Sint32 i = 0; i < 1000; i++)
	{
		atan_t0[i] = Foliage::Fixed(std::atan(i * (0.1f / 1000)));
		atan_t1[i] = Foliage::Fixed(std::atan(i * (1.0f / 1000)));
		atan_t2[i] = Foliage::Fixed(std::atan(i * (10.0f / 1000)));
		atan_t3[i] = Foliage::Fixed(std::atan(i * (100.0f / 1000)));
		cos_t[i] = Foliage::Fixed(std::cos(i * ((float)M_PI_2 / 1000)));
	}
}
