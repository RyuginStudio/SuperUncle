/*
*���ܣ�����Ļ�ϴ�������ҡ�˺Ͱ���
*���ߣ�vszed
*ʱ�䣺2017��8��5��01:28:43
*�ص㣺������
*/

#ifndef __VIRTUAL_ROCKER_AND_BUTTON_H__
#define __VIRTUAL_ROCKER_AND_BUTTON_H__
#include "cocos2d.h"

class VirtualRockerAndButton
{
private:

	//==============����ģʽ==============//
	static VirtualRockerAndButton *virtualRockerAndButton;

	//���캯��Ϊ˽�У�ֻ������
	VirtualRockerAndButton();
	//==============����ģʽ==============//

public:

	~VirtualRockerAndButton();

	static VirtualRockerAndButton* getInstance(); //��ȡ��������

	//�����¼��ص�
	bool onTouchBegan(cocos2d::Touch * tTouch, cocos2d::Event * eEvent);
	void onTouchesMoved(cocos2d::Touch * tTouch, cocos2d::Event * eEvent);
	void onTouchEnded(cocos2d::Touch * tTouch, cocos2d::Event * eEvent);

	static void touchMoveControl();
};

#endif // !__VIRTUAL_ROCKER_AND_BUTTON_H__