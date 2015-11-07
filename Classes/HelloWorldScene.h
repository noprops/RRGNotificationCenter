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
    cocos2d::Size _size;
    cocos2d::Rect _rect;
    int _HP;
    std::string _displayName;
    Node* _node;
public:
    const cocos2d::Vec2& getTileCoord(){return _tileCoord;}
    DEFINE_SETTER_CONST_REF(cocos2d::Vec2, _tileCoord, setTileCoord);
    
    const cocos2d::Size& getSize(){return _size;}
    DEFINE_SETTER_CONST_REF(cocos2d::Size, _size, setSize);
    
    const cocos2d::Rect& getRect(){return _rect;}
    DEFINE_SETTER_CONST_REF(cocos2d::Rect, _rect, setRect);
    
    int getHP(){return _HP;}
    DEFINE_SETTER(int, _HP, setHP);
    
    const std::string& getDisplayName(){return _displayName;}
    DEFINE_SETTER_CONST_REF(std::string, _displayName, setDisplayName);
    
    Node* getNode(){return _node;}
    DEFINE_SETTER_RETAIN(Node*, _node, setNode);
    
    RRGLevelObject():_tileCoord(cocos2d::Vec2::ZERO),_HP(0),_displayName(""),_node(nullptr){}
    ~RRGLevelObject(){CC_SAFE_RELEASE_NULL(_node);}
    
    CREATE_FUNC(RRGLevelObject);
};

#endif // __HELLOWORLD_SCENE_H__
