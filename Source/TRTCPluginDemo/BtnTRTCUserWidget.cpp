//  Copyright (c) 2022 Tencent All rights reserved.

#include "BtnTRTCUserWidget.h"

#include <string>

#include "Engine/Engine.h"

// Debug Only
#include "DebugDefs.h"
#include "TRTCTestTool.h"

void UBtnTRTCUserWidget::NativeConstruct() {
  Super::NativeConstruct();
#if PLATFORM_ANDROID
  if (JNIEnv* Env = FAndroidApplication::GetJavaEnv()) {
    void* activity = (void*)FAndroidApplication::GetGameActivityThis();
    pTRTCCloud = liteav::ue::TRTCCloud::getSharedInstance(activity);
  }
#else
  pTRTCCloud = liteav::ue::TRTCCloud::getSharedInstance();
#endif
  pTRTCCloud->addCallback(this);
  std::string version = pTRTCCloud->getSDKVersion();
  BtnEnterRoom->OnClicked.AddDynamic(this, &UBtnTRTCUserWidget::OnEnterRoom_Click);
  BtnExitRoom->OnClicked.AddDynamic(this, &UBtnTRTCUserWidget::OnExitRoom_Click);

  BtnStartLocalPreview->OnClicked.AddDynamic(this, &UBtnTRTCUserWidget::OnStartLocalPreview_Click);
  BtnStopLocalPreview->OnClicked.AddDynamic(this, &UBtnTRTCUserWidget::OnStopLocalPreview_Click);

  std::string stdStrTemp1("20221001");
  FString tempRoomId = stdStrTemp1.c_str();
  TextRoomID->SetText(FText::FromString(tempRoomId));

  std::string stdStrTemp2("demo_user");
  FString tempUserText = stdStrTemp2.c_str();
  TextUserID->SetText(FText::FromString(tempUserText));
  writeLblLog(version.c_str());
}

void UBtnTRTCUserWidget::NativeDestruct() {
  Super::NativeDestruct();
  if (pTRTCCloud != nullptr) {
    pTRTCCloud->exitRoom();
    pTRTCCloud->removeCallback(this);
    pTRTCCloud->destroySharedInstance();
    pTRTCCloud = nullptr;
  }
  delete[] localBuffer;
  delete localUpdateTextureRegion;
  delete[] remoteBuffer;
  delete remoteUpdateTextureRegion;
}

void UBtnTRTCUserWidget::OnEnterRoom_Click() {
  writeLblLog("start OnEnterRoom_Click");

  // Construct Params for EnterRoom Call
  trtc::TRTCParams params;
  params.role = trtc::TRTCRoleAnchor;
  params.userDefineRecordId = "";
  fLocalUserId = TextUserID->GetText().ToString();
  std::string strUserId(TCHAR_TO_UTF8(*fLocalUserId));
  const char* charsUser = strUserId.c_str();
  params.userId = charsUser;

  FString cRoomId = TextRoomID->GetText().ToString();
  int32 intRoomId = FCString::Atoi(*cRoomId);
  params.roomId = (uint32_t)intRoomId;
  params.sdkAppId = SDKAppID;
  std::string sSig = liteav::ue::TestUserSigGenerator().gen(params.userId, SDKAppID, SECRETKEY);
  params.userSig = sSig.c_str();
  pTRTCCloud->enterRoom(params, trtc::TRTCAppSceneVideoCall);
  pTRTCCloud->startLocalAudio(trtc::TRTCAudioQualityDefault);
}

void UBtnTRTCUserWidget::OnExitRoom_Click() {
  writeLblLog("start OnExitRoom_Click");
  pTRTCCloud->exitRoom();
}

void UBtnTRTCUserWidget::OnStartLocalPreview_Click() {
  writeLblLog("start OnStartLocalPreview_Click");
#if TARGET_PLATFORM_PHONE
  pTRTCCloud->startLocalPreview(true, nullptr);
#else
  pTRTCCloud->startLocalPreview(nullptr);
#endif
#if PLATFORM_ANDROID
  pTRTCCloud->setLocalVideoRenderCallback(trtc::TRTCVideoPixelFormat_RGBA32, trtc::TRTCVideoBufferType_Buffer, this);
#else
  pTRTCCloud->setLocalVideoRenderCallback(trtc::TRTCVideoPixelFormat_BGRA32, trtc::TRTCVideoBufferType_Buffer, this);
#endif
  writeLblLog("end OnStartLocalPreview_Click");
}

void UBtnTRTCUserWidget::OnStopLocalPreview_Click() {
  writeLblLog("start OnStopLocalPreview_Click");
  pTRTCCloud->stopLocalPreview();
  ResetBuffer(true);
  if (localBuffer) {
    localRenderTargetTexture->UpdateTextureRegions(0, 1, localUpdateTextureRegion, localWidth * 4, (uint32)4,
                                                   localBuffer);
  }
}

void UBtnTRTCUserWidget::onRenderVideoFrame(const char* userId,
                                            trtc::TRTCVideoStreamType streamType,
                                            trtc::TRTCVideoFrame* videoFrame) {
  int frameLength = videoFrame->length;
  std::string strUserId(TCHAR_TO_UTF8(*fLocalUserId));
  if (LocalPreviewImage && RemoteImage && frameLength > 1) {
    bool isLocalUser = ((strcmp(strUserId.c_str(), userId) == 0 || nullptr == userId || strlen(userId) == 0) &&
                        streamType == trtc::TRTCVideoStreamTypeBig)
                           ? true
                           : false;
    UpdateBuffer(videoFrame->data, videoFrame->width, videoFrame->height, frameLength, isLocalUser);
  }
}

void UBtnTRTCUserWidget::UpdateBuffer(char* RGBBuffer,
                                      uint32_t NewWidth,
                                      uint32_t NewHeight,
                                      uint32_t NewSize,
                                      bool isLocal) {
  if (isLocal) {
    FScopeLock lock(&localMutex);
    if (!RGBBuffer) {
      return;
    }
    if (NewHeight != localHeight) {
      UE_LOG(LogTemp, Warning, TEXT("NewHeight != localHeight ,localHeight=%d, NewHeight=%d"), localHeight, NewHeight);
    }
    if (!localRefresh) {
      // First Local Video Frame
      UE_LOG(LogTemp, Warning, TEXT("localRefresh==false; NewSize=%d ,NewWidth=%d, NewHeight=%d"), NewSize, NewWidth,
             NewHeight);
      localRefresh = true;
      AsyncTask(ENamedThreads::GameThread, [=]() {
        localWidth = NewWidth;
        localHeight = NewHeight;
#if PLATFORM_ANDROID
        localRenderTargetTexture = UTexture2D::CreateTransient(localWidth, localHeight, PF_R8G8B8A8);
#else
                    localRenderTargetTexture = UTexture2D::CreateTransient(localWidth, localHeight);
#endif
        localRenderTargetTexture->UpdateResource();
        localBufferSize = localWidth * localHeight * 4;
        localBuffer = new uint8[localBufferSize];
        for (uint32 i = 0; i < localWidth * localHeight; ++i) {
          localBuffer[i * 4 + 0] = 0x32;
          localBuffer[i * 4 + 1] = 0x32;
          localBuffer[i * 4 + 2] = 0x32;
          localBuffer[i * 4 + 3] = 0xFF;
        }
        localUpdateTextureRegion = new FUpdateTextureRegion2D(0, 0, 0, 0, localWidth, localHeight);
        localRenderTargetTexture->UpdateTextureRegions(0, 1, localUpdateTextureRegion, localWidth * 4, (uint32)4,
                                                       localBuffer);
        localBrush.SetResourceObject(localRenderTargetTexture);
        LocalPreviewImage->SetBrush(localBrush);
      });
      return;
    }
    if (localBufferSize == NewSize) {
      localWidth = NewWidth;
      localHeight = NewHeight;
      std::copy(RGBBuffer, RGBBuffer + NewSize, localBuffer);
    } else {
      delete[] localBuffer;
      localBufferSize = NewSize;
      localWidth = NewWidth;
      localHeight = NewHeight;
      localBuffer = new uint8[localBufferSize];
      std::copy(RGBBuffer, RGBBuffer + NewSize, localBuffer);
    }
    localRefresh = true;
  } else {
    FScopeLock lock(&remoteMutex);
    if (!RGBBuffer) {
      return;
    }
    if (NewHeight != remoteHeight) {
      UE_LOG(LogTemp, Warning, TEXT("NewHeight != remoteHeight ,remoteHeight=%d, NewHeight=%d"), remoteHeight,
             NewHeight);
    }
    if (!remoteRefresh) {
      // Fist Remote User Video Frame
      UE_LOG(LogTemp, Warning, TEXT("remoteRefresh==false; NewSize=%d ,NewWidth=%d, NewHeight=%d"), NewSize, NewWidth,
             NewHeight);
      remoteRefresh = true;
      AsyncTask(ENamedThreads::GameThread, [=]() {
        remoteWidth = NewWidth;
        remoteHeight = NewHeight;
#if PLATFORM_ANDROID
        remoteRenderTargetTexture = UTexture2D::CreateTransient(remoteWidth, remoteHeight, PF_R8G8B8A8);
#else
                    remoteRenderTargetTexture = UTexture2D::CreateTransient(remoteWidth, remoteHeight);
#endif
        remoteRenderTargetTexture->UpdateResource();
        remoteBufferSize = remoteWidth * remoteHeight * 4;
        remoteBuffer = new uint8[remoteBufferSize];
        for (uint32 i = 0; i < remoteWidth * remoteHeight; ++i) {
          remoteBuffer[i * 4 + 0] = 0x32;
          remoteBuffer[i * 4 + 1] = 0x32;
          remoteBuffer[i * 4 + 2] = 0x32;
          remoteBuffer[i * 4 + 3] = 0xFF;
        }
        remoteUpdateTextureRegion = new FUpdateTextureRegion2D(0, 0, 0, 0, remoteWidth, remoteHeight);
        remoteRenderTargetTexture->UpdateTextureRegions(0, 1, remoteUpdateTextureRegion, remoteWidth * 4, (uint32)4,
                                                        remoteBuffer);
        remoteBrush.SetResourceObject(remoteRenderTargetTexture);
        RemoteImage->SetBrush(remoteBrush);
      });
      return;
    }
    if (remoteBufferSize == NewSize) {
      remoteWidth = NewWidth;
      remoteHeight = NewHeight;
      std::copy(RGBBuffer, RGBBuffer + NewSize, remoteBuffer);
    } else {
      delete[] remoteBuffer;
      remoteBufferSize = NewSize;
      remoteWidth = NewWidth;
      remoteHeight = NewHeight;
      remoteBuffer = new uint8[remoteBufferSize];
      std::copy(RGBBuffer, RGBBuffer + NewSize, remoteBuffer);
    }
    remoteRefresh = true;
  }
}

void UBtnTRTCUserWidget::ResetBuffer(bool isLocal) {
  if (isLocal && localBuffer) {
    for (uint32 i = 0; i < localWidth * localHeight; ++i) {
      localBuffer[i * 4 + 0] = 0x32;
      localBuffer[i * 4 + 1] = 0x32;
      localBuffer[i * 4 + 2] = 0x32;
      localBuffer[i * 4 + 3] = 0xFF;
    }
    localRefresh = false;
  } else if (remoteBuffer) {
    for (uint32 i = 0; i < remoteWidth * remoteHeight; ++i) {
      remoteBuffer[i * 4 + 0] = 0x32;
      remoteBuffer[i * 4 + 1] = 0x32;
      remoteBuffer[i * 4 + 2] = 0x32;
      remoteBuffer[i * 4 + 3] = 0xFF;
    }
    remoteRefresh = false;
  }
}

void UBtnTRTCUserWidget::NativeTick(const FGeometry& MyGeometry, float DeltaTime) {
  Super::NativeTick(MyGeometry, DeltaTime);
  // Update Local Preview
  if (localRefresh && localRenderTargetTexture && localBuffer && LocalPreviewImage && localBufferSize > 4) {
    FScopeLock lock(&localMutex);
    if (localRenderTargetTexture->GetSizeX() != localWidth || localRenderTargetTexture->GetSizeY() != localHeight) {
      UE_LOG(LogTemp, Warning,
             TEXT("localBufferSize=%d ,localWidth=%d, localHeight=%d localRenderTargetTexture->GetSizeX =%d , "
                  "localRenderTargetTexture->GetSizeY =%d"),
             localBufferSize, localWidth, localHeight, localRenderTargetTexture->GetSizeX(),
             localRenderTargetTexture->GetSizeY());
      auto NewUpdateTextureRegion = new FUpdateTextureRegion2D(0, 0, 0, 0, localWidth, localHeight);
// PF_R8G8B8A8
// macos PF_B8G8R8A8 --> TRTCVideoPixelFormat_BGRA32
#if PLATFORM_ANDROID
      auto NewRenderTargetTexture = UTexture2D::CreateTransient(localWidth, localHeight, PF_R8G8B8A8);
#else
      auto NewRenderTargetTexture = UTexture2D::CreateTransient(localWidth, localHeight);
#endif
      NewRenderTargetTexture->UpdateResource();
      NewRenderTargetTexture->UpdateTextureRegions(0, 1, NewUpdateTextureRegion, localWidth * 4, (uint32)4,
                                                   localBuffer);
      localBrush.SetResourceObject(NewRenderTargetTexture);
      LocalPreviewImage->SetBrush(localBrush);
      FUpdateTextureRegion2D* TmpUpdateTextureRegion = localUpdateTextureRegion;
      localRenderTargetTexture = NewRenderTargetTexture;
      localUpdateTextureRegion = NewUpdateTextureRegion;
      delete TmpUpdateTextureRegion;
      return;
    }
    localRenderTargetTexture->UpdateTextureRegions(0, 1, localUpdateTextureRegion, localWidth * 4, (uint32)4,
                                                   localBuffer);
  }
  // Update Remote User View
  if (remoteRefresh && remoteRenderTargetTexture && remoteBuffer && RemoteImage && remoteBufferSize > 4) {
    FScopeLock lock(&remoteMutex);
    if (remoteRenderTargetTexture->GetSizeX() != remoteWidth || remoteRenderTargetTexture->GetSizeY() != remoteHeight) {
      UE_LOG(LogTemp, Warning,
             TEXT("remoteBufferSize=%d, remoteWidth=%d, remoteHeight=%d  remoteRenderTargetTexture->GetSizeX =%d , "
                  "remoteRenderTargetTexture->GetSizeY =%d"),
             remoteBufferSize, remoteWidth, remoteHeight, remoteRenderTargetTexture->GetSizeX(),
             remoteRenderTargetTexture->GetSizeY());
      auto NewRMUpdateTextureRegion = new FUpdateTextureRegion2D(0, 0, 0, 0, remoteWidth, remoteHeight);
#if PLATFORM_ANDROID
      auto NewRMRenderTargetTexture = UTexture2D::CreateTransient(remoteWidth, remoteHeight, PF_R8G8B8A8);
#else
      auto NewRMRenderTargetTexture = UTexture2D::CreateTransient(remoteWidth, remoteHeight);
#endif
      NewRMRenderTargetTexture->UpdateResource();
      NewRMRenderTargetTexture->UpdateTextureRegions(0, 1, NewRMUpdateTextureRegion, remoteWidth * 4, (uint32)4,
                                                     remoteBuffer);
      remoteBrush.SetResourceObject(NewRMRenderTargetTexture);
      RemoteImage->SetBrush(remoteBrush);
      FUpdateTextureRegion2D* TmpRMUpdateTextureRegion = remoteUpdateTextureRegion;
      remoteRenderTargetTexture = NewRMRenderTargetTexture;
      remoteUpdateTextureRegion = NewRMUpdateTextureRegion;
      delete TmpRMUpdateTextureRegion;
      return;
    }
    remoteRenderTargetTexture->UpdateTextureRegions(0, 1, remoteUpdateTextureRegion, remoteWidth * 4, (uint32)4,
                                                    remoteBuffer);
  }
}

void UBtnTRTCUserWidget::writeLblLog(const char* logStr) {
  std::string stdStrLog(logStr);
  FString log = stdStrLog.c_str();
  UE_LOG(LogTemp, Log, TEXT("==> %s"), *log);
  if (TextLog != nullptr) {
    AsyncTask(ENamedThreads::GameThread, [=]() { TextLog->SetText(FText::FromString(log)); });
  } else {
    AsyncTask(ENamedThreads::GameThread, [=]() { UE_LOG(LogTemp, Warning, TEXT("TextLog not find")); });
  }
}

void UBtnTRTCUserWidget::writeCallbackLog(const char* logStr) {
  std::string stdStrLog(logStr);
  FString log = stdStrLog.c_str();
  UE_LOG(LogTemp, Log, TEXT("<== %s"), *log);
  if (TextCallback != nullptr) {
    AsyncTask(ENamedThreads::GameThread, [=]() { TextCallback->SetText(FText::FromString(log)); });
  } else {
    AsyncTask(ENamedThreads::GameThread, [=]() { UE_LOG(LogTemp, Warning, TEXT("TextCallback not find")); });
  }
}

void UBtnTRTCUserWidget::onUserVideoAvailable(const char* userId, bool available) {
  writeCallbackLog("onUserVideoAvailable");
  writeCallbackLog(userId);
  if (available) {
    pTRTCCloud->startRemoteView(userId, trtc::TRTCVideoStreamTypeBig, nullptr);
#if PLATFORM_ANDROID
    pTRTCCloud->setRemoteVideoRenderCallback(userId, trtc::TRTCVideoPixelFormat_RGBA32,
                                             trtc::TRTCVideoBufferType_Buffer, this);
#else
    pTRTCCloud->setRemoteVideoRenderCallback(userId, trtc::TRTCVideoPixelFormat_BGRA32,
                                             trtc::TRTCVideoBufferType_Buffer, this);
#endif
  } else {
    pTRTCCloud->stopRemoteView(userId, trtc::TRTCVideoStreamTypeBig);
    ResetBuffer(false);
    AsyncTask(ENamedThreads::GameThread, [=]() {
      remoteRenderTargetTexture->UpdateTextureRegions(0, 1, remoteUpdateTextureRegion, remoteWidth * 4, (uint32)4,
                                                      remoteBuffer);
    });
  }
}

void UBtnTRTCUserWidget::onUserSubStreamAvailable(const char* userId, bool available) {
  writeCallbackLog("onUserSubStreamAvailable");
  writeCallbackLog(userId);
  if (available) {
    pTRTCCloud->startRemoteView(userId, trtc::TRTCVideoStreamTypeSub, nullptr);
#if PLATFORM_ANDROID
    pTRTCCloud->setRemoteVideoRenderCallback(userId, trtc::TRTCVideoPixelFormat_RGBA32,
                                             trtc::TRTCVideoBufferType_Buffer, this);
#else
    pTRTCCloud->setRemoteVideoRenderCallback(userId, trtc::TRTCVideoPixelFormat_BGRA32,
                                             trtc::TRTCVideoBufferType_Buffer, this);
#endif
  } else {
    pTRTCCloud->stopRemoteView(userId, trtc::TRTCVideoStreamTypeSub);
    ResetBuffer(false);
    remoteRenderTargetTexture->UpdateTextureRegions(0, 1, remoteUpdateTextureRegion, remoteWidth * 4, (uint32)4,
                                                    remoteBuffer);
  }
}

void UBtnTRTCUserWidget::onWarning(TXLiteAVWarning warningCode, const char* warningMsg, void* extraInfo) {
  writeCallbackLog(warningMsg);
}

void UBtnTRTCUserWidget::onError(TXLiteAVError errCode, const char* errMsg, void* extraInfo) {
  writeCallbackLog(errMsg);
}

void UBtnTRTCUserWidget::onEnterRoom(int result) {
  writeCallbackLog("onEnterRoom");
  UE_LOG(LogTemp, Log, TEXT("<== %d"), result);
}

void UBtnTRTCUserWidget::onExitRoom(int reason) {
  writeCallbackLog("onExitRoom");
  UE_LOG(LogTemp, Log, TEXT("<== %d"), reason);
}
