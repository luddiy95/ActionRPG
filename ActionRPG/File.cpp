#include "File.h"
#include "GameLib/Framework.h"
using namespace GameLib;

#include <fstream>
using namespace std;

File::File(const char* fileName) : mData(0), mSize(0) {
	ifstream in = ifstream(fileName, ifstream::binary);
	if (in) {
		in.seekg(0, ifstream::end);
		mSize = static_cast<int>(in.tellg());
		in.seekg(0, ifstream::beg);
		mData = new char[mSize];
		in.read(mData, mSize);
	}
	else {
		HALT("can't open file");
	}
}

File::~File() {
	SAFE_DELETE(mData);
}

char* File::data() const {
	return mData;
}

unsigned File::getUnsigned(int p) {
	const unsigned char* up = reinterpret_cast<const unsigned char*>(mData);
	unsigned ret = up[p];
	ret |= up[p + 1] << 8;
	ret |= up[p + 2] << 16;
	ret |= up[p + 3] << 24;
	return ret;
}