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
		int getStageNum() const;
	private:
		Image * mImage;
		int cursorCount;
		int mCursorPosition;
		int stageNum;
	};
}

#endif