#ifndef INCLUDED_IMAGE_H
#define INCLUDED_IMAGE_H

#include "Vector2.h"

namespace GameLib{
	class Texture;
}

class Image {
public:
	Image(const char* fileName);
	~Image();
	void draw(int dstX, int dstY, int srcX, int srcY, int width, int height) const;
	void draw(const Vector2&, const Vector2&, const Vector2&, const Vector2&,
		double srcX,
		double srcY, 
		int width,
		int height) const; //˜c‚ñ‚¾Œ`
	void draw(double dstX, double dstY, double dstWidth, double dstHeight,
		double srcX, double srcY, double srcWidth, double srcHeight) const;
	void draw(double dstX, double dstY, double srcX, double srcY) const;
	int width() const;
	int height() const;
	int textureWidth() const;
	int textureHeight() const;
	unsigned* data() const;
private:
	unsigned* mData;
	GameLib::Texture* mTexture;
	int mWidth;
	int mHeight;
	int mTextureWidth;
	int mTextureHeight;
};

#endif