/*
*ʱ�䣺2017��8��12��00:31:21
*���ߣ�vszed
*���ܣ���Ϸ�ܿ�
*/

#ifndef __CONTROLER_H__
#define __CONTROLER_H__
#include "cocos2d.h"

class Controler : public cocos2d::Sprite
{
public:

	static void createCloud(cocos2d::Layer *, cocos2d::Node *, cocos2d::Size visSize); //��2����ɫ����

	static void cloudPosControl(cocos2d::Layer * layer);

	static void CreateUpdateUI(cocos2d::Layer *, cocos2d::Size, int &gameTime, int &coin, int &score); //����|ˢ��UI�����ʱ���

	static void tiledMapScroll(cocos2d::Layer *, cocos2d::Layer *, cocos2d::Layer *, cocos2d::Layer *, cocos2d::TMXTiledMap * tiledMap, cocos2d::Node * character, float delta); //��ͼ����

	static void createBackGround(cocos2d::Layer *, cocos2d::Size); //BG����

	static void GamePauseAndSettings(cocos2d::Layer *, cocos2d::Layer *, cocos2d::Size); //��Ϸ��ͣ������
};

#endif // !__CONTROLER_H__