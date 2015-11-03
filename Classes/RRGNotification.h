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

#include "RRGNotification_Private.h"

#endif /* defined(__RRGNotification__RRGNotification__) */
