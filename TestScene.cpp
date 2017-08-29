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

Layer* Layer_BG;            //����+��
Layer* Layer_UI;            //���+�÷�+ʱ��
Layer* Layer_Control;       //ҡ��+��ť
Layer* Layer_GameSettings;  //��Ϸ�������
Layer* Layer_GameScene;     //��Ϸ����

extern map<string, bool> map_keyPressed;
extern Size visSize;


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

	Layer_GameScene = this;


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
	Character::getInstance()->initBeginPos(tiledMap);
	Character::getInstance()->sp_man->setGlobalZOrder(1);
	this->addChild(Character::getInstance()->sp_man);

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
	eventListenerKeyboard->onKeyPressed = CC_CALLBACK_2(Controler::onKeyPressed, Controler::getInstance());
	eventListenerKeyboard->onKeyReleased = CC_CALLBACK_2(Controler::onKeyReleased, Controler::getInstance());
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

void TestScene::update_per_second(float delta) //1s��ʱ��
{
	//Controler::CreateUpdateUI(Layer_UI, visSize, gameTime, coin, score); //����|ˢ��ʱ���UI
	Controler::createCloud(Layer_BG, Character::getInstance()->sp_man);
}

void TestScene::update(float delta)  //֡��ʱ��
{
	Controler::tiledMapScroll(this, Layer_BG, Layer_UI, Layer_Control, tiledMap, Character::getInstance()->sp_man, delta);  //��ͼ����
	Controler::cloudPosControl();       //�ƶ�λ�ÿ���
	Controler::keyBoardControler(delta);        //���̿�������һ������  =>  ����Ҳ����
	VirtualRockerAndButton::touchMoveControl(); //�������߿���
}

void TestScene::onAcceleration(Acceleration * acc, Event * unused_event)  //��ҡһҡ����ͣ��Ϸ
{
	static Acceleration lastAcc = *acc;

	struct timeval nowTimeval;
	gettimeofday(&nowTimeval, NULL);
	static long lLastTime = 0;

	//x,y,z����ļ��ٵ�ƽ������������һ����ֵ��Ϊ��ҡһҡ��
	if ((acc->x - lastAcc.x)*(acc->x - lastAcc.x) + (acc->y - lastAcc.y)*(acc->y - lastAcc.y) + (acc->z - lastAcc.z)*(acc->z - lastAcc.z) >= 10)
	{
		if (nowTimeval.tv_sec - lLastTime > 1.0)
		{
			if (!Director::getInstance()->isPaused())
				Controler::GamePauseAndSettings(this, Layer_GameSettings, visSize);
			else
			{
				Layer_GameSettings->setVisible(false);
				Director::getInstance()->resume();
				SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
				SimpleAudioEngine::getInstance()->resumeAllEffects();
			}
			lLastTime = nowTimeval.tv_sec;
		}
	}
	lastAcc = *acc;
}
