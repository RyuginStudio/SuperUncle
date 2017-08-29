#ifndef __LOGO_SCENE_H__
#define __LOGO_SCENE_H__

#include "cocos2d.h"

class LogoScene : public cocos2d::Layer
{
public:

	LogoScene();

	~LogoScene();

	static cocos2d::Scene* createScene();

	virtual bool init();

	void replaceScene();

	CREATE_FUNC(LogoScene);

};

#endif // !__LOGO_SCENE_H__