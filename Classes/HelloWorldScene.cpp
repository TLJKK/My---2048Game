/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include<sstream>
#include<string>
#include "ui/CocosGUI.h"
#include<iostream>
#include "card.h"
#include"SimpleAudioEngine.h"
#include"gameOver.h"
#pragma execution_character_set("utf-8")
#define SIZE 4
USING_NS_CC;
using namespace CocosDenshion;

card* HelloWorld::ali = nullptr;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

bool HelloWorld::MaddChild(std::string& name, cocos2d::Vec2 now, float scale)
{
	auto sprite = Sprite::create(name);
	sprite->setAnchorPoint(cocos2d::Vec2(0, 1));
	sprite->setScale(scale);
	if (sprite == nullptr)
	{
		problemLoading("'photoshop'");
		return false;
	}
	else
	{
		// position the sprite on the center of the screen
		sprite->setPosition(now);

		// add the sprite as a child to this layer
		this->addChild(sprite, 0);
	}

	return true;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    if ( !Scene::init() )
    {
        return false;
    }

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	//this->addChild(gameOver::create(),1);
	//skeletonNode = spine::SkeletonAnimation::createWithFile("dragon.json", "dragon.atlas", 0.2f);//0.2是设置图片的缩放比例
	//skeletonNode->setPosition(80, visibleSize.height / 2);
	//skeletonNode->setAnchorPoint(Point(0.5, 0.5));
	//skeletonNode->addAnimation(0, "flying", true);//true是指循环播放walk动作
	//schedule(schedule_selector(Dragon::timer_Hander), 0);
	//skeletonNode->setVisible(false);
	//skeletonNode->getSkeleton()->flipX = 0.75;
	//addChild(skeletonNode, 1);
	SimpleAudioEngine::getInstance()->preloadEffect("see.wav");

	auto m_emitter1 = ParticleSystemQuad::create("snow.plist");
	m_emitter1->setPosition(500, 300);
	this->addChild(m_emitter1);

	auto gameRec = Sprite::create("bg.png");
	gameRec->setPosition(visibleSize / 2 - (cocos2d::Size)Vec2(0, 55));

	ali = card::getInstance();
	ali->init();
	ali->Minit(gameRec);
	this->addChild(gameRec);

	CCDirector::sharedDirector()->setProjection(kCCDirectorProjection2D);
	CCDirector::getInstance()->setDisplayStats(false);
	this->MaddChild(std::string("topbg.png"), cocos2d::Vec2(0, 720), 1);
	this->MaddChild(std::string("highest.png"), cocos2d::Vec2(10, 650), 0.9);
	this->MaddChild(std::string("score.png"), cocos2d::Vec2(370, 648), 0.9);
	//this->MaddChild(std::string("4.gif"), cocos2d::Vec2(100, 350), 1);

	bestScore = Label::createWithTTF("0", "fonts/Marker Felt.ttf", 45);
	bestScore->setAnchorPoint(Vec2(0, 1));
	bestScore->setPosition(Vec2(610, 650));
	bestScore->setColor(Color3B(Color3B::BLUE));
	this->addChild(bestScore);
	
	auto reBegin = cocos2d::ui::Button::create("rebegin.png");
	reBegin->setScale(0.4);
	reBegin->setAnchorPoint(Vec2(0, 1));
	reBegin->setPosition(Vec2(840, 655));
	this->addChild(reBegin);
	schedule(schedule_selector(HelloWorld::tiemer_hander, 0));

	FILE* fp = NULL;

	if ((fp = fopen("data.txt", "r")) == NULL)
	{
		printf("The file %s can not be opened.\n", "data.txt");
		return false;
	}
	fscanf(stdin, "%d", &ScoreHighest);
	//fprintf(fp, "%s %5d %4d %8ld", name, nClass, nAge, number);
	fclose(fp);


	auto highest = Label::createWithTTF(std::to_string(ScoreHighest),
		"fonts/Marker Felt.ttf", 45);
	highest->setAnchorPoint(Vec2(0, 1));
	highest->setPosition(Vec2(190, 650));
	highest->setColor(Color3B(Color3B::BLUE));
	this->addChild(highest, 1);
	dir = -1;
	auto listen = EventListenerKeyboard::create();
	listen->onKeyPressed = ([=](EventKeyboard::KeyCode code, Event* event)
		{
			int ScoreNow = 0;
			switch (code)
			{
			case EventKeyboard::KeyCode::KEY_W:
			case EventKeyboard::KeyCode::KEY_CAPITAL_W:
				ScoreNow = moveUp(ali->data);
				if (-1 == ScoreNow)press = 0;
				else press = 1;

				break;
			case EventKeyboard::KeyCode::KEY_S:
			case EventKeyboard::KeyCode::KEY_CAPITAL_S:
				ScoreNow = moveDown(ali->data);
				if (-1 == ScoreNow)press = 0;
				else press = 1;

				break;
			case EventKeyboard::KeyCode::KEY_A:
			case EventKeyboard::KeyCode::KEY_CAPITAL_A:
				ScoreNow = moveLeft(ali->data);
				if (-1 == ScoreNow)press = 0;
				else press = 1;

				break;
			case EventKeyboard::KeyCode::KEY_D:
			case EventKeyboard::KeyCode::KEY_CAPITAL_D:
				ScoreNow = moveRight(ali->data);
				if (-1 == ScoreNow)press = 0;
				else press = 1;

				break;
			default:
				break;
			}

			ScoreNow = 0;
			for (int i = 0; i < 4; i++)
				for (int j = 0; j < 4; j++)
					ScoreNow += ali->data[i][j];

			 bestScore->setString(std::to_string(ScoreNow));

		});

	auto flush = EventListenerTouchOneByOne::create();
	//flush->setSwallowTouches(true); //阻止向下传递
	flush->onTouchBegan = [=](Touch* touch, Event* event)
	{
		/* 获取事件绑定的精灵 */
		auto target = static_cast<Sprite*>(event->getCurrentTarget());
		Point pos = Director::getInstance()->convertToGL(touch->getLocationInView());

		/* 检测是否触摸到精灵 ⁄ */
		if (target->getBoundingBox().containsPoint(pos))
		{
			auto tran = TransitionTurnOffTiles::create(2, HelloWorld::createScene());
			Director::getInstance()->replaceScene(tran);
			return true;
		}

		return false;
	};

	auto dispatcher = Director::getInstance()->getEventDispatcher();

	//添加到事件分发器
	dispatcher->addEventListenerWithSceneGraphPriority(listen, this);
	//dispatcher->addEventListenerWithSceneGraphPriority(flush, reBegin);
	reBegin->addClickEventListener([this](Ref* ref)
		{

			int ScoreNow = 0;
			for (int i = 0; i < 4; i++)
				for (int j = 0; j < 4; j++)
					ScoreNow += ali->data[i][j];

			FILE* fp = fopen("data.txt", "a+"); //第一个逗号前是文件位置。逗号之后是打开文件方式
			int tmp = 0;
			fread(&tmp, 4, 1, fp);
			rewind(fp);
			if (tmp < ScoreNow)
			{
				fprintf(fp, "%d", ScoreNow);
			}
			fclose(fp);  //记得用完关闭文件
			auto tran = TransitionZoomFlipX::create(2.0, HelloWorld::createScene());
			Director::getInstance()->replaceScene(tran);
		});
	press = false;
    return true;
}

void HelloWorld::tiemer_hander(float dt)
{
	/*if (skeletonNode->getPositionY() < -150)dir = 1;
	else if (skeletonNode->getPositionY()  > 880) dir = -1;
	skeletonNode->setPositionY(skeletonNode->getPositionY() + dir);
	skeletonNode->setPositionX(skeletonNode->getPositionX() + dir * 0.2);
	skeletonNode->getSkeleton()->flipX = dir > 0 ? 0 : 1;*/

	if (press)
	{
		//Sleep(50);
		int a, b;
		updata(a, b);
		press = 0;
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				if (ali->data[i][j] > 0)
				{
					ali->dataShow[i][j]->setVisible(true);
					ali->dataShow[i][j]->setString(std::to_string(ali->data[i][j]));
					int digit = 0, love = ali->data[i][j], color = ali->data[i][j], coloridx = 0;
					while (color > 1)color /= 2, coloridx++;
					while (love)digit++, love /= 10;
					ali->dataShow[i][j]->setColor(ali->colorRec[coloridx]);
					ali->dataShow[i][j]->setScale(ali->dataSize[digit]);
				}
				else ali->dataShow[i][j]->setVisible(false);

		int digit = 0, love = ali->data[a][b];
		while (love)digit++, love /= 10;
		Sequence* seq = Sequence::create(EaseElasticIn::create(ScaleTo::create(0.0f, 0.0f, 0.0f)),
			EaseElasticOut::create(ScaleTo::create(1.0f, 1.0f, 1.0f)), NULL);
		ali->dataShow[a][b]->runAction(seq);//cocos2d::FadeIn::create(0.5)
		ali->dataShow[a][b]->setScale(ali->dataSize[digit]);

		int kj[4][4];
		memcpy(kj, ali->data, sizeof(ali->data));

		if (-1 == moveLeft(kj) && -1 == moveRight(kj) &&
			-1 == moveUp(kj) && -1 == moveDown(kj))
		{
			int ScoreNow = 0;
			for (int i = 0; i < 4; i++)
				for (int j = 0; j < 4; j++)
					ScoreNow += ali->data[i][j];

			auto fp = fopen("data.txt", "w+"); //第一个逗号前是文件位置。逗号之后是打开文件方式
			int tmp;
			fscanf(fp, "%d", &tmp);
			rewind(fp);
			if (tmp < ScoreNow)
			{
				fprintf(fp, "%d", ScoreNow);
			}
			fclose(fp);  //记得用完关闭文件
			this->addChild(gameOver::create());
		}


		
	}
	return;

}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);
}

void HelloWorld::updata(int& ii, int& jj)
{

	while (1)
	{
		int i = CCRANDOM_0_1() * 4;
		int j = CCRANDOM_0_1() * 4;
		if (4 == i)i--;
		if (4 == j)j--;
		int flag = CCRANDOM_0_1() * 2;
		
		if (0 == ali->data[i][j])
		{
			ii = i, jj = j;
			if (flag)ali->data[i][j] = 2;
			else ali->data[i][j] = 4;
			break;
		}

	}

}

int HelloWorld::moveLeft(int board[SIZE][SIZE]) {
	int i, j, score = 0, flag = -1;

	for (i = 0; i < SIZE; i++)
	{
		for (j = 0; j < SIZE; j++)
		{
			int cell = board[i][j];//cell单词用的不太恰当，表示当前元素，你可以采用更有意义的命名
			if (cell != 0)
			{
				int k = j + 1;
				while (k < SIZE)
				{
					int nextcell = board[i][k];
					if (nextcell != 0)
					{
						if (cell == nextcell)
						{
							flag = 0;//相邻两个元素相同，就说明能移动，所以改变flag的值
							board[i][j] += board[i][k];
							SimpleAudioEngine::getInstance()->playEffect("see.mp3", false);
							board[i][k] = 0;
						}
						k = SIZE;
						break;
					}
					k++;
				}
			}
		}
	}

	//修改部分：for循环中的i或者j的循环条件

	for (i = 0; i < SIZE; i++)
	{
		for (j = 0; j < SIZE - 1; j++)
		{
			int cell = board[i][j];
			if (cell == 0)
			{
				int k = j + 1;
				while (k < SIZE)
				{
					int nextcell = board[i][k];
					if (nextcell != 0)
					{
						flag = 0;//
						board[i][j] = nextcell;
						board[i][k] = 0;
						k = SIZE;
					}
					k++;
				}
			}
		}
	}

	if (flag != -1)
		return score;
	else
		return -1;
}

int HelloWorld::moveRight(int board[SIZE][SIZE]) 
{
	int i, j, score = 0, flag = -1;
	for (i = 0; i < SIZE; i++)
	{
		for (j = SIZE - 1; j >= 0; j--)
		{
			int cell = board[i][j];
			if (cell != 0)
			{
				int k = j - 1;
				while (k >= 0)
				{
					int nextcell = board[i][k];
					if (nextcell != 0)
					{
						if (cell == nextcell)
						{
							flag = 0;
							board[i][j] += board[i][k];
							SimpleAudioEngine::getInstance()->playEffect("see.mp3", false);
							board[i][k] = 0;
						}
						k = -1;
						break;
					}
					k--;
				}
			}
		}
	}

	//修改部分：for循环中的i或者j的循环条件

	for (i = 0; i < SIZE; i++)
	{
		for (j = SIZE - 1; j > 0; j--)
		{
			int cell = board[i][j];
			if (cell == 0)
			{

				int k = j - 1;
				while (k >= 0)
				{
					int nextcell = board[i][k];
					if (nextcell != 0)
					{
						flag = 0;//当前元素为0，说明能移动，改变flag的值
						board[i][j] = nextcell;
						board[i][k] = 0;
						k = -1;
					}
					k--;
				}
			}
		}
	}

	if (flag != -1)
		return score;
	else
		return -1;
}

int HelloWorld::moveDown(int board[SIZE][SIZE]) {
	int i, j, score = 0, flag = -1;
	for (i = SIZE - 1; i >= 0; i--)
	{
		for (j = 0; j < SIZE; j++)
		{
			int cell = board[i][j];

			if (cell != 0)
			{
				int k = i - 1;
				while (k >= 0)
				{
					int nextcell = board[k][j];
					if (nextcell != 0)
					{
						if (board[i][j] == board[k][j])
						{
							flag = 0;
							board[i][j] += board[k][j];
							SimpleAudioEngine::getInstance()->playEffect("see.mp3", false);
							board[k][j] = 0;
						}
						k = 0;
						break;
					}
					k--;
				}
			}
		}
	}

	//修改部分：for循环中的i或者j的循环条件
	for (i = SIZE - 1; i > 0; i--)
	{
		for (j = 0; j < SIZE; j++)
		{
			int cell = board[i][j];
			if (cell == 0)
			{
				int k = i - 1;
				while (k >= 0)
				{
					int nextcell = board[k][j];
					if (nextcell != 0)
					{
						flag = 0;
						board[i][j] = nextcell;
						board[k][j] = 0;
						k = 0;
					}
					k--;
				}
			}
		}
	}

	if (flag != -1)
		return score;
	else
		return -1;
}

int HelloWorld::moveUp(int board[SIZE][SIZE])
{
	int i, j, score = 0, flag = -1;
	for (i = 0; i < SIZE; i++)
	{
		for (j = 0; j < SIZE; j++)
		{
			int cell = board[i][j];

			if (cell != 0)
			{
				int k = i + 1;
				while (k < SIZE)
				{
					int nextcell = board[k][j];
					if (nextcell != 0)
					{
						if (cell == nextcell)
						{
							flag = 0;
							board[i][j] += board[k][j];
							SimpleAudioEngine::getInstance()->playEffect("see.mp3", false);
							board[k][j] = 0;
						}
						k = SIZE;
						break;
					}
					k++;
				}
			}
		}
	}

	//修改部分：for循环中的i或者j的循环条件
	for (i = 0; i < SIZE - 1; i++)
	{
		for (j = 0; j < SIZE; j++)
		{
			int cell = board[i][j];
			if (cell == 0)
			{

				int k = i + 1;
				while (k < SIZE)
				{
					int nextcell = board[k][j];
					if (nextcell != 0)
					{
						flag = 0;
						board[i][j] = nextcell;
						board[k][j] = 0;
						k = SIZE;
					}
					k++;
				}
			}
		}
	}

	if (flag != -1)
		return score;
	else
		return -1;
}

