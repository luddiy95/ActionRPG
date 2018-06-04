#include "Game/GameParent.h"
#include "Game/Field.h"
#include "Game/Player.h"
#include "Game/Status.h"
#include "Sequence/Parent.h"
#include "Game/Enemy/Enemy.h"
#include "Game/Enemy/Bird.h"
#include "Game/Enemy/Turtle.h"
#include "Game/Enemy/Lama.h"
#include "Game/Enemy/Bear.h"

namespace Game {
	GameParent::GameParent(const int n) : mPlayer(0), mStatus(0), mEnemyNumber(0), mMode(SELECT) {
		SAFE_DELETE(mField);
		mField = new Field(50.0, 75.0, n);
		int x = 0, y = 0;
		int i = 0;
		while ('e' != gStageArray[n][i]) {
			switch (gStageArray[n][i]) {
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
	}
	GameParent::~GameParent() {
		SAFE_DELETE(mPlayer);
		SAFE_DELETE(mStatus);
		for (int i = 0; i < mEnemyNumber; i++) {
			SAFE_DELETE(mEnemies[i]);
		}
	}
	void GameParent::update(Sequence::Parent* parent) {
		//�X�V
		if (mMode == BATTLE) {
			mPlayer->update(mField);
			if (mPlayer->getAttackTime()) {
				mPlayer->attack()->attackUpdate(mPlayer, mField);
			}
			if (!mPlayer->getAttackTime()) { //�U���I��
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
			if (!mEnemies[i]->isDeleted()) { //��̂ł������Ă��Ȃ�enemy��������
				d = false;
				break;
			}
		}
		if (d) {
			parent->moveTo(Sequence::Parent::NEXT_CLEAR);
		}
		
		//�Փ˔���
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

		//�`��
		mField->draw();
		for (int y = 0; y < gLayerNumber; y++) {
			if(mPlayer->getAttackTime())
				mPlayer->attack()->drawAttack(ALL_BEFORE, *mField);
			for (int i = 0; i < mEnemyNumber; i++) {
				if (mEnemies[i]->attackY() == y) {
					mEnemies[i]->drawAttack(ALL_BEFORE, *mField); //��O��enemy�Ǝ����̗��enemy�ɉB���
				}
				if (mEnemies[i]->bodyY() == y) {
					mEnemies[i]->drawBody(*mField);
				}
			}
			for (int i = 0; i < mEnemyNumber; i++) {
				if (mEnemies[i]->attackY() == y) {
					mEnemies[i]->drawAttack(BEFORE, *mField); //��O��enemy�ɂ͉B��邪�����̗��enemy�ɂ͉B��Ȃ�
				}
			}
			if (mPlayer->mY() == y) {
				if (mPlayer->getAttackTime()) {
					mPlayer->attack()->drawAttack(BEFORE, *mField); //���Ƃňʒu�ς��邩��
				}
				mPlayer->draw(*mField);
			}
		}
		if(mPlayer->getAttackTime())
			mPlayer->attack()->drawAttack(AFTER, *mField);
		for (int i = 0; i < mEnemyNumber; i++) {
			mEnemies[i]->drawAttack(AFTER, *mField); //��O�ɂ��B��Ȃ�
		}
		mStatus->draw(*mPlayer);

		//attackFlag�����Z�b�g
		for (int y = 0; y < gLayerNumber; y++) {
			for (int x = 0; x < gLineNumber; x++) {
				mField->resetAttackFlag(x, y);
			}
		}
	}
}