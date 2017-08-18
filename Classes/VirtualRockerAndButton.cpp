/*
功能：在屏幕上创建虚拟摇杆和按键
作者：张天翼
时间：2017年8月5日01:28:43
特点：单例类
*/

#include "VirtualRockerAndButton.h"

USING_NS_CC;

//按键容器声明
extern std::map<std::string, bool> map_keyPressed; //存放键盘按键状态


//摇杆和托盘路径信息
const std::string circlePath = "PICTURE/VirtualCircle.png";
const std::string pointPath = "PICTURE/VirtualPoint.png";


//临时层
Layer *tempLayer;


//单例对象
VirtualRockerAndButton *VirtualRockerAndButton::virtualRockerAndButton = nullptr;



void VirtualRockerAndButton::create(cocos2d::Layer *layer, cocos2d::Size visSize)
{
	auto sp_VirtualRockerCircle = Sprite::create(circlePath);
	sp_VirtualRockerCircle->setName("sp_VirtualCircle");
	auto sp_VirtualRockerPoint = Sprite::create(pointPath);
	sp_VirtualRockerPoint->setName("sp_VirtualPoint");

	sp_VirtualRockerCircle->setPosition(visSize.width * 0.165, visSize.height * 0.21);
	sp_VirtualRockerPoint->setPosition(visSize.width * 0.165, visSize.height * 0.21);
	layer->addChild(sp_VirtualRockerCircle, 100);
	layer->addChild(sp_VirtualRockerPoint, 100);

	tempLayer = layer;
}


VirtualRockerAndButton * VirtualRockerAndButton::getInstance(Layer* layer, Size visSize)
{
	if (virtualRockerAndButton == nullptr)
	{
		virtualRockerAndButton = new VirtualRockerAndButton();
		create(layer, visSize);
	}

	return virtualRockerAndButton;
}

bool VirtualRockerAndButton::onTouchBegan(cocos2d::Touch * tTouch, cocos2d::Event * eEvent)
{
	CCLOG("%f, %f", tTouch->getLocation().x, tTouch->getLocation().y);

	//触摸起始点在摇杆范围内才返回true
	if (tempLayer->getChildByName("sp_VirtualCircle")->getBoundingBox().containsPoint(tTouch->getLocation()))
	{
		tempLayer->getChildByName("sp_VirtualPoint")->setPosition(tTouch->getLocation());
		return true;
	}
	else
		return false; //false会导致下二函数不执行
}

void VirtualRockerAndButton::onTouchesMoved(cocos2d::Touch * tTouch, cocos2d::Event * eEvent)
{
	auto VisSize = Director::getInstance()->getVisibleSize();

	auto Nowdistance = std::abs(
		cocos2d::ccpDistance(tTouch->getLocation(), tempLayer->getChildByName("sp_VirtualCircle")->getPosition()));  //当前触摸点与托盘圆心距离

	auto maxDistance = 130;   //摇杆与托盘最大距离

	if (Nowdistance < maxDistance) //手指滑出托盘
	{
		tempLayer->getChildByName("sp_VirtualPoint")->setPosition(tTouch->getLocation());
	}
	else //滑出托盘
	{
		auto Distancedifferent = Nowdistance - 130;  //手指超出额定90的距离差

		if (Distancedifferent > 0)  //确定滑出
		{
			Nowdistance = std::abs(
				cocos2d::ccpDistance(tTouch->getLocation(), tempLayer->getChildByName("sp_VirtualCircle")->getPosition()));  //再取值

			auto outSideA = std::abs(tTouch->getLocation().y - tempLayer->getChildByName("sp_VirtualCircle")->getPosition().y);  //外部三角形A边的高度

			auto sinAngle = outSideA / Nowdistance;  //三角函数求出斜边与直角边（A边）的比例

			auto inSideA = sinAngle * 130;  //因为共享一个角，同一个比例，可求小三角形（A边）的高度

			auto inSideB = std::sqrt(130 * 130 - inSideA*inSideA);  //勾股定理

			if (tTouch->getLocation().y < tempLayer->getChildByName("sp_VirtualCircle")->getPosition().y)  //摇杆托盘也有其“坐标系”，原点为正中心
				inSideA *= -1;
			if (tTouch->getLocation().x < tempLayer->getChildByName("sp_VirtualCircle")->getPosition().x)
				inSideB *= -1;


			//摇杆的最终坐标要在圆盘的原点坐标基础上加
			tempLayer->getChildByName("sp_VirtualPoint")
				->setPosition(Vec2(inSideB + tempLayer->getChildByName("sp_VirtualCircle")->getPosition().x, inSideA + tempLayer->getChildByName("sp_VirtualCircle")->getPosition().y));

			CCLOG("Nowdistance:%f, lengthA:%f, sinAngle:%f, x:%f, y:%f", Nowdistance, outSideA, sinAngle, inSideB, inSideA);
		}
	}
}

void VirtualRockerAndButton::onTouchEnded(cocos2d::Touch * tTouch, cocos2d::Event * eEvent)
{
	tempLayer->getChildByName("sp_VirtualPoint")
		->setPosition(tempLayer->getChildByName("sp_VirtualCircle")->getPosition());
}

void VirtualRockerAndButton::touchMoveControl()
{
	if (tempLayer->getChildByName("sp_VirtualPoint")->getPosition().x
		- tempLayer->getChildByName("sp_VirtualCircle")->getPosition().x == 0) //静止状态
	{
		map_keyPressed.find("left")->second = false;
		map_keyPressed.find("right")->second = false;
	}
	else if (tempLayer->getChildByName("sp_VirtualPoint")->getPosition().x
		- tempLayer->getChildByName("sp_VirtualCircle")->getPosition().x > 63) //向右走
	{
		map_keyPressed.find("right")->second = true;
		map_keyPressed.find("left")->second = false;
	}
	else if (tempLayer->getChildByName("sp_VirtualPoint")->getPosition().x
		- tempLayer->getChildByName("sp_VirtualCircle")->getPosition().x < -63) //向左走
	{
		map_keyPressed.find("left")->second = true;
		map_keyPressed.find("right")->second = false;
	}
}


VirtualRockerAndButton::VirtualRockerAndButton()
{
}
VirtualRockerAndButton::~VirtualRockerAndButton()
{
}