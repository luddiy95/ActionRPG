#ifndef INCLUDED_SEQUENCE_GAMECLEAR_H
#define INCLUDED_SEQUENCE_GAMECLEAR_H

class Image;

namespace Sequence {
	class Parent;
	class GameClear {
	public:
		GameClear();
		~GameClear();
		void update(Parent*);
	private:
		int count;
	};
}

#endif