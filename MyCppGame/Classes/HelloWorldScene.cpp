#include "HelloWorldScene.h"
#include "PopupLayer.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

void HelloWorld::update(float delta)
{
    Node::update(delta);
}

const int MAX_COL = 8, MAX_ROW = 8;

float originX,originY,finalX,finalY,distanceX,distanceY;
Size size;
float cellWidth, cellHeight;
Vec2 pos[MAX_COL][MAX_ROW];
Node *selectedNode;
bool isWallTime;

//横0竖1
bool isWall[MAX_COL][MAX_ROW][2];

struct nodePos{
    int xIndex;
    int yIndex;
    
    int wall;
};

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    //get screen size
    size = Director::getInstance()->getWinSize();
    CCLOG("screen height is %f,width is %f",size.width,size.height);
    
    
    cellWidth = size.width / MAX_COL;
    cellHeight = size.height / MAX_ROW;

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    
    
    DrawNode *drawNode = DrawNode::create();
    drawNode->drawRect(Vec2(0, 0), Vec2(size.width, size.height), Color4F(1.0f, 1.0f, 1.0f, 1.0f));
    
    drawGrid(drawNode);
    
    this->addChild(drawNode);
    
    // Make positions
    
    for (int i = 0; i < MAX_COL; i++) {
        for(int j = 0; j < MAX_ROW; j++){
            for(int x = 0; x < 2; x++){
                isWall[i][j][x]=false;
            }
            pos[i][j] = Vec2(cellWidth / 2 + i * cellWidth, cellHeight / 2 +j * cellHeight);
        }
    }
    
    DrawNode *objectA = DrawNode::create();
    auto sprite1 = CCSprite::create("P1.png");
    sprite1->setScale(cellWidth / sprite1->getContentSize().width, cellHeight / sprite1->getContentSize().height);
    objectA->addChild(sprite1);
    
    DrawNode *objectB = DrawNode::create();
    auto sprite2 = CCSprite::create("P2.png");
    sprite2->setScale(cellWidth / sprite2->getContentSize().width, cellHeight / sprite2->getContentSize().height);    objectB->addChild(sprite2);
    
    objectA->setPosition(pos[0][0]);
    objectB->setPosition(pos[MAX_ROW-1][MAX_COL-1]);
    
    this->addChild(objectA);
    this->addChild(objectB);
    
    auto posA = new nodePos();
    auto posB = new nodePos();
    posA->xIndex = posB->yIndex = 0;
    posB->xIndex = MAX_COL - 1;
    posB->yIndex = MAX_ROW - 1;
    posA->wall = posB->wall = 10;
    
    objectA->setUserData(posA);
    objectB->setUserData(posB);
    selectedNode = objectA;

    //  Create a "one by one" touch event listener
    // (processes one touch at a time)
    auto listener1 = EventListenerTouchOneByOne::create();
    
    
    // trigger when you push down
    listener1->onTouchBegan = [](Touch* touch, Event* event){
        // your code
        originX = touch->getLocation().x;
        originY = touch->getLocation().y;
        return true; // if you are consuming it
    };
    
    // trigger when moving touch
    listener1->onTouchMoved = [](Touch* touch, Event* event){
        // your code
    };
    
    // trigger when you let up
    listener1->onTouchEnded = [=](Touch* touch, Event* event){
        // your code
        finalX = touch->getLocation().x;
        finalY = touch->getLocation().y;
        distanceX = finalX - originX;
        distanceY = finalY - originY;
        int x=0,y=0;
        if(std::abs(distanceX) > std::abs(distanceY))
        {
            if(distanceX > 0)
            {x++;}
            else{x--;}
        
        }
        else
        {
            if(distanceY > 0)
            {y++;}
            else{y--;}
        
        }
        
        
          CCLOG("X is %d,Y is %d",x,y);
        
        
        if(!isWallTime){
            nodePos *posIndex = (nodePos *) selectedNode->getUserData();
            posIndex->xIndex+=x;
            posIndex->yIndex+=y;
            if ((posIndex->xIndex<0)||(posIndex->yIndex<0)||(posIndex->xIndex>=MAX_COL)||(posIndex->yIndex>=MAX_ROW)
           ||(isWallBlock(posIndex->xIndex,posIndex->yIndex,x,y)==true))
            {
                posIndex->xIndex-=x;
                posIndex->yIndex-=y;
                isWallTime = !isWallTime;
                return;
            }
            selectedNode->setPosition(pos[posIndex->xIndex][posIndex->yIndex]);
            if(posIndex->xIndex == MAX_ROW - 1 && posIndex->yIndex == MAX_COL - 1){
                if(selectedNode == objectA){
                    popupLayer(true);
                }else{
                    popupLayer(false);
                }
            }
            
        }else{
            bool isJump;
            nodePos *posIndex = (nodePos *) selectedNode->getUserData();
            if(posIndex->wall == 0) {
                if(selectedNode == objectA){
                    selectedNode = objectB;
                }else{
                    selectedNode = objectA;
                }
                isWallTime = !isWallTime;
                return;
            }
            
            DrawNode *objectT = DrawNode::create();
            if(x==0){
            
            //objectT->drawSolidRect(Vec2(0, 0), Vec2(5, cellHeight), Color4F(1.0f, 1.0f, 1.0f, 1.0f));
                auto sprite3 = CCSprite::create("wall.png");
                sprite3->setScale(5 / sprite3->getContentSize().width, cellHeight / sprite3->getContentSize().height);
                objectT->addChild(sprite3);
            objectT->setPosition(Vec2(lroundf(finalX/cellWidth) * cellWidth-2.5, (lroundf(finalY/cellHeight) + 0.5) * cellHeight));
                
                isWall[lroundf(finalX/cellWidth) ][lroundf(finalY/cellHeight)][1]=true;
            }
            else{
                
                //objectT->drawSolidRect(Vec2(cellWidth, 5), Vec2(0, 0), Color4F(1.0f, 1.0f, 1.0f, 1.0f));
                auto sprite4 = CCSprite::create("wall.png");
                sprite4->setScale(cellWidth / sprite4->getContentSize().width, 5 / sprite4->getContentSize().height);
                objectT->addChild(sprite4);
                objectT->setPosition(Vec2((lroundf(finalX/cellWidth) - 0.5) * cellWidth ,  lroundf(finalY/cellHeight) * cellHeight-2.5));
                
                isWall[lroundf(finalX/cellWidth) ][lroundf(finalY/cellHeight)][0]=true;
            
            }
            

                this->addChild(objectT);

            posIndex->wall--;
            
            if(selectedNode == objectA){
                selectedNode = objectB;
            }else{
                selectedNode = objectA;
            }
            
        }
        isWallTime = !isWallTime;
    };
    
    // Add listener
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener1, this);
//    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener1->clone(), objectB);
    
    
    
    return true;
}


void HelloWorld::drawGrid(DrawNode *drawNode){
    for (int i = 1; i < MAX_COL; i++) {
    drawNode->drawLine(Vec2(i * cellWidth, 0), Vec2(i * cellWidth, size.height), Color4F(1.0f, 1.0f, 1.0f, 1.0f));
        
    drawNode->drawLine(Vec2(0, i * cellHeight), Vec2(size.width, i * cellHeight), Color4F(1.0f, 1.0f, 1.0f, 1.0f));
        
    }
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}


bool HelloWorld::isWallBlock(int xIndex,int yIndex,int x,int y){
    //考虑边际情况
    //1.四个角上 只需要两块木桩就能锁住一个游戏人物
    //2.四条边上，组成三边只有两种情况，在中间则参与四组
    //3.可以利用补集思想
    
    //以上是废话
    
    if(x==1 && isWall[xIndex][yIndex][1]){
        return true;
    }else if(y==1 && isWall[xIndex+1][yIndex][0]){
        return true;
    }else if(x==-1 && isWall[xIndex+1][yIndex][1]){
        return true;
    }else if(y==-1 && isWall[xIndex+1][yIndex+1][0]){
        return true;
    }
        
    
    return false;
}

void HelloWorld::popupLayer(bool isCharaA)
{
    // 定义一个弹出层，传入一张背景图
    PopupLayer* pl = PopupLayer::create("cell.png");
    // ContentSize 是可选的设置，可以不设置，如果设置把它当作 9 图缩放
    pl->setContentSize(CCSizeMake(400, 360));
    pl->setTitle("标题党");
    if(isCharaA)
    {
        pl->setContentText("A赢了！", 20, 50, 150);
    }
    else
    {
        pl->setContentText("B赢了！", 20, 50, 150);
    }
    // 设置回调函数，回调传回一个 CCNode 以获取 tag 判断点击的按钮
    // 这只是作为一种封装实现，如果使用 delegate 那就能够更灵活的控制参数了
    pl->setCallbackFunc(this, callfuncN_selector(HelloWorld::buttonCallback));
    // 添加按钮，设置图片，文字，tag 信息
    pl->addButton("cell.png", "cell.png", "确定", 0);
    pl->addButton("cell.png", "cell.png", "取消", 1);
    // 添加到当前层
    this->addChild(pl);
}


void HelloWorld::buttonCallback(cocos2d::CCNode *pNode){
    CCLog("button call back. tag: %d", pNode->getTag());
}






