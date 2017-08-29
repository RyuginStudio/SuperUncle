/*
*时间：2017年8月12日00:31:21
*作者：vszed
*功能：游戏总控
*说明：单例类
*/

#ifndef __CONTROLER_H__
#define __CONTROLER_H__
#include "cocos2d.h"

class Controler : public cocos2d::Sprite
{
public:

	//==============单例模式==============//
	static Controler *controler;
	static Controler* getInstance();
	~Controler();
	//==============单例模式==============//


	static void createCloud();

	static void cloudPosControl();

	static void CreateUpdateUI();            //创建|刷新UI：金币时间等

	static void tiledMapScroll(float delta); //地图滚动

	static void createBackGround();          //背景图片

	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event * event);   //键盘按下回调

	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event * event);  //键盘抬起回调

	static void keyBoardControler(float delta);  //键盘控制器进一步处理 => 放入帧定时器

	void onAcceleration(Acceleration * acc, Event * unused_event);  //重力计：“摇一摇”暂停游戏

	static void GamePauseAndSettings();      //游戏暂停与设置

private:

	Controler();
};

#endif // !__CONTROLER_H__