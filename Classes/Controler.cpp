/*
*ʱ�䣺2017��8��12��00:31:21
*���ߣ�vszed
*���ܣ���Ϸ�ܿ�
*˵����������
*/

#include <iostream>
#include "Character.h"
#include "Controler.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

using namespace std;
using namespace CocosDenshion;

#define PTM_RATIO 32


//���壺�������� => �ƶ���ͼʱ�������и��岢λ��
vector<b2Body *> vector_MapBody;

//���壺��ʾ�ߴ�
Size visSize;

//������Characterʵ��
Character* CharaIns;

//��������ɫ�˶��ж�
extern bool isSporting;

//���壺����
static vector<Sprite *> vector_Cloud;  //�ƶ侫��
map<string, bool> map_keyPressed;      //����״̬

//���壺UI��Դ
static Sprite* icon_Time;
static Sprite* icon_Coin;
static Label *time_left;
static Label *get_coin_number;
static Label *get_score;

//������UI�������
extern int coin;
extern int gameTime;
extern int score;

//��������ز�
extern Layer* Layer_BG;
extern Layer* Layer_UI;
extern Layer* Layer_Control;
extern Layer* Layer_GameSettings;
extern Layer* Layer_TitledMap;


//��������Ƭ��ͼ
extern TMXTiledMap *tiledMap;

//���壺��Ƭ��ͼ����
Size mapNumbers;
Size tiledSize;
Size MapSize;


//==============����ģʽ==============//
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
	//��ʼ�������ٿص�map����
	map_keyPressed.insert(pair<string, bool>("up", false));
	map_keyPressed.insert(pair<string, bool>("down", false));
	map_keyPressed.insert(pair<string, bool>("left", false));
	map_keyPressed.insert(pair<string, bool>("right", false));

	//��ȡCharacterʵ��
	CharaIns = Character::getInstance();

	//��ȡ��Ƭ��ͼ����
	mapNumbers = tiledMap->getMapSize();  //Ԫ�ظ���
	tiledSize = tiledMap->getTileSize();  //��λԪ�سߴ�
	MapSize = Size(mapNumbers.width*tiledSize.width, mapNumbers.height*tiledSize.height); //��Ƭ��ͼ�ߴ�
}
//==============����ģʽ==============//


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
	if (vector_Cloud.size() > 0) //��ֹ����Ϊ��
	{
		for (int idx = 0; idx != vector_Cloud.size(); idx++)
		{
			int pos_x = (int)vector_Cloud[idx]->getPosition().x;
			int pos_y = (int)vector_Cloud[idx]->getPosition().y;
			vector_Cloud[idx]->setPosition(Point(pos_x -= 1, pos_y));  //ÿ֡���ı��ƶ��λ��

			if (pos_x < 0)
			{
				Layer_BG->removeChild(vector_Cloud[idx]);
				vector_Cloud.erase(vector_Cloud.begin() + idx);  //ɾ����ȫ��û�ĵ�ǰԪ��
				idx--;
			}
		}
	}
}

void Controler::CreateUpdateUI()
{
	static bool onCreate = true; //����UI����

	if (onCreate == true)
	{
		onCreate = false;
		icon_Time = Sprite::create("res/PICTURE/time_UI.png");  //��Ϸ��ʱ��ʣ��ͼ��
		icon_Coin = Sprite::create("res/PICTURE/coin_UI.png");  //��Ϸ�л�ȡ���ͼ��

		icon_Time->setPosition(Point(visSize.width / 1.12, visSize.height / 1.08));
		icon_Coin->setPosition(Point(visSize.width / 20, visSize.height / 1.08));

		Layer_UI->addChild(icon_Time, 2);
		Layer_UI->addChild(icon_Coin, 2);
	}

	Layer_UI->removeChild(time_left);
	Layer_UI->removeChild(get_coin_number);
	Layer_UI->removeChild(get_score);

	auto* str_gameTime = __String::createWithFormat("%d", gameTime);

	--gameTime; //ʱ��˥��

	time_left = Label::createWithTTF(str_gameTime->getCString(), "NewSuperMarioFontU.ttf", 45);
	time_left->setName("time_left");

	time_left->enableOutline(Color4B(0, 0, 0, 255), 4);  //��ɫ���  ���һ��������͸����

	time_left->setPosition(Point(visSize.width / 1.06, visSize.height / 1.086));

	Layer_UI->addChild(time_left, 2);


	//=======================����Ϊ��ʾ��Ҹ�����label===============================//
	if (coin < 10)
		get_coin_number = Label::createWithTTF("x0" + __String::createWithFormat("%d", coin)->_string, "NewSuperMarioFontU.ttf", 45);  //���ؿ��л���˶��ٽ��
	else
		get_coin_number = Label::createWithTTF("x" + __String::createWithFormat("%d", coin)->_string, "NewSuperMarioFontU.ttf", 45);  //���ؿ��л���˶��ٽ��

	get_coin_number->setName("get_coin_number");

	get_coin_number->setAnchorPoint(Vec2(0, 0.5));

	get_coin_number->enableOutline(Color4B(0, 0, 0, 255), 4);  //��ɫ���  ���һ��������͸����

	get_coin_number->setPosition(Point(Point(visSize.width * 0.063, visSize.height / 1.086)));

	Layer_UI->addChild(get_coin_number, 2);


	//=======================����Ϊ��ʾ���ؿ��÷ֵ�label=======================//

	int k = 0; //�������Ϊ����λ����
	auto temp_score = score;

	string numberOfZero; //0�ĸ���

	while (temp_score)
	{
		temp_score /= 10;
		k++;
	}

	for (int i = 0; i < 8 - k; i++) //��ಹ8����
		numberOfZero += "0";

	get_score = Label::createWithTTF(numberOfZero + __String::createWithFormat("%d", score)->_string, "NewSuperMarioFontU.ttf", 40);  //���ؿ��л���˶��ٽ��

	get_score->setAnchorPoint(Vec2(1, 0.5));

	get_score->setName("get_score");

	get_score->setTextColor(Color4B(0, 0, 0, 255)); //��ɫ����

	get_score->setAdditionalKerning(8.0f);  //����������֮��Ŀ��

	get_score->enableOutline(Color4B(255, 255, 255, 255), 4);  //��ɫ���  ���һ��������͸����

	get_score->setPosition(Point(Point(visSize.width * 0.85, visSize.height / 1.086)));

	Layer_UI->addChild(get_score, 2);


}

void Controler::tiledMapScroll(float delta, b2World *world)
{
	auto pos_Character = CharaIns->sp_man->getPosition();

	if (pos_Character.x >= visSize.width / 2 && CharaIns->characterStatus.MoveRight
		&& Layer_TitledMap->getPosition().x >= visSize.width - MapSize.width + tiledSize.width * 1.5) //��+1.5���߿�ȷ��ڱ�
	{
		//��ɫ���ƶ�
		CharaIns->set_speed(0);

		//���ƶ����ٶȺͽ�ɫ֮ǰ�ٶȡ��Ӿ��ϡ�һ�� => �ٶ�+1
		auto pos_map = Layer_TitledMap->getPosition();
		Layer_TitledMap->setPosition(Point(pos_map.x - delta * (6 * PTM_RATIO), pos_map.y));
		auto pos_bg = Layer_BG->getPosition();
		Layer_BG->setPosition(Point(pos_bg.x - delta * 20, pos_bg.y));

		//������ͼ������=>�ƶ�
		for (auto body : vector_MapBody)
		{
			body->SetTransform(b2Vec2(body->GetPosition().x - 6 * delta, body->GetPosition().y), 0);
		}
	}
	else
		CharaIns->set_speed(5);  //��ɫ��ʵ�ƶ�
}

void Controler::createBackGround()  //������Ϸ����
{
	Sprite* background = Sprite::create("res/MAP/backGround1.png");  //��Ϸ����
	background->setAnchorPoint(Point(0, 0));
	background->setPosition(Point(0, visSize.height*0.14));  //�趨����λ��
	Layer_BG->addChild(background, 2);  //������Ϸ����
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

			Controler::GamePauseAndSettings(); //���أ���ͣ�������
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
	case cocos2d::EventKeyboard::KeyCode::KEY_F1:   //�÷ְ����
		score += 10000;
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_F2:   //�����������
		coin += 10000;
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_F3:   //��Ϸ��ͣ����
		Controler::GamePauseAndSettings();
	case cocos2d::EventKeyboard::KeyCode::KEY_F5:   //��Ƭ��ͼ����1�ƶ�����
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

void Controler::keyBoardControler(float delta) //���̿�������һ������ => ����֡��ʱ��
{
	if ((map_keyPressed.find("left")->second && map_keyPressed.find("right")->second)
		|| (!map_keyPressed.find("left")->second && !map_keyPressed.find("right")->second)) //ͬʱ����/̧���ɫͣ�Ͳ�ǰ
	{
		//���Ľ�ɫ״̬
		CharaIns->characterStatus.MoveLeft = false;
		CharaIns->characterStatus.MoveRight = false;

		CharaIns->move(0, delta);
		return;
	}
	if (map_keyPressed.find("left")->second == true)            //��
	{
		CharaIns->characterStatus.MoveLeft = true;
		CharaIns->characterStatus.MoveRight = false;

		isSporting = true;
		CharaIns->move(-1, delta);
	}
	else if (map_keyPressed.find("right")->second == true)      //��
	{
		CharaIns->characterStatus.MoveRight = true;
		CharaIns->characterStatus.MoveLeft = false;

		isSporting = true;
		CharaIns->move(1, delta);
	}
}

void Controler::onAcceleration(Acceleration * acc, Event * unused_event)  //�����ƣ���ҡһҡ����ͣ��Ϸ
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
			{
				Layer_Control->setVisible(false);
				Director::getInstance()->pause();
				SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
				SimpleAudioEngine::getInstance()->pauseAllEffects();
				SimpleAudioEngine::getInstance()->playEffect("SE/invalid.mp3");

				Controler::GamePauseAndSettings(); //���أ���ͣ�������
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

void Controler::GamePauseAndSettings() //��ͣ�������
{
	auto sp_test = Sprite::create("PICTURE/time_UI.png");
	sp_test->setPosition(Vec2(visSize.width / 2, visSize.height / 2));
	Layer_GameSettings->addChild(sp_test);
	Layer_GameSettings->setVisible(true);
}