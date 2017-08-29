/*
*时间：2017年8月12日00:31:21
*作者：vszed
*功能：游戏总控
*说明：单例类
*/

#include <iostream>
#include "Character.h"
#include "Controler.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

using namespace std;
using namespace CocosDenshion;


//声明：角色运动判定
extern bool isSporting;

//定义：容器
static vector<Sprite *> vector_Cloud;  //云朵精灵
map<string, bool> map_keyPressed;      //键盘状态

//定义：UI资源
static Sprite* icon_Time;
static Sprite* icon_Coin;
static Label *time_left;
static Label *get_coin_number;
static Label *get_score;

//定义：显示尺寸
Size visSize;

//声明：相关数据
extern int coin;
extern int gameTime;
extern int score;

//声明：相关层
extern Layer* Layer_BG;
extern Layer* Layer_UI;
extern Layer* Layer_Control;
extern Layer* Layer_GameSettings;
extern Layer* Layer_GameScene;

//声明：瓦片地图
extern TMXTiledMap *tiledMap;


//==============单例模式==============//
Controler* Controler::controler = nullptr;

Controler* Controler::getInstance()
{
	if (controler == nullptr)
		controler = new Controler();
	return controler;
}

Controler::~Controler()
{
	CC_SAFE_DELETE(controler);
}

Controler::Controler()
{
}
//==============单例模式==============//


void Controler::createCloud()
{
	auto cloud_Big = Sprite::create("PICTURE/cloud_Big.png");
	vector_Cloud.push_back(cloud_Big);
	cloud_Big->setPosition(
		Vec2(visSize.width*1.1 + Character::getInstance()->sp_man->getPosition().x, visSize.height / random(1.0, 1.2)));
	Layer_BG->addChild(cloud_Big, 4);
}

void Controler::cloudPosControl()
{
	if (vector_Cloud.size() > 0) //防止容器为空
	{
		for (int idx = 0; idx != vector_Cloud.size(); idx++)
		{
			int pos_x = (int)vector_Cloud[idx]->getPosition().x;
			int pos_y = (int)vector_Cloud[idx]->getPosition().y;
			vector_Cloud[idx]->setPosition(Point(pos_x -= 1, pos_y));  //每帧都改变云朵的位置

			if (pos_x < -vector_Cloud[idx]->getContentSize().width)  //如果精灵的坐标（完全隐没） 
			{
				Layer_BG->removeChild(vector_Cloud[idx]);
				vector_Cloud.erase(vector_Cloud.begin() + idx);  //删除完全隐没的当前元素
				idx--;
			}
		}
	}
}

void Controler::CreateUpdateUI()
{
	static bool onCreate = true; //创建UI开关

	if (onCreate == true)
	{
		onCreate = false;
		icon_Time = Sprite::create("res/PICTURE/time_UI.png");  //游戏中时间剩余图标
		icon_Coin = Sprite::create("res/PICTURE/coin_UI.png");  //游戏中获取金币图标

		icon_Time->setPosition(Point(visSize.width / 1.12, visSize.height / 1.08));
		icon_Coin->setPosition(Point(visSize.width / 20, visSize.height / 1.08));

		Layer_UI->addChild(icon_Time, 2);
		Layer_UI->addChild(icon_Coin, 2);
	}

	Layer_UI->removeChild(time_left);
	Layer_UI->removeChild(get_coin_number);
	Layer_UI->removeChild(get_score);

	auto* str_gameTime = __String::createWithFormat("%d", gameTime);

	--gameTime; //时间衰减

	time_left = Label::createWithTTF(str_gameTime->getCString(), "NewSuperMarioFontU.ttf", 45);
	time_left->setName("time_left");

	time_left->enableOutline(Color4B(0, 0, 0, 255), 4);  //黑色描边  最后一个参数是透明度

	time_left->setPosition(Point(visSize.width / 1.06, visSize.height / 1.086));

	Layer_UI->addChild(time_left, 2);


	//=======================以下为显示金币个数的label===============================//
	if (coin < 10)
		get_coin_number = Label::createWithTTF("x0" + __String::createWithFormat("%d", coin)->_string, "NewSuperMarioFontU.ttf", 45);  //本关卡中获得了多少金币
	else
		get_coin_number = Label::createWithTTF("x" + __String::createWithFormat("%d", coin)->_string, "NewSuperMarioFontU.ttf", 45);  //本关卡中获得了多少金币

	get_coin_number->setName("get_coin_number");

	get_coin_number->setAnchorPoint(Vec2(0, 0.5));

	get_coin_number->enableOutline(Color4B(0, 0, 0, 255), 4);  //黑色描边  最后一个参数是透明度

	get_coin_number->setPosition(Point(Point(visSize.width * 0.063, visSize.height / 1.086)));

	Layer_UI->addChild(get_coin_number, 2);


	//=======================以下为显示本关卡得分的label=======================//

	int k = 0; //计算分数为多少位数字
	auto temp_score = score;

	string numberOfZero; //0的个数

	while (temp_score)
	{
		temp_score /= 10;
		k++;
	}

	for (int i = 0; i < 8 - k; i++) //最多补8个零
		numberOfZero += "0";

	get_score = Label::createWithTTF(numberOfZero + __String::createWithFormat("%d", score)->_string, "NewSuperMarioFontU.ttf", 40);  //本关卡中获得了多少金币

	get_score->setAnchorPoint(Vec2(1, 0.5));

	get_score->setName("get_score");

	get_score->setTextColor(Color4B(0, 0, 0, 255)); //黑色字体

	get_score->setAdditionalKerning(8.0f);  //设置字与字之间的宽度

	get_score->enableOutline(Color4B(255, 255, 255, 255), 4);  //白色描边  最后一个参数是透明度

	get_score->setPosition(Point(Point(visSize.width * 0.85, visSize.height / 1.086)));

	Layer_UI->addChild(get_score, 2);


}

void Controler::tiledMapScroll(float delta)
{
	Size winSize = Director::getInstance()->getWinSize();   //获取屏幕的尺寸

	Size mapNumbers = tiledMap->getMapSize();  //获取地图方块数量（元素个数）
	Size tiledSize = tiledMap->getTileSize();  //获取瓦片元素的尺寸（单位元素尺寸）
	Size MapSize = Size(mapNumbers.width*tiledSize.width, mapNumbers.height*tiledSize.height); //计算得出整个瓦片地图的尺寸

	auto sprite_pos = Character::getInstance()->sp_man->getPosition(); //获取角色坐标

	 //如果主角坐标小于屏幕的一半，那么取屏幕中点坐标，否则，取角色坐标
	float x = std::max(sprite_pos.x, winSize.width / 2);
	float y = std::max(sprite_pos.y, winSize.height / 2);

	//如果x,y的坐标大于右上角的极限值，则取极限值的坐标（极限值是指不让地图超过屏幕造成出现黑边的极限坐标）
	x = std::min(x, MapSize.width - winSize.width / 2);
	y = std::min(y, MapSize.height - winSize.height / 2);

	//目标点
	Point destPos = Point(x, y);

	//屏幕中心点
	Point centerPos = Point(winSize.width / 2, winSize.height / 2);

	//计算屏幕中心点和要移动至的目的点之间的距离
	Point distance = centerPos - destPos;

	static Point temp_distance;
	if (temp_distance.x > distance.x)  //场景左移
	{
		auto pos_layer = Layer_GameScene->getPosition();
		if (x >= MapSize.width - winSize.width / 2) //防地图黑边
			Layer_GameScene->setPosition(distance);
		else
			Layer_GameScene->setPosition(Point(pos_layer.x - delta * 300, pos_layer.y));


		auto pos_layerBG = Layer_BG->getPosition();
		Layer_BG->setPosition(Point(pos_layerBG.x + delta * 280, pos_layerBG.y));

		auto pos_layerUI = Layer_UI->getPosition();
		Layer_UI->setPosition(Point(pos_layerUI.x + delta * 300, pos_layerUI.y));

		auto pos_layerControler = Layer_Control->getPosition();
		Layer_Control->setPosition(Point(pos_layerControler.x + delta * 300, pos_layerControler.y));

		temp_distance.x = distance.x;
	}
	else if (temp_distance.x < distance.x)  //场景右移
	{
		auto pos_layer = Layer_GameScene->getPosition();
		if (x <= winSize.width / 2)
			Layer_GameScene->setPosition(distance);
		else
			Layer_GameScene->setPosition(Point(pos_layer.x + delta * 300, pos_layer.y));


		auto pos_layerBG = Layer_BG->getPosition();
		Layer_BG->setPosition(Point(pos_layerBG.x - delta * 280, pos_layerBG.y));

		auto pos_layerUI = Layer_UI->getPosition();
		Layer_UI->setPosition(Point(pos_layerUI.x - delta * 300, pos_layerUI.y));

		auto pos_layerControler = Layer_Control->getPosition();
		Layer_Control->setPosition(Point(pos_layerControler.x - delta * 300, pos_layerControler.y));

		temp_distance.x = distance.x;
	}
}

void Controler::createBackGround()  //创建游戏背景
{
	Sprite* background = Sprite::create("res/MAP/backGround1.png");  //游戏背景
	background->setAnchorPoint(Point(0, 0));
	background->setPosition(Point(0, visSize.height*0.14));  //设定背景位置
	Layer_BG->addChild(background, 2);  //创建游戏背景
}

void Controler::onKeyPressed(EventKeyboard::KeyCode keyCode, Event * event)
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
		Character::getInstance()->jump();
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_P:
	{
		if (!Director::getInstance()->isPaused())
		{
			Layer_Control->setVisible(false);
			Director::getInstance()->pause();
			SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
			SimpleAudioEngine::getInstance()->pauseAllEffects();
			SimpleAudioEngine::getInstance()->playEffect("SE/invalid.mp3");

			Controler::GamePauseAndSettings(); //加载：暂停相关设置
		}
		else
		{
			Layer_Control->setVisible(true);
			Layer_GameSettings->setVisible(false);
			Director::getInstance()->resume();
			SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
			SimpleAudioEngine::getInstance()->resumeAllEffects();
		}
		break;
	}
	case cocos2d::EventKeyboard::KeyCode::KEY_F1:   //得分板测试
		score += 10000;
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_F2:   //金币数量测试
		coin += 10000;
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_F3:   //游戏暂停测试
		Controler::GamePauseAndSettings();
		break;
	default:
		break;
	}
}

void Controler::onKeyReleased(EventKeyboard::KeyCode keyCode, Event * event)
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

void Controler::keyBoardControler(float delta) //键盘控制器进一步处理 => 放入帧定时器
{
	if ((map_keyPressed.find("left")->second && map_keyPressed.find("right")->second)
		|| (!map_keyPressed.find("left")->second && !map_keyPressed.find("right")->second)) //同时按下/抬起角色停滞不前
	{
		Character::getInstance()->move(0, delta);
		return;
	}
	if (map_keyPressed.find("left")->second == true)  //左键按下
	{
		isSporting = true;
		Character::getInstance()->move(-1, delta);
	}
	if (map_keyPressed.find("right")->second == true)  //右键按下
	{
		isSporting = true;
		Character::getInstance()->move(1, delta);
	}
}

void Controler::onAcceleration(Acceleration * acc, Event * unused_event)  //重力计：“摇一摇”暂停游戏
{
	static Acceleration lastAcc = *acc;

	struct timeval nowTimeval;
	gettimeofday(&nowTimeval, NULL);
	static long lLastTime = 0;

	//x,y,z方向的加速的平方加起来超过一个阈值视为“摇一摇”
	if ((acc->x - lastAcc.x)*(acc->x - lastAcc.x) + (acc->y - lastAcc.y)*(acc->y - lastAcc.y) + (acc->z - lastAcc.z)*(acc->z - lastAcc.z) >= 10)
	{
		if (nowTimeval.tv_sec - lLastTime > 1.0)
		{
			if (!Director::getInstance()->isPaused())
			{
				Layer_Control->setVisible(false);
				Director::getInstance()->pause();
				SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
				SimpleAudioEngine::getInstance()->pauseAllEffects();
				SimpleAudioEngine::getInstance()->playEffect("SE/invalid.mp3");

				Controler::GamePauseAndSettings(); //加载：暂停相关设置
			}
			else
			{
				Layer_Control->setVisible(true);
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

void Controler::GamePauseAndSettings() //暂停相关设置
{
	auto sp_test = Sprite::create("PICTURE/time_UI.png");
	sp_test->setPosition(Vec2(visSize.width / 2, visSize.height / 2));
	Layer_GameSettings->addChild(sp_test);
	Layer_GameSettings->setPosition(Vec2(-Layer_GameScene->getPosition().x, -Layer_GameScene->getPosition().y));
	Layer_GameSettings->setVisible(true);
}