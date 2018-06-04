#ifndef INCLUDED_GAME_ENEMY_TURTLE_H
#define INCLUDED_GAME_ENEMY_TURTLE_H

#include "Game/Enemy/Enemy.h"
#include "Vector2.h"

class Image;

namespace Game {
	class Field;
	class Player;
	class GameParent;
	namespace Enemy {
		class Turtle : public Enemy {
		private:
		public:
			class Body {
			public:
				Body(int, int, Field*);
				~Body();
				void update(const Player&, Field*);
				void draw(const Field&) const;

				void changeAttackFrame();
				bool attackFrame() const;

				int mtX() const;
				int mX() const;
				int mY() const;
				double mdX() const;

				void damage(int);
				int getHP() const;
			private:
				int mx, my;
				int mtx; //äÆëSÇ…à⁄ìÆÇ∑ÇÈÇ‹Ç≈ïœÇÌÇÁÇ»Ç¢
				double mdx; //âÊëúÇÃê^ÇÒíÜÇÃç¿ïW
				double nextmdx;
				double moveDistance;
				double dx;

				int mImageStatus;
				int mImageStatusCount;

				int mMoveCount; //Ç«ÇÃÇ≠ÇÁÇ¢Ç≈ìÆÇ≠Ç©
				int mAttackCount; //Ç«ÇÃÇ≠ÇÁÇ¢Ç≈çUåÇÇ∑ÇÈÇ©
				const int robAreaFrame = 50;

				bool isAttackFrame;

				const double mWidth = 169.0 / 4.0;
				const double mHeight = 104.0 / 4.0;
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


			Turtle(int, int, int, Field*);
			~Turtle();
			int bodyX() const;
			int bodyY() const;
			int attackY() const;
			double mdX(const Field&) const;
			double mdY(const Field&) const;
			double getDisplayWidth() const;
			double getDisplayHeight() const;

			void enemyDelete(Field*);
			bool isDeleted() const;

			void update(const Player&, Field*);
			void attackCollision(Player*, Field*);
			void drawBody(const Field&) const;
			void drawAttack(Game::GameParent::DrawAttackTiming, const Field&) const;
			void damage(int);

			int getIndex() const;
		private:
			Body* mBody;
			AttackTrout* mAttackTrout;
			const int mDamage = 50;

			bool deleted;

			const double mWidth = 169.0 / 4.0;
			const double mHeight = 104.0 / 4.0;

			const int index;

			const Game::GameParent::DrawAttackTiming mAttackDrawTiming = Game::GameParent::AFTER;
		};
	}
}

#endif