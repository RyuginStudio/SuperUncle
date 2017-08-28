#include <string>
#include <vector>
#include <iostream>
#include "TestScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"
#include "Character.h"
#include "Controler.h"
#include "VirtualRockerAndButton.h"

USING_NS_CC;

using namespace std;
using namespace CocosDenshion;

static Layer* Layer_BG;            //背景+云
static Layer* Layer_UI;            //金币+得分+时间
static Layer* Layer_Control;       //摇杆+按钮
static Layer* Layer_GameSettings;  //游戏相关设置


map<string, bool> map_keyPressed;  //存放键盘按键状态

extern bool isSporting;            //角色是否处于运动中

static Size visSize;               //游戏显示尺寸


Scene* TestScene::createScene()
{
	auto scene = Scene::create();
	auto layer = TestScene::create();
	scene->addChild(layer);
	return scene;
}

bool TestScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	visSize = Director::getInstance()->getVisibleSize();

	Layer_BG = Layer::create();
	Layer_BG->setName("Layer_BG");
	this->addChild(Layer_BG, 0);

	Layer_UI = Layer::create();
	Layer_UI->setName("Layer_UI");
	this->addChild(Layer_UI, 10);

	Layer_Control = Layer::create();
	Layer_Control->setName("Layer_Control");
	this->addChild(Layer_Control, 11);

	Layer_GameSettings = Layer::create();
	Layer_GameSettings->setName("Layer_GameSettings");
	this->addChild(Layer_GameSettings, 12);

	//背景图片
	Controler::createBackGround(Layer_BG, visSize); 

	//虚拟摇杆及事件
	VirtualRockerAndButton::getInstance(Layer_Control, visSize);

	//抗锯齿
	auto pChildrenArray = tiledMap->getChildren();

	SpriteBatchNode* child = NULL;
	Ref* pObject = NULL;

	for (auto it = pChildrenArray.begin(); it != pChildrenArray.end(); it++) {
		pObject = *it;
		child = (SpriteBatchNode*)pObject;
		child->getTexture()->setAliasTexParameters();
	}

	//正交视图
    Director::getInstance()->setProjection(Director::Projection::_2D);  

	//角色初始化相关
	character.initBeginPos(tiledMap);
	character.sp_man->setGlobalZOrder(1);
	this->addChild(character.sp_man);

	//加载瓦片地图
	this->addChild(tiledMap, 9); 

	//BGM
	SimpleAudioEngine::getInstance()->playBackgroundMusic("res/BGM/Mission1BGM.mp3", true);

	//定时器
	this->schedule(CC_CALLBACK_1(TestScene::update_per_second, this), 1.0f, "oneSecond");    //游戏时间衰减，每1.0秒后调用
	this->scheduleUpdate();

	//重力计监听事件 => 摇一摇暂停游戏
	auto eventListenerAcceleration = EventListenerAcceleration::create(CC_CALLBACK_2(TestScene::onAcceleration, this));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListenerAcceleration, this);
    Device::setAccelerometerEnabled(true); //开启重力计

	//键盘监听事件
	auto eventListenerKeyboard = EventListenerKeyboard::create();
	eventListenerKeyboard->onKeyPressed = CC_CALLBACK_2(TestScene::onKeyPressed, this);
	eventListenerKeyboard->onKeyReleased = CC_CALLBACK_2(TestScene::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListenerKeyboard, this);

	//监听遥感按钮等触摸事件
	auto eventListenerTouch = EventListenerTouchOneByOne::create();
	eventListenerTouch->onTouchBegan = CC_CALLBACK_2(VirtualRockerAndButton::onTouchBegan, VirtualRockerAndButton::getInstance(Layer_UI, visSize));
	eventListenerTouch->onTouchMoved = CC_CALLBACK_2(VirtualRockerAndButton::onTouchesMoved, VirtualRockerAndButton::getInstance(Layer_UI, visSize));
	eventListenerTouch->onTouchEnded = CC_CALLBACK_2(VirtualRockerAndButton::onTouchEnded, VirtualRockerAndButton::getInstance(Layer_UI, visSize));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListenerTouch, this);

	//初始化即将操控的map容器
	map_keyPressed.insert(pair<string, bool>("up", false));
	map_keyPressed.insert(pair<string, bool>("down", false));
	map_keyPressed.insert(pair<string, bool>("left", false));
	map_keyPressed.insert(pair<string, bool>("right", false));

	return true;
}

void TestScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event * event)
{
	switch (keyCode)
	{
	case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
		map_keyPressed.find("up")->second = true;
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		map_keyPressed.find("down")->second = true;
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		map_keyPressed.find("left")->second = true;
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		map_keyPressed.find("right")->second = true;
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_Z:
		character.jump();
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_PAUSE:   //pause键暂停
	{
		if (!Director::getInstance()->isPaused())
		{
			Director::getInstance()->pause();
			SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
			SimpleAudioEngine::getInstance()->pauseAllEffects();
		}
		else
		{
			Director::getInstance()->resume();
			SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
			SimpleAudioEngine::getInstance()->resumeAllEffects();
		}
		break;
	}
	case cocos2d::EventKeyboard::KeyCode::KEY_F1:   //得分板测试
		score += 100000000000;
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_F2:   //金币数量测试
		coin += 100000000000;
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_F3:   //游戏暂停测试
		Controler::GamePauseAndSettings(this, Layer_GameSettings, visSize);
		break;
	default:
		break;
	}
}

void TestScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event * event)
{
	switch (keyCode)
	{
	case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
		map_keyPressed.find("up")->second = false;
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		map_keyPressed.find("down")->second = false;
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		map_keyPressed.find("left")->second = false;
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		map_keyPressed.find("right")->second = false;
		break;
	default:
		break;
	}
}

void TestScene::keyBoardControler(float delta) //键盘控制器进一步处理 => 放入帧定时器
{
	if ((map_keyPressed.find("left")->second && map_keyPressed.find("right")->second)
		|| (!map_keyPressed.find("left")->second && !map_keyPressed.find("right")->second)) //同时按下/抬起角色停滞不前
	{
		character.move(0, delta);
		return;
	}
	if (map_keyPressed.find("left")->second == true)  //左键按下
	{
		isSporting = true;
		character.move(-1, delta);
	}
	if (map_keyPressed.find("right")->second == true)  //右键按下
	{
		isSporting = true;
		character.move(1, delta);
	}
}

void TestScene::update_per_second(float delta) //1s定时器
{
	//Controler::CreateUpdateUI(Layer_UI, visSize, gameTime, coin, score); //创建|刷新时间等UI
	Controler::createCloud(Layer_BG, character.sp_man, visSize); 
}

void TestScene::update(float delta)  //帧定时器
{
	Controler::tiledMapScroll(this, Layer_BG, Layer_UI, Layer_Control, tiledMap, character.sp_man, delta);  //地图滚动
	Controler::cloudPosControl(Layer_BG);       //云朵位置控制
	keyBoardControler(delta);                   //键盘控制器进一步处理  =>  触控也调用
	VirtualRockerAndButton::touchMoveControl(); //触摸行走控制
}

void TestScene::onAcceleration(Acceleration * acc, Event * unused_event)  //“摇一摇”暂停游戏
{
	//CCLOG("onAcceleration: acc->x: %f, acc->y: %f, acc->z: %f", acc->x, acc->y, acc->z);
    
    static Acceleration *tempAcc = nullptr;
    
    //CCLOG("%lf", acc->timestamp);
    
    if(tempAcc != nullptr)
    CCLOG("%f   %f   %f", acc->x - tempAcc->x, acc->y - tempAcc->y, acc->z - tempAcc->z);
    
       
    if(tempAcc != nullptr && (std::abs(acc->x - tempAcc->x) > 2 || std::abs(acc->y - tempAcc->y) > 2 || std::abs(acc->z - tempAcc->z) > 2))  // last version => 0.5
    {
        //未处理完毕
    }
}
