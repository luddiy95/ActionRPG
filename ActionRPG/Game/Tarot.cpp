#include "Game/Tarot.h"
#include "Image.h"
#include "GameLib/Framework.h"
using namespace GameLib;

namespace Game {
	Tarot::Tarot(): tarotImage(0), tarotColor(0) {
		SAFE_DELETE(tarotColor);
		tarotColor = new Image("data/image/tarotFigure.dds");
	}
	Tarot::~Tarot() {
		SAFE_DELETE(tarotColor);
		SAFE_DELETE(tarotImage);
	}
	void Tarot::setTarot(Tarots t) {
		tarot = t;
		SAFE_DELETE(tarotImage);
		switch (tarot) {
		case LASER: tarotImage = new Image("data/image/laserTarot1.dds"); break;
		case LASER2: tarotImage = new Image("data/image/laser2Tarot1.dds"); break;
		case ROB: tarotImage = new Image("data/image/robTarot1.dds"); break;
		case RECOVERY: tarotImage = new Image("data/image/recoveryTarot1.dds"); break;
		case CANON: tarotImage = new Image("data/image/canonTarot1.dds"); break;
		}
	}
	void Tarot::setColor(TarotColor c) {
		color = c;
	}
	void Tarot::setTarotandColor(Tarots t, TarotColor c) {
		setTarot(t);
		setColor(c);
	}
	Tarot::Tarots Tarot::getTarot() const {
		return tarot;
	}
	Tarot::TarotColor Tarot::getColor() const {
		return color;
	}
	void Tarot::draw(double dstX, double dstY) const {
		tarotImage->draw(dstX, dstY, 0, 0);
		tarotColor->draw(dstX, dstY, color * 36, 0, 36, 40);
	}
	void Tarot::switchTarot(Tarot* t) {
		TarotColor c = t->getColor();
		Tarots ta = t->getTarot();
		t->setTarotandColor(tarot, color);
		setTarotandColor(ta, c);
	}
}