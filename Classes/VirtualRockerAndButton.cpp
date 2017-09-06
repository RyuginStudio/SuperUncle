/*
*功能：在屏幕上创建虚拟摇杆和按键
*作者：vszed
*时间：2017年8月5日01:28:43
*特点：单例类&平台宏自适应
*/

#include "VirtualRockerAndButton.h"
#include "Character.h"

USING_NS_CC;

//按键容器声明
extern std::map<std::string, bool> map_keyPressed; //存放键盘按键状态


//摇杆和托盘路径信息
const std::string circlePath = "PICTURE/VirtualCircle.png";
const std::string pointPath = "PICTURE/VirtualPoint.png";

//按钮A和B的路径信息
const std::string btnApath = "PICTURE/VirtualButtonA.png";
const std::string btnBpath = "PICTURE/VirtualButtonB.png";


//声明：层
extern Layer* Layer_Control;

//声明：显示尺寸
extern Size visSize;


//======================单例模式======================//
VirtualRockerAndButton *VirtualRockerAndButton::virtualRockerAndButton = nullptr;

VirtualRockerAndButton * VirtualRockerAndButton::getInstance()
{
	/*if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		return nullptr;*/

	if (virtualRockerAndButton == nullptr)
		virtualRockerAndButton = new VirtualRockerAndButton();

	return virtualRockerAndButton;
}

VirtualRockerAndButton::VirtualRockerAndButton()
{
	//if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	//	return;

	//摇杆、托盘
	auto sp_VirtualRockerCircle = Sprite::create(circlePath);
	sp_VirtualRockerCircle->setName("sp_VirtualCircle");
	auto sp_VirtualRockerPoint = Sprite::create(pointPath);
	sp_VirtualRockerPoint->setName("sp_VirtualPoint");

	sp_VirtualRockerCircle->setPosition(visSize.width * 0.165, visSize.height * 0.21);
	sp_VirtualRockerPoint->setPosition(visSize.width * 0.165, visSize.height * 0.21);

	Layer_Control->addChild(sp_VirtualRockerCircle, 100);
	Layer_Control->addChild(sp_VirtualRockerPoint, 100);

    //按钮A、B
	auto sp_BtnA = Sprite::create(btnApath);
	sp_BtnA->setName("sp_BtnA");
	auto sp_BtnB = Sprite::create(btnBpath);
	sp_BtnB->setName("sp_BtnB");

	sp_BtnA->setPosition(visSize.width * 0.88, visSize.height * 0.3);
	sp_BtnB->setPosition(visSize.width * 0.75, visSize.height * 0.14);

	Layer_Control->addChild(sp_BtnA, 100);
	Layer_Control->addChild(sp_BtnB, 100);
}
//======================单例模式======================//


bool VirtualRockerAndButton::onTouchBegan(Touch * tTouch, Event * eEvent)
{
	/*if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		return false;*/

	CCLOG("%f, %f", tTouch->getLocation().x, tTouch->getLocation().y);

	//按钮A按下 => 跳跃
	if (Layer_Control->getChildByName("sp_BtnA")->getBoundingBox().containsPoint(tTouch->getLocation()))
	{
		Character::getInstance()->jump();
	}

	//触摸起始点在摇杆范围内才返回true
	if (Layer_Control->getChildByName("sp_VirtualCircle")->getBoundingBox().containsPoint(tTouch->getLocation()))
	{
		//摇杆移动
		Layer_Control->getChildByName("sp_VirtualPoint")->setPosition(tTouch->getLocation());
		return true;
	}
	else
		return false; //false会导致下二函数不执行
}

void VirtualRockerAndButton::onTouchesMoved(Touch * tTouch, Event * eEvent)
{
	/*if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		return;*/

	auto Nowdistance = std::abs(tTouch->getLocation().getDistance(Layer_Control->getChildByName("sp_VirtualCircle")->getPosition()));  //当前触摸点与托盘圆心距离

	auto maxDistance = 130;   //摇杆与托盘最大距离

	if (Nowdistance < maxDistance) //手指滑出托盘
	{
		Layer_Control->getChildByName("sp_VirtualPoint")->setPosition(tTouch->getLocation());
	}
	else //滑出托盘
	{
		auto Distancedifferent = Nowdistance - 130;  //手指超出额定90的距离差

		if (Distancedifferent > 0)  //确定滑出
		{
			Nowdistance = std::abs(tTouch->getLocation().getDistance(Layer_Control->getChildByName("sp_VirtualCircle")->getPosition()));  //再取值

			auto outSideA = std::abs(tTouch->getLocation().y - Layer_Control->getChildByName("sp_VirtualCircle")->getPosition().y);  //外部三角形A边的高度

			auto sinAngle = outSideA / Nowdistance;  //三角函数求出斜边与直角边（A边）的比例

			auto inSideA = sinAngle * 130;  //因为共享一个角，同一个比例，可求小三角形（A边）的高度

			auto inSideB = std::sqrt(130 * 130 - inSideA*inSideA);  //勾股定理

			if (tTouch->getLocation().y < Layer_Control->getChildByName("sp_VirtualCircle")->getPosition().y)  //摇杆托盘也有其“坐标系”，原点为正中心
				inSideA *= -1;
			if (tTouch->getLocation().x < Layer_Control->getChildByName("sp_VirtualCircle")->getPosition().x)
				inSideB *= -1;


			//摇杆的最终坐标要在圆盘的原点坐标基础上加
			Layer_Control->getChildByName("sp_VirtualPoint")
				->setPosition(Vec2(inSideB + Layer_Control->getChildByName("sp_VirtualCircle")->getPosition().x, inSideA + Layer_Control->getChildByName("sp_VirtualCircle")->getPosition().y));

			CCLOG("Nowdistance:%f, lengthA:%f, sinAngle:%f, x:%f, y:%f", Nowdistance, outSideA, sinAngle, inSideB, inSideA);
		}
	}
}

void VirtualRockerAndButton::onTouchEnded(Touch * tTouch, Event * eEvent)
{
	/*if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		return;*/

	Layer_Control->getChildByName("sp_VirtualPoint")
		->setPosition(Layer_Control->getChildByName("sp_VirtualCircle")->getPosition());
}

void VirtualRockerAndButton::touchMoveControl()
{
	/*if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		return;*/

	if (Layer_Control->getChildByName("sp_VirtualPoint")->getPosition().x
		- Layer_Control->getChildByName("sp_VirtualCircle")->getPosition().x == 0) //静止状态
	{
		map_keyPressed.find("left")->second = false;
		map_keyPressed.find("right")->second = false;
	}
	else if (Layer_Control->getChildByName("sp_VirtualPoint")->getPosition().x
		- Layer_Control->getChildByName("sp_VirtualCircle")->getPosition().x > 30) //向右走
	{
		map_keyPressed.find("right")->second = true;
		map_keyPressed.find("left")->second = false;
	}
	else if (Layer_Control->getChildByName("sp_VirtualPoint")->getPosition().x
		- Layer_Control->getChildByName("sp_VirtualCircle")->getPosition().x < -30) //向左走
	{
		map_keyPressed.find("left")->second = true;
		map_keyPressed.find("right")->second = false;
	}
}

VirtualRockerAndButton::~VirtualRockerAndButton()
{
	CC_SAFE_DELETE(virtualRockerAndButton);
}
