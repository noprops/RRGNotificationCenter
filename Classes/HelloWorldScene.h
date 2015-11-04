#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "RRGNotification.h"

class RRGLevelObject;
class HelloWorld : public cocos2d::Layer
{
private:
    RRGLevelObject* _levelObject;
public:
    HelloWorld();
    ~HelloWorld();
    
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    void item1Callback(cocos2d::Ref* sender);
    void item2Callback(cocos2d::Ref* sender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
};

class RRGLevelObject : public cocos2d::Node
{
private:
    cocos2d::Vec2 _tileCoord;
    int _HP;
    std::string _displayName;
public:
    const cocos2d::Vec2& getTileCoord(){return _tileCoord;}
    DEFINE_SETTER_CONST_REF(cocos2d::Vec2, _tileCoord, setTileCoord);
    int getHP(){return _HP;}
    DEFINE_SETTER(int, _HP, setHP);
    const std::string& getDisplayName(){return _displayName;}
    DEFINE_SETTER_CONST_REF(std::string, _displayName, setDisplayName);
    
    RRGLevelObject():_tileCoord(cocos2d::Vec2::ZERO),_HP(0),_displayName(""){}
    ~RRGLevelObject(){}
    
    CREATE_FUNC(RRGLevelObject);
};

#endif // __HELLOWORLD_SCENE_H__
