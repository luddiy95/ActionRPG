#ifndef INCLUDED_FILE_H
#define INCLUDED_FILE_H

class File {
public:
	File(const char* fileName);
	~File();
	unsigned getUnsigned(int p);
	char* data() const;
private:
	int mSize;
	char* mData;
};

#endif