//
//  RRGNotification_Private.h
//  RRGNotification
//
//  Created by 山本政徳 on 2015/11/02.
//
//

#ifndef __RRGNotification__RRGNotification_Private__
#define __RRGNotification__RRGNotification_Private__

#include "RRGNotification.h"

template <typename T>
inline void RRGNotification::addValue(T value, const std::string& key)
{
    _valueMap[key] = value;
}
template <>
inline void RRGNotification::addValue(const cocos2d::Vec2& value, const std::string& key)
{
    _vec2Map[key] = value;
}
template <>
inline void RRGNotification::addValue(const cocos2d::Size& value, const std::string& key)
{
    _sizeMap[key] = value;
}
template <>
inline void RRGNotification::addValue(const cocos2d::Rect& value, const std::string& key)
{
    _rectMap[key] = value;
}
template <>
inline void RRGNotification::addValue(cocos2d::Ref* value, const std::string& key)
{
    _objectMap.insert(key, value);
}

#pragma mark - get value

template <typename T>
inline T RRGNotification::getValue(const std::string& key)
{
    return _objectMap.at(key);
}
template <>
inline int RRGNotification::getValue(const std::string& key)
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

#endif /* defined(__RRGNotification__RRGNotification_Private__) */
