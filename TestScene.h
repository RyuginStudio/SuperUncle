#ifndef __TEST_SCENE_H__
#define __TEST_SCENE_H__
#include "cocos2d.h"
#include "Character.h"

class TestScene : public cocos2d::Layer
{
public:

	TMXTiledMap *tiledMap = cocos2d::TMXTiledMap::create("MAP/Mission1.tmx");//地图

	static cocos2d::Scene* createScene();

	virtual bool init();

	void update_per_second(float delta);

	virtual void update(float delta);

	void onAcceleration(Acceleration* acc, Event* unused_event); //重写重力加速度函数 => “摇一摇”暂停游戏

	CREATE_FUNC(TestScene);
};

#endif // !__GAME_TITLE_SCENE_H__