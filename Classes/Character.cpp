/*
*ʱ�䣺2017-8-29 13:54:08
*���ߣ�vszed
*���ܣ���Ϸ��ɫ��
*˵�����������ģʽ
*/

#include "Character.h"
#include "TestScene.h"
#include "Box2D\Box2D.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;


bool moveAnimtionFinished = true;       //�ƶ������������
bool isSporting = false;                //��ɫ�Ƿ����˶���


//=================����ģʽ=================//
Character *Character::character = nullptr;

Character* Character::getInstance()
{
	if (character == nullptr)
		character = new Character();

	return character;
}

Character::Character()
{
	this->speed = 5;

	this->jump_height_small = 10;
	this->jump_height_big = 10;


	sp_man = Sprite3D::create("3DspriteAndAction/sprite_stand.c3b");  //�����زģ�����ز��ļ�ʹ��

	sp_man->setScale(1.5f);     //���÷Ŵ����

	auto ac_RotateTo = RotateTo::create(0.001f, Vec3(0, 90, 0));//�趨��ת����������Sprite => ��������Ϊ0.f���ֻ��Ͳ�ִ��

	sp_man->runAction(ac_RotateTo);

}

Character::~Character()
{
	CC_SAFE_DELETE(character);
}
//=================����ģʽ=================//


void Character::move(int speedPlus_minus, float delta) //��ɫ�����ƶ�������
{
	auto animation_bone = Animation3D::create("3DspriteAndAction/run_to_stand_final.c3b");    //��ȡ������Ϣ
	auto animation_run = Animate3D::create(animation_bone, 0.f, 0.5f);                        //3D�ܶ�����
	auto ac_repeatForever = RepeatForever::create(animation_run);                             //��װ�ܶ���������
	auto animation_recover = Animate3D::create(animation_bone, 0.5f, 0.68f);                  //3D�ָ�վ������


	if (speedPlus_minus == 0) //ִ���ܲ��ָ���վ���Ķ���
	{
		if (isSporting == false)  //��ɫ�Ƿ����˶���
			return;

		sp_man->stopAllActions();

		isSporting = false;            //��ɫ�����˶���
		moveAnimtionFinished = true;   //��ɫ�ƶ���������

		animation_recover->setSpeed(100.0f);
		sp_man->runAction(animation_recover);

		return;
	}

	//��ֹ��������
	if (moveAnimtionFinished == true)
	{
		moveAnimtionFinished = false;
		sp_man->runAction(ac_repeatForever);
	}

	auto pos = sp_man->getPosition();
	sp_man->setRotation3D(Vec3(0, speedPlus_minus * 90, 0));  //ת��

	body_man->SetTransform(b2Vec2(body_man->GetPosition().x + speedPlus_minus * speed / PTM_RATIO, body_man->GetPosition().y), 0);  //�ƶ�

}

double Character::get_speed()
{
	return this->speed;
}
void Character::set_speed(double speed)
{
	this->speed = speed;
}

void Character::jump()
{
	if (true)//characterStatus.OnGround)  //�жϽ�ɫ״̬
	{
		auto force = b2Vec2(0, 7000);

		body_man->ApplyForce(force, body_man->GetWorldCenter(), true);

		SimpleAudioEngine::getInstance()->playEffect("SE/jump.mp3");
	}	
}

void Character::initBeginPos()  //��ʼ����ɫλ��
{
	sp_man->setPosition(Point(350.6, 371)); 
}