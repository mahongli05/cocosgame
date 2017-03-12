//
// Created by Administrator on 2017/3/12.
//

#include "Hero.h"

bool Hero::init() {
    return Sprite::init();
}

Hero *Hero::createHero(MessageReceiver *receiver, MapHelper *mapHelper) {
    Hero* hero = Hero::create();
    if (hero != nullptr) {
        hero->setReceiver(receiver);
        hero->mMapHelper = mapHelper;
    }
    return hero;
}

void Hero::onMove(float dt, bool toRight) {

    if(mMapHelper == NULL) {
        CCAssert(false,"Error:No Map!");
        return;
    }

    //判断是否可以移动
    bool bCanMove = true;
    Sprite* pTileSpriteTop = NULL;
    Sprite* pTileSpriteMid = NULL;
    Sprite* pTileSpriteBottom = NULL;

    Point position = getPosition();
    Size size = getBoundingBox().size;
    Point ptPosTop = Point(position.x + toRight ? (size.width / 2) : -(size.width / 2), position.y + size.height);
    Point ptPosMid =
    CCPoint ptPosMid = bToRight?ccp(getPositionX()+boundingBox().size.width,getPositionY()+boundingBox().size.height/2):
                       ccp(getPositionX(),getPositionY()+boundingBox().size.height/2);
    CCPoint ptPosBottom = bToRight?ccp(getPositionX()+boundingBox().size.width,getPositionY()+5):
                          ccp(getPositionX(),getPositionY()+5);

    //检查马里奥前进方向是否有障碍(检查 上 中 下 三个方向)
    pTileSpriteTop = m_pGameMap->TileMapLayerPosToTileSprite(ptPosTop);
    pTileSpriteMid = m_pGameMap->TileMapLayerPosToTileSprite(ptPosMid);
    pTileSpriteBottom = m_pGameMap->TileMapLayerPosToTileSprite(ptPosBottom);

    if (pTileSpriteTop!=NULL || pTileSpriteMid!=NULL || pTileSpriteBottom!=NULL)
    {
        bCanMove = false;//若在前进方向找到了砖块 则 禁止移动
    }
    //判断马里奥是否移动出屏幕边界
    if(bToRight==false)//判断是否左出界
    {
        CCPoint ptMarioInWorld = m_pGameMap->convertToWorldSpace(getPosition());
        if(ptMarioInWorld.x<=0)
        {
            bCanMove = false;
        }
    }
    else//判断是否右出界（假定，实际不应该出现这种情况）
    {
        CCPoint ptMarioInWorld = m_pGameMap->convertToWorldSpace(getPosition());
        if(ptMarioInWorld.x>=SCREEN_WIDTH-boundingBox().size.width)
        {
            bCanMove = false;
        }
    }

    //根据马里奥当前状态 来处理
    switch(m_eCurMarioStatus)
    {
        case enMarioStatusStandLeft:		//如果是待机状态
        case enMarioStatusStandRight:
        {
            //根据状态播放动画
            switch(m_eCurMarioLevel)
            {
                case enMarioLevelSmall:
                {
                    m_pCcbReader->getAnimationManager()->runAnimationsForSequenceNamed(_CCB_MARIO_SMALL_RUN_);
                }break;
                case enMarioLevelBig:
                {
                    m_pCcbReader->getAnimationManager()->runAnimationsForSequenceNamed(_CCB_MARIO_BIG_RUN_);
                }break;
                case enMarioLevelMax:
                {
                    m_pCcbReader->getAnimationManager()->runAnimationsForSequenceNamed(_CCB_MARIO_MAX_RUN_);
                }break;
            }

            //变更位移
            if(bCanMove==true)
            {
                float fCurPosX = getPositionX();
                if(bToRight)
                {
                    fCurPosX += _MARIO_BASIC_SPEED_PER_SEC_*fT;	//向右移动
                    m_bFaceToRight = true;
                }
                else
                {
                    fCurPosX -= _MARIO_BASIC_SPEED_PER_SEC_*fT;	//向左移动
                    m_bFaceToRight = false;
                }
                setPositionX(fCurPosX);
            }

            //改变马里奥状态
            m_eCurMarioStatus = bToRight?enMarioStatusRunRight:enMarioStatusRunLeft;

            //设置马里奥面对方向
            MarioTurn(bToRight);

        }break;
        case enMarioStatusRunLeft:	//若马里奥正在向左移动
        case enMarioStatusRunRight:	//这里设计成允许直接变向，如果要增加额外动作效果，可在这里区分
        {
            //变更位移
            if(bCanMove==true)
            {
                float fCurPosX = getPositionX();
                if(bToRight)
                {
                    fCurPosX += _MARIO_BASIC_SPEED_PER_SEC_*fT;	//向右移动
                }
                else
                {
                    fCurPosX -= _MARIO_BASIC_SPEED_PER_SEC_*fT;	//向左移动
                }
                setPositionX(fCurPosX);
            }
            //改变马里奥状态
            m_eCurMarioStatus = bToRight?enMarioStatusRunRight:enMarioStatusRunLeft;

            //设置马里奥面对方向
            MarioTurn(bToRight);
        }break;
        case enMarioStatusOnAirLeft:		//同方向跳跃正常位移 反方向跳跃 不改变面对方向 且位移量减半
        case enMarioStatusOnAirRight:		//
        {
            if(bCanMove==true)
            {
                //变更位移
                float fCurPosX = getPositionX();

                //计算位移量
                float fMoveDis = 0.f;
                if((m_eCurMarioStatus==enMarioStatusOnAirLeft&&bToRight==true)||(m_eCurMarioStatus==enMarioStatusOnAirRight&&bToRight==false))
                {
                    fMoveDis = _MARIO_BASIC_SPEED_PER_SEC_*fT*0.5f;	//若反向 则 位移量减半
                }
                else
                {
                    fMoveDis = _MARIO_BASIC_SPEED_PER_SEC_*fT;			//若同向 则 正常位移
                }

                if(bToRight)
                {
                    fCurPosX += fMoveDis;	//向右移动
                }
                else
                {
                    fCurPosX -= fMoveDis;	//向左移动
                }
                setPositionX(fCurPosX);
            }
        }break;
    }
}

void Hero::onJump(float dt) {

}

void Hero::onStop(float dt) {

}

Hero::Hero() {
    mMessageReceiver = nullptr;
    mMapHelper = nullptr;
}

Hero::~Hero() {
    mMessageReceiver = nullptr;
    mMapHelper = nullptr;
}













