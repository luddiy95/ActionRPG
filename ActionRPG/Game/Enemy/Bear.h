#ifndef INCLUDED_GAME_ENEMY_BEAR_H
#define INCLUDED_GAME_ENEMY_BEAR_H

#include "Game/Enemy/Enemy.h"
#include "Vector2.h"

class Image;

namespace Game {
	class Field;
	class Player;
	class GameParent;
	namespace Enemy {
		class Bear : public Enemy {
		private:
		public:
			class Body {
			public:
				enum DirectionX {
					LEFT = -1,
					RIGHT = +1,
					NONEX = 0
				};
				enum DirectionY {
					BACK = -1,
					FRONT = +1,
					NONEY = 0
				};
				Body(int, int, Field*);
				~Body();
				void update(const Player&, Field*);
				void draw(const Field&) const;

				void changeAttackFrame();
				bool getAttackFrame() const;

				void AttackEnd();

				int mtX() const;
				int mtY() const;
				int mX() const;
				int mY() const;
				double mdX() const;
				double mdY() const;

				void setDirectionX(DirectionX);
				void setDirectionY(DirectionY);

				void setImageStatus(int);

				int getHP() const;
				
				void damage(int);
			private:
				int mx, my;
				int mtx, mty; //完全に移動するまで変わらない
				double mdx; //画像の真ん中の座標
				double mdy; //画像の真ん中の座標
				double nextmdx;
				double nextmdy;
				double moveDistanceX;
				double moveDistanceY;
				const int moveAreaFrame = 50;
				double dx;
				double dy;

				int mImageStatus;
				DirectionX mDirectionX;
				DirectionY mDirectionY;
				int mImageStatusCount;

				bool mFirstFrame;
				int mIntervalCount;
				bool mAttack;
				bool afterAttack;
				const int attackPercent = 30;

				bool isAttackFrame;

				const double mWidth = 21.0;
				const double mHeight = 32.0;
				Image* mImage;

				int HP;
			};


			class Sensor {
			public:
				Sensor(int, int, const Field&);
				~Sensor();
				int mX() const;
				int mY() const;
				void update();
				void draw(const Field&) const;

				bool isVanished() const;
				void Vanish();
				bool isSensorOn() const;
			private:
				const int mx, my;
				double mdx, mdy;
				bool vanish;
				bool sensor;
				int mSensorCount;
				int mCount;

				const double mWidth = 16.0;
				const double mHeight = 16.0;
				const double mDepth = 15.0 / 14.0;

				Image* mImage;
			};

			class Laser {
			public:
				Laser(int, int, const Field&);
				~Laser();
				void update();
				void draw(const Field&) const;

				int mX() const;
				int mY() const;

				bool isLaserOn() const;
				bool isVanished() const;
				void Vanish();
			private:
				const int mx, my;
				double mdx, mdy;

				int mLaserCount;

				const double mWidth = 32.0;
				const double mHeight = 32.0;

				Image* mImage;
				bool laser;
				bool vanish;
			};


			Bear(int, int, int, Field*);
			~Bear();
			int bodyX() const;
			int bodyY() const;
			int attackY() const;
			double mdX(const Field&) const;
			double mdY(const Field&) const;
			double getDisplayWidth() const;
			double getDisplayHeight() const;

			void update(const Player&, Field*);
			void attackCollision(Player*, Field*);
			void drawBody(const Field&) const;
			void drawAttack(Game::GameParent::DrawAttackTiming, const Field&) const;
			void damage(int);

			bool isDeleted() const;
			void enemyDelete(Field*);

			int getIndex() const;
		private:
			Body * mBody;
			Sensor* mSensor;
			Laser* mLaser;
			const int mDamage = 80;

			bool deleted;

			const int mWidth = 21.0;
			const int mHeight = 32.0;

			const int index;

			bool laserVanishing;
			int laserVanishingCount; //playerに当たったら一瞬で消えない

			int mSensorCount;
		};
	}
}

#endif