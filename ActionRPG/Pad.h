#ifndef INCLUDED_PAD_H
#define INCLUDED_PAD_H

class Pad {
public:
	enum Button {
		A, //��(�����ύX)
		B, //��(���_�ړ�)
		C, //�~(�L�����Z��)
		D, //�Z(����)
		U,
		L,
		DW,
		R,
		LEFT,
		RIGHT
	};

	static bool isOn(Button);
	static bool isTriggered(Button);
	//static�Ȃ̂ŃR���X�g���N�g���Ȃ��Ă���
};

#endif