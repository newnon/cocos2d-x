//
//  NDKHelper.cpp
//  EasyNDK-for-cocos2dx
//
//  Created by Amir Ali Jiwani on 23/02/2013.
//
//

#include "NDKHelper.h"
#include "JsonHelper.h"

#define __CALLED_METHOD__           "calling_method_name"
#define __CALLED_METHOD_PARAMS__    "calling_method_params"

vector<NDKCallbackData> NDKHelper::selectorList;

void NDKHelper::addSelector(char const *groupName, char const *name, CCObject *target, SEL_CallFuncO selector) {
    NDKHelper::selectorList.push_back(NDKCallbackData(groupName, name, target, selector));
}

void NDKHelper::removeSelectorsInGroup(char const *groupName) {
    std::vector<int> markedIndices;

    for (unsigned int i = 0; i < NDKHelper::selectorList.size(); ++i) {
        if (NDKHelper::selectorList[i].getGroup().compare(groupName) == 0) {
            markedIndices.push_back(i);
        }
    }

    for (unsigned int i = 0; i < markedIndices.size(); ++i) {
        selectorList[markedIndices[i]] = selectorList.back();
        selectorList.pop_back();
    }
}

void NDKHelper::printSelectorList() {
    for (unsigned int i = 0; i < NDKHelper::selectorList.size(); ++i) {
        string s = NDKHelper::selectorList[i].getGroup();
        s.append(NDKHelper::selectorList[i].getName());
        CCLog("%s", s.c_str());
    }
}

void NDKHelper::handleMessage(json_t *methodName, json_t *methodParams) {
    if (methodName == NULL) {
        return;
    }

    const char *methodNameStr = json_string_value(methodName);

    for (unsigned int i = 0; i < NDKHelper::selectorList.size(); ++i) {
        if (NDKHelper::selectorList[i].getName().compare(methodNameStr) == 0) {
            CCObject *dataToPass = JsonHelper::getCCObjectFromJson(methodParams);
            SEL_CallFuncO sel = NDKHelper::selectorList[i].getSelector();
            CCObject *target = NDKHelper::selectorList[i].getTarget();

            (target->* sel)(dataToPass);

            break;
        }
    }
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "../../cocos2d/cocos/platform/android/jni/JniHelper.h"
#include <android/log.h>
#include <jni.h>
    #define  LOG_TAG    "EasyNDK-for-cocos2dx"

    #define CLASS_NAME "com/easyndk/AndroidNDKHelper"
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#import "IOSNDKHelper-C-Interface.h"

#endif

extern "C"
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    // Method for recieving NDK messages from Java, Android
    void Java_com_easyndk_AndroidNDKHelper_cppNativeCallHandler(JNIEnv* env, jobject thiz, jstring json)
    {
        string jsonString = JniHelper::jstring2string(json);
        const char *jsonCharArray = jsonString.c_str();
        
        json_error_t error;
        json_t *root;
        root = json_loads(jsonCharArray, 0, &error);
        
        if (!root)
        {
            fprintf(stderr, "error: on line %d: %s\n", error.line, error.text);
            return;
        }
        
        json_t *jsonMethodName, *jsonMethodParams;
        jsonMethodName = json_object_get(root, __CALLED_METHOD__);
        jsonMethodParams = json_object_get(root, __CALLED_METHOD_PARAMS__);
        
        // Just to see on the log screen if messages are propogating properly
        __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "%s", jsonCharArray);
        
        NDKHelper::handleMessage(jsonMethodName, jsonMethodParams);
        json_decref(root);
    }
    #endif

// Method for sending message from CPP to the targeted platform
CCObject *NDKHelper::sendMessageWithParams(string methodName, CCObject *methodParams) {
    CCDictionary *retParams = CCDictionary::create();

    if (0 == strcmp(methodName.c_str(), "")) {
        return retParams;
    }

    json_t *toBeSentJson = json_object();
    json_object_set_new(toBeSentJson, __CALLED_METHOD__, json_string(methodName.c_str()));

    if (methodParams != NULL) {
        json_t *paramsJson = JsonHelper::getJsonFromCCObject(applyPlatformParams(methodParams));
        json_object_set_new(toBeSentJson, __CALLED_METHOD_PARAMS__, paramsJson);
    }

    json_t *retJsonParams = NULL;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        JniMethodInfo t;
        
		if (JniHelper::getStaticMethodInfo(t,
                                           CLASS_NAME,
                                           "receiveCppMessage",
                                           "(Ljava/lang/String;)Ljava/lang/String;")) {

            char* jsonStrLocal = json_dumps(toBeSentJson, JSON_COMPACT | JSON_ENSURE_ASCII);
            string jsonStr(jsonStrLocal);
            free(jsonStrLocal);
            
            jstring stringArg1 = t.env->NewStringUTF(jsonStr.c_str());
            jstring retString = (jstring) t.env->CallStaticObjectMethod(t.classID, t.methodID, stringArg1);

            t.env->DeleteLocalRef(stringArg1);
			t.env->DeleteLocalRef(t.classID);

		    const char *nativeString = t.env->GetStringUTFChars(retString, 0);
		    string retParamsStr(nativeString);
		    t.env->ReleaseStringUTFChars(retString, nativeString);


            const char *jsonCharArray = retParamsStr.c_str();

            json_error_t error;
            retJsonParams = json_loads(jsonCharArray, 0, &error);

            if (!retJsonParams) {
                fprintf(stderr, "error: on line %d: %s\n", error.line, error.text);
                return retParams;
            }
		}
        #elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    json_t *jsonMessageName = json_string(methodName.c_str());

    if (methodParams != NULL) {
        json_t *jsonParams = JsonHelper::getJsonFromCCObject(methodParams);
        retJsonParams = IOSNDKHelperImpl::receiveCPPMessage(jsonMessageName, jsonParams);
        json_decref(jsonParams);
    } else {
        retJsonParams = IOSNDKHelperImpl::receiveCPPMessage(jsonMessageName, NULL);
    }

    if (!retJsonParams) {
        return retParams;
    }

    json_decref(jsonMessageName);
#endif

    json_decref(toBeSentJson);
    return JsonHelper::getCCObjectFromJson(retJsonParams);
}

CCObject *NDKHelper::applyPlatformParams(CCObject *pObject) {

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    string platformPrefix = "android.";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    string platformPrefix = "ios.";
#endif

    if (dynamic_cast<CCDictionary *>(pObject) != NULL) {
        CCDictionary *pDict = dynamic_cast<CCDictionary *>(pObject);
        CCArray *keys = pDict->allKeys();
        CCObject *key;
        CCString *keyStr;
        CCARRAY_FOREACH(keys, key) {
                keyStr = (CCString *) key;
                if (keyStr->_string.find(platformPrefix) == 0)
                {
                    string newKey = keyStr->_string.substr(platformPrefix.length());
                    pDict->setObject(pDict->objectForKey(keyStr->_string), newKey);
                    pDict->removeObjectForKey(keyStr->_string);
                }
            }

        keys = pDict->allKeys();
        CCARRAY_FOREACH(keys, key) {
                keyStr = (CCString *) key;
                applyPlatformParams(pDict->objectForKey(keyStr->_string));
            }
    } else if (dynamic_cast<CCArray *>(pObject) != NULL) {
        CCArray *pArr = dynamic_cast<CCArray *>(pObject);
        CCObject *obj;
        CCARRAY_FOREACH(pArr, obj) {
                applyPlatformParams(obj);
            }
    }
    return pObject;
}

}


