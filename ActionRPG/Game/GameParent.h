#ifndef GAME_GAMEPARENT_H
#define GAME_GAMEPARENT_H


namespace { 
	//ステージのバランス
	const double gFieldWidth = 32.0;
	const double gFieldHeight = 18.0;
	const double gFieldTheta = 60.0;
	const double gFieldScale = 10.0 / 9.0;
	const int gLineNumber = 10;
	const int gLayerNumber = 6;
	
	//プレイヤーのバランス
	const double gPlayerWidth = 20.0;
	const double gPlayerHeight = 32.0;
	const double gPlayerDepth = 4.0 / 6.0;
	const double gPlayerScale = 15.0 / 14.0;

	//敵のバランス
	const double gEnemyDepth = 4.0 / 6.0;
	const double gEnemyScale = 15.0 / 14.0;
}

namespace Sequence {
	class Parent;
}

namespace Game {
	class Field;
	class Player;
	class Status;

	enum Mode {
		BATTLE,
		SELECT
	};

	namespace Enemy {
		class Enemy;
		class Bird;
		class Turtle;
		class Lama;
	}

	class GameParent {
	public:
		enum DrawAttackTiming {
			ALL_BEFORE, //同じ行でも一番最初に描画
			BEFORE, //playerとenemyと同じタイミング(enemyとplayerの間)
			AFTER //playerとenemyの次に描画
		};
		GameParent(const char*);
		~GameParent();
		void update(Sequence::Parent*);
	private:
		Player* mPlayer;
		Status* mStatus;
		Field* mField;

		Enemy::Enemy* mEnemies[10];
		
		int mEnemyNumber;
		Mode mMode;
	};
}

#endif