#include "GameTitleScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"
#include "TestScene.h"
#include "LivesScene.h"
#include "QuitScene.h"
USING_NS_CC;

using namespace CocosDenshion;
using namespace cocostudio::timeline;

Scene* GameTitleScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = GameTitleScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool GameTitleScene::init()
{
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("res/BGM/SuperMario.mp3");   //加载title背景音乐


	if (!Layer::init())
	{
		return false;
	}


	auto rootNode = CSLoader::createNode("GameTitle.csb");
	addChild(rootNode);
	//加载场景csb文件



	auto menu_Story = MenuItemImage::create("res/PICTURE/story_unselected.png","res/PICTURE/story_selected.png",CC_CALLBACK_1(GameTitleScene::menu_Image_Story_callback, this));
	auto menu_Web = MenuItemImage::create("res/PICTURE/web_unselected.png", "res/PICTURE/web_selected.png", CC_CALLBACK_1(GameTitleScene::menu_Image_Web_callback, this));
	auto menu_Quit = MenuItemImage::create("res/PICTURE/quit_unselected.png", "res/PICTURE/quit_selected.png", CC_CALLBACK_1(GameTitleScene::menu_Image_Quit_callback, this));
	//创建游戏菜单项：故事模式、网络模式、退出游戏
	
	auto mode_menu = Menu::create(menu_Story, menu_Web, menu_Quit, NULL);

	Size winSize = Director::getInstance()->getVisibleSize(); //获取窗口尺寸

	mode_menu->setPosition(winSize.width / 1.18, winSize.height / 3.5);
	mode_menu->alignItemsVertically(); //将菜单项排列好
	this->addChild(mode_menu);


	SimpleAudioEngine::getInstance()->playBackgroundMusic("res/BGM/SuperMario.mp3",true);

	return true;
}


void GameTitleScene::menu_Image_Story_callback(Ref* menu_Image_call_back)   //点选故事模式的函数执行内容
{
	Director::getInstance()->replaceScene(TransitionFade::create(2.5f, LivesScene::createScene()));
	SimpleAudioEngine::getInstance()->stopBackgroundMusic("res/BGM/title.mp3");
	SimpleAudioEngine::getInstance()->playEffect("res/SE/confirm.mp3");
}

void GameTitleScene::menu_Image_Web_callback(Ref* menu_Image_call_back)    //点选网络模式的函数执行内容
{
	SimpleAudioEngine::getInstance()->playEffect("res/SE/invalid.mp3");
}

void GameTitleScene::menu_Image_Quit_callback(Ref* menu_Image_call_back)   //点选退出游戏的函数执行内容
{
	Director::getInstance()->replaceScene(TransitionFade::create(2.0f, QuitScene::createScene()));
	SimpleAudioEngine::getInstance()->stopBackgroundMusic("res/BGM/title.mp3");
	SimpleAudioEngine::getInstance()->playEffect("res/SE/confirm.mp3");
}