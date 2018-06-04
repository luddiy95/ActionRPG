#include "Sequence/Parent.h"
#include "Font.h"
#include "GameLib/Framework.h"
using namespace GameLib;

bool gFirst = true;

void Framework::update() {
	sleep(16);
	if (gFirst) {
		gFirst = false;
		Font::create();
		Sequence::Parent::create();
	}
	Sequence::Parent::instance()->update();
	if (isEndRequested()) {
		Font::destroy();
		Sequence::Parent::destroy();
	}
}