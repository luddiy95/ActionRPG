#include "Status.h"
#include "Image.h"
#include "Font.h"
#include "GameLib/Framework.h"
#include "Pad.h"
#include "Game/Player.h"
#include "Game/Tarot.h"
#include "Game/Field.h"
#include <sstream>
using namespace GameLib;
using namespace std;

namespace Game {
	Status::Select::Select() : mCursor(0), tarotShadow(0), buttonCount(0),
		tarotCursor(0) {
		tarotShadow = new Image("data/image/shadow.dds");
		tarotCursor = new Image("data/image/tarotCursor.dds");
	}
	Status::Select::~Select() {
		SAFE_DELETE(tarotShadow);
		SAFE_DELETE(tarotCursor);
	}
	void Status::Select::update(Player* player) {

		int headIndex = player->getHeadIndex();
		if (headIndex < maxTarotNum) {
			++buttonCount;
			if (buttonCount > 10) {
				if (Pad::isOn(Pad::L)) {
					++mCursor;
					buttonCount = 0;
				}
				else if (Pad::isOn(Pad::R)) {
					--mCursor;
					buttonCount = 0;
				}
			}

			int showTarotNum = (6 < (maxTarotNum - headIndex)) ? 6 : (maxTarotNum - headIndex);
			if (headIndex < maxTarotNum) {
				if (mCursor < 0) {
					mCursor += showTarotNum;
				}
				else if (mCursor > showTarotNum - 1) {
					mCursor -= showTarotNum;
				}
			}

			int selectTarotNum = player->getSelectTarotNum();
			if (Pad::isTriggered(Pad::D)) {
				if (mCursor > selectTarotNum - 1) { //���I���̃^���b�g
					Tarot* deck = player->deck();
					const Tarot& t = deck[headIndex + mCursor]; //���ݑI�𒆂̃^���b�g
					bool canSelect = true;
					for (int i = headIndex; i < headIndex + selectTarotNum; i++) {
						if ((t.getTarot() != deck[i].getTarot()) && (t.getColor() != deck[i].getColor())) {
							canSelect = false;
							break;
						}
					}
					if (canSelect) {
						player->setSelectTarotNum(selectTarotNum + 1);
						deck[headIndex + player->getSelectTarotNum() - 1].switchTarot(&deck[headIndex + mCursor]); //����
					}
				}
			}
			if (Pad::isTriggered(Pad::C)) {
				if (selectTarotNum > 0) {
					player->setSelectTarotNum(selectTarotNum - 1);
				}
			}
		}
	}
	void Status::Select::draw(const Player& player, Mode mode) const {
		int headIndex = player.getHeadIndex();
		for (int i = headIndex; i < headIndex + 6; i++) {
			if (i >= 30) {
				//index��30 - 1���傫���̂̓f�b�L�ɑ��݂��Ȃ��̂ŕ`�悵�Ȃ���
					//�c�肪6���ȉ��ɂȂ�����c�肾���E�l�߂ŕ`��
			}
			else {
				player.deck()[i].draw((5 - (i - headIndex)) * 36, 21); //�E���獶
				if (i - headIndex >= player.getSelectTarotNum()) {
					tarotShadow->draw((5 - (i - headIndex)) * 36, 21, 0, 0);
				}
			}
		}
		if (mode == Mode::SELECT && headIndex < maxTarotNum) {
			tarotCursor->draw((5 - mCursor) * 36, 24, 0, 0); //�J�[�\�����E���獶
		}
	}


	Status::Status(const Player& player) : playerHP(player.hp()),
		mGauge(maxGauge), increaseCount(0), decreaseCount(0), lastGaugeFlash(true), mMode(SELECT){
		Status1Image = new Image("data/image/status0.dds");
		Gauge1Image = new Image("data/image/gauge1.dds");
		Gauge2Image = new Image("data/image/gauge2.dds");
		selectHead = new Image("data/image/selectHead.dds");

		mSelect = new Select();
	}
	Status::~Status() {
		SAFE_DELETE(Status1Image);
		SAFE_DELETE(Gauge1Image);
		SAFE_DELETE(Gauge2Image);
	}
	void Status::update(Player* player) {
		//�Q�[�W
		if (mMode == BATTLE) {
			++increaseCount;
			if (increaseCount == 25) {
				increaseCount = 0;
				if (mGauge < maxGauge) {
					++mGauge;
				}
			}
			playerHP = player->hp();

			if (mGauge >= canSelectConcentration + 2) {
				if (Pad::isTriggered(Pad::RIGHT)) {
					mMode = SELECT;
					player->setHeadIndex(player->getHeadIndex() + player->getSelectTarotNum());
					player->setSelectTarotNum(0);
				}
			}
		}
		if (mMode == SELECT) {
			if (decreaseCount == 25) {
				decreaseCount = 0;
				if (mGauge <= canSelectConcentration + 2) {
					mMode = BATTLE;
					mGauge = 0;
				}
				else {
					--mGauge; //�����ōŌ����ς���
				}
			}
			++decreaseCount;
			if (decreaseCount == 5) {
				lastGaugeFlash = false; //�_�ł̖�
			}
			else if (decreaseCount == 10) {
				lastGaugeFlash = true;
			}
			else if (decreaseCount == 15) {
				lastGaugeFlash = false;
			}
			else if (decreaseCount == 20) {
				lastGaugeFlash = true;
			}
			else if (decreaseCount == 25) {
				lastGaugeFlash = false; //���̎��_�ł͂܂��Ō���̃Q�[�W�͏����ĂȂ�
			}

			if (Pad::isTriggered(Pad::LEFT)) {
				mMode = BATTLE;
				mGauge = 0;
			} 

			if (mMode == BATTLE) {
				mSelect->mCursor = 0;
			}
		}

		//select��update���邩�ǂ���
		if (mMode == SELECT) {
			mSelect->update(player);
		}
	}
	void Status::draw(const Player& player) const {
		Font* font = Font::instance();

		Status1Image->draw(0, 0, 0, 0);
		ostringstream oss;
		oss << " HP:" << playerHP;
		font->drawString(0, 0, 4, 10, oss.str().c_str());

		for (int i = 0; i < mGauge - 1; i++) {
			Gauge1Image->draw(60 + i * 4, 5, 0, 0, 4, 16);
		}
		if (lastGaugeFlash) {
			Gauge1Image->draw(60 + (mGauge - 1) * 4, 5, 0, 0, 4, 16);
		}

		Gauge2Image->draw(60 + 4 * canSelectConcentration, 5, 0, 0, 4, 16);

		mSelect->draw(player, mMode); //�f�b�L�̑I���͏�ɕ\��(update��mMode == SELECT�̂Ƃ�����)
		if (mMode == BATTLE && player.getSelectTarotNum() > 0) {
			//�g����^���b�g���c���Ă���擪�ɘg
			selectHead->draw(180, 21, 0, 0);
		}
	}
	
	Mode Status::mode() const {
		return mMode;
	}
}