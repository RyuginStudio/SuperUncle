/*
时间：2017年9月11日00:47:10
作者：nono_o
修改：张天翼
功能：把精灵或图片进行模糊处理
思路：
1.为将要进行模糊处理的精灵或图片截图
2.利用官方自带的模糊shader(着色器)对截图进行模糊处理
3.用模糊化后的精灵或图片生成一个临时精灵置为最上层(假背景)
CSDN：http://blog.csdn.net/nono_o/article/details/45891757
*/

#ifndef __SPRITE_BLUR_H__
#define __SPRITE_BLUR_H__

#include "cocos2d.h"

class SpriteBlur : public cocos2d::Sprite
{
public:

	SpriteBlur();
	~SpriteBlur();

	static SpriteBlur* create(cocos2d::Sprite* pSprite, const float fRadius = 8.0f, const float fSampleNum = 8.0f);
	bool initWithTexture(cocos2d::Texture2D* pTexture, const cocos2d::Rect&  rRect);
	void initGLProgram();

	void setBlurRadius(float fRadius);
	void setBlurSampleNum(float fSampleNum);

	static cocos2d::RenderTexture* ScreenShot(const bool bIsSave, std::function<void(cocos2d::RenderTexture*, const std::string&)> pFuncCallback);
	static cocos2d::RenderTexture* SpriteBlurer(cocos2d::Sprite* pSprite, const float fRadius = 8.0f, const float fSampleNum = 8.0f);

protected:
	float fBlurRadius_;
	float fBlurSampleNum_;
};

#endif // !__SPRITE_BLUR_H__