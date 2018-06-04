#ifndef INCLUDED_GAME_STATUS_H
#define INCLUDED_GAME_STATUS_H

#include "Game/GameParent.h"

class Image;

namespace Game {
	class Player;
	class Tarot;
	class Status {
	public:
		class Select {
		public:
			Select();
			~Select();
			void draw(const Player&, Mode) const;
			void update(Player*);

			int mCursor;
		private:
			Image* tarotShadow;
			Image* tarotCursor;

			int buttonCount;

			const int maxTarotNum = 30;
		};

		Status(const Player&);
		~Status();
		void update(Player*);
		void draw(const Player&) const;
		Mode mode() const;
	private:
		int playerHP;
		Image* Status1Image;
		Image* Gauge1Image;
		Image* Gauge2Image;
		Image* selectHead;
		const int canSelectConcentration = 25;
		const int maxGauge = 50;
		int mGauge;
		bool lastGaugeFlash;
		int increaseCount;
		int decreaseCount;

		Select* mSelect;
		Mode mMode;
	};
}

#endif