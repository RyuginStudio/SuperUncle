/*
*功能：在屏幕上创建虚拟摇杆和按键
*作者：vszed
*时间：2017年8月5日01:28:43
*特点：单例类
*/

#ifndef __VIRTUAL_ROCKER_AND_BUTTON_H__
#define __VIRTUAL_ROCKER_AND_BUTTON_H__
#include "cocos2d.h"

USING_NS_CC;


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
	void onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event);
	void onTouchesMoved(const std::vector<Touch*>& touches, Event *unused_event);
	void onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event);

	static void touchMoveControl();

	//是否允许摇杆move开关
	bool admitRockerMove;

	//触摸id
	int btnAtouchID;
	int btnBtouchID;
	int rockerTouchID;
	

	//摇杆和托盘路径信息
	std::string circlePath = "PICTURE/VirtualCircle.png";
	std::string pointPath = "PICTURE/VirtualPoint.png";

	//按钮A和B的路径信息
	std::string btnApath = "PICTURE/VirtualButtonA.png";
	std::string btnBpath = "PICTURE/VirtualButtonB.png";

};

#endif // !__VIRTUAL_ROCKER_AND_BUTTON_H__