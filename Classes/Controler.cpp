/*
时间：2017年8月12日00:31:21
作者：张天翼
功能：游戏总控
*/

#include <iostream>
#include "Controler.h"

USING_NS_CC;
using namespace std;


//UI资源声明
static Sprite* icon_Time;
static Sprite* icon_Coin;
static Label *time_left;
static Label *get_coin_number;
static Label *get_score;


static vector<cocos2d::Sprite *> vector_Cloud; //存放云朵的容器

void Controler::createCloud(Layer * layer, Node *character, Size visSize)
{
	auto cloud_Big = Sprite::create("PICTURE/cloud_Big.png");
	vector_Cloud.push_back(cloud_Big);
	cloud_Big->setPosition(Vec2(visSize.width*1.1 + character->getPosition().x, visSize.height / random(1.0, 1.2)));
	layer->addChild(cloud_Big, 4);
}

void Controler::cloudPosControl(Layer * layer)
{
	if (vector_Cloud.size() > 0) //防止容器为空
	{
		for (int idx = 0; idx < vector_Cloud.size(); idx++)
		{
			int pos_x = (int)vector_Cloud[idx]->getPosition().x;
			int pos_y = (int)vector_Cloud[idx]->getPosition().y;
			vector_Cloud[idx]->setPosition(Point(pos_x -= 1, pos_y));  //每帧都改变云朵的位置

			if (pos_x < -vector_Cloud[idx]->getContentSize().width)  //如果精灵的坐标（完全隐没） 
			{
				layer->removeChild(vector_Cloud[idx]);
				vector_Cloud.erase(vector_Cloud.begin() + idx);  //删除完全隐没的当前元素
				idx--;
			}
		}
	}

}

void Controler::CreateUpdateUI(Layer * Layer_UI, Size visSize, int &gameTime, int &coin, int &score)
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

void Controler::tiledMapScroll(Layer * layer, Layer * layer_BG, Layer *  layer_UI, Layer *  layer_Controler, TMXTiledMap * tiledMap, Node *character, float delta)
{
	Size winSize = Director::getInstance()->getWinSize();   //获取屏幕的尺寸

	Size mapNumbers = tiledMap->getMapSize();  //获取地图方块数量（元素个数）
	Size tiledSize = tiledMap->getTileSize();  //获取瓦片元素的尺寸（单位元素尺寸）
	Size MapSize = Size(mapNumbers.width*tiledSize.width, mapNumbers.height*tiledSize.height); //计算得出整个瓦片地图的尺寸

	auto sprite_pos = character->getPosition(); //获取角色坐标

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
		auto pos_layer = layer->getPosition();
		if (x >= MapSize.width - winSize.width / 2) //防地图黑边
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
	else if (temp_distance.x < distance.x)  //场景右移
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

void Controler::createBackGround(Layer *Layer_BG, Size visSize)//创建游戏背景
{
	Sprite* background = Sprite::create("res/MAP/backGround1.png");//游戏背景
	background->setAnchorPoint(Point(0, 0));
	background->setPosition(Point(0, visSize.height*0.14));  //设定背景位置
	Layer_BG->addChild(background, 2);  //创建游戏背景
}