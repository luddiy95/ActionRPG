#include "Game/GameParent.h"
#include "Game/Field.h"
#include "Game/Player.h"
#include "Game/Status.h"
#include "Sequence/Parent.h"
#include "File.h"
#include "Game/Enemy/Enemy.h"
#include "Game/Enemy/Bird.h"
#include "Game/Enemy/Turtle.h"
#include "Game/Enemy/Lama.h"
#include "Game/Enemy/Bear.h"

namespace Game {
	GameParent::GameParent(const char* stagedata) : mPlayer(0), mStatus(0), mEnemyNumber(0), mMode(SELECT) {
		File* stageFile = new File(stagedata);
		SAFE_DELETE(mField);
		mField = new Field(50.0, 75.0, *stageFile);
		int x = 0, y = 0;
		int i = 0;
		while ('e' != stageFile->data()[i]) {
			switch (stageFile->data()[i]) {
			case '2': case '0': case '1': ++x; break;
			case 'p': mPlayer = new Player(x, y, mField); ++x; break;
			case 'b': mEnemies[mEnemyNumber] = new Enemy::Bird(x, y, mEnemyNumber, mField);
				++mEnemyNumber; ++x; break;
			case 't': mEnemies[mEnemyNumber] = new Enemy::Turtle(x, y, mEnemyNumber, mField);
				++mEnemyNumber; ++x; break;
			case 'l': mEnemies[mEnemyNumber] = new Enemy::Lama(x, y, mEnemyNumber, mField);
				++mEnemyNumber; ++x; break;
			case 'r': mEnemies[mEnemyNumber] = new Enemy::Bear(x, y, mEnemyNumber, mField);
				++mEnemyNumber; ++x; break;
			case 'n': ++y; x = 0; break;
			}
			++i;
		}
		mStatus = new Status(*mPlayer);

		SAFE_DELETE(stageFile);
	}
	GameParent::~GameParent() {
		SAFE_DELETE(mPlayer);
		SAFE_DELETE(mStatus);
		for (int i = 0; i < mEnemyNumber; i++) {
			SAFE_DELETE(mEnemies[i]);
		}
	}
	void GameParent::update(Sequence::Parent* parent) {
		//XV
		if (mMode == BATTLE) {
			mPlayer->update(mField);
			if (mPlayer->getAttackTime()) {
				mPlayer->attack()->attackUpdate(mPlayer, mField);
			}
			if (!mPlayer->getAttackTime()) { //UŒ‚I—¹
				mPlayer->endAttack();
			}
			for (int i = 0; i < mEnemyNumber; i++) {
				if (mEnemies[i]) {
					mEnemies[i]->update(*mPlayer, mField);
				}
			}
		}
		mStatus->update(mPlayer);
		mMode = mStatus->mode();

		if (mPlayer->hp() <= 0) {
			parent->moveTo(Sequence::Parent::NEXT_GAMEOVER);
		}
		bool d = true;
		for (int i = 0; i < mEnemyNumber; i++) {
			if (!mEnemies[i]->isDeleted()) { //ˆê‘Ì‚Å‚àÁ‚¦‚Ä‚¢‚È‚¢enemy‚ª‚¢‚½‚ç
				d = false;
				break;
			}
		}
		if (d) {
			parent->moveTo(Sequence::Parent::NEXT_CLEAR);
		}
		
		//Õ“Ë”»’è
		for (int i = 0; i < mEnemyNumber; i++) {
			mEnemies[i]->attackCollision(mPlayer, mField);
		}
		if (mPlayer->getAttackTime()) {
			for (int i = 0; i < mEnemyNumber; i++) {
				if (!mEnemies[i]->isDeleted()) {
					mPlayer->attack()->attackCollision(mEnemies[i], mPlayer, *mField);
				}
			}
		}

		//•`‰æ
		mField->draw();
		for (int y = 0; y < gLayerNumber; y++) {
			if(mPlayer->getAttackTime())
				mPlayer->attack()->drawAttack(ALL_BEFORE, *mField);
			for (int i = 0; i < mEnemyNumber; i++) {
				if (mEnemies[i]->attackY() == y) {
					mEnemies[i]->drawAttack(ALL_BEFORE, *mField); //è‘O‚Ìenemy‚Æ©•ª‚Ì—ñ‚Ìenemy‚É‰B‚ê‚é
				}
				if (mEnemies[i]->bodyY() == y) {
					mEnemies[i]->drawBody(*mField);
				}
			}
			for (int i = 0; i < mEnemyNumber; i++) {
				if (mEnemies[i]->attackY() == y) {
					mEnemies[i]->drawAttack(BEFORE, *mField); //è‘O‚Ìenemy‚É‚Í‰B‚ê‚é‚ª©•ª‚Ì—ñ‚Ìenemy‚É‚Í‰B‚ê‚È‚¢
				}
			}
			if (mPlayer->mY() == y) {
				if (mPlayer->getAttackTime()) {
					mPlayer->attack()->drawAttack(BEFORE, *mField); //‚ ‚Æ‚ÅˆÊ’u•Ï‚¦‚é‚©‚à
				}
				mPlayer->draw(*mField);
			}
		}
		if(mPlayer->getAttackTime())
			mPlayer->attack()->drawAttack(AFTER, *mField);
		for (int i = 0; i < mEnemyNumber; i++) {
			mEnemies[i]->drawAttack(AFTER, *mField); //è‘O‚É‚à‰B‚ê‚È‚¢
		}
		mStatus->draw(*mPlayer);

		//attackFlag‚ğƒŠƒZƒbƒg
		for (int y = 0; y < gLayerNumber; y++) {
			for (int x = 0; x < gLineNumber; x++) {
				mField->resetAttackFlag(x, y);
			}
		}
	}
}