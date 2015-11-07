//
//  RRGNotification_Private.h
//  RRGNotification
//
//  Created by 山本政徳 on 2015/11/03.
//
//

#ifndef __RRGNotification__RRGNotification_Private__
#define __RRGNotification__RRGNotification_Private__

#include "RRGNotification.h"

#pragma mark - notification

template <>
inline void RRGNotification::addValue<cocos2d::Vec2,nullptr>
(cocos2d::Vec2 value, const std::string& key)
{
    //CCLOG("%s", __PRETTY_FUNCTION__);
    _vec2Map[key] = value;
}
template <>
inline void RRGNotification::addValue<cocos2d::Size,nullptr>
(cocos2d::Size value, const std::string& key)
{
    //CCLOG("%s", __PRETTY_FUNCTION__);
    _sizeMap[key] = value;
}
template <>
inline void RRGNotification::addValue<cocos2d::Rect,nullptr>
(cocos2d::Rect value, const std::string& key)
{
    //CCLOG("%s", __PRETTY_FUNCTION__);
    _rectMap[key] = value;
}

#pragma mark - get value

template <>
inline int RRGNotification::getValue<int,nullptr>(const std::string& key)
{
    if (_valueMap.find(key) == _valueMap.end()) {
        return 0;
    } else {
        return _valueMap.at(key).asInt();
    }
}
template <>
inline float RRGNotification::getValue(const std::string& key)
{
    if (_valueMap.find(key) == _valueMap.end()) {
        return 0.0f;
    } else {
        return _valueMap.at(key).asFloat();
    }
}
template <>
inline double RRGNotification::getValue(const std::string& key)
{
    if (_valueMap.find(key) == _valueMap.end()) {
        return 0.0;
    } else {
        return _valueMap.at(key).asDouble();
    }
}
template <>
inline std::string RRGNotification::getValue(const std::string& key)
{
    if (_valueMap.find(key) == _valueMap.end()) {
        return "";
    } else {
        return _valueMap.at(key).asString();
    }
}
template <>
inline bool RRGNotification::getValue(const std::string& key)
{
    if (_valueMap.find(key) == _valueMap.end()) {
        return false;
    } else {
        return _valueMap.at(key).asBool();
    }
}
template <>
inline cocos2d::Vec2 RRGNotification::getValue(const std::string& key)
{
    if (_vec2Map.find(key) == _vec2Map.end()) {
        return cocos2d::Vec2::ZERO;
    } else {
        return _vec2Map.at(key);
    }
}
template <>
inline cocos2d::Size RRGNotification::getValue(const std::string& key)
{
    if (_sizeMap.find(key) == _sizeMap.end()) {
        return cocos2d::Size::ZERO;
    } else {
        return _sizeMap.at(key);
    }
}
template <>
inline cocos2d::Rect RRGNotification::getValue(const std::string& key)
{
    if (_rectMap.find(key) == _rectMap.end()) {
        return cocos2d::Rect::ZERO;
    } else {
        return _rectMap.at(key);
    }
}

#pragma mark - notification center

namespace {
    const char* kNotificationOldKey = "__old__";
    const char* kNotificationNewKey = "__new__";
}

template <typename T>
inline void RRGNotificationCenter::addKeyValueObserver(Ref* observer,
                                                       const std::string& key,
                                                       Ref* sender,
                                                       const std::function<void(Ref* sender, T oldVal, T newVal)>& callback)
{
    if (_KVOMap.find(sender) == _KVOMap.end()) {
        NameKeyMap nameKeyMap;
        _KVOMap.insert(std::make_pair(sender, nameKeyMap));
    }
    NameKeyMap& nameKeyMap = _KVOMap.at(sender);
    if (nameKeyMap.find(key) == nameKeyMap.end()) {
        ObserverToCallbackMap observerToCallbackMap;
        nameKeyMap.insert(std::make_pair(key, observerToCallbackMap));
    }
    ObserverToCallbackMap& observerToCallbackMap = nameKeyMap.at(key);
    auto callback2 = 
    [callback](RRGNotification* note){
        T oldVal = note->getValue<T>(kNotificationOldKey);
        T newVal = note->getValue<T>(kNotificationNewKey);
        callback(note->getSender(), oldVal, newVal);
    };
    observerToCallbackMap.insert(std::make_pair(observer, callback2));
}

template <typename T>
inline void RRGNotificationCenter::postDidChangeValueNotification(const std::string& key,
                                                                  cocos2d::Ref* sender,
                                                                  T oldVal,
                                                                  T newVal)
{
    RRGNotification* notification = RRGNotification::create(key, sender);
    notification->addValue(oldVal, kNotificationOldKey);
    notification->addValue(newVal, kNotificationNewKey);
    
    auto it1 = _KVOMap.find(sender);
    if (it1 != _KVOMap.end()) {
        NameKeyMap& nameKeyMap = it1->second;
        auto it2 = nameKeyMap.find(key);
        if (it2 != nameKeyMap.end()) {
            ObserverToCallbackMap& observerToCallbackMap = it2->second;
            for (auto it3 = observerToCallbackMap.begin();
                 it3 != observerToCallbackMap.end();
                 ++it3)
            {
                it3->second(notification);
            }
        }
    }
}

#endif /* defined(__RRGNotification__RRGNotification_Private__) */