/*
���ܣ�����Ļ�ϴ�������ҡ�˺Ͱ���
���ߣ�������
ʱ�䣺2017��8��5��01:28:43
�ص㣺������
*/

#include "VirtualRockerAndButton.h"

USING_NS_CC;

//������������
extern std::map<std::string, bool> map_keyPressed; //��ż��̰���״̬


//ҡ�˺�����·����Ϣ
const std::string circlePath = "PICTURE/VirtualCircle.png";
const std::string pointPath = "PICTURE/VirtualPoint.png";


//��ʱ��
Layer *tempLayer;


//��������
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

	//������ʼ����ҡ�˷�Χ�ڲŷ���true
	if (tempLayer->getChildByName("sp_VirtualCircle")->getBoundingBox().containsPoint(tTouch->getLocation()))
	{
		tempLayer->getChildByName("sp_VirtualPoint")->setPosition(tTouch->getLocation());
		return true;
	}
	else
		return false; //false�ᵼ���¶�������ִ��
}

void VirtualRockerAndButton::onTouchesMoved(cocos2d::Touch * tTouch, cocos2d::Event * eEvent)
{
	auto VisSize = Director::getInstance()->getVisibleSize();

	auto Nowdistance = std::abs(
		cocos2d::ccpDistance(tTouch->getLocation(), tempLayer->getChildByName("sp_VirtualCircle")->getPosition()));  //��ǰ������������Բ�ľ���

	auto maxDistance = 130;   //ҡ��������������

	if (Nowdistance < maxDistance) //��ָ��������
	{
		tempLayer->getChildByName("sp_VirtualPoint")->setPosition(tTouch->getLocation());
	}
	else //��������
	{
		auto Distancedifferent = Nowdistance - 130;  //��ָ�����90�ľ����

		if (Distancedifferent > 0)  //ȷ������
		{
			Nowdistance = std::abs(
				cocos2d::ccpDistance(tTouch->getLocation(), tempLayer->getChildByName("sp_VirtualCircle")->getPosition()));  //��ȡֵ

			auto outSideA = std::abs(tTouch->getLocation().y - tempLayer->getChildByName("sp_VirtualCircle")->getPosition().y);  //�ⲿ������A�ߵĸ߶�

			auto sinAngle = outSideA / Nowdistance;  //���Ǻ������б����ֱ�Ǳߣ�A�ߣ��ı���

			auto inSideA = sinAngle * 130;  //��Ϊ����һ���ǣ�ͬһ������������С�����Σ�A�ߣ��ĸ߶�

			auto inSideB = std::sqrt(130 * 130 - inSideA*inSideA);  //���ɶ���

			if (tTouch->getLocation().y < tempLayer->getChildByName("sp_VirtualCircle")->getPosition().y)  //ҡ������Ҳ���䡰����ϵ����ԭ��Ϊ������
				inSideA *= -1;
			if (tTouch->getLocation().x < tempLayer->getChildByName("sp_VirtualCircle")->getPosition().x)
				inSideB *= -1;


			//ҡ�˵���������Ҫ��Բ�̵�ԭ����������ϼ�
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
		- tempLayer->getChildByName("sp_VirtualCircle")->getPosition().x == 0) //��ֹ״̬
	{
		map_keyPressed.find("left")->second = false;
		map_keyPressed.find("right")->second = false;
	}
	else if (tempLayer->getChildByName("sp_VirtualPoint")->getPosition().x
		- tempLayer->getChildByName("sp_VirtualCircle")->getPosition().x > 63) //������
	{
		map_keyPressed.find("right")->second = true;
		map_keyPressed.find("left")->second = false;
	}
	else if (tempLayer->getChildByName("sp_VirtualPoint")->getPosition().x
		- tempLayer->getChildByName("sp_VirtualCircle")->getPosition().x < -63) //������
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