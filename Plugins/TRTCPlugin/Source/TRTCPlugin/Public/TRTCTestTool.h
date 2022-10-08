// Copyright (c) 2022 Tencent. All rights reserved.

#pragma once

#include <string>

namespace liteav {
namespace ue {

//
// The correct UserSig distribution method is to integrate the calculation code of UserSig into your server and provide
// an application-oriented API. When UserSig is needed, your application can send a request to the business server for a
// dynamic UserSig. For more information, please see [How do I calculate UserSig on the
// server?](https://www.tencentcloud.com/document/product/647/35166#Server).
//
class TRTCPLUGIN_API TestUserSigGenerator {
 public:
  static std::string gen(const char* identifier, int sdkAppId, const char* secretKey);
};

}  // namespace ue
}  // namespace liteav
