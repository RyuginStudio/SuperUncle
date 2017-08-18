#ifndef __LIVES_SCENE_H__
#define __LIVES_SECNE_H__

#include "cocos2d.h"

class LivesScene : public cocos2d::Layer
{
public:

	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	CREATE_FUNC(LivesScene);


	void missionJump(float dt); //¹Ø¿¨Ìø×ªº¯Êý
};

#endif // !__LIVE_SCENE_H__