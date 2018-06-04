#ifndef INCLUDED_GAME_ENEMY_BIRD_H
#define INCLUDED_GAME_ENEMY_BIRD_H

#include "Game/Enemy/Enemy.h"
#include "Vector2.h"

class Image;

namespace Game {
	class Player;
	class GameParent;
	namespace Enemy {
		class Bird : public Enemy {
		private:
			const double mBeamWidth = 16.0;
			const double mBeamHeight = 16.0;
		public:
			class Beam;
			class Body {
			public:
				Body(int, int, Field*);
				~Body();
				void update(const Player&, Field*);
				void draw(const Field&) const;

				void changeAttackFrame();
				bool attackFrame() const;

				int mX() const;
				int mY() const;

				int getHP() const;

				void damage(int);
			private:
				int mx, my;
				int mImageStatus;
				int mImageStatusCount; //�H���ǂ̎����Ńo�^�o�^�����邩

				int mMoveCount; //�ǂ̎����ŏ㉺�ɓ�����
				int mBeamCount; //�����𔭂����Ƃ��Î~����J�E���g
				int mAttackCount; //�ǂ̃^�C�~���O�Ō����𔭎˂��邩

				bool isAttackFrame; //�U���������t���[���ł����H

				const double mWidth = 48.0;
				const double mHeight = 36.0;
				Image* mImage;

				int HP;
			};

			class Beam {
			public:
				Beam(const Body&, const Field&);
				~Beam();
				void update(const Player&, const Field&);
				void draw(const Field&) const;

				bool isVanished() const;
				void vanish();

				int mY() const;
				double mdX() const;
				double mdY() const;

				void getInformation(const Beam&);
			private:
				Image * mImage;
				const double mV = 2.0;
				bool mVanish;
				const int my;
				double mdx, mdy; //�摜�̐^�񒆂Ō��_�̓t�B�[���h�̍���
				const int mWidth = 16.0;
				const int mHeight = 16.0;
			};

			Bird(int, int, int, Field*);
			~Bird();

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
			Body* mBody;
			Beam* mBeam;
			const int mDamage = 40;

			bool deleted;

			const double mWidth = 48.0;
			const double mHeight = 36.0;

			const int index;

			const Game::GameParent::DrawAttackTiming mAttackDrawTiming = Game::GameParent::BEFORE;
		};
	}
}

#endif