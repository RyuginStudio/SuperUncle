/*
时间：2017年7月17日10:38:23
作者：张天翼
功能：测试场景头文件用于游戏测试
*/

#ifndef __B2TEST_SCENE_H__
#define __B2TEST_SCENE_H__

#include "cocos2d.h"
#include "Box2D/Box2D.h"  //引入box2d所需的头文件

#define PTM_RATIO 62     //宏定义 => 屏幕上的32像素为1米

class b2TestScene : public cocos2d::Layer, public b2ContactListener
{

	b2World* world;  //b2World类型指针world => 数据成员  => 声明一个box2d的世界

public:

	static cocos2d::Scene * createScene();

	virtual bool init();

	virtual void update(float dt);  //帧定时器回调函数

	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);  //重写父类layer中的onTouchBegan函数 => 触摸点击函数
	
	void onKeyPressed(cocos2d::EventKeyboard::KeyCode, cocos2d::Event*);   //键盘按下与抬起的回调
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode, cocos2d::Event*);

	CREATE_FUNC(b2TestScene);

	void iniPhysics();    //初始化物理引擎函数
	void addNewSpriteAtPosition(cocos2d::Point p);  //在指定的坐标添加精灵对象

	virtual void BeginContact(b2Contact* contact);
	virtual void EndContact(b2Contact* contact);

	void delete_contact_body();

	b2TestScene();
	~b2TestScene();
};

#endif // !__B2TEST_SCENE_H__
