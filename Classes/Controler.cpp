/*
ʱ�䣺2017��8��12��00:31:21
���ߣ�������
���ܣ���Ϸ�ܿ�
*/

#include <iostream>
#include "Controler.h"

USING_NS_CC;
using namespace std;


//UI��Դ����
static Sprite* icon_Time;
static Sprite* icon_Coin;
static Label *time_left;
static Label *get_coin_number;
static Label *get_score;


static vector<cocos2d::Sprite *> vector_Cloud; //����ƶ������

void Controler::createCloud(Layer * layer, Node *character, Size visSize)
{
	auto cloud_Big = Sprite::create("PICTURE/cloud_Big.png");
	vector_Cloud.push_back(cloud_Big);
	cloud_Big->setPosition(Vec2(visSize.width*1.1 + character->getPosition().x, visSize.height / random(1.0, 1.2)));
	layer->addChild(cloud_Big, 4);
}

void Controler::cloudPosControl(Layer * layer)
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
				layer->removeChild(vector_Cloud[idx]);
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

void Controler::createBackGround(Layer *Layer_BG, Size visSize)//������Ϸ����
{
	Sprite* background = Sprite::create("res/MAP/backGround1.png");//��Ϸ����
	background->setAnchorPoint(Point(0, 0));
	background->setPosition(Point(0, visSize.height*0.14));  //�趨����λ��
	Layer_BG->addChild(background, 2);  //������Ϸ����
}