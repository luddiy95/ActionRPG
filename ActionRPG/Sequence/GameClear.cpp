#include "Sequence/GameClear.h"
#include "Sequence/Parent.h"
#include "Image.h"
#include "Pad.h"
#include "GameLib/Framework.h"
using namespace GameLib;

namespace Sequence {
	GameClear::GameClear() : count(0) {
	}
	GameClear::~GameClear() {

	}
	void GameClear::update(Parent* parent) {
		++count;
		if (count == 100) {
			parent->moveTo(Parent::NEXT_STAGESELECT);
		}

		Framework f = Framework::instance();
		f.drawDebugString(2, 10, "�ްѸر");
		f.drawDebugString(2, 11, "ָ�޷ϼ�");
	}
}