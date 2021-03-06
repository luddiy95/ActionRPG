#ifndef INCLUDED_SEQUENCE_STAGE_SELECT_H
#define INCLUDED_SEQUENCE_STAGE_SELECT_H

class Image;

namespace Sequence {
	class Parent;
	class StageSelect {
	public:
		StageSelect();
		~StageSelect();
		void update(Parent*);
		const char* getStagedata() const;
	private:
		Image * mImage;
		int drawCount;
		int imageYposition;
		int cursorCount;
		int mCursorPosition;
		char* mStagedata;
	};
}

#endif