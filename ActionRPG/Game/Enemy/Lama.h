#ifndef INCLUDED_GAME_ENEMY_LAMA_H
#define INCLUDED_GAME_ENEMY_LAMA_H

#include "Game/Enemy/Enemy.h"
#include "Vector2.h"

class Image;

namespace Game {
	class Field;
	class Player;
	class GameParent;
	namespace Enemy {
		class Lama : public Enemy {
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
				bool attackFrame() const;

				int mtX() const;
				int mtY() const;
				int mX() const;
				int mY() const;
				double mdX() const;
				double mdY() const;

				int getHP() const;

				DirectionX getDirectionX() const;
				DirectionY getDirectionY() const;

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
				const int moveAreaFrame = 100;
				double dx;
				double dy;

				int mImageStatus;
				DirectionX mDirectionX;
				DirectionY mDirectionY;
				int mImageStatusCount;

				int mMoveCount; //どのくらいで動くか
				int mAttackCount; //どのくらいで攻撃するか
				int mDirectionCount;
				const int robAreaFrame = 50;
				const int robPercent = 50;

				bool isAttackFrame;

				const double mWidth = 24.0;
				const double mHeight = 32.0;
				Image* mImage;

				int HP;
			};
			class AttackTrout {
			public:
				AttackTrout(int, int, const Field&);
				~AttackTrout();
				int mX() const;
				int mY() const;
				void update();
				void draw(const Field&);
				bool isVanished() const;
			private:
				int mx, my;
				const Vector2 m0, m1, m2, m3;
				Vector2 mTranslation;
				const double mFirstDistance = 80.0;
				int mAttackTroutCount;
				const int robAreaFrame = 50;
				bool vanish;
				Image* mImage;
			};


			Lama(int, int, int, Field*);
			~Lama();
			int bodyX() const;
			int bodyY() const;
			int attackY() const;
			double mdX(const Field&) const;
			double mdY(const Field&) const;
			double getDisplayWidth() const;
			double getDisplayHeight() const;

			bool isDeleted() const;
			void enemyDelete(Field*);

			void update(const Player&, Field*);
			void attackCollision(Player*, Field*);
			void drawBody(const Field&) const;
			void drawAttack(Game::GameParent::DrawAttackTiming, const Field&) const;

			void damage(int);

			int getIndex() const;
		private:
			Body * mBody;
			AttackTrout* mAttackTrout;
			const int mDamage = 50;

			bool deleted;

			const double mWidth = 24.0;
			const double mHeight = 32.0;

			const int index;

			const Game::GameParent::DrawAttackTiming mAttackDrawTiming = Game::GameParent::AFTER;
		};
	}
}

#endif