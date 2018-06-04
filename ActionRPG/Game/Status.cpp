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
				if (mCursor > selectTarotNum - 1) { //未選択のタロット
					Tarot* deck = player->deck();
					const Tarot& t = deck[headIndex + mCursor]; //現在選択中のタロット
					bool canSelect = true;
					for (int i = headIndex; i < headIndex + selectTarotNum; i++) {
						if ((t.getTarot() != deck[i].getTarot()) && (t.getColor() != deck[i].getColor())) {
							canSelect = false;
							break;
						}
					}
					if (canSelect) {
						player->setSelectTarotNum(selectTarotNum + 1);
						deck[headIndex + player->getSelectTarotNum() - 1].switchTarot(&deck[headIndex + mCursor]); //交換
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
				//indexが30 - 1より大きいのはデッキに存在しないので描画しない→
					//残りが6枚以下になったら残りだけ右詰めで描画
			}
			else {
				player.deck()[i].draw((5 - (i - headIndex)) * 36, 21); //右から左
				if (i - headIndex >= player.getSelectTarotNum()) {
					tarotShadow->draw((5 - (i - headIndex)) * 36, 21, 0, 0);
				}
			}
		}
		if (mode == Mode::SELECT && headIndex < maxTarotNum) {
			tarotCursor->draw((5 - mCursor) * 36, 24, 0, 0); //カーソルも右から左
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
		//ゲージ
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
					--mGauge; //ここで最後尾を変える
				}
			}
			++decreaseCount;
			if (decreaseCount == 5) {
				lastGaugeFlash = false; //点滅の滅
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
				lastGaugeFlash = false; //この時点ではまだ最後尾のゲージは消えてない
			}

			if (Pad::isTriggered(Pad::LEFT)) {
				mMode = BATTLE;
				mGauge = 0;
			} 

			if (mMode == BATTLE) {
				mSelect->mCursor = 0;
			}
		}

		//selectのupdateするかどうか
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

		mSelect->draw(player, mMode); //デッキの選択は常に表示(updateはmMode == SELECTのときだけ)
		if (mMode == BATTLE && player.getSelectTarotNum() > 0) {
			//使えるタロットが残ってたら先頭に枠
			selectHead->draw(180, 21, 0, 0);
		}
	}
	
	Mode Status::mode() const {
		return mMode;
	}
}