#define AttrBlink          0b10000000

#define AttrBackBlack      0b00000000
#define AttrBackBlue       0b00010000
#define AttrBackGreen      0b00100000
#define AttrBackCyan       0b00110000
#define AttrBackRed        0b01000000
#define AttrBackMagenta    0b01010000
#define AttrBackBrown      0b01100000
#define AttrBackGray       0b01110000

#define AttrTextBlack      0b00000000
#define AttrTextBlue       0b00000001
#define AttrTextGreen      0b00000010
#define AttrTextCyan       0b00000011
#define AttrTextRed        0b00000100
#define AttrTextMagenta    0b00000101
#define AttrTextBrown      0b00000110
#define AttrTextGray       0b00000111
#define AttrTextDarkGray   0b00001000
#define AttrTextLightBlue  0b00001001
#define AttrTextLightGreen 0b00001010
#define AttrTextLightCyan  0b00001011
#define AttrTextLightRed   0b00001100
#define AttrTextPink       0b00001101
#define AttrTextYellow     0b00001110
#define AttrTextWhite      0b00001111

typedef struct
{
    char text;
    char attr;
} ConsoleChar;

#define textVGAPtr (ConsoleChar(*)[25][80])0xb8000
#define textVGA (*textVGAPtr)