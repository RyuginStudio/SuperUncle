#ifndef __CHARACTER_H__
#define __CHARACTER_H__

#include "cocos2d.h" 
USING_NS_CC;

class Character : public Node                       //�̳�Node��Node������Щ���ھ�������Ժͷ����Ѿ�д�ã�����ֱ�Ӽ̳й���
{
public:
	Character();
	~Character();

	void move(int speedPlus_minus);     //��ɫ�����ƶ�������(�����ߴ���-1�������ߴ���1��ֹͣ��������0)
	void moveAnimtionCallback();        //�ƶ�����������Ļص�

	void jump();     //��ɫ��������
	void initBeginPos(TMXTiledMap *map);   //��ʼ����ɫλ�ã�ͨ����Ƭ��ͼ��

	
	int get_speed();	
	
	Sprite3D * sp_man;  //3D����


private:
	int hp;  //Ѫ��
	int sp;  //����
	int speed; //����
	
	bool big_small; //��С״̬    ��ʼΪfalse => ���ͣ�С

	int jump_height_small; //С�ͽ�ɫ�����߶�
	int jump_height_big;   //����ɫ�����߶�
};


#endif // !__CHARACTER_H__