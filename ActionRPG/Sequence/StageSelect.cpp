#include "Sequence/StageSelect.h"
#include "Image.h"
#include "Pad.h"
#include "Sequence/Parent.h"
#include "GameLib/Framework.h"
using namespace GameLib;

namespace Sequence {
	StageSelect::StageSelect() : cursorCount(0), mCursorPosition(0), mImage(0), mStagedata(0) {
		mImage = new Image("data/image/background1.dds");
	}
	StageSelect::~StageSelect() {
		SAFE_DELETE(mImage);
	}
	void StageSelect::update(Parent* parent) {
		++cursorCount;
		if (cursorCount > 10) {
			if (Pad::isTriggered(Pad::U)) {
				--mCursorPosition;
				if (mCursorPosition < 0) {
					mCursorPosition = 5;
				}
				cursorCount = 0;
			}
			else if (Pad::isTriggered(Pad::DW)) {
				++mCursorPosition;
				if (mCursorPosition > 5) {
					mCursorPosition = 0;
				}
				cursorCount = 0;
			}
		}
		if (Pad::isTriggered(Pad::D)) {
			char* stageArray[]{
				"data/stagedata/stagedata0.txt",
				"data/stagedata/stagedata1.txt",
				"data/stagedata/stagedata2.txt",
				"data/stagedata/stagedata3.txt",
				"data/stagedata/stagedata4.txt"
			};
			if (mCursorPosition == 5) {
				//random
				int r = Framework::instance().getRandom(5);
				mStagedata = stageArray[r];
			}
			else if (0 <= mCursorPosition && mCursorPosition <= 4) {
				mStagedata = stageArray[mCursorPosition];
			}
			parent->moveTo(Parent::NEXT_GAME);
		}
		if (Pad::isTriggered(Pad::C)) {
			parent->moveTo(Parent::NEXT_TITLE);
		}

		mImage->draw(0, 0, 0, 0, 320, 240);

		Framework f = Framework::instance();
		f.drawDebugString(2, 20, "[½Ã°¼Þ¾Ú¸Ä]");
		f.drawDebugString(2, 21, "½Ã°¼Þ1");
		f.drawDebugString(2, 22, "½Ã°¼Þ2");
		f.drawDebugString(2, 23, "½Ã°¼Þ3");
		f.drawDebugString(2, 24, "½Ã°¼Þ4");
		f.drawDebugString(2, 25, "½Ã°¼Þ5");
		f.drawDebugString(2, 27, "×ÝÀÞÑ");
		f.drawDebugString(2, 29, "ÊÞÂÃÞÓÄÞÙ");

		if (mCursorPosition <= 4) {
			f.drawDebugString(1, 21 + mCursorPosition, ">");
		}
		else if (mCursorPosition == 5) {
			f.drawDebugString(1, 21 + mCursorPosition + 1, ">");
		}

	}
	const char* StageSelect::getStagedata() const {
		return mStagedata;
	}
}