/*
	���ܣ�����Ļ�ϴ�������ҡ�˺Ͱ���
	���ߣ�������
	ʱ�䣺2017��8��5��01:28:43
	�ص㣺������
*/

#ifndef __VIRTUAL_ROCKER_AND_BUTTON_H__
#define __VIRTUAL_ROCKER_AND_BUTTON_H__
#include "cocos2d.h"

class VirtualRockerAndButton
{
private:

	static VirtualRockerAndButton *virtualRockerAndButton; //��������

	//���캯��Ϊ˽�У�ֻ������
	VirtualRockerAndButton();

	static void create(cocos2d::Layer* layer, cocos2d::Size visSize);

public:

	~VirtualRockerAndButton();

	static VirtualRockerAndButton* getInstance(cocos2d::Layer* layer, cocos2d::Size visSize); //��ȡ��������

	//�����¼��ص�
	bool onTouchBegan(cocos2d::Touch * tTouch, cocos2d::Event * eEvent);
	void onTouchesMoved(cocos2d::Touch * tTouch, cocos2d::Event * eEvent);
	void onTouchEnded(cocos2d::Touch * tTouch, cocos2d::Event * eEvent);

	static void touchMoveControl();
};

#endif // !__VIRTUAL_ROCKER_AND_BUTTON_H__