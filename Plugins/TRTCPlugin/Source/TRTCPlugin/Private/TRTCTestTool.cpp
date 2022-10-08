// Copyright (c) 2022 Tencent. All rights reserved.

#include "TRTCTestTool.h"

#include <UserSig/include/GenerateTestUserSig.h>

#if PLATFORM_ANDROID
#include "Runtime/ApplicationCore/Public/Android/AndroidApplication.h"
#include "Runtime/Core/Public/Android/AndroidJavaEnv.h"
#include "Runtime/Launch/Public/Android/AndroidJNI.h"
#endif

std::string liteav::ue::TestUserSigGenerator::gen(const char* identifier, int sdkAppId, const char* secretKey) {
#if PLATFORM_ANDROID
  if (JNIEnv* Env = FAndroidApplication::GetJavaEnv()) {
    jmethodID GetPackageNameMethodID =
        FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "genTestUserSig",
                                 "(ILjava/lang/String;Ljava/lang/String;)Ljava/lang/String;", false);
    jstring jsUserId = Env->NewStringUTF(identifier);
    jstring jsKey = Env->NewStringUTF(secretKey);
    jstring JstringResult = (jstring)FJavaWrapper::CallObjectMethod(Env, FJavaWrapper::GameActivityThis,
                                                                    GetPackageNameMethodID, sdkAppId, jsUserId, jsKey);
    FString FinalResult = FJavaHelper::FStringFromLocalRef(Env, JstringResult);
    auto twoHundredAnsi = StringCast<ANSICHAR>(*FinalResult);
    const char* userSig = twoHundredAnsi.Get();
    return userSig;
  } else {
    return "";
  }
#else
  return GenerateTestUserSig::genTestUserSig(identifier, sdkAppId, secretKey);
#endif
}
