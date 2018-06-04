#include "Sequence/Explanation.h"
#include "Sequence/Parent.h"
#include "Pad.h"
#include "Image.h"
#include "GameLib/Framework.h"

using namespace GameLib;

namespace Sequence {
	Explanation::Explanation() : mImage1(0), mImage2(0), mImage3(0), mImage4(0), page(1) {
		//mImage‚Ìnew
		SAFE_DELETE(mImage1);
		SAFE_DELETE(mImage2);
		SAFE_DELETE(mImage3);
		SAFE_DELETE(mImage4);
		mImage1 = new Image("data/image/explanation1.dds");
		mImage2 = new Image("data/image/explanation2.dds");
		mImage3 = new Image("data/image/explanation3.dds");
		mImage4 = new Image("data/image/explanation4.dds");
	}
	Explanation::~Explanation() {
		SAFE_DELETE(mImage1);
		SAFE_DELETE(mImage2);
		SAFE_DELETE(mImage3);
		SAFE_DELETE(mImage4);
	}
	void Explanation::update(Parent* parent) {
		if (Pad::isTriggered(Pad::C)) {
			if (page == 1) {
				parent->moveTo(Parent::NEXT_TITLE);
			}
			else {
				--page;
			}
		}
		else if (Pad::isTriggered(Pad::D)) {
			if (page < 4) {
				++page;
			}
		}

		switch (page) {
		case 1:
			mImage1->draw(0, 0, 0, 0, 320, 240); break;
		case 2: 
			mImage2->draw(0, 0, 0, 0, 320, 240); break;
		case 3:
			mImage3->draw(0, 0, 0, 0, 320, 240); break;
		case 4:
			mImage4->draw(0, 0, 0, 0, 320, 240); break;
		}

		Framework f = Framework::instance();
		if (page == 1) {
			f.drawDebugString(60, 28, "ÏÙÃŞÂ·ŞÉÍß°¼Ş");
			f.drawDebugString(60, 29, "ÊŞÂÃŞÀ²ÄÙÆÓÄŞÙ");
		}
		else if (page == 4) {
			f.drawDebugString(60, 29, "ÊŞÂÃŞÓÄŞÙ");
		}
		else {
			f.drawDebugString(60, 28, "ÏÙÃŞÂ·ŞÉÍß°¼Ş");
			f.drawDebugString(60, 29, "ÊŞÂÃŞÓÄŞÙ");
		}
	}
}