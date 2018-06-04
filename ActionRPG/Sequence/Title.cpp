#include "Sequence/Title.h"
#include "GameLib/GameLib.h"
#include "GameLib/Framework.h"
#include "Image.h"
#include "Pad.h"
#include "Sequence/Parent.h"
using namespace GameLib;

namespace Sequence {
	Title::Title() : cursorCount(0), mCursorPosition(0), mImage(0) {
		mImage = new Image("data/image/background1.dds");
	}
	Title::~Title() {
		SAFE_DELETE(mImage);
	}
	void Title::update(Parent* parent) {
		++cursorCount;
		if (cursorCount > 10) {
			if (Pad::isTriggered(Pad::U)) {
				--mCursorPosition;
				if (mCursorPosition < 0) {
					mCursorPosition = 1;
				}
				cursorCount = 0;
			}
			else if (Pad::isTriggered(Pad::DW)) {
				++mCursorPosition;
				if (mCursorPosition > 1) {
					mCursorPosition = 0;
				}
				cursorCount = 0;
			}
		}
		if (Pad::isTriggered(Pad::D)) {
			if (mCursorPosition == 0) {
				parent->moveTo(Parent::NEXT_STAGESELECT);
			}
			else if (mCursorPosition == 1) {
				parent->moveTo(Parent::NEXT_EXPLANATION);
			}
		}

		mImage->draw(0, 0, 0, 0);

		Framework f = Framework::instance();
		f.drawDebugString(2, 20, "±¿ÎÞ³");
		f.drawDebugString(2, 21, "¾ÂÒ²ÖÝÃÞÈ");

		f.drawDebugString(1, 20 + mCursorPosition, ">");
	}

}