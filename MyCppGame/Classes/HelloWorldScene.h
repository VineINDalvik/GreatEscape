#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    virtual void update(float delta);
    
    void drawGrid(cocos2d::DrawNode *drawNode);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
    
    void moveUp(cocos2d::Node *drawNode);
    void moveRight(cocos2d::Node *drawNode);
    void moveDown(cocos2d::Node *drawNode);
    void moveLeft(cocos2d::Node *drawNode);
    
};

#endif // __HELLOWORLD_SCENE_H__
