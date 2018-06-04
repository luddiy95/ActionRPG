#include "Font.h"
#include "Image.h"
#include "GameLib/GameLib.h"
using namespace GameLib;

Font* Font::mFont = 0;

void Font::create() {
	STRONG_ASSERT(!mFont);
	mFont = new Font();
}

void Font::destroy() {
	STRONG_ASSERT(mFont);
	SAFE_DELETE(mFont);
}

Font* Font::instance() {
	STRONG_ASSERT(mFont);
	return mFont;
}

void Font::drawString(double dstX, double dstY, double dstWidth, double dstHeight, const char* string, unsigned color) {
	const int charWidth = 8;
	const int charHeight = 16;
	for (unsigned i = 0; string[i] != '\0'; i++) {
		int t = string[i];
		if (t < 32 || t >= 128) { //�͈͊O
			t = 127;
		}
		t -= 32;
		int srcX = (t % 16) * charWidth;
		int srcY = (t / 16) * charHeight;
		mFont->mImage->draw(dstX + i * dstWidth, dstY, dstWidth, dstHeight, srcX, srcY, charWidth, charHeight);
	}
}

Font::Font() {
	mImage = new Image("data/image/font.dds");
}

Font::~Font() {
	SAFE_DELETE(mImage);
}