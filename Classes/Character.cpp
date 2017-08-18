#include "Character.h"

bool moveAnimtionFinished = true;       //�ƶ������������
bool isSporting = false;                //��ɫ�Ƿ����˶���

Character::Character()
{
	this->hp = 100;
	this->sp = 100;

	this->speed = 6;

	this->jump_height_small = 10;
	this->jump_height_big = 10;


	sp_man = Sprite3D::create("res/3Dsprite&action/sprite_stand.c3b");  //�����زģ�����ز��ļ�ʹ��

	sp_man->setGlobalZOrder(5);    //����3D��ɫ�ڵ�

	Size winsize = Director::getInstance()->getWinSize();

	sp_man->setScale(1.5f);     //���÷Ŵ����

	auto ac_RotateTo = RotateTo::create(0.001f, Vec3(0, 90, 0));//�趨��ת����������Sprite => ��������Ϊ0.f���ֻ��Ͳ�ִ��

	sp_man->runAction(ac_RotateTo);
}


Character::~Character()
{
}


void Character::move(int speedPlus_minus) //��ɫ�����ƶ�������
{	
	auto animation = Animation3D::create("res/3Dsprite&action/run_to_stand_final.c3b");  //��ȡ������Ϣ

	if (speedPlus_minus == 0) //ִ���ܲ��ָ���վ���Ķ���
	{
		if (isSporting == false)  //��ɫ�Ƿ����˶���
			return;

		sp_man->stopAllActions();

		isSporting = false;            //��ɫ�����˶���
		moveAnimtionFinished = true;   //��ɫ�ƶ���������

		auto play_recover_stand = Animate3D::create(animation, 0.5f, 0.68f);   //����3D�ָ�����	
		play_recover_stand->setSpeed(100.0f);
		
		sp_man->runAction(play_recover_stand);
	
		return;
	}

	auto play_move = Animate3D::create(animation, 0.f, 0.5f);   //����3D����

	auto ac_move = MoveBy::create(0.1f, Point(speedPlus_minus*speed, 0)); //�ƶ�����

	auto action_turn = RotateTo::create(0, Vec3(0, speedPlus_minus*90, 0));   //�趨��ת����������Spriteת��

	//����ִ�����Ժ�Ļص� => ��ֹһ������δ��������һ�������Ϳ�ʼ
	auto *actionMove =
		Sequence::create(play_move, CallFunc::create(CC_CALLBACK_0(Character::moveAnimtionCallback, this)), NULL);

	if (moveAnimtionFinished == true)   //�������ɸ��� => ����Sequenceһ��һ��ִ�е��ص㣬��ִ���궯��֮���ִ�лص�->��֪��������
	{
		moveAnimtionFinished = false;
		sp_man->runAction(actionMove);    //ִ�ж���
	}
	auto ac_spawn = Spawn::create(action_turn, ac_move, NULL);   //�ƶ���ÿ֡����
	sp_man->runAction(ac_spawn);
}

void Character::moveAnimtionCallback()  //�ƶ�����������Ļص�
{
	moveAnimtionFinished = true;
}



int Character::get_speed()
{
	return this->speed;
}

void Character::jump()  //��Ծ
{
	auto ac_jump = JumpBy::create(0.1f, Point(0, 0),40, 1);
	sp_man->runAction(ac_jump);
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