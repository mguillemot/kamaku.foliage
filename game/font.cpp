#include <iostream>
#include "font.hpp"

Font::Font()
{
	_characters['a'] = Foliage::BmpLoader::loadBmp("a.bmp");
	_characterSize = _characters['a']->getSize();
	Foliage::Instancizator::instancize(_characters['a']);
	_characters['b'] = Foliage::BmpLoader::loadBmp("b.bmp");
	Foliage::Instancizator::instancize(_characters['b']);
	_characters['c'] = Foliage::BmpLoader::loadBmp("c.bmp");
	Foliage::Instancizator::instancize(_characters['c']);
	_characters['d'] = Foliage::BmpLoader::loadBmp("d.bmp");
	Foliage::Instancizator::instancize(_characters['d']);
	_characters['e'] = Foliage::BmpLoader::loadBmp("e.bmp");
	Foliage::Instancizator::instancize(_characters['e']);
	_characters['f'] = Foliage::BmpLoader::loadBmp("f.bmp");
	Foliage::Instancizator::instancize(_characters['f']);
	_characters['g'] = Foliage::BmpLoader::loadBmp("g.bmp");
	Foliage::Instancizator::instancize(_characters['g']);
	_characters['h'] = Foliage::BmpLoader::loadBmp("h.bmp");
	Foliage::Instancizator::instancize(_characters['h']);
	_characters['i'] = Foliage::BmpLoader::loadBmp("i.bmp");
	Foliage::Instancizator::instancize(_characters['i']);
	_characters['j'] = Foliage::BmpLoader::loadBmp("j.bmp");
	Foliage::Instancizator::instancize(_characters['j']);
	_characters['k'] = Foliage::BmpLoader::loadBmp("k.bmp");
	Foliage::Instancizator::instancize(_characters['k']);
	_characters['l'] = Foliage::BmpLoader::loadBmp("l.bmp");
	Foliage::Instancizator::instancize(_characters['l']);
	_characters['m'] = Foliage::BmpLoader::loadBmp("m.bmp");
	Foliage::Instancizator::instancize(_characters['m']);
	_characters['n'] = Foliage::BmpLoader::loadBmp("n.bmp");
	Foliage::Instancizator::instancize(_characters['n']);
	_characters['o'] = Foliage::BmpLoader::loadBmp("o.bmp");
	Foliage::Instancizator::instancize(_characters['o']);
	_characters['p'] = Foliage::BmpLoader::loadBmp("p.bmp");
	Foliage::Instancizator::instancize(_characters['p']);
	_characters['q'] = Foliage::BmpLoader::loadBmp("q.bmp");
	Foliage::Instancizator::instancize(_characters['q']);
	_characters['r'] = Foliage::BmpLoader::loadBmp("r.bmp");
	Foliage::Instancizator::instancize(_characters['r']);
	_characters['s'] = Foliage::BmpLoader::loadBmp("s.bmp");
	Foliage::Instancizator::instancize(_characters['s']);
	_characters['t'] = Foliage::BmpLoader::loadBmp("t.bmp");
	Foliage::Instancizator::instancize(_characters['t']);
	_characters['u'] = Foliage::BmpLoader::loadBmp("u.bmp");
	Foliage::Instancizator::instancize(_characters['u']);
	_characters['v'] = Foliage::BmpLoader::loadBmp("v.bmp");
	Foliage::Instancizator::instancize(_characters['v']);
	_characters['w'] = Foliage::BmpLoader::loadBmp("w.bmp");
	Foliage::Instancizator::instancize(_characters['w']);
	_characters['x'] = Foliage::BmpLoader::loadBmp("x.bmp");
	Foliage::Instancizator::instancize(_characters['x']);
	_characters['y'] = Foliage::BmpLoader::loadBmp("y.bmp");
	Foliage::Instancizator::instancize(_characters['y']);
	_characters['z'] = Foliage::BmpLoader::loadBmp("z.bmp");
	Foliage::Instancizator::instancize(_characters['z']);
	_characters['A'] = Foliage::BmpLoader::loadBmp("a.bmp");
	_characters['B'] = Foliage::BmpLoader::loadBmp("b.bmp");
	_characters['C'] = Foliage::BmpLoader::loadBmp("c.bmp");
	_characters['D'] = Foliage::BmpLoader::loadBmp("d.bmp");
	_characters['E'] = Foliage::BmpLoader::loadBmp("e.bmp");
	_characters['F'] = Foliage::BmpLoader::loadBmp("f.bmp");
	_characters['G'] = Foliage::BmpLoader::loadBmp("g.bmp");
	_characters['H'] = Foliage::BmpLoader::loadBmp("h.bmp");
	_characters['I'] = Foliage::BmpLoader::loadBmp("i.bmp");
	_characters['J'] = Foliage::BmpLoader::loadBmp("j.bmp");
	_characters['K'] = Foliage::BmpLoader::loadBmp("k.bmp");
	_characters['L'] = Foliage::BmpLoader::loadBmp("l.bmp");
	_characters['M'] = Foliage::BmpLoader::loadBmp("m.bmp");
	_characters['N'] = Foliage::BmpLoader::loadBmp("n.bmp");
	_characters['O'] = Foliage::BmpLoader::loadBmp("o.bmp");
	_characters['P'] = Foliage::BmpLoader::loadBmp("p.bmp");
	_characters['Q'] = Foliage::BmpLoader::loadBmp("q.bmp");
	_characters['R'] = Foliage::BmpLoader::loadBmp("r.bmp");
	_characters['S'] = Foliage::BmpLoader::loadBmp("s.bmp");
	_characters['T'] = Foliage::BmpLoader::loadBmp("t.bmp");
	_characters['U'] = Foliage::BmpLoader::loadBmp("u.bmp");
	_characters['V'] = Foliage::BmpLoader::loadBmp("v.bmp");
	_characters['W'] = Foliage::BmpLoader::loadBmp("w.bmp");
	_characters['X'] = Foliage::BmpLoader::loadBmp("x.bmp");
	_characters['Y'] = Foliage::BmpLoader::loadBmp("y.bmp");
	_characters['Z'] = Foliage::BmpLoader::loadBmp("z.bmp");
	//_characters['.'] = Foliage::BmpLoader::loadBmp("s_dot.bmp");
	//Foliage::Instancizator::instancize(_characters['.']);
	_characters[':'] = Foliage::BmpLoader::loadBmp("s_col.bmp");
	Foliage::Instancizator::instancize(_characters[':']);
	/*
	_characters[';'] = Foliage::BmpLoader::loadBmp("s_sco.bmp");
	Foliage::Instancizator::instancize(_characters[';']);
	_characters['!'] = Foliage::BmpLoader::loadBmp("s_exc.bmp");
	Foliage::Instancizator::instancize(_characters['!']);
	_characters['?'] = Foliage::BmpLoader::loadBmp("s_int.bmp");
	Foliage::Instancizator::instancize(_characters['?']);
	_characters['+'] = Foliage::BmpLoader::loadBmp("s_plu.bmp");
	Foliage::Instancizator::instancize(_characters['+']);
	_characters['-'] = Foliage::BmpLoader::loadBmp("s_min.bmp");
	Foliage::Instancizator::instancize(_characters['-']);
	_characters['('] = Foliage::BmpLoader::loadBmp("s_pao.bmp");
	Foliage::Instancizator::instancize(_characters['(']);
	_characters[')'] = Foliage::BmpLoader::loadBmp("s_pac.bmp");
	Foliage::Instancizator::instancize(_characters[')']);
	_characters['['] = Foliage::BmpLoader::loadBmp("s_cro.bmp");
	Foliage::Instancizator::instancize(_characters['[']);
	_characters[']'] = Foliage::BmpLoader::loadBmp("s_crc.bmp");
	Foliage::Instancizator::instancize(_characters[']']);
	_characters['{'] = Foliage::BmpLoader::loadBmp("s_bro.bmp");
	Foliage::Instancizator::instancize(_characters['{']);
	_characters['}'] = Foliage::BmpLoader::loadBmp("s_brc.bmp");
	Foliage::Instancizator::instancize(_characters['}']);
	_characters['='] = Foliage::BmpLoader::loadBmp("s_equ.bmp");
	Foliage::Instancizator::instancize(_characters['=']);
	_characters['*'] = Foliage::BmpLoader::loadBmp("s_sta.bmp");
	Foliage::Instancizator::instancize(_characters['*']);
	_characters['#'] = Foliage::BmpLoader::loadBmp("s_die.bmp");
	Foliage::Instancizator::instancize(_characters['#']);
	*/
	_characters[' '] = Foliage::BmpLoader::loadBmp("s_spa.bmp");
	Foliage::Instancizator::instancize(_characters[' ']);
	_characters['0'] = Foliage::BmpLoader::loadBmp("0.bmp");
	Foliage::Instancizator::instancize(_characters['0']);
	_characters['1'] = Foliage::BmpLoader::loadBmp("1.bmp");
	Foliage::Instancizator::instancize(_characters['1']);
	_characters['2'] = Foliage::BmpLoader::loadBmp("2.bmp");
	Foliage::Instancizator::instancize(_characters['2']);
	_characters['3'] = Foliage::BmpLoader::loadBmp("3.bmp");
	Foliage::Instancizator::instancize(_characters['3']);
	_characters['4'] = Foliage::BmpLoader::loadBmp("4.bmp");
	Foliage::Instancizator::instancize(_characters['4']);
	_characters['5'] = Foliage::BmpLoader::loadBmp("5.bmp");
	Foliage::Instancizator::instancize(_characters['5']);
	_characters['6'] = Foliage::BmpLoader::loadBmp("6.bmp");
	Foliage::Instancizator::instancize(_characters['6']);
	_characters['7'] = Foliage::BmpLoader::loadBmp("7.bmp");
	Foliage::Instancizator::instancize(_characters['7']);
	_characters['8'] = Foliage::BmpLoader::loadBmp("8.bmp");
	Foliage::Instancizator::instancize(_characters['8']);
	_characters['9'] = Foliage::BmpLoader::loadBmp("9.bmp");
	Foliage::Instancizator::instancize(_characters['9']);

	std::cout << "Font loaded." << std::endl;
}

Font::~Font()
{
	// TODO: delete character surfaces (?)
}

void Font::drawString(const std::string s, const Foliage::Point p) const
{
	Foliage::Point cursor = p;
	for (std::string::const_iterator i = s.begin(); i != s.end(); ++i)
	{
		const char c = *i;
		CharactersMap::const_iterator surf = _characters.find(c);
		if (surf != _characters.end())
		{
			(*surf).second->drawAt(cursor);
			cursor.x += _characterSize.w + 1;
			if ((cursor.x + _characterSize.w) >= Foliage::Screen::Width)
			{
				// remaining of the string is off-screen
				return;
			}
		}
	}
}
