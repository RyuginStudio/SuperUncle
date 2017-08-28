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

static Layer* Layer_BG;            //����+��
static Layer* Layer_UI;            //���+�÷�+ʱ��
static Layer* Layer_Control;       //ҡ��+��ť
static Layer* Layer_GameSettings;  //��Ϸ�������


map<string, bool> map_keyPressed;  //��ż��̰���״̬

extern bool isSporting;            //��ɫ�Ƿ����˶���

static Size visSize;               //��Ϸ��ʾ�ߴ�


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

	//����ͼƬ
	Controler::createBackGround(Layer_BG, visSize); 

	//����ҡ�˼��¼�
	VirtualRockerAndButton::getInstance(Layer_Control, visSize);

	//�����
	auto pChildrenArray = tiledMap->getChildren();

	SpriteBatchNode* child = NULL;
	Ref* pObject = NULL;

	for (auto it = pChildrenArray.begin(); it != pChildrenArray.end(); it++) {
		pObject = *it;
		child = (SpriteBatchNode*)pObject;
		child->getTexture()->setAliasTexParameters();
	}

	//������ͼ
    Director::getInstance()->setProjection(Director::Projection::_2D);  

	//��ɫ��ʼ�����
	character.initBeginPos(tiledMap);
	character.sp_man->setGlobalZOrder(1);
	this->addChild(character.sp_man);

	//������Ƭ��ͼ
	this->addChild(tiledMap, 9); 

	//BGM
	SimpleAudioEngine::getInstance()->playBackgroundMusic("res/BGM/Mission1BGM.mp3", true);

	//��ʱ��
	this->schedule(CC_CALLBACK_1(TestScene::update_per_second, this), 1.0f, "oneSecond");    //��Ϸʱ��˥����ÿ1.0������
	this->scheduleUpdate();

	//�����Ƽ����¼� => ҡһҡ��ͣ��Ϸ
	auto eventListenerAcceleration = EventListenerAcceleration::create(CC_CALLBACK_2(TestScene::onAcceleration, this));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListenerAcceleration, this);
    Device::setAccelerometerEnabled(true); //����������

	//���̼����¼�
	auto eventListenerKeyboard = EventListenerKeyboard::create();
	eventListenerKeyboard->onKeyPressed = CC_CALLBACK_2(TestScene::onKeyPressed, this);
	eventListenerKeyboard->onKeyReleased = CC_CALLBACK_2(TestScene::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListenerKeyboard, this);

	//����ң�а�ť�ȴ����¼�
	auto eventListenerTouch = EventListenerTouchOneByOne::create();
	eventListenerTouch->onTouchBegan = CC_CALLBACK_2(VirtualRockerAndButton::onTouchBegan, VirtualRockerAndButton::getInstance(Layer_UI, visSize));
	eventListenerTouch->onTouchMoved = CC_CALLBACK_2(VirtualRockerAndButton::onTouchesMoved, VirtualRockerAndButton::getInstance(Layer_UI, visSize));
	eventListenerTouch->onTouchEnded = CC_CALLBACK_2(VirtualRockerAndButton::onTouchEnded, VirtualRockerAndButton::getInstance(Layer_UI, visSize));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListenerTouch, this);

	//��ʼ�������ٿص�map����
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
	case cocos2d::EventKeyboard::KeyCode::KEY_PAUSE:   //pause����ͣ
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
	case cocos2d::EventKeyboard::KeyCode::KEY_F1:   //�÷ְ����
		score += 100000000000;
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_F2:   //�����������
		coin += 100000000000;
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_F3:   //��Ϸ��ͣ����
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

void TestScene::keyBoardControler(float delta) //���̿�������һ������ => ����֡��ʱ��
{
	if ((map_keyPressed.find("left")->second && map_keyPressed.find("right")->second)
		|| (!map_keyPressed.find("left")->second && !map_keyPressed.find("right")->second)) //ͬʱ����/̧���ɫͣ�Ͳ�ǰ
	{
		character.move(0, delta);
		return;
	}
	if (map_keyPressed.find("left")->second == true)  //�������
	{
		isSporting = true;
		character.move(-1, delta);
	}
	if (map_keyPressed.find("right")->second == true)  //�Ҽ�����
	{
		isSporting = true;
		character.move(1, delta);
	}
}

void TestScene::update_per_second(float delta) //1s��ʱ��
{
	//Controler::CreateUpdateUI(Layer_UI, visSize, gameTime, coin, score); //����|ˢ��ʱ���UI
	Controler::createCloud(Layer_BG, character.sp_man, visSize); 
}

void TestScene::update(float delta)  //֡��ʱ��
{
	Controler::tiledMapScroll(this, Layer_BG, Layer_UI, Layer_Control, tiledMap, character.sp_man, delta);  //��ͼ����
	Controler::cloudPosControl(Layer_BG);       //�ƶ�λ�ÿ���
	keyBoardControler(delta);                   //���̿�������һ������  =>  ����Ҳ����
	VirtualRockerAndButton::touchMoveControl(); //�������߿���
}

void TestScene::onAcceleration(Acceleration * acc, Event * unused_event)  //��ҡһҡ����ͣ��Ϸ
{
	//CCLOG("onAcceleration: acc->x: %f, acc->y: %f, acc->z: %f", acc->x, acc->y, acc->z);
    
    static Acceleration *tempAcc = nullptr;
    
    //CCLOG("%lf", acc->timestamp);
    
    if(tempAcc != nullptr)
    CCLOG("%f   %f   %f", acc->x - tempAcc->x, acc->y - tempAcc->y, acc->z - tempAcc->z);
    
       
    if(tempAcc != nullptr && (std::abs(acc->x - tempAcc->x) > 2 || std::abs(acc->y - tempAcc->y) > 2 || std::abs(acc->z - tempAcc->z) > 2))  // last version => 0.5
    {
        //δ�������
    }
}
