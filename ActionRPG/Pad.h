#ifndef INCLUDED_PAD_H
#define INCLUDED_PAD_H

class Pad {
public:
	enum Button {
		A, //△(方向変更)
		B, //□(視点移動)
		C, //×(キャンセル)
		D, //〇(決定)
		U,
		L,
		DW,
		R,
		LEFT,
		RIGHT
	};

	static bool isOn(Button);
	static bool isTriggered(Button);
	//staticなのでコンストラクトしなくていい
};

#endif