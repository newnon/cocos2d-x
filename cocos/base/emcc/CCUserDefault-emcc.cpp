/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2013-2014 Chukong Technologies Inc.

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
#include "base/CCUserDefault.h"
#include "platform/CCPlatformConfig.h"
#include "base/ccUtils.h"
#include "platform/CCCommon.h"
#include "base/base64.h"
#include "base/CCDirector.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_EMSCRIPTEN)
#include "platform/CCFileUtils.h"

using namespace std;

extern "C" {
    void StorageEngine_save(const char *, const char *);
    const char *StorageEngine_load(const char *);
    void StorageEngine_remove(const char *);
};


NS_CC_BEGIN

/**
 * implements of UserDefault
 */

UserDefault* UserDefault::_userDefault = nullptr;
string UserDefault::_filePath = string("");
bool UserDefault::_isFilePathInitialized = false;
std::string UserDefault::_emccPrefix = "";

UserDefault::~UserDefault()
{
}

UserDefault::UserDefault()
{
}

// FIXME:: deprecated
void UserDefault::purgeSharedUserDefault()
{
    UserDefault::destroyInstance();
}

void UserDefault::destroyInstance()
{
    CC_SAFE_DELETE(_userDefault);
}

bool UserDefault::getBoolForKey(const char* pKey)
{
    return getBoolForKey(pKey, false);
}

bool UserDefault::getBoolForKey(const char* pKey, bool defaultValue)
{
    const std::string &key = _emccPrefix + "_" + pKey;
    const char *ret = StorageEngine_load(key.c_str());
    return !ret ? defaultValue : static_cast<bool>(std::stoi(ret, nullptr, 10));
}

int UserDefault::getIntegerForKey(const char* pKey)
{
    return getIntegerForKey(pKey, 0);
}

int UserDefault::getIntegerForKey(const char* pKey, int defaultValue)
{
    const std::string &key = _emccPrefix + "_" + pKey;
    const char *ret = StorageEngine_load(key.c_str());
    return !ret ? defaultValue : std::stoi(ret, nullptr, 10);
}

float UserDefault::getFloatForKey(const char* pKey)
{
    return getFloatForKey(pKey, 0.0f);
}

float UserDefault::getFloatForKey(const char* pKey, float defaultValue)
{
    const std::string &key = _emccPrefix + "_" + pKey;
    const char *ret = StorageEngine_load(key.c_str());
    return !ret ? defaultValue : std::stof(ret);
}

double  UserDefault::getDoubleForKey(const char* pKey)
{
    return getDoubleForKey(pKey, 0.0);
}

double UserDefault::getDoubleForKey(const char* pKey, double defaultValue)
{
    const std::string &key = _emccPrefix + "_" + pKey;
    const char *ret = StorageEngine_load(key.c_str());
    return !ret ? defaultValue : std::stod(ret);
}

std::string UserDefault::getStringForKey(const char* pKey)
{
    return getStringForKey(pKey, "");
}

string UserDefault::getStringForKey(const char* pKey, const std::string & defaultValue)
{
    const std::string &key = _emccPrefix + "_" + pKey;
    const char *ret = StorageEngine_load(key.c_str());
    return !ret ? defaultValue : ret;
}

Data UserDefault::getDataForKey(const char* pKey)
{
    return getDataForKey(pKey, Data::Null);
}

Data UserDefault::getDataForKey(const char* pKey, const Data& defaultValue)
{
    const std::string &key = _emccPrefix + "_" + pKey;
    const char *encodedStr = StorageEngine_load(key.c_str());

    if (encodedStr)
    {
        unsigned char * decodedData = NULL;
        size_t length = strlen(encodedStr);
        int decodedDataLen = base64Decode((unsigned char*)encodedStr, (unsigned int)length, &decodedData);

        if (decodedData && decodedDataLen)
        {
            Data ret;
            ret.fastSet(decodedData, decodedDataLen);
            return ret;
        }
    }

    return defaultValue;
}


void UserDefault::setBoolForKey(const char* pKey, bool value)
{
    const std::string &key = _emccPrefix + "_" + pKey;
    const std::string &str = std::to_string(value);
    StorageEngine_save(key.c_str(), str.c_str());
}

void UserDefault::setIntegerForKey(const char* pKey, int value)
{
    const std::string &key = _emccPrefix + "_" + pKey;
    const std::string &str = std::to_string(value);
    StorageEngine_save(key.c_str(), str.c_str());
}

void UserDefault::setFloatForKey(const char* pKey, float value)
{
    const std::string &key = _emccPrefix + "_" + pKey;
    const std::string &str = std::to_string(value);
    StorageEngine_save(key.c_str(), str.c_str());
}

void UserDefault::setDoubleForKey(const char* pKey, double value)
{
    const std::string &key = _emccPrefix + "_" + pKey;
    const std::string &str = std::to_string(value);
    StorageEngine_save(key.c_str(), str.c_str());
}

void UserDefault::setStringForKey(const char* pKey, const std::string & value)
{
    const std::string &key = _emccPrefix + "_" + pKey;
    StorageEngine_save(key.c_str(), value.c_str());
}

void UserDefault::setDataForKey(const char* pKey, const Data& value)
{
    char * encodedData = nullptr;
    unsigned int encodedDataLen = base64Encode(value.getBytes(), value.getSize(), &encodedData);

    const std::string &key = _emccPrefix + "_" + pKey;
    StorageEngine_save(key.c_str(), encodedData);

    if (encodedData)
        free(encodedData);
}

// FIXME:: deprecated
UserDefault* UserDefault::sharedUserDefault()
{
    return UserDefault::getInstance();
}

UserDefault* UserDefault::getInstance()
{
    if (! _userDefault)
    {
        _userDefault = new (std::nothrow) UserDefault();
    }

    return _userDefault;
}

bool UserDefault::isXMLFileExist()
{
    return true;
}

void UserDefault::initXMLFilePath()
{
}

// create new xml file
bool UserDefault::createXMLFile()
{
    return false;
}

const string& UserDefault::getXMLFilePath()
{
    return _filePath;
}

void UserDefault::flush()
{
}

void UserDefault::setPrefixForEmcc(const std::string &prefix)
{
    _emccPrefix = prefix;
}

void UserDefault::deleteValueForKey(const char* pKey)
{
    // check the params
    if (!pKey)
    {
        CCLOG("the key is invalid");
    }
    
    const std::string &key = _emccPrefix + "_" + pKey;
    StorageEngine_remove(key.c_str());
}
NS_CC_END

#endif // (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
