#ifndef INCLUDED_SEQUENCE_EXPLANATION_H
#define INCLUDED_SEQUENCE_EXPLANATION_H

class Image;

namespace Sequence {
	class Parent;
	class Explanation {
	public:
		Explanation();
		~Explanation();
		void update(Parent*);
	private:
		Image * mImage1;
		Image* mImage2;
		Image* mImage3;
		Image* mImage4;
		int page;
	};
}

#endif