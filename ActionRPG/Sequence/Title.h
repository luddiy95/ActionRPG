#ifndef INCLUDED_SEQUENCE_TITLE_H
#define INCLUDED_SEQUENCE_TITLE_H

class Image;

namespace Sequence {
	class Parent;
	class Title {
	public:
		Title();
		~Title();
		void update(Parent*);
		void draw() const;
	private:
		Image * mImage;
		int drawCount;
		int imageYposition;
		int mCursorPosition;
		int cursorCount;
	};
}

#endif