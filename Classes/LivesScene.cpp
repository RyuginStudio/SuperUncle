#include <string>
#include <iostream>
#include "LivesScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"
#include "GameData.h"
#include "TestScene.h"

USING_NS_CC;

#define TEST 9999

using namespace std;
using namespace CocosDenshion;
using namespace cocostudio::timeline;

extern int mario_lives;
extern int mission;

Scene* LivesScene::createScene()
{
	auto scene = Scene::create();
	auto layer = LivesScene::create();
	scene->addChild(layer);
	return scene;
}

bool LivesScene::init()
{

	if (!Layer::init())
	{
		return false;
	}

	auto visSize = Director::getInstance()->getVisibleSize();

	string string_mario_lives = to_string(mario_lives);

	auto pic_lives = Sprite::create("res/PICTURE/lives.png");                              //���������ͷ��
	auto label_lives = Label::create("x"+string_mario_lives,"NewSuperMarioFontU.ttf",80);  //����»��м�����
	label_lives->setAnchorPoint(Vec2(0, 0.5));
	
	pic_lives->setPosition(Point(visSize.width / 2 - 50, visSize.height / 2));
	label_lives->setPosition(Point(visSize.width * 0.5, visSize.height / 2));

	this->addChild(label_lives);
	this->addChild(pic_lives);

	//�趨��ʱ���ȴ�������ȷ����ҿ������ж�������
	this->scheduleOnce(CC_CALLBACK_0(LivesScene::missionJump, this), 3.f, "jumpToMissionScene");  //3�����ת���ؿ�

	return true;
}

void LivesScene::missionJump()
{
	//��������ת��XX�����ؿ�
	switch (mission)//����mission����ֵ��ת�ؿ�
	{
	case 1://��һ��

		break;
	case 2:

		break;
	case 3:

		break;
	case 4:

		break;
	case 5:

		break;
	case 6:

		break;
	case 7:

		break;
	case 8:

		break;
	case 9:

		break;
	case 10://��ʮ��

		break;

	case TEST://���Թ�
	{		
		Director::getInstance()->replaceScene(TransitionFade::create(2.f, TestScene::createScene()));
		break;
	}
		
	default://���Ǵ��ĳЩ����������Ϊ���عؿ�
		break;
	}

}