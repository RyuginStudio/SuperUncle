/*============��������Ϊ��Ϸ������أ��ɿ����ƶ���txt�ĵ�������i/o�����������û���д��Ϸ����==========

*���ڰ�ȫ����Ŀǰδʵ���������ܣ���ֹ�����д������Ϸ��������ɥʧ������

*��Ϸ����δ���������ݿ⣬��ͷ�ļ������ݿ�֮�ã��Ժ���в������ݿ�Ĺ���

*δ�����������

*/


#ifndef __GAME_DATA_H__
#define __GAME_DATA_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

using namespace cocostudio::timeline;


int mario_lives = 6; //�趨��ʼ�����"������"    0-99������Ӱ����ʾЧ��������Ϊ����������Ҳ����ȥ

int mission = 9999;     //�趨��ʼ��Ϸ�ؿ�����һ��    9999 => ��Ϸ����

int specialSceneScore = 0; //�趨���������Ϸ��������ĵ��������ؽ�ַ���

int latest_draw = 99;   //������

#endif 