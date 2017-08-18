#include "Character.h"

bool moveAnimtionFinished = true;       //移动动画播放完成
bool isSporting = false;                //角色是否处于运动中

Character::Character()
{
	this->hp = 100;
	this->sp = 100;

	this->speed = 6;

	this->jump_height_small = 10;
	this->jump_height_big = 10;


	sp_man = Sprite3D::create("res/3Dsprite&action/sprite_stand.c3b");  //精灵素材，配合素材文件使用

	sp_man->setGlobalZOrder(5);    //控制3D角色遮挡

	Size winsize = Director::getInstance()->getWinSize();

	sp_man->setScale(1.5f);     //设置放大比例

	auto ac_RotateTo = RotateTo::create(0.001f, Vec3(0, 90, 0));//设定旋转动作，操纵Sprite => 不能设置为0.f部分机型不执行

	sp_man->runAction(ac_RotateTo);
}


Character::~Character()
{
}


void Character::move(int speedPlus_minus) //角色横向移动及动画
{	
	auto animation = Animation3D::create("res/3Dsprite&action/run_to_stand_final.c3b");  //获取骨骼信息

	if (speedPlus_minus == 0) //执行跑步恢复至站立的动画
	{
		if (isSporting == false)  //角色是否处于运动中
			return;

		sp_man->stopAllActions();

		isSporting = false;            //角色不在运动中
		moveAnimtionFinished = true;   //角色移动动画结束

		auto play_recover_stand = Animate3D::create(animation, 0.5f, 0.68f);   //创建3D恢复动画	
		play_recover_stand->setSpeed(100.0f);
		
		sp_man->runAction(play_recover_stand);
	
		return;
	}

	auto play_move = Animate3D::create(animation, 0.f, 0.5f);   //创建3D动画

	auto ac_move = MoveBy::create(0.1f, Point(speedPlus_minus*speed, 0)); //移动动作

	auto action_turn = RotateTo::create(0, Vec3(0, speedPlus_minus*90, 0));   //设定旋转动作，操纵Sprite转动

	//动画执行完以后的回调 => 防止一个动画未结束，另一个动画就开始
	auto *actionMove =
		Sequence::create(play_move, CallFunc::create(CC_CALLBACK_0(Character::moveAnimtionCallback, this)), NULL);

	if (moveAnimtionFinished == true)   //动画不可附加 => 利用Sequence一个一个执行的特点，在执行完动画之后才执行回调->告知动画结束
	{
		moveAnimtionFinished = false;
		sp_man->runAction(actionMove);    //执行动画
	}
	auto ac_spawn = Spawn::create(action_turn, ac_move, NULL);   //移动可每帧进行
	sp_man->runAction(ac_spawn);
}

void Character::moveAnimtionCallback()  //移动动画播放完的回调
{
	moveAnimtionFinished = true;
}



int Character::get_speed()
{
	return this->speed;
}

void Character::jump()  //跳跃
{
	auto ac_jump = JumpBy::create(0.1f, Point(0, 0),40, 1);
	sp_man->runAction(ac_jump);
}

void Character::initBeginPos(TMXTiledMap *map)  //初始化角色位置（通过瓦片地图对象）
{
	//创建一个存放对象的Group
	TMXObjectGroup* objGroup = map->getObjectGroup("objects");  //objects表示瓦片地图对象层的名字
	//加载PlayerPoint对象
	ValueMap PlayerPoint = objGroup->getObject("PlayerPoint"); //获取对象组中的PlayerPoint对象

	float posBegin_X = PlayerPoint.at("x").asFloat();  //得到对象的x，y坐标
	float posBegin_Y = PlayerPoint.at("y").asFloat();

	sp_man->setPosition(Point(posBegin_X, posBegin_Y));   //设定角色初始化的位置
}