#ifndef INCLUDED_FONT_H
#define INCLUDED_FONT_H

class Image;

class Font {
public:
	static void create();
	static void destroy();
	static Font* instance();

	static void drawString(double, double, double, double, const char*, unsigned color = 0xffffffff);
private:
	Font();
	~Font();
	static Font* mFont;
	Image* mImage;
};

#endif