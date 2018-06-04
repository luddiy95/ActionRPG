#include "GameLib/GameLib.h"
#include "Pad.h"
#include "GameLib/Input/Manager.h"
#include "GameLib/Input/Keyboard.h"
#include "GameLib/Input/Joystick.h"
using namespace GameLib;

bool Pad::isOn(Button b) {
	Input::Manager manager = Input::Manager::instance();
	Input::Keyboard keyboard = manager.keyboard();
	bool ret = false; //�{�^��b�������ꂽ��
	int table[] = { 'o', 'j', 'm', 'n', 'w', 'a', 'z', 's', 'e', 'q' };
	//Button�񋓎q�Ɠ���
	ret = keyboard.isOn(table[b]);
	if (manager.joystickNumber() > 0) {
		//joystick���h�����Ă���
		Input::Joystick joystick = manager.joystick();
		int table[] = {
			0,
			3,
			2,
			1,
			joystick.analog(1),
			joystick.analog(0),
			joystick.analog(1),
			joystick.analog(0),
			6,
			7
		};
		switch (b) {
		case U: case L:
			if (table[b] == 0) {
				ret = true;
			}break;
		case DW: case R:
			if (table[b] == 65535) {
				ret = true;
			}
			break;
		default:
			if (joystick.isOn(table[b])) {
				ret = true;
			}
		}
		//ret = joystick.isOn(table[b])�͎h�����Ă邾����false�ɂȂ�̂Ń_��
	}
	return ret;
}

bool Pad::isTriggered(Button b) {
	Input::Manager manager = Input::Manager::instance();
	Input::Keyboard keyboard = manager.keyboard();
	bool ret = false; //�{�^��b�������ꂽ��
	int table[] = { 'o', 'j', 'm', 'n', 'w', 'a', 'z', 's', 'e', 'q' };
	//Button�񋓎q�Ɠ���
	ret = keyboard.isTriggered(table[b]);
	if (manager.joystickNumber() > 0) {
		//joystick���h�����Ă���
		Input::Joystick joystick = manager.joystick();
		int table[] = {
			0,
			3,
			2,
			1,
			joystick.analog(1),
			joystick.analog(0),
			joystick.analog(1),
			joystick.analog(0),
			6,
			7
		};
		switch (b) {
		case U: case L:
			if (table[b] == 0) {
				ret = true;
			}break;
		case DW: case R:
			if (table[b] == 65535) {
				ret = true;
			}
			break;
		default:
			if (joystick.isTriggered(table[b])) {
				ret = true;
			}
		}
	}
	return ret;
}