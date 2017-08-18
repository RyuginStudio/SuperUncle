#ifndef __TEST_SCENE_H__
#define __TEST_SCENE_H__
#include "cocos2d.h"
#include "Character.h"

class TestScene : public cocos2d::Layer
{
public:

	unsigned long long coin = 0;      //本关卡获得的金币数 => longlong防止修改器导致游戏崩溃

	unsigned long long gameTime = 500;  //本关卡游戏时间总长

	unsigned long long score = 0;      //本关卡计分板

	Character character;

	TMXTiledMap *tiledMap = cocos2d::TMXTiledMap::create("res/MAP/Mission1.tmx");//地图

	static cocos2d::Scene* createScene();

	virtual bool init();


	//键盘对角色的行为操控（事件回调+进一步处理）
	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event * event);
	void onKeyReleased(EventKeyboard::KeyCode keyCode, Event * event);
	void keyBoardControler(); //键盘控制器

	void update_per_second(float delta);  //设定每秒的监听事件回调函数

	virtual void update(float delta); //设定每帧的监听事件回调函数

	CREATE_FUNC(TestScene);
};
#endif // !__GAME_TITLE_SCENE_H__