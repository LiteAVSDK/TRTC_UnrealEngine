//  Copyright (c) 2022 Tencent All rights reserved.

#pragma once

#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "CoreMinimal.h"
#include "Engine/Texture2D.h"
#include "Misc/CString.h"
#include "Modules/ModuleManager.h"
#include "SlateOptMacros.h"

#if PLATFORM_ANDROID
#include "Runtime/ApplicationCore/Public/Android/AndroidApplication.h"
#include "Runtime/Core/Public/Android/AndroidJavaEnv.h"
#include "Runtime/Launch/Public/Android/AndroidJNI.h"
#endif

#include <map>
#include <mutex>
#include "TRTCCloud.h"

#include "BtnTRTCUserWidget.generated.h"

namespace trtc = liteav;

struct VideoTextureFrame {
  UTexture2D* Texture;
  FSlateBrush Brush;
  std::unique_ptr<FUpdateTextureRegion2D> UpdateTextureRegion;
  bool Fresh;
  int Width;
  int Height;
  const char* userId;
  VideoTextureFrame() : Texture(nullptr), UpdateTextureRegion(nullptr), Fresh(false), Width(0), Height(0), userId(0) {}
};

#if PLATFORM_WINDOWS
#define UpdateResource UpdateResource
#endif
/**
 *
 */
UCLASS()
class UBtnTRTCUserWidget : public UUserWidget, public trtc::ITRTCVideoRenderCallback, public trtc::ITRTCCloudCallback {
  GENERATED_BODY()
 private:
  void onExitRoom(int reason) override;
  void onEnterRoom(int result) override;
  void onUserVideoAvailable(const char* userId, bool available) override;
  void onUserSubStreamAvailable(const char* userId, bool available) override;
  void onError(TXLiteAVError errCode, const char* errMsg, void* extraInfo) override;
  void onWarning(TXLiteAVWarning warningCode, const char* warningMsg, void* extraInfo) override;
  void writeLblLog(const char* log);
  void writeCallbackLog(const char* log);
  liteav::ue::TRTCCloud* pTRTCCloud;

 public:
  UFUNCTION(BlueprintCallable, Category = "trtcDemoFunction")
  void OnEnterRoom_Click();

  UFUNCTION(BlueprintCallable, Category = "trtcDemoFunction")
  void OnExitRoom_Click();

  UFUNCTION(BlueprintCallable, Category = "trtcDemoFunction")
  void OnStartLocalPreview_Click();

  UFUNCTION(BlueprintCallable, Category = "trtcDemoFunction")
  void OnStopLocalPreview_Click();

  UPROPERTY(VisibleAnywhere, Meta = (BindWidget))
  UEditableTextBox* TextRoomID = nullptr;

  UPROPERTY(VisibleAnywhere, Meta = (BindWidget))
  UEditableTextBox* TextUserID = nullptr;

  UPROPERTY(VisibleAnywhere, Meta = (BindWidget))
  UTextBlock* TextLog;

  UPROPERTY(VisibleAnywhere, Meta = (BindWidget))
  UTextBlock* TextCallback;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
  UButton* BtnEnterRoom = nullptr;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
  UButton* BtnExitRoom = nullptr;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
  UButton* BtnStartLocalPreview = nullptr;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
  UButton* BtnStopLocalPreview = nullptr;

  UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
  UImage* LocalPreviewImage = nullptr;

  UPROPERTY(EditDefaultsOnly)
  UTexture2D* localRenderTargetTexture = nullptr;
  FUpdateTextureRegion2D* localUpdateTextureRegion = nullptr;
  FSlateBrush localBrush;
  FCriticalSection localMutex;
  uint8* localBuffer = nullptr;
  uint32_t localWidth = 0;
  uint32_t localHeight = 0;
  bool localRefresh = false;
  uint32 localBufferSize = 0;

  UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
  UImage* RemoteImage = nullptr;

  UPROPERTY(EditDefaultsOnly)
  UTexture2D* remoteRenderTargetTexture = nullptr;
  FUpdateTextureRegion2D* remoteUpdateTextureRegion = nullptr;
  FSlateBrush remoteBrush;
  FCriticalSection remoteMutex;
  uint8* remoteBuffer = nullptr;
  uint32_t remoteWidth = 0;
  uint32_t remoteHeight = 0;
  bool remoteRefresh = false;
  uint32 remoteBufferSize = 0;

  FString fLocalUserId;

  void UpdateBuffer(char* RGBBuffer, uint32_t Width, uint32_t Height, uint32_t Size, bool isLocal);

  void ResetBuffer(bool isLocal);

  void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;

  void NativeConstruct() override;

  void NativeDestruct() override;

  // Customize Video Rendering Callback
  void onRenderVideoFrame(const char* userId,
                          trtc::TRTCVideoStreamType streamType,
                          trtc::TRTCVideoFrame* frame) override;
};
