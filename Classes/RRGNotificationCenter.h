//
//  RRGNotificationCenter.h
//  RRGNotification
//
//  Created by 山本政徳 on 2015/11/02.
//
//

#ifndef __RRGNotification__RRGNotificationCenter__
#define __RRGNotification__RRGNotificationCenter__

#include "cocos2d.h"

#define sharedNotificationCenter RRGNotificationCenter::getInstance()

class RRGNotification : public cocos2d::Ref
{
protected:
    std::string _name;
    cocos2d::Ref* _sender;
    
    std::unordered_map<std::string, cocos2d::Vec2> _vec2Map;
    std::unordered_map<std::string, cocos2d::Size> _sizeMap;
    std::unordered_map<std::string, cocos2d::Rect> _rectMap;
    cocos2d::ValueMap _valueMap;
    cocos2d::Map<std::string, cocos2d::Ref*> _objectMap;
public:
    const std::string& getName(){return _name;}
    cocos2d::Ref* getSender(){return _sender;}
    
    RRGNotification();
    virtual ~RRGNotification();
    static RRGNotification* create(const std::string& name, Ref* sender);
    bool init(const std::string& name, Ref* sender);
    
    template <typename T>
    inline void addValue(T value, const std::string& key);
    
    template <typename T>
    inline T getValue(const std::string& key);
};

class RRGNotificationCenter : public cocos2d::Ref
{
public:
    typedef std::function<void (RRGNotification*)> RRGNotificationCallback;
private:
    typedef std::unordered_map<Ref*, RRGNotificationCallback> ObserverToCallbackMap;
    typedef std::unordered_map<std::string, ObserverToCallbackMap> NameKeyMap;
    typedef std::unordered_map<Ref*, NameKeyMap> SenderKeyMap;
    
    SenderKeyMap _notificationMap;
    NameKeyMap _noSenderMap;
    SenderKeyMap _KVOMap;
    
public:
    RRGNotificationCenter();
    ~RRGNotificationCenter();
    
    static RRGNotificationCenter* getInstance();
    static void destroyInstance();
    
    void addObserver(Ref* observer,
                     const std::string& name,
                     Ref* sender,
                     const RRGNotificationCallback& callback);
    void removeObserver(Ref* observer,
                        const std::string& name = "",
                        Ref* sender = nullptr);
    void postNotification(RRGNotification* notification);
    void postNotification(const std::string& name,
                          Ref* sender);
    
#pragma mark - key value observing
    
    template <typename T>
    inline void addKeyValueObserver(Ref* observer,
                                    const std::string& key,
                                    Ref* sender,
                                    const std::function<void(T oldVal, T newVal)>& callback);
    
    template <typename T>
    inline void postDidChangeValueNotification(const std::string& key,
                                               cocos2d::Ref* sender,
                                               T oldVal,
                                               T newVal);
};

#include "RRGNotificationCenter_Private.h"

#endif /* defined(__RRGNotification__RRGNotificationCenter__) */