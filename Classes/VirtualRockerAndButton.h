/*
*功能：在屏幕上创建虚拟摇杆和按键
*作者：vszed
*时间：2017年8月5日01:28:43
*特点：单例类
*/

#ifndef __VIRTUAL_ROCKER_AND_BUTTON_H__
#define __VIRTUAL_ROCKER_AND_BUTTON_H__
#include "cocos2d.h"

class VirtualRockerAndButton
{
private:

	//==============单例模式==============//
	static VirtualRockerAndButton *virtualRockerAndButton;

	//构造函数为私有，只允许单例
	VirtualRockerAndButton();
	//==============单例模式==============//

public:

	~VirtualRockerAndButton();

	static VirtualRockerAndButton* getInstance(); //获取单例对象

	//触摸事件回调
	bool onTouchBegan(cocos2d::Touch * tTouch, cocos2d::Event * eEvent);
	void onTouchesMoved(cocos2d::Touch * tTouch, cocos2d::Event * eEvent);
	void onTouchEnded(cocos2d::Touch * tTouch, cocos2d::Event * eEvent);

	static void touchMoveControl();
};

#endif // !__VIRTUAL_ROCKER_AND_BUTTON_H__