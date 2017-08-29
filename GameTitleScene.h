#ifndef __GAME_TITLE_SCENE_H__
#define __GAME_TITLE_SCENE_H__
#include "cocos2d.h"

class GameTitleScene : public cocos2d::Layer
{
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	void menu_Image_Story_callback(Ref * menu_Image_call_back);  //����ģʽ�ص�

	void menu_Image_Web_callback(Ref * menu_Image_call_back);    //����ģʽ�ص�

	void menu_Image_Quit_callback(Ref * menu_Image_call_back);   //�˳���Ϸ�ص�


	// implement the "static create()" method manually
	CREATE_FUNC(GameTitleScene);
};
#endif // !__GAME_TITLE_SCENE_H__




