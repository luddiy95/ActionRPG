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

	char* gStageArray[]{
		"0021011011n000000111tn00p1b211b1n000011r1l1n000000111rn000001l121e",
		"00000011b1n00001l1111n000p0r1111n0000111121n00000t111rn0000000111e",
		"0011111r11n000000011tn00p000b11ln0011l22111n0011111111n0000111t11e",
		"000011r11rn00p1111b11n0000111111n1111111111n000011t111n000011111le",
		"00000011lln0000l111l1n00000001lln0000021111n00p01111l1n000000111le"
	};
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
		GameParent(const int);
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