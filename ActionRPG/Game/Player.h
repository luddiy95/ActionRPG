#ifndef INCLUDED_GAME_PLAYER_H
#define INCLUDED_GAME_PLAYER_H

#include "Game/Tarot.h"
#include "Game/GameParent.h"
#include "Vector2.h"

class Image;

namespace Game {
	class Status;
	class Tarot;
	class Field;
	namespace Enemy {
		class Enemy;
	}
	class Player {
	public:
		class playerAttack {
		public:
			virtual void attackUpdate(Player*, Field*) = 0;
			virtual void drawAttack(GameParent::DrawAttackTiming, const Field&) const = 0;
			virtual void attackCollision(Enemy::Enemy*, Player*, const Field&) = 0;
			virtual ~playerAttack() {};
		};
		enum Direction {
			FRONT = 0,
			LEFT = 1,
			RIGHT = 2,
			BACK = 3
		};


		class attackLaser : public playerAttack {
		public:
			attackLaser(const Player&);
			~attackLaser();
			void attackUpdate(Player*, Field*);
			void drawAttack(GameParent::DrawAttackTiming, const Field&) const;
			void attackCollision(Enemy::Enemy*, Player*, const Field&);
		private:
			Image * mImage;
			int mLaserCount;
			bool laserCollision;
			const int laserDamage = 40;
			int laserCollisionCount;
			const int playerX, playerY;

			int* collisionIndex;
			int collisionEnemyNum;

			const int mWidth = 32;
			const int mHeight = 32;

			int firstAttackInterval;
			const int firstAttackIntervalMax = 30;

			const GameParent::DrawAttackTiming mDrawAttackTiming = GameParent::BEFORE;
		};


		class attackLaser2 : public playerAttack {
		public:
			attackLaser2(const Player&);
			~attackLaser2();
			void attackUpdate(Player*, Field*);
			void drawAttack(GameParent::DrawAttackTiming, const Field&) const;
			void attackCollision(Enemy::Enemy*, Player*, const Field&);
		private:
			Image * mImage1;
			Image* mImage2;
			int mLaser2Count;
			bool laser2Collision;
			const int laser2Damage = 40;
			int laser2CollisionCount;
			const int playerX, playerY;

			int* collisionIndex2;
			int collisionEnemyNum2;

			const int mWidth = 32;
			const int mHeight = 32;

			int firstAttackInterval;
			const int firstAttackIntervalMax = 30;

			const GameParent::DrawAttackTiming mDrawAttackTiming = GameParent::BEFORE;
		};


		class Canon : public playerAttack {
		public:
			Canon(const Player&, const Field&);
			~Canon();
			void attackUpdate(Player*, Field*);
			void drawAttack(GameParent::DrawAttackTiming, const Field&) const;
			void attackCollision(Enemy::Enemy*, Player*, const Field&);
		private:
			Image * mImage;
			const double mV = 3.0;
			double xmV;
			bool mVanish;
			const int playerX, playerY;
			double mdx, mdy; //画像の真ん中で原点はフィールドの左上
			const int mWidth = 16.0;
			const int mHeight = 16.0;

			const Player::Direction playerDirection;

			int firstAttackInterval;
			const int firstAttackIntervalMax = 30;

			const int mCanonDamage = 30;

			const GameParent::DrawAttackTiming mDrawAttackTiming = GameParent::BEFORE;
		};


		class Recovery : public playerAttack {
		public:
			Recovery(const Player&);
			~Recovery();
			void attackUpdate(Player*, Field*);
			void drawAttack(GameParent::DrawAttackTiming, const Field&) const;
			void attackCollision(Enemy::Enemy*, Player*, const Field&);
		};


		class Rob : public playerAttack {
		public:
			Rob(const Player&, const Field&);
			~Rob();
			void attackUpdate(Player*, Field*);
			void drawAttack(GameParent::DrawAttackTiming, const Field&) const;
			void attackCollision(Enemy::Enemy*, Player*, const Field&);
		private:
			const int playerX, playerY;
			const Player::Direction playerDirection;
			int robX, robY;

			int firstAttackInterval;
			const int firstAttackIntervalMax = 0;

			int mAttackTroutCount;

			Vector2 m0, m1, m2, m3;
			Vector2 mTranslation;

			const double mFirstDistance = 80.0;

			const int robAreaFrame = 50;

			bool isChangeArea;

			const int robDamage = 50;

			const GameParent::DrawAttackTiming mDrawAttackTiming = GameParent::DrawAttackTiming::AFTER;

			Image* mImage;
		};

		Player(int, int, Field*);
		~Player();
		int mX() const;
		int mY() const;
		double mdX() const;
		double mdY() const;
		double getDisplayWidth() const;
		double getDisplayHeight() const;

		Direction getDirection() const;

		void update(Field*);
		void draw(const Field&) const;
		int hp() const;
		void decreaseHP(int);
		void increaseHP(int);

		Tarot* deck() const;
		int getHeadIndex() const;
		void setHeadIndex(int);
		void setSelectTarotNum(int);
		int getSelectTarotNum() const;

		bool getAttackTime() const;
		void changeAttackTime();
		void endAttack();

		playerAttack* attack() const;
	private:
		Image* mImage;
		int mx, my;
		double mdx, mdy;
		double mDisplayWidth, mDisplayHeight;
		Direction mDirection;
		int buttonCount;
		int HP;
		const int maxHP;

		Tarot* mDeck;
		const int maxTarotNum = 30;
		int selectTarotNum;
		int headIndex;
		bool attackTime;
		int attackInterval;

		playerAttack* mAttack;
	};
}

#endif