#ifndef INCLUDED_SEQUENCE_PARENT_H]
#define INCLUDED_SEQUENCE_PARENT_H

namespace Game {
	class GameParent;
}

namespace Sequence {
	class Title;
	class GameOver;
	class GameClear;
	class StageSelect;
	class Explanation;

	class Parent {
	public:
		enum NextSequence {
			NEXT_TITLE,
			NEXT_GAME,
			NEXT_GAMEOVER,
			NEXT_CLEAR,
			NEXT_STAGESELECT,
			NEXT_EXPLANATION,

			NEXT_NONE
		};
		void update();
		void moveTo(NextSequence);

		static void create();
		static void destroy();
		static Parent* instance();
	private:
		Parent();
		~Parent();

		NextSequence mNextSequence;

		Title* mTitle;
		Game::GameParent* mGame;
		GameOver* mGameOver;
		GameClear* mGameClear;
		StageSelect* mStageSelect;
		Explanation* mExplanation;

		static Parent* mInstance;
	};
}


#endif