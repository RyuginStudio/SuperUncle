#include "LogoScene.h"
#include "SimpleAudioEngine.h"
#include "GameTitleScene.h"

USING_NS_CC;
using namespace CocosDenshion;

LogoScene::LogoScene()
{
}


LogoScene::~LogoScene()
{
}

cocos2d::Scene * LogoScene::createScene()
{
	auto scene = Scene::create();
	auto layer = LogoScene::create();
	scene->addChild(layer);
	return scene;
}

bool LogoScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto size = Director::getInstance()->getVisibleSize();
	auto sp_logo = Sprite::create("vszedLogo.png");
	sp_logo->setPosition(size.width / 2, size.height / 2);
	this->addChild(sp_logo);


	SimpleAudioEngine::getInstance()->playEffect("SE/vszed.wav");

	this->scheduleOnce(CC_CALLBACK_0(LogoScene::replaceScene, this), 2.5f, "replaceScene");

	return true;
}

void LogoScene::replaceScene()
{
	Director::getInstance()->replaceScene(TransitionTurnOffTiles::create(.7f, GameTitleScene::createScene()));
}
