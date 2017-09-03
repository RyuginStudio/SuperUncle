#ifndef __TEST_SCENE_H__
#define __TEST_SCENE_H__
#include "cocos2d.h"
#include "Box2D\Box2D.h"
#include "Character.h"
#include "GLES-Render.h"

#define PTM_RATIO 32        

class TestScene : public cocos2d::Layer
{
public:

	b2World *world; //物理世界

	void initPysics();

	void createPhysicalUnCross();

	GLESDebugDraw *_debugDraw;

	virtual void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags);

	static cocos2d::Scene* createScene();

	virtual bool init();

	void update_per_second(float delta);

	virtual void update(float delta);

	TestScene();
	~TestScene();

	CREATE_FUNC(TestScene);
};

#endif // !__GAME_TITLE_SCENE_H__