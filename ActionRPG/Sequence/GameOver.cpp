#include "Sequence/GameOver.h"
#include "Sequence/Parent.h"
#include "Image.h"
#include "Pad.h"
#include "GameLib/Framework.h"
using namespace GameLib;

namespace Sequence {
	GameOver::GameOver() : count(0) {
	}
	GameOver::~GameOver() {

	}
	void GameOver::update(Parent* parent) {
		++count;
		if (count == 200) {
			parent->moveTo(Parent::NEXT_STAGESELECT);
		}

		Framework f = Framework::instance();
		f.drawDebugString(2, 10, "¹Þ°Ñµ°ÊÞ°");
		f.drawDebugString(2, 11, "ÏÀ¶ÞÝÊÞØÏ¼®³");

		f.drawDebugString(2, 13, "µ¸ÉÃ·¶×Àµ¼ÃÐÖ³");
	}
}