#ifndef __TEST_SCENE_H__
#define __TEST_SCENE_H__
#include "cocos2d.h"
#include "Character.h"

class TestScene : public cocos2d::Layer
{
public:

	int coin = 0;        //金币数
	int gameTime = 500;  //游戏时间总长
	int score = 0;       //计分板

	Character character;

	TMXTiledMap *tiledMap = cocos2d::TMXTiledMap::create("MAP/Mission1.tmx");//地图

	static cocos2d::Scene* createScene();

	virtual bool init();


	//键盘对角色的行为操控（事件回调+进一步处理）
	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event * event);
	void onKeyReleased(EventKeyboard::KeyCode keyCode, Event * event);
	void keyBoardControler(float delta); //键盘控制器

	void update_per_second(float delta);

	virtual void update(float delta);

	void onAcceleration(Acceleration* acc, Event* unused_event); //重写重力加速度函数 => “摇一摇”暂停游戏

	CREATE_FUNC(TestScene);
};

#endif // !__GAME_TITLE_SCENE_H__