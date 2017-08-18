#ifndef __LIVES_SCENE_H__
#define __LIVES_SECNE_H__

#include "cocos2d.h"

class LivesScene : public cocos2d::Layer
{
public:

	static cocos2d::Scene* createScene();

	virtual bool init();

	void missionJump();

	CREATE_FUNC(LivesScene);
};

#endif // !__LIVE_SCENE_H__