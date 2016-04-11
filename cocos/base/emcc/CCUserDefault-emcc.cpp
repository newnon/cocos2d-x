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
    string encodedStr = StorageEngine_load(pKey);
    
    unsigned char * decodedData = NULL;
    int decodedDataLen = base64Decode((unsigned char*)encodedStr.c_str(), (unsigned int)encodedStr.length(), &decodedData);
    
    if (decodedData && decodedDataLen)
    {
        std::string ret((const char*)decodedData, decodedDataLen);
        bool result = static_cast<bool>(std::stoi(ret, nullptr, 10));
        return result;
    }
    
    return defaultValue;
}

int UserDefault::getIntegerForKey(const char* pKey)
{
    return getIntegerForKey(pKey, 0);
}

int UserDefault::getIntegerForKey(const char* pKey, int defaultValue)
{
    string encodedStr = StorageEngine_load(pKey);
    
    unsigned char * decodedData = NULL;
    int decodedDataLen = base64Decode((unsigned char*)encodedStr.c_str(), (unsigned int)encodedStr.length(), &decodedData);
    
    if (decodedData && decodedDataLen)
    {
        std::string ret((const char*)decodedData, decodedDataLen);
        int result = std::stoi(ret, nullptr, 10);
        return result;
    }
    
    return defaultValue;
}

float UserDefault::getFloatForKey(const char* pKey)
{
    return getFloatForKey(pKey, 0.0f);
}

float UserDefault::getFloatForKey(const char* pKey, float defaultValue)
{
    string encodedStr = StorageEngine_load(pKey);
    
    unsigned char * decodedData = NULL;
    int decodedDataLen = base64Decode((unsigned char*)encodedStr.c_str(), (unsigned int)encodedStr.length(), &decodedData);
    
    if (decodedData && decodedDataLen)
    {
        std::string ret((const char*)decodedData, decodedDataLen);
        float result = std::stof(ret);
        return result;
    }
    
    return defaultValue;
}

double  UserDefault::getDoubleForKey(const char* pKey)
{
    return getDoubleForKey(pKey, 0.0);
}

double UserDefault::getDoubleForKey(const char* pKey, double defaultValue)
{
    string encodedStr = StorageEngine_load(pKey);
    
    unsigned char * decodedData = NULL;
    int decodedDataLen = base64Decode((unsigned char*)encodedStr.c_str(), (unsigned int)encodedStr.length(), &decodedData);
    
    if (decodedData && decodedDataLen)
    {
        std::string ret((const char*)decodedData, decodedDataLen);
        double result = std::stod(ret);
        return result;
    }
    
    return defaultValue;
}

std::string UserDefault::getStringForKey(const char* pKey)
{
    return getStringForKey(pKey, "");
}

string UserDefault::getStringForKey(const char* pKey, const std::string & defaultValue)
{
    string encodedStr = StorageEngine_load(pKey);
    
    unsigned char * decodedData = NULL;
    int decodedDataLen = base64Decode((unsigned char*)encodedStr.c_str(), (unsigned int)encodedStr.length(), &decodedData);
    
    if (decodedData && decodedDataLen)
    {
        std::string ret((const char*)decodedData, decodedDataLen);
        return ret;
    }
    
    return defaultValue;
}

Data UserDefault::getDataForKey(const char* pKey)
{
    return getDataForKey(pKey, Data::Null);
}

Data UserDefault::getDataForKey(const char* pKey, const Data& defaultValue)
{
    string encodedStr = StorageEngine_load(pKey);

    unsigned char * decodedData = NULL;
    int decodedDataLen = base64Decode((unsigned char*)encodedStr.c_str(), (unsigned int)encodedStr.length(), &decodedData);

    if (decodedData && decodedDataLen)
    {
        Data ret;
        ret.fastSet(decodedData, decodedDataLen);
        return ret;
    }

    return defaultValue;
}


void UserDefault::setBoolForKey(const char* pKey, bool value)
{
    const std::string &str = std::to_string(value);

    char * encodedData = nullptr;
    unsigned int encodedDataLen = base64Encode((unsigned char*)str.c_str(), str.length(), &encodedData);
    
    StorageEngine_save(pKey, encodedData);
    
    if (encodedData)
        free(encodedData);
}

void UserDefault::setIntegerForKey(const char* pKey, int value)
{
    const std::string &str = std::to_string(value);
    char * encodedData = nullptr;
    unsigned int encodedDataLen = base64Encode((unsigned char*)str.c_str(), str.length(), &encodedData);
    
    StorageEngine_save(pKey, encodedData);
    
    if (encodedData)
        free(encodedData);
}

void UserDefault::setFloatForKey(const char* pKey, float value)
{
    const std::string &str = std::to_string(value);
    char * encodedData = nullptr;
    unsigned int encodedDataLen = base64Encode((unsigned char*)str.c_str(), str.length(), &encodedData);
    
    StorageEngine_save(pKey, encodedData);
    
    if (encodedData)
        free(encodedData);
}

void UserDefault::setDoubleForKey(const char* pKey, double value)
{
    const std::string &str = std::to_string(value);
    char * encodedData = nullptr;
    unsigned int encodedDataLen = base64Encode((unsigned char*)str.c_str(), str.length(), &encodedData);
 
    StorageEngine_save(pKey, encodedData);
    
    if (encodedData)
        free(encodedData);
}

void UserDefault::setStringForKey(const char* pKey, const std::string & value)
{
    char * encodedData = nullptr;
    unsigned int encodedDataLen = base64Encode((unsigned char*)value.c_str(), value.length(), &encodedData);
    
    StorageEngine_save(pKey, encodedData);
    
    if (encodedData)
        free(encodedData);
}

void UserDefault::setDataForKey(const char* pKey, const Data& value)
{
    char * encodedData = nullptr;
    unsigned int encodedDataLen = base64Encode(value.getBytes(), value.getSize(), &encodedData);

    StorageEngine_save(pKey, encodedData);

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

void UserDefault::deleteValueForKey(const char* pKey)
{
    // check the params
    if (!pKey)
    {
        CCLOG("the key is invalid");
    }
    
    StorageEngine_remove(pKey);
}
NS_CC_END

#endif // (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
