#ifndef INCLUDED_GAME_FIELD_H
#define INCLUDED_GAME_FIELD_H

#include "Array2D.h"

class Vector2;
class Image;
class File;

namespace GameLib {
	class Texture;
}

namespace Game {
	class Field {
	public:
		class Line { //‚»‚ê‚¼‚ê‚Ìcü
		public:
			Line();
			~Line();
			void setSlope(double);
			void setInterceptX(double);
			double getX(int y) const;
			double getX(double y) const;
			double getY(int y) const;
		private:
			double mSlope; //’¼ü‚ÌŒX‚«
			double mInterceptX; //xØ•Ğ
		};
		class Trout {
		public:
			enum State {
				HOLE = (1 << 0),
				PLAYER_AREA = (1 << 1),
				ENEMY_AREA = (1 << 2),

				ATTACKED = (1 << 8)
			};
			enum Charactor {
				PLAYER,
				ENEMY_BIRD,
				ENEMY_TURTLE,
				ENEMY_LAMA,
				ENEMY_BEAR,

				NONE
			};
			Trout();
			~Trout();
			void setCoordinate(double, double, double, double, double, double);
			Vector2* getCoordinate() const;

			void draw(const Image&, const Vector2& translation) const;

			void initializeState(unsigned);
			unsigned getState() const;
			void setFlag(unsigned);
			void resetFlag(unsigned);

			void setCharactor(Charactor);
			Charactor getCharactor() const;
		private:
			unsigned mState;
			Charactor mCharactor;
			Vector2* mCoordinate; //ƒ}ƒX‚Ìl‹÷‚ğ‚Ü‚Æ‚ß‚½‚à‚Ì
		};

		static Field* instance();
		static void create(double, double, const File&);
		static void destroy();

		void moveOrigin(double);
		Vector2 getOrigin() const;

		void draw() const;

		void setAttackFlag(int, int);
		void resetAttackFlag(int, int);
		void enemyChangeField(int, int);
		void playerChangeField(int, int);
		int getTroutState(int, int) const;

		Trout::Charactor getCharactor(int, int) const;
		void setCharactor(int, int, Trout::Charactor);

		Vector2* getTroutCoordinate(int, int) const;
		double getLineX(int, double) const;

		Field(double, double, const File&z);
		~Field();
	private:
		//static Field* mInstance;
		

		int mOriginX;
		int mOriginY;
		
		Image* mImage;
		Line* mLines;
		Array2D<Trout> mTrouts;
		Array2D<Trout> mCharactors;
	};
}

#endif