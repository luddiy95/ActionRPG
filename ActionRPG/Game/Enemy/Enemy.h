#ifndef INCLUDED_GAME_ENEMY_ENEMY_H
#define INCLUDED_GAME_ENEMY_ENEMY_H

#include "Game/GameParent.h"
#include "Game/Field.h"

namespace Game {
	class Player;
	namespace Enemy {
		class Enemy {
		public:
			virtual void update(const Player&, Field*) = 0;
			virtual void attackCollision(Player*, Field*) = 0;
			virtual void drawBody(const Field&) const = 0;
			virtual void drawAttack(Game::GameParent::DrawAttackTiming, const Field&) const = 0;
			virtual int bodyX() const = 0;
			virtual int bodyY() const = 0;
			virtual int attackY() const = 0;
			virtual double mdX(const Field&) const = 0;
			virtual double mdY(const Field&) const = 0;
			virtual double getDisplayWidth() const = 0;
			virtual double getDisplayHeight() const = 0;
			virtual void damage(int) = 0;
			virtual int getIndex() const = 0;
			virtual bool isDeleted() const = 0;
			virtual void enemyDelete(Field*) = 0;
			virtual ~Enemy() {};
		};
	}
}

#endif