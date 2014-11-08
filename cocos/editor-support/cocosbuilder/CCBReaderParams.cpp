#include "CCBReaderParams.h"
#include "platform/CCFileUtils.h"
#include "CCApplication.h"
#include "base/CCDirector.h"
#include "2d/CCSpriteFrameCache.h"
#include <unordered_set>

using namespace cocos2d;

namespace cocosbuilder {

CCBReaderParams::CCBReaderParams():_loaded(false),_curentLanguage(Application::getInstance()->getCurrentLanguageCode()),_curLanguageMap(nullptr) {
}

CCBReaderParams::~CCBReaderParams() {
}
    
bool CCBReaderParams::initWithPath(const std::string &path)
{
    _loaded = loadParams(path);
    loadLocalization(path);
    FileUtils::getInstance()->loadFilenameLookupDictionaryFromFile(path+"fileLookup.plist");
    SpriteFrameCache::getInstance()->loadSpriteFrameLookupDictionaryFromFile(path+"spriteFrameFileList.plist");
    return true;
}
    
bool CCBReaderParams::isLoaded() const
{
    return _loaded;
}
    
bool CCBReaderParams::loadParams(const std::string &path)
{
    std::string fullConfigPath = FileUtils::getInstance()->fullPathForFilename(path+"configCocos2d.plist");
    ValueMap configDict = FileUtils::getInstance()->getValueMapFromFile(fullConfigPath);
    
    if(configDict.empty())
        return false;
    
    auto scale = configDict.find("CCSetupDesignResourceScale");
    if (scale != configDict.end())
        _designResolutionscale = scale->second.asDouble();
    else
        _designResolutionscale = 1.0f;
    
    auto height = configDict.find("CCSetupDesignSizeHeight");
    if (height != configDict.end())
        _designResolution.height = height->second.asInt();
    else
        _designResolution.height = Director::getInstance()->getWinSize().height;
    
    auto width = configDict.find("CCSetupDesignSizeWidth");
    if (width != configDict.end())
        _designResolution.width = width->second.asInt();
    else
        _designResolution.width = Director::getInstance()->getWinSize().width;
    
    return true;
}
    

bool CCBReaderParams::loadLocalization(const std::string &path)
{
    std::string fullTranslationPath = FileUtils::getInstance()->fullPathForFilename(path+"Strings.ccbLang");
    ValueMap translationDict = FileUtils::getInstance()->getValueMapFromFile(fullTranslationPath);
    
    auto fileVersion = translationDict.find("fileVersion");
    if(fileVersion ==translationDict.end() )
        return false;
    if(fileVersion->second.asInt() != 1)
        return false;
    auto activeLanguages = translationDict.find("activeLanguages");
    if (activeLanguages != translationDict.end()) {
        std::vector<std::string> languages;
        for(const auto &language : activeLanguages->second.asValueVector())
            languages.push_back(language.asString());
        _defaultLanguage = *languages.begin();
        if(std::find(languages.begin(), languages.end(), _curentLanguage) == languages.end())
            _curentLanguage = *languages.begin();
        for(auto it = languages.begin(); it != languages.end();++it)
        {
            _languages.emplace(*it,std::map<std::string,std::string>());
        }
    }
    auto translations = translationDict.find("translations");
    if (translations != translationDict.end()) {
        const ValueVector &valueVector = translations->second.asValueVector();
        for(const auto &value: valueVector)
        {
            const ValueMap &valueMap = value.asValueMap();
            auto key = valueMap.find("key");
            if(key!= valueMap.end())
            {
                auto strings = valueMap.find("translations");
                for(const auto &localizedString : strings->second.asValueMap())
                {
                    auto it = _languages.find(localizedString.first);
                    if(it != _languages.end())
                    {
                        it->second[key->second.asString()]  = localizedString.second.asString();
                    }
                }
            }
        }
    }
    _curLanguageMap = &_languages[_curentLanguage];
    return true;
}
    
void CCBReaderParams::setLanguage(const std::string &language)
{
    auto it = _languages.find(language);
    if(it == _languages.end())
    {
        _curentLanguage = language;
        _curLanguageMap = &it->second;
    }
    else
    {
        _curentLanguage = _defaultLanguage;
        _curLanguageMap = &_languages[_defaultLanguage];
    }
}
const std::string &CCBReaderParams::getLanguage() const
{
    return _curentLanguage;
}
    
const std::string &CCBReaderParams::getLocalizedString(const std::string &key) const
{
    if(!_curLanguageMap)
        return key;
    auto recordIt = _curLanguageMap->find(key);
    if(recordIt == _curLanguageMap->end())
        return key;
    return recordIt->second;
}
    
static CCBReaderParams * sSharedReaderParams = NULL;

CCBReaderParams * CCBReaderParams::getInstance()
{
    if(sSharedReaderParams == NULL)
    {
        sSharedReaderParams = new CCBReaderParams();
        sSharedReaderParams->initWithPath();
    }
    return sSharedReaderParams;
}
    
void CCBReaderParams::destroyInstance()
{
    CC_SAFE_DELETE(sSharedReaderParams);
}
    
}