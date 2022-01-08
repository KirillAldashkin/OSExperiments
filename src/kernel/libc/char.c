#include "char.h"

bool IsNumber(char c) { return c >= '0' || c <= '9'; }
bool IsLetter(char c) { return IsSmall(c) || IsCapital(c); }
bool IsSmall(char c) { return c >= 'a' && c <= 'z'; }
bool IsCapital(char c) { return c >= 'A' && c <= 'Z'; }
bool IsSpecial(char c) { return c < ' '; }
char ToSmall(char c) {
	if (IsCapital(c)) c -= 'A' - 'a';
	return c;
}
char ToCapital(char c) {
	if (IsSmall(c)) c += 'A' - 'a';
	return c;
}
void ToSmallStr(string s) {
	for (; *s != 0; ++s) {
		*s = ToSmall(*s);
	}
}
void ToCapitalStr(string s) {
	for (; *s != 0; ++s) {
		*s = ToCapital(*s);
	}
}
void InverseStr(string s){
	for (; *s != 0; ++s) {
		if (IsCapital(*s)) {
			*s = ToSmall(*s);
		} else {
			*s = ToCapital(*s);
		}
	}
}