/*
	功能：在屏幕上创建虚拟摇杆和按键
	作者：张天翼
	时间：2017年8月5日01:28:43
	特点：单例类
*/

#ifndef __VIRTUAL_ROCKER_AND_BUTTON_H__
#define __VIRTUAL_ROCKER_AND_BUTTON_H__
#include "cocos2d.h"

class VirtualRockerAndButton
{
private:

	static VirtualRockerAndButton *virtualRockerAndButton; //单例对象

	//构造函数为私有，只允许单例
	VirtualRockerAndButton();

	static void create(cocos2d::Layer* layer, cocos2d::Size visSize);

public:

	~VirtualRockerAndButton();

	static VirtualRockerAndButton* getInstance(cocos2d::Layer* layer, cocos2d::Size visSize); //获取单例对象

	//触摸事件回调
	bool onTouchBegan(cocos2d::Touch * tTouch, cocos2d::Event * eEvent);
	void onTouchesMoved(cocos2d::Touch * tTouch, cocos2d::Event * eEvent);
	void onTouchEnded(cocos2d::Touch * tTouch, cocos2d::Event * eEvent);

	static void touchMoveControl();
};

#endif // !__VIRTUAL_ROCKER_AND_BUTTON_H__