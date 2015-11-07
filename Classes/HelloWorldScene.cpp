#include "HelloWorldScene.h"

namespace {
    const char* kTestNotification = "test";
    const char* kTileCoord = "_tileCoord";
    const char* kSize = "_size";
    const char* kRect = "_rect";
    const char* kHP = "_HP";
    const char* kDisplayName = "_displayName";
    const char* kNode = "_node";
}

using namespace std;
USING_NS_CC;

HelloWorld::HelloWorld()
:_levelObject(nullptr)
{
    
};
HelloWorld::~HelloWorld()
{
    sharedNotificationCenter->removeObserver(this);
    CC_SAFE_RELEASE_NULL(_levelObject);
};

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

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.
    
    // add a "close" icon to exit the progress. it's an autorelease object
    auto item1 = MenuItemImage::create("CloseNormal.png",
                                       "CloseSelected.png",
                                       CC_CALLBACK_1(HelloWorld::item1Callback, this));
    
    item1->setPosition(origin + Vec2(visibleSize.width - item1->getContentSize().width / 2,
                                     item1->getContentSize().height / 2));
    
    auto item2 = MenuItemImage::create("CloseNormal.png",
                                       "CloseSelected.png",
                                       CC_CALLBACK_1(HelloWorld::item2Callback, this));
    
    item2->setPosition(item1->getPosition() + Vec2(0,item2->getContentSize().height));

    // create menu, it's an autorelease object
    auto menu = Menu::create(item1, item2, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
    
    _levelObject = RRGLevelObject::create();
    CC_SAFE_RETAIN(_levelObject);
    
    sharedNotificationCenter->addObserver(this,
                                          kTestNotification,
                                          nullptr,
                                          [](RRGNotification* note){
                                              CCLOG("%s", note->getName().c_str());
                                          });
    sharedNotificationCenter->addKeyValueObserver<Vec2>
    (this,
     kTileCoord,
     _levelObject,
     [](Ref* sender, const Vec2& oldVal, const Vec2& newVal){
         CCLOG("old = {%.0f,%.0f} new = {%.0f,%.0f}",
               oldVal.x,oldVal.y,newVal.x,newVal.y);
     });
    sharedNotificationCenter->addKeyValueObserver<Size>
    (this,
     kSize,
     _levelObject,
     [](Ref* sender, const Size& oldVal, const Size& newVal){
         CCLOG("old = {%.0f,%.0f} new = {%.0f,%.0f}",
               oldVal.width,oldVal.height,newVal.width,newVal.height);
     });
    sharedNotificationCenter->addKeyValueObserver<Rect>
    (this,
     kRect,
     _levelObject,
     [](Ref* sender, const Rect& oldVal, const Rect& newVal){
         CCLOG("old = {{%.0f,%.0f}{%.0f,%.0f}} new = {{%.0f,%.0f}{%.0f,%.0f}}",
               oldVal.origin.x,oldVal.origin.y,oldVal.size.width,oldVal.size.height,
               newVal.origin.x,newVal.origin.y,newVal.size.width,newVal.size.height);
     });
    sharedNotificationCenter->addKeyValueObserver<int>
    (this,
     kHP,
     _levelObject,
     [](Ref* sender, const int& oldVal, const int& newVal){
         CCLOG("old = %d new = %d",oldVal,newVal);
     });
    
    sharedNotificationCenter->addKeyValueObserver<std::string>
    (this,
     kDisplayName,
     _levelObject,
     [](Ref* sender, const std::string& oldVal, const std::string& newVal){
         CCLOG("old = %s new = %s",oldVal.c_str(),newVal.c_str());
     });
    
    sharedNotificationCenter->addKeyValueObserver<Node*>
    (this,
     kNode,
     _levelObject,
     [](Ref* sender, Node* oldVal, Node* newVal){
         string oldStr = (oldVal)?oldVal->getDescription():"null";
         string newStr = (newVal)?newVal->getDescription():"null";
         CCLOG("old = %s new = %s",oldStr.c_str(),newStr.c_str());
     });
    
    return true;
}


void HelloWorld::item1Callback(Ref* sender)
{
    CCLOG("post test notification");
    sharedNotificationCenter->postNotification(kTestNotification, this);
}

void HelloWorld::item2Callback(Ref* sender)
{
    CCLOG("********************************\nchange variables of levelobject\n********************************");
    
    static Vec2 tileCoord = Vec2::ZERO;
    static Size size = Size::ZERO;
    static Rect rect = Rect::ZERO;
    static int HP = 0;
    static std::string name = "";
    
    tileCoord += Vec2(1,1);
    size = Size(size.width + 1, size.height + 1);
    rect = Rect(rect.origin.x + 1, rect.origin.y + 1, rect.size.width + 1, rect.size.height + 1);
    HP += 1;
    name += "a";
    
    _levelObject->setTileCoord(tileCoord);
    _levelObject->setSize(size);
    _levelObject->setRect(rect);
    _levelObject->setHP(HP);
    _levelObject->setDisplayName(name);
    _levelObject->setNode((_levelObject->getNode())?nullptr:Node::create());
}