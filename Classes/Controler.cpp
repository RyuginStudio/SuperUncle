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

#define PTM_RATIO 32


//定义：刚体容器 => 移动地图时遍历所有刚体并位移
vector<b2Body *> vector_MapBody;

//定义：显示尺寸
Size visSize;

//声明：Character实例
Character* CharaIns;

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

//声明：UI相关数据
extern int coin;
extern int gameTime;
extern int score;

//声明：相关层
extern Layer* Layer_BG;
extern Layer* Layer_UI;
extern Layer* Layer_Control;
extern Layer* Layer_GameSettings;
extern Layer* Layer_TitledMap;


//声明：瓦片地图
extern TMXTiledMap *tiledMap;

//定义：瓦片地图数据
Size mapNumbers;
Size tiledSize;
Size MapSize;


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
	//初始化即将操控的map容器
	map_keyPressed.insert(pair<string, bool>("up", false));
	map_keyPressed.insert(pair<string, bool>("down", false));
	map_keyPressed.insert(pair<string, bool>("left", false));
	map_keyPressed.insert(pair<string, bool>("right", false));

	//获取Character实例
	CharaIns = Character::getInstance();

	//获取瓦片地图数据
	mapNumbers = tiledMap->getMapSize();  //元素个数
	tiledSize = tiledMap->getTileSize();  //单位元素尺寸
	MapSize = Size(mapNumbers.width*tiledSize.width, mapNumbers.height*tiledSize.height); //瓦片地图尺寸
}
//==============单例模式==============//


void Controler::createCloud()
{
	auto cloud_Big = Sprite::create("PICTURE/cloud_Big.png");
	cloud_Big->setAnchorPoint(Vec2(1, 0.5));
	vector_Cloud.push_back(cloud_Big);
	cloud_Big->setPosition(
		Vec2(visSize.width + cloud_Big->getContentSize().width - Layer_BG->getPosition().x, visSize.height / random(1.0, 1.2)));
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

			if (pos_x < 0)
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

void Controler::tiledMapScroll(float delta, b2World *world)
{
	auto pos_Character = CharaIns->sp_man->getPosition();

	if (pos_Character.x >= visSize.width / 2 && CharaIns->characterStatus.MoveRight
		&& Layer_TitledMap->getPosition().x >= visSize.width - MapSize.width + tiledSize.width * 1.5) //多+1.5块瓦宽度防黑边
	{
		//角色不移动
		CharaIns->set_speed(0);

		//层移动：速度和角色之前速度“视觉上”一致 => 速度+1
		auto pos_map = Layer_TitledMap->getPosition();
		Layer_TitledMap->setPosition(Point(pos_map.x - delta * (6 * PTM_RATIO), pos_map.y));
		auto pos_bg = Layer_BG->getPosition();
		Layer_BG->setPosition(Point(pos_bg.x - delta * 20, pos_bg.y));

		//遍历地图：刚体=>移动
		for (auto body : vector_MapBody)
		{
			body->SetTransform(b2Vec2(body->GetPosition().x - 6 * delta, body->GetPosition().y), 0);
		}
	}
	else
		CharaIns->set_speed(5);  //角色真实移动
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
		CharaIns->jump();
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
	case cocos2d::EventKeyboard::KeyCode::KEY_F5:   //瓦片地图向上1移动像素
	{
		auto pos = Layer_TitledMap->getPosition();
		Layer_TitledMap->setPosition(Vec2(pos.x, ++pos.y));
		break;
	}
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
		//更改角色状态
		CharaIns->characterStatus.MoveLeft = false;
		CharaIns->characterStatus.MoveRight = false;

		CharaIns->move(0, delta);
		return;
	}
	if (map_keyPressed.find("left")->second == true)            //左
	{
		CharaIns->characterStatus.MoveLeft = true;
		CharaIns->characterStatus.MoveRight = false;

		isSporting = true;
		CharaIns->move(-1, delta);
	}
	else if (map_keyPressed.find("right")->second == true)      //右
	{
		CharaIns->characterStatus.MoveRight = true;
		CharaIns->characterStatus.MoveLeft = false;

		isSporting = true;
		CharaIns->move(1, delta);
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
	Layer_GameSettings->setVisible(true);
}