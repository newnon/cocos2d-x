#ifndef _CCB_CCBREADERPARAMS_H_
#define _CCB_CCBREADERPARAMS_H_

#include "CCBReader.h"

namespace cocosbuilder {

class CCBReaderParams : public cocos2d::Ref
{
public:
    
    /**
     * @js NA
     * @lua NA
     */
    virtual ~CCBReaderParams();
    
    /**
     * @js NA
     * @lua NA
     */
    static CCBReaderParams * getInstance();
    /**
     * @js NA
     * @lua NA
     */
    static void destroyInstance();
    
    /**
     * @js NA
     * @lua NA
     */
    bool initWithPath(const std::string &path = "");
    
    /**
     * @js NA
     * @lua NA
     */
    bool loadParams(const std::string &path = "");
    
    /**
     * @js NA
     * @lua NA
     */
    bool loadLocalization(const std::string &path = "");
    
    /**
     * @js NA
     * @lua NA
     */
    const cocos2d::Size &getDesignResolution() const { return _designResolution; }
    const float &getDesignResolutionScale() const { return _designResolutionscale; }
    
    void setLanguage(const std::string &language);
    const std::string &getLanguage() const;
    const std::string &getLocalizedString(const std::string &key) const;
    
    bool isLoaded() const;
    
protected:
    
    /**
     * @js NA
     * @lua NA
     */
    CCBReaderParams();
    
private:
    cocos2d::Size _designResolution;
    float _designResolutionscale;
    std::string _curentLanguage;
    std::string _defaultLanguage;
    std::unordered_map<std::string,std::map<std::string,std::string>> _languages;
    std::map<std::string,std::string> *_curLanguageMap;
    bool _loaded;
};

}

#endif
