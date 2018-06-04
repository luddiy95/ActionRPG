#ifndef INCLUDED_SEQUENCE_GAMEOVET_H
#define INCLUDED_SEQUENCE_GAMEOVET_H

class Image;

namespace Sequence {
	class Parent;
	class GameOver {
	public:
		GameOver();
		~GameOver();
		void update(Parent*);
	private:
		int count;
	};
}

#endif