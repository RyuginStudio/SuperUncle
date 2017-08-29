#ifndef __QUIT_SCENE_H__
#define __QUIT_SCENE_H__

#include "cocos2d.h"

class QuitScene : public cocos2d::Layer
{
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	CREATE_FUNC(QuitScene);

	void quitgame();

};

#endif // !__QUIT_SCENE_H__