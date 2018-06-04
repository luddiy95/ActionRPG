#include "GameLib/GameLib.h"
#include "Sequence/Parent.h"
#include "Sequence/Title.h"
#include "Sequence/Explanation.h"
#include "Sequence/StageSelect.h"
#include "Sequence/GameOver.h"
#include "Sequence/GameClear.h"

#include "Game/GameParent.h"
using namespace GameLib;

namespace Sequence {
	Parent* Parent::mInstance = 0;

	void Parent::create() {
		ASSERT(!mInstance);
		mInstance = new Parent();
	}
	void Parent::destroy() {
		ASSERT(mInstance);
		SAFE_DELETE(mInstance);
	}
	Parent* Parent::instance() {
		return mInstance;
	}

	Parent::Parent() : mNextSequence(NEXT_NONE), mTitle(0), mGame(0),
		mGameOver(0), mGameClear(0), mStageSelect(0), mExplanation(0) {
		mTitle = new Title();
	}
	Parent::~Parent() {
		SAFE_DELETE(mTitle);
		SAFE_DELETE(mGame);
		SAFE_DELETE(mGameOver);
		SAFE_DELETE(mGameClear);
		SAFE_DELETE(mStageSelect);
		SAFE_DELETE(mExplanation);
	}
	void Parent::update() {
		if (mTitle) {
			mTitle->update(this);
		}
		else if (mGame) {
			mGame->update(this);
		}
		else if (mGameOver) {
			mGameOver->update(this);
		}
		else if (mGameClear) {
			mGameClear->update(this);
		}
		else if (mStageSelect) {
			mStageSelect->update(this);
		}
		else if (mExplanation) {
			mExplanation->update(this);
		}

		switch (mNextSequence) {
		case NEXT_TITLE:
			ASSERT(!mTitle && !mGame && !mGameOver && !mGameClear && (mStageSelect || mExplanation));
			SAFE_DELETE(mStageSelect);
			SAFE_DELETE(mExplanation);
			mTitle = new Title();
			break;
		case NEXT_GAME:
			ASSERT(!mTitle && mStageSelect && !mExplanation && !mGameOver && !mGameClear && !mGame);
			mGame = new Game::GameParent(mStageSelect->getStagedata());
			SAFE_DELETE(mStageSelect);
			break;
		case NEXT_STAGESELECT:
			ASSERT((mTitle || mGameOver || mGameClear) && !mGame && !mExplanation && !mStageSelect);
			SAFE_DELETE(mTitle);
			SAFE_DELETE(mGameOver);
			SAFE_DELETE(mGameClear);
			mStageSelect = new StageSelect();
			break;
		case NEXT_GAMEOVER:
			ASSERT(!mTitle && mGame && !mGameOver && !mGameClear && !mStageSelect && !mExplanation);
			SAFE_DELETE(mGame);
			mGameOver = new GameOver();
			break;
		case NEXT_CLEAR:
			ASSERT(!mTitle && mGame && !mGameOver && !mGameClear && !mStageSelect && !mExplanation);
			SAFE_DELETE(mGame);
			mGameClear = new GameClear();
			break;
		case NEXT_EXPLANATION:
			ASSERT(mTitle && !mGame && !mGameOver && !mGameClear && !mStageSelect && !mExplanation);
			SAFE_DELETE(mTitle);
			mExplanation = new Explanation();
			break;
		}
		mNextSequence = NEXT_NONE;
	}

	void Parent::moveTo(NextSequence next) {
		ASSERT(mNextSequence == NEXT_NONE);
		mNextSequence = next;
	}

}