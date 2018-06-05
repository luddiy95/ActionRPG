#include "File.h"
#include "Image.h"
#include "GameLib/Framework.h"
using namespace GameLib;

Image::Image(const char* fileName) : 
mData(0),
mTexture(0),
mTextureWidth(0),
mTextureHeight(0),
mWidth(0),
mHeight(0){
	File imageFile(fileName);
	mWidth = imageFile.getUnsigned(16);
	mHeight = imageFile.getUnsigned(12);
	mData = new unsigned[mWidth * mHeight];
	for (int i = 0; i < mWidth * mHeight; i++) {
		mData[i] = imageFile.getUnsigned(128 + i * 4);
	}

	mTextureWidth = 1; mTextureHeight = 1;
	while (mTextureWidth < mWidth) {
		mTextureWidth *= 2;
	}
	while (mTextureHeight < mHeight) {
		mTextureHeight *= 2;
	}

	Framework::instance().createTexture(&mTexture, mTextureWidth, mTextureWidth, mData, mWidth, mHeight);
	SAFE_DELETE(mData);
}

Image::~Image() {
	Framework::instance().destroyTexture(&mTexture);
}

void Image::draw(int dstX, int dstY, int srcX, int srcY, int width, int height) const {
	Framework f = Framework::instance();

	f.setTexture(mTexture);
	f.setBlendMode(Framework::BLEND_LINEAR);
	double p1[2] = { dstX, dstY };
	double p2[2] = { dstX + width, dstY };
	double p3[2] = { dstX, dstY + height };
	double p4[2] = { dstX + width, dstY + height };
	double rcpTw = 1.0 / static_cast<double>(mTextureWidth);
	double rcpTh = 1.0 / static_cast<double>(mTextureHeight);
	double u0 = static_cast<double>(srcX) * rcpTw;
	double v0 = static_cast<double>(srcY) * rcpTh;
	double u1 = u0 + static_cast<double>(width) * rcpTw;
	double v1 = v0 + static_cast<double>(height) * rcpTh;
	double t1[2] = { u0, v0 };
	double t2[2] = { u1, v0 };
	double t3[2] = { u0, v1 };
	double t4[2] = { u1, v1 };
	f.drawTriangle2D(p1, p2, p3, t1, t2, t3);
	f.drawTriangle2D(p2, p3, p4, t2, t3, t4);
}

//òcÇÒÇæå`
void Image::draw(const Vector2& p0, const Vector2& p1, const Vector2& p2, const Vector2& p3,
	double srcX,
	double srcY,
	int width,
	int height) const {
	Framework f = Framework::instance();

	f.setTexture(mTexture);
	f.setBlendMode(Framework::BLEND_LINEAR);
	double rcpTw = 1.0 / static_cast<double>(mTextureWidth);
	double rcpTh = 1.0 / static_cast<double>(mTextureHeight);
	double u0 = srcX * rcpTw;
	double v0 = srcY * rcpTh;
	double u1 = u0 + static_cast<double>(width) * rcpTw;
	double v1 = v0 + static_cast<double>(height) * rcpTh;
	double t0[2] = { u0, v0 };
	double t1[2] = { u1, v0 };
	double t2[2] = { u0, v1 };
	double t3[2] = { u1, v1 };
	f.drawTriangle2D(&p0.x, &p1.x, &p2.x, t0, t1, t2);
	f.drawTriangle2D(&p1.x, &p2.x, &p3.x, t1, t2, t3);

}

//âÊëúÇêLÇŒÇµÇΩÇË
void Image::draw(double dstX, double dstY, double dstWidth, double dstHeight,
	double srcX, double srcY, double srcWidth, double srcHeight) const {
	Framework f = Framework::instance();

	f.setTexture(mTexture);
	f.setBlendMode(Framework::BLEND_LINEAR);
	double p1[2] = { dstX, dstY };
	double p2[2] = { dstX + dstWidth, dstY };
	double p3[2] = { dstX, dstY + dstHeight };
	double p4[2] = { dstX + dstWidth, dstY + dstHeight };
	double rcpTw = 1.0 / static_cast<double>(mTextureWidth);
	double rcpTh = 1.0 / static_cast<double>(mTextureHeight);
	double u0 = static_cast<double>(srcX) * rcpTw;
	double v0 = static_cast<double>(srcY) * rcpTh;
	double u1 = u0 + static_cast<double>(srcWidth) * rcpTw;
	double v1 = v0 + static_cast<double>(srcHeight) * rcpTh;
	double t1[2] = { u0, v0 };
	double t2[2] = { u1, v0 };
	double t3[2] = { u0, v1 };
	double t4[2] = { u1, v1 };
	f.drawTriangle2D(p1, p2, p3, t1, t2, t3);
	f.drawTriangle2D(p2, p3, p4, t2, t3, t4);
}

void Image::draw(double dstX, double dstY, double srcX, double srcY) const {
	draw(dstX, dstY, srcX, srcY, mWidth, mHeight);
}

int Image::width() const {
	return mWidth;
}

int Image::height() const {
	return mHeight;
}

int Image::textureWidth() const {
	return mTextureWidth;
}

int Image::textureHeight() const {
	return mTextureHeight;
}

unsigned* Image::data() const {
	return mData;
}
