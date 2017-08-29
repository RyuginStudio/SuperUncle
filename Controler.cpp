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


//��ɫ�˶��ж�
extern bool isSporting;

//����
static vector<Sprite *> vector_Cloud;  //�ƶ侫��
map<string, bool> map_keyPressed;      //����״̬

//UI��Դ
static Sprite* icon_Time;
static Sprite* icon_Coin;
static Label *time_left;
static Label *get_coin_number;
static Label *get_score;

//��ʾ�ߴ�
Size visSize;

//�������
extern int coin;
extern int gameTime;
extern int score;

//��ز�
extern Layer* Layer_BG;
extern Layer* Layer_UI;
extern Layer* Layer_Control;
extern Layer* Layer_GameSettings;
extern Layer* Layer_GameScene;


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
}
//==============����ģʽ==============//


void Controler::createCloud(Layer * layer, Node *character)
{
	auto cloud_Big = Sprite::create("PICTURE/cloud_Big.png");
	vector_Cloud.push_back(cloud_Big);
	cloud_Big->setPosition(Vec2(visSize.width*1.1 + character->getPosition().x, visSize.height / random(1.0, 1.2)));
	layer->addChild(cloud_Big, 4);
}

void Controler::cloudPosControl()
{
	if (vector_Cloud.size() > 0) //��ֹ����Ϊ��
	{
		for (int idx = 0; idx < vector_Cloud.size(); idx++)
		{
			int pos_x = (int)vector_Cloud[idx]->getPosition().x;
			int pos_y = (int)vector_Cloud[idx]->getPosition().y;
			vector_Cloud[idx]->setPosition(Point(pos_x -= 1, pos_y));  //ÿ֡���ı��ƶ��λ��

			if (pos_x < -vector_Cloud[idx]->getContentSize().width)  //�����������꣨��ȫ��û�� 
			{
				Layer_BG->removeChild(vector_Cloud[idx]);
				vector_Cloud.erase(vector_Cloud.begin() + idx);  //ɾ����ȫ��û�ĵ�ǰԪ��
				idx--;
			}
		}
	}

}

void Controler::CreateUpdateUI(Layer * Layer_UI, Size visSize, int &gameTime, int &coin, int &score)
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

void Controler::tiledMapScroll(Layer * layer, Layer * layer_BG, Layer *  layer_UI, Layer *  layer_Controler, TMXTiledMap * tiledMap, Node *character, float delta)
{
	Size winSize = Director::getInstance()->getWinSize();   //��ȡ��Ļ�ĳߴ�

	Size mapNumbers = tiledMap->getMapSize();  //��ȡ��ͼ����������Ԫ�ظ�����
	Size tiledSize = tiledMap->getTileSize();  //��ȡ��ƬԪ�صĳߴ磨��λԪ�سߴ磩
	Size MapSize = Size(mapNumbers.width*tiledSize.width, mapNumbers.height*tiledSize.height); //����ó�������Ƭ��ͼ�ĳߴ�

	auto sprite_pos = character->getPosition(); //��ȡ��ɫ����

	 //�����������С����Ļ��һ�룬��ôȡ��Ļ�е����꣬����ȡ��ɫ����
	float x = std::max(sprite_pos.x, winSize.width / 2);
	float y = std::max(sprite_pos.y, winSize.height / 2);

	//���x,y������������Ͻǵļ���ֵ����ȡ����ֵ�����꣨����ֵ��ָ���õ�ͼ������Ļ��ɳ��ֺڱߵļ������꣩
	x = std::min(x, MapSize.width - winSize.width / 2);
	y = std::min(y, MapSize.height - winSize.height / 2);

	//Ŀ���
	Point destPos = Point(x, y);

	//��Ļ���ĵ�
	Point centerPos = Point(winSize.width / 2, winSize.height / 2);

	//������Ļ���ĵ��Ҫ�ƶ�����Ŀ�ĵ�֮��ľ���
	Point distance = centerPos - destPos;

	static Point temp_distance;
	if (temp_distance.x > distance.x)  //��������
	{
		auto pos_layer = layer->getPosition();
		if (x >= MapSize.width - winSize.width / 2) //����ͼ�ڱ�
			layer->setPosition(distance);
		else
			layer->setPosition(Point(pos_layer.x - delta * 300, pos_layer.y));


		auto pos_layerBG = layer_BG->getPosition();
		layer_BG->setPosition(Point(pos_layerBG.x + delta * 280, pos_layerBG.y));

		auto pos_layerUI = layer_UI->getPosition();
		layer_UI->setPosition(Point(pos_layerUI.x + delta * 300, pos_layerUI.y));

		auto pos_layerControler = layer_Controler->getPosition();
		layer_Controler->setPosition(Point(pos_layerControler.x + delta * 300, pos_layerControler.y));

		temp_distance.x = distance.x;
	}
	else if (temp_distance.x < distance.x)  //��������
	{
		auto pos_layer = layer->getPosition();
		if (x <= winSize.width / 2)
			layer->setPosition(distance);
		else
			layer->setPosition(Point(pos_layer.x + delta * 300, pos_layer.y));


		auto pos_layerBG = layer_BG->getPosition();
		layer_BG->setPosition(Point(pos_layerBG.x - delta * 280, pos_layerBG.y));

		auto pos_layerUI = layer_UI->getPosition();
		layer_UI->setPosition(Point(pos_layerUI.x - delta * 300, pos_layerUI.y));

		auto pos_layerControler = layer_Controler->getPosition();
		layer_Controler->setPosition(Point(pos_layerControler.x - delta * 300, pos_layerControler.y));

		temp_distance.x = distance.x;
	}
}

void Controler::createBackGround(Layer *Layer_BG, Size visSize)  //������Ϸ����
{
	Sprite* background = Sprite::create("res/MAP/backGround1.png");  //��Ϸ����
	background->setAnchorPoint(Point(0, 0));
	background->setPosition(Point(0, visSize.height*0.14));  //�趨����λ��
	Layer_BG->addChild(background, 2);  //������Ϸ����
}

void Controler::GamePauseAndSettings(Layer *father_layer, Layer *Layer_GameSettings, Size visSize) //��Ϸ��ͣ������
{
	Director::getInstance()->pause();
	SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
	SimpleAudioEngine::getInstance()->pauseAllEffects();
	SimpleAudioEngine::getInstance()->playEffect("SE/invalid.mp3");

	auto sp_test = Sprite::create("PICTURE/time_UI.png");
	sp_test->setPosition(Vec2(visSize.width / 2, visSize.height / 2));
	Layer_GameSettings->addChild(sp_test);

	Layer_GameSettings->setPosition(Vec2(-father_layer->getPosition().x, -father_layer->getPosition().y));

	Layer_GameSettings->setVisible(true);  //��Ϸ��ͣʱ���ò�=>�ɼ�
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
		Controler::GamePauseAndSettings(Layer_GameScene, Layer_GameSettings, visSize);
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

void Controler::keyBoardControler(float delta) //���̿�������һ������ => ����֡��ʱ��
{
	if ((map_keyPressed.find("left")->second && map_keyPressed.find("right")->second)
		|| (!map_keyPressed.find("left")->second && !map_keyPressed.find("right")->second)) //ͬʱ����/̧���ɫͣ�Ͳ�ǰ
	{
		Character::getInstance()->move(0, delta);
		return;
	}
	if (map_keyPressed.find("left")->second == true)  //�������
	{
		isSporting = true;
		Character::getInstance()->move(-1, delta);
	}
	if (map_keyPressed.find("right")->second == true)  //�Ҽ�����
	{
		isSporting = true;
		Character::getInstance()->move(1, delta);
	}
}