#include "Sequence/Title.h"
#include "GameLib/GameLib.h"
#include "GameLib/Framework.h"
#include "Image.h"
#include "Pad.h"
#include "Sequence/Parent.h"
using namespace GameLib;

namespace Sequence {
	Title::Title() : cursorCount(0), mCursorPosition(0), mImage(0), drawCount(0),
		imageYposition(0){
		SAFE_DELETE(mImage);
		mImage = new Image("data/image/player1.dds");
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

		switch (drawCount) {
		case 0: imageYposition = 0; break;
		case 30: imageYposition = 1; break;
		case 60: imageYposition = 3; break;
		case 90: imageYposition = 2; break;
		case 119: drawCount = -1; break;
		}
		++drawCount;

		mImage->draw(200, 150, 0, 32 * imageYposition, 20, 32);

		Framework f = Framework::instance();
		f.drawDebugString(2, 20, "[¶ÞÝÊÞÛ³Ò²ÄÞ»Ý]");
		f.drawDebugString(2, 21, "±¿ÎÞ³");
		f.drawDebugString(2, 22, "¾ÂÒ²ÖÝÃÞÈ");

		f.drawDebugString(1, 21 + mCursorPosition, ">");
	}

}