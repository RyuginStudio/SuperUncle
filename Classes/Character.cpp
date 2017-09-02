/*
*ʱ�䣺2017-8-29 13:54:08
*���ߣ�vszed
*���ܣ���Ϸ��ɫ��
*˵�����������ģʽ
*/

#include "Character.h"
#include "TestScene.h"
#include "Box2D\Box2D.h"

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
	this->speed = 300;

	this->jump_height_small = 10;
	this->jump_height_big = 10;


	sp_man = Sprite3D::create("3DspriteAndAction/sprite_stand.c3b");  //�����زģ�����ز��ļ�ʹ��

	sp_man->setScale(1.5f);     //���÷Ŵ����

	auto ac_RotateTo = RotateTo::create(0.001f, Vec3(0, 90, 0));//�趨��ת����������Sprite => ��������Ϊ0.f���ֻ��Ͳ�ִ��

	sp_man->runAction(ac_RotateTo);


	////��������ʼ��
	//b2BodyDef bodydef;
	//bodydef.type = b2_dynamicBody;
	//bodydef.position.Set(Character::getInstance()->sp_man->getPosition().x / PTM_RATIO, Character::getInstance()->sp_man->getPosition().y / PTM_RATIO);

	//body_man = TestScene::getInstance()->world->CreateBody(&bodydef);
	//body_man->SetUserData(Character::getInstance()->sp_man);

	////PhysicsEditor:�������������
	///*b2Vec2 points[27] = {
	//b2Vec2(12.00000 / PTM_RATIO,30.00000 / PTM_RATIO), b2Vec2(12.00000 / PTM_RATIO,36.00000 / PTM_RATIO),b2Vec2(20.00000 / PTM_RATIO,34.00000 / PTM_RATIO),
	//b2Vec2(12.00000 / PTM_RATIO,36.00000 / PTM_RATIO),b2Vec2(12.00000 / PTM_RATIO,22.00000 / PTM_RATIO), b2Vec2(-8.00000 / PTM_RATIO,9.00000 / PTM_RATIO),
	//b2Vec2(-12.00000 / PTM_RATIO,9.00000 / PTM_RATIO), b2Vec2(-20.00000 / PTM_RATIO,30.00000 / PTM_RATIO),b2Vec2(-4.00000 / PTM_RATIO,43.00000 / PTM_RATIO),
	//b2Vec2(9.00000 / PTM_RATIO,46.00000 / PTM_RATIO),b2Vec2(21.00000 / PTM_RATIO,17.00000 / PTM_RATIO), b2Vec2(5.00000 / PTM_RATIO,7.00000 / PTM_RATIO),
	//b2Vec2(-8.00000 / PTM_RATIO,9.00000 / PTM_RATIO), b2Vec2(12.00000 / PTM_RATIO,22.00000 / PTM_RATIO),b2Vec2(20.00000 / PTM_RATIO,23.00000 / PTM_RATIO),
	//b2Vec2(11.00000 / PTM_RATIO,-46.00000 / PTM_RATIO),b2Vec2(-11.00000 / PTM_RATIO,-46.00000 / PTM_RATIO), b2Vec2(4.00000 / PTM_RATIO,-35.00000 / PTM_RATIO),
	//b2Vec2(11.00000 / PTM_RATIO,-38.00000 / PTM_RATIO), b2Vec2(-11.00000 / PTM_RATIO,-46.00000 / PTM_RATIO),b2Vec2(-8.00000 / PTM_RATIO,9.00000 / PTM_RATIO),
	//b2Vec2(5.00000 / PTM_RATIO,7.00000 / PTM_RATIO),b2Vec2(6.00000 / PTM_RATIO,-17.00000 / PTM_RATIO), b2Vec2(4.00000 / PTM_RATIO,-35.00000 / PTM_RATIO),
	//b2Vec2(6.00000 / PTM_RATIO,-17.00000 / PTM_RATIO), b2Vec2(5.00000 / PTM_RATIO,7.00000 / PTM_RATIO),b2Vec2(12.00000 / PTM_RATIO,-12.00000 / PTM_RATIO) };*/
	//b2Vec2 points[27] = {
	//	b2Vec2(20.000 / PTM_RATIO,34.000 / PTM_RATIO), b2Vec2(12.000 / PTM_RATIO,36.000 / PTM_RATIO),b2Vec2(12.000 / PTM_RATIO,30.000 / PTM_RATIO),
	//	b2Vec2(9.000 / PTM_RATIO,46.000 / PTM_RATIO),b2Vec2(-4.000 / PTM_RATIO,43.000 / PTM_RATIO), b2Vec2(-20.000 / PTM_RATIO,30.000 / PTM_RATIO),
	//	b2Vec2(-12.00000 / PTM_RATIO,9.00000 / PTM_RATIO), b2Vec2(-8.000 / PTM_RATIO,9.000 / PTM_RATIO),b2Vec2(12.000 / PTM_RATIO,22.000 / PTM_RATIO),
	//	b2Vec2(12.000 / PTM_RATIO,36.000 / PTM_RATIO),

	//	b2Vec2(20.000 / PTM_RATIO,23.000 / PTM_RATIO), b2Vec2(12.000 / PTM_RATIO,22.000 / PTM_RATIO),
	//	b2Vec2(-8.00000 / PTM_RATIO,9.00000 / PTM_RATIO), b2Vec2(5.000 / PTM_RATIO,7.000 / PTM_RATIO),b2Vec2(21.000 / PTM_RATIO,17.000 / PTM_RATIO),




	//	b2Vec2(11.00000 / PTM_RATIO,-38.000 / PTM_RATIO),b2Vec2(4.000 / PTM_RATIO,-35.000 / PTM_RATIO), b2Vec2(-11.000 / PTM_RATIO,-46.000 / PTM_RATIO),
	//	b2Vec2(11.000 / PTM_RATIO,-46.000 / PTM_RATIO),b2Vec2(4.000 / PTM_RATIO,-35.000 / PTM_RATIO), b2Vec2(6.000 / PTM_RATIO,-17.000 / PTM_RATIO),
	//	b2Vec2(5.000 / PTM_RATIO,7.000 / PTM_RATIO),b2Vec2(-8.000 / PTM_RATIO,9.000 / PTM_RATIO),b2Vec2(-11.000 / PTM_RATIO,-46.000 / PTM_RATIO),
	//	b2Vec2(12.000 / PTM_RATIO,-12.000 / PTM_RATIO),b2Vec2(5.000 / PTM_RATIO,7.000 / PTM_RATIO), b2Vec2(6.000 / PTM_RATIO,-17.000 / PTM_RATIO) };


	//b2PolygonShape shape_sp_man;
	//shape_sp_man.Set(points, 27);

	//b2FixtureDef fixturedef;
	//fixturedef.density = 1.0f;   //�ܶ�
	//fixturedef.friction = 0.3f;  //Ħ��
	//fixturedef.shape = &shape_sp_man;
	//body_man->CreateFixture(&fixturedef);








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
	sp_man->setRotation3D(Vec3(0, speedPlus_minus * 90, 0));                     //ת��
	//sp_man->setPosition(Point(pos.x + speedPlus_minus * delta * speed, pos.y));  //�ƶ�
}

int Character::get_speed()
{
	return this->speed;
}
void Character::set_speed(int speed)
{
	this->speed = speed;
}

void Character::jump()
{
	//if (!characterStatus.InSky)  //�жϽ�ɫ״̬
	//{
	//	characterStatus.InSky = true;

	//	auto ac_jump = JumpBy::create(0.5f, Point(0, 0), 40, 1);
	//	sp_man->runAction(ac_jump);
	//}
	characterStatus.InSky = true;

	auto ac_jump = JumpBy::create(0.5f, Point(0, 0), 40, 1);
	b2Vec2(0, 40);
}

void Character::initBeginPos(TMXTiledMap *map)  //��ʼ����ɫλ�ã�ͨ����Ƭ��ͼ����
{
	//����һ����Ŷ����Group
	TMXObjectGroup* objGroup = map->getObjectGroup("objects");  //objects��ʾ��Ƭ��ͼ����������
	//����PlayerPoint����
	ValueMap PlayerPoint = objGroup->getObject("PlayerPoint"); //��ȡ�������е�PlayerPoint����

	float posBegin_X = PlayerPoint.at("x").asFloat();  //�õ������x��y����
	float posBegin_Y = PlayerPoint.at("y").asFloat();

	sp_man->setPosition(Point(posBegin_X, posBegin_Y));   //�趨��ɫ��ʼ����λ��
}
