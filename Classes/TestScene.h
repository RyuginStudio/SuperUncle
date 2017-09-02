#ifndef __TEST_SCENE_H__
#define __TEST_SCENE_H__
#include "cocos2d.h"
#include "Box2D\Box2D.h"
#include "Character.h"

#define PTM_RATIO 32        

class TestScene : public cocos2d::Layer
{
public:

	b2World *world; //��������

	void initPysics();

	static cocos2d::Scene* createScene();

	virtual bool init();

	void update_per_second(float delta);

	virtual void update(float delta);

	TestScene();
	~TestScene();

	CREATE_FUNC(TestScene);
};

#endif // !__GAME_TITLE_SCENE_H__