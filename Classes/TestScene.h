#ifndef __TEST_SCENE_H__
#define __TEST_SCENE_H__
#include "cocos2d.h"
#include "Character.h"

class TestScene : public cocos2d::Layer
{
public:

	static cocos2d::Scene* createScene();

	virtual bool init();

	void update_per_second(float delta);

	virtual void update(float delta);

	CREATE_FUNC(TestScene);
};

#endif // !__GAME_TITLE_SCENE_H__