//
//  RRGNotification.h
//  RRGNotification
//
//  Created by 山本政徳 on 2015/11/02.
//
//

#ifndef __RRGNotification__RRGNotification__
#define __RRGNotification__RRGNotification__

#include "cocos2d.h"

#define sharedNotificationCenter RRGNotificationCenter::getInstance()

#define DEFINE_SETTER(varType, pvarName, setterName)\
public: virtual void setterName(varType var)\
{\
    varType oldVal = pvarName;\
    pvarName = var;\
    varType newVal = pvarName;\
    sharedNotificationCenter->postDidChangeValueNotification(#pvarName,this,oldVal,newVal);\
}

#define DEFINE_SETTER_CONST_REF(varType, pvarName, setterName)\
public: virtual void setterName(const varType& var)\
{\
    varType oldVal = pvarName;\
    pvarName = var;\
    varType newVal = pvarName;\
    sharedNotificationCenter->postDidChangeValueNotification(#pvarName,this,oldVal,newVal);\
}

#define DEFINE_SETTER_RETAIN(varType, pvarName, setterName)\
public: virtual void setterName(varType var)\
{ \
    if (pvarName != var) \
    { \
        varType oldVal = pvarName;\
        CC_SAFE_RETAIN(oldVal);\
        \
        varType newVal = var;\
        CC_SAFE_RETAIN(newVal);\
        \
        CC_SAFE_RETAIN(var); \
        CC_SAFE_RELEASE(pvarName); \
        pvarName = var;\
        sharedNotificationCenter->postDidChangeValueNotification<cocos2d::Ref*>(#pvarName,this,oldVal,newVal);\
        \
        CC_SAFE_RELEASE(oldVal);\
        CC_SAFE_RELEASE(newVal);\
    } \
}

extern const char* kNotificationOldKey;
extern const char* kNotificationNewKey;

class RRGNotificationCenter;
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
    
    template <typename T,
    typename std::enable_if<!std::is_convertible<T, cocos2d::Ref*>::value,
    std::nullptr_t>::type = nullptr>
    inline void addValue(T value, const std::string& key)
    {
        //CCLOG("%s", __PRETTY_FUNCTION__);
        _valueMap[key] = value;
    }

    void addValue(cocos2d::Ref* value, const std::string& key)
    {
        //CCLOG("%s", __PRETTY_FUNCTION__);
        if (value) {
            _objectMap.insert(key, value);
        }
    }
    
    template <typename T,
    typename std::enable_if<!std::is_convertible<T, cocos2d::Ref*>::value,
    std::nullptr_t>::type = nullptr>
    inline T getValue(const std::string& key)
    {
        //CCLOG("%s", __PRETTY_FUNCTION__);
        return 0;
    }
    template <typename T,
    typename std::enable_if<std::is_convertible<T, cocos2d::Ref*>::value,
    std::nullptr_t>::type = nullptr>
    inline T getValue(const std::string& key)
    {
        //CCLOG("%s", __PRETTY_FUNCTION__);
        return dynamic_cast<T>(_objectMap.at(key));
    }
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
                                    const std::function<void(Ref* sender, T oldVal, T newVal)>& callback);
    template <typename T>
    inline void postDidChangeValueNotification(const std::string& key,
                                               cocos2d::Ref* sender,
                                               T oldVal,
                                               T newVal);
};

#include "RRGNotification_Private.h"

#endif /* defined(__RRGNotification__RRGNotification__) */