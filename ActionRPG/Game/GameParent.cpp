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
		for (int y = 0; y < gLayerNumber; y++) {
			for (int x = 0; x < gLineNumber; x++) {
				switch (stageFile->data()[y * (gLineNumber + 1) + x]) {
				case 'p': SAFE_DELETE(mPlayer);  mPlayer = new Player(x, y, mField); break;
				case 'b': SAFE_DELETE(mEnemies[mEnemyNumber]);
					mEnemies[mEnemyNumber] = new Enemy::Bird(x, y, mEnemyNumber, mField);
					++mEnemyNumber; break;
				case 't': SAFE_DELETE(mEnemies[mEnemyNumber]);
					mEnemies[mEnemyNumber] = new Enemy::Turtle(x, y, mEnemyNumber, mField);
					++mEnemyNumber; break;
				case 'l': SAFE_DELETE(mEnemies[mEnemyNumber]); 
					mEnemies[mEnemyNumber] = new Enemy::Lama(x, y, mEnemyNumber, mField);
					++mEnemyNumber; break;
				case 'r': SAFE_DELETE(mEnemies[mEnemyNumber]); 
					mEnemies[mEnemyNumber] = new Enemy::Bear(x, y, mEnemyNumber, mField);
					++mEnemyNumber; break;
				}
			}
		}
		SAFE_DELETE(mStatus);
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