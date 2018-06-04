#ifndef INCLUDED_GAME_TAROT_H
#define INCLUDED_GAME_TAROT_H

class Image;

namespace Game {
	class Tarot {
	public:
		enum Tarots {
			LASER,
			CANON,
			ROB,
			LASER2,
			RECOVERY,
			NONETarot
		};
		enum TarotColor {
			RED,
			GREEN,
			BLUE,
			BLACK,
			WHITE,
			NONEColor
		};

		Tarot();
		~Tarot();
		Tarots getTarot() const;
		TarotColor getColor() const;
		void setTarot(Tarots);
		void setColor(TarotColor);
		void setTarotandColor(Tarots, TarotColor);
		void draw(double, double) const;
		void switchTarot(Tarot*);
	private:
		Tarots tarot;
		TarotColor color;
		Image* tarotColor;
		Image* tarotImage;
	};
}

#endif