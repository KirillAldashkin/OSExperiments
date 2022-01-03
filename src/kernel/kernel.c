#include "textvga.c"

void main() {
	TextVGA[1][0].text = 'K';
	TextVGA[1][0].attr = AttrBackGreen | AttrTextLightGreen;
	TextVGA[1][1].text = 'e';
	TextVGA[1][1].attr = AttrBackRed | AttrTextLightRed;
	TextVGA[1][2].text = 'r';
	TextVGA[1][2].attr = AttrBackMagenta | AttrTextPink;
	TextVGA[1][3].text = 'n';
	TextVGA[1][3].attr = AttrBackBlue | AttrTextLightBlue;
	TextVGA[1][4].text = 'e';
	TextVGA[1][4].attr = AttrBackBrown | AttrTextYellow;
	TextVGA[1][5].text = 'l';
	TextVGA[1][5].attr = AttrBackCyan | AttrTextLightCyan;
}