// Copyright (c) 2022 Tencent. All rights reserved.

#include "TRTCCloud.h"

namespace liteav {
namespace ue {

TRTCCloud::TRTCCloud(const TRTCCloud& other) : trtc_cloud_(other.trtc_cloud_) {}

TRTCCloud::TRTCCloud(TRTCCloud&& other) noexcept : trtc_cloud_(std::exchange(other.trtc_cloud_, nullptr)) {}

TRTCCloud::TRTCCloud(liteav::ITRTCCloud* trtc_cloud) : trtc_cloud_(trtc_cloud) {}

TRTCCloud::~TRTCCloud() {}

#if PLATFORM_ANDROID
TRTCCloud* TRTCCloud::getSharedInstance(void* context) {
  liteav::ITRTCCloud* trtc_cloud = getTRTCShareInstance(context);
  if (!trtc_cloud) {
    return nullptr;
  }
  return new TRTCCloud(trtc_cloud);
}
#else
TRTCCloud* TRTCCloud::getSharedInstance() {
  liteav::ITRTCCloud* trtc_cloud = liteav::ITRTCCloud::getTRTCShareInstance();
  if (!trtc_cloud) {
    return nullptr;
  }
  return new TRTCCloud(trtc_cloud);
}
#endif

void TRTCCloud::destroySharedInstance() {
#if PLATFORM_ANDROID
  destroyTRTCShareInstance();
#else
  liteav::ITRTCCloud::destroyTRTCShareInstance();
#endif
}

void TRTCCloud::addCallback(ITRTCCloudCallback* callback) {
  trtc_cloud_->addCallback(callback);
}

void TRTCCloud::removeCallback(ITRTCCloudCallback* callback) {
  trtc_cloud_->removeCallback(callback);
}

void TRTCCloud::enterRoom(const TRTCParams& param, TRTCAppScene scene) {
  trtc_cloud_->callExperimentalAPI("{\"api\": \"setFramework\", \"params\": {\"framework\": 35}}");
  trtc_cloud_->enterRoom(param, scene);
}

void TRTCCloud::exitRoom() {
  trtc_cloud_->exitRoom();
}

void TRTCCloud::switchRole(TRTCRoleType role) {
  trtc_cloud_->switchRole(role);
}

void TRTCCloud::switchRole(TRTCRoleType role, const char* privateMapKey) {
  trtc_cloud_->switchRole(role, privateMapKey);
}

void TRTCCloud::switchRoom(const TRTCSwitchRoomConfig& config) {
  trtc_cloud_->switchRoom(config);
}

void TRTCCloud::connectOtherRoom(const char* param) {
  trtc_cloud_->connectOtherRoom(param);
}

void TRTCCloud::disconnectOtherRoom() {
  trtc_cloud_->disconnectOtherRoom();
}

void TRTCCloud::setDefaultStreamRecvMode(bool autoRecvAudio, bool autoRecvVideo) {
  trtc_cloud_->setDefaultStreamRecvMode(autoRecvAudio, autoRecvVideo);
}

void TRTCCloud::startPublishing(const char* streamId, TRTCVideoStreamType streamType) {
  trtc_cloud_->startPublishing(streamId, streamType);
}

void TRTCCloud::stopPublishing() {
  trtc_cloud_->stopPublishing();
}

void TRTCCloud::startPublishCDNStream(const TRTCPublishCDNParam& param) {
  trtc_cloud_->startPublishCDNStream(param);
}

void TRTCCloud::stopPublishCDNStream() {
  trtc_cloud_->stopPublishCDNStream();
}

void TRTCCloud::setMixTranscodingConfig(TRTCTranscodingConfig* config) {
  trtc_cloud_->setMixTranscodingConfig(config);
}

void TRTCCloud::startPublishMediaStream(TRTCPublishTarget* target,
                                        TRTCStreamEncoderParam* params,
                                        TRTCStreamMixingConfig* config) {
  trtc_cloud_->startPublishMediaStream(target, params, config);
}

void TRTCCloud::updatePublishMediaStream(const char* taskId,
                                         TRTCPublishTarget* target,
                                         TRTCStreamEncoderParam* params,
                                         TRTCStreamMixingConfig* config) {
  trtc_cloud_->updatePublishMediaStream(taskId, target, params, config);
}

void TRTCCloud::stopPublishMediaStream(const char* taskId) {
  trtc_cloud_->stopPublishMediaStream(taskId);
}

#if TARGET_PLATFORM_PHONE
void TRTCCloud::startLocalPreview(bool frontCamera, TXView view) {
  trtc_cloud_->startLocalPreview(frontCamera, view);
}
#endif

#if TARGET_PLATFORM_DESKTOP
void TRTCCloud::startLocalPreview(TXView view) {
  trtc_cloud_->startLocalPreview(view);
}
#endif

void TRTCCloud::updateLocalView(TXView view) {
  trtc_cloud_->updateLocalView(view);
}

void TRTCCloud::stopLocalPreview() {
  trtc_cloud_->stopLocalPreview();
}

void TRTCCloud::muteLocalVideo(TRTCVideoStreamType streamType, bool mute) {
  trtc_cloud_->muteLocalVideo(streamType, mute);
}

void TRTCCloud::setVideoMuteImage(TRTCImageBuffer* image, int fps) {
  trtc_cloud_->setVideoMuteImage(image, fps);
}

void TRTCCloud::startRemoteView(const char* userId, TRTCVideoStreamType streamType, TXView view) {
  trtc_cloud_->startRemoteView(userId, streamType, view);
}

void TRTCCloud::updateRemoteView(const char* userId, TRTCVideoStreamType streamType, TXView view) {
  trtc_cloud_->updateRemoteView(userId, streamType, view);
}

void TRTCCloud::stopRemoteView(const char* userId, TRTCVideoStreamType streamType) {
  trtc_cloud_->stopRemoteView(userId, streamType);
}

void TRTCCloud::stopAllRemoteView() {
  trtc_cloud_->stopAllRemoteView();
}

void TRTCCloud::muteRemoteVideoStream(const char* userId, TRTCVideoStreamType streamType, bool mute) {
  trtc_cloud_->muteRemoteVideoStream(userId, streamType, mute);
}

void TRTCCloud::muteAllRemoteVideoStreams(bool mute) {
  trtc_cloud_->muteAllRemoteVideoStreams(mute);
}

void TRTCCloud::setVideoEncoderParam(const TRTCVideoEncParam& param) {
  trtc_cloud_->setVideoEncoderParam(param);
}

void TRTCCloud::setNetworkQosParam(const TRTCNetworkQosParam& param) {
  trtc_cloud_->setNetworkQosParam(param);
}

void TRTCCloud::setLocalRenderParams(const TRTCRenderParams& params) {
  trtc_cloud_->setLocalRenderParams(params);
}

void TRTCCloud::setRemoteRenderParams(const char* userId,
                                      TRTCVideoStreamType streamType,
                                      const TRTCRenderParams& params) {
  trtc_cloud_->setRemoteRenderParams(userId, streamType, params);
}

void TRTCCloud::setVideoEncoderRotation(TRTCVideoRotation rotation) {
  trtc_cloud_->setVideoEncoderRotation(rotation);
}

void TRTCCloud::setVideoEncoderMirror(bool mirror) {
  trtc_cloud_->setVideoEncoderMirror(mirror);
}

void TRTCCloud::enableSmallVideoStream(bool enable, const TRTCVideoEncParam& smallVideoEncParam) {
  trtc_cloud_->enableSmallVideoStream(enable, smallVideoEncParam);
}

void TRTCCloud::setRemoteVideoStreamType(const char* userId, TRTCVideoStreamType streamType) {
  trtc_cloud_->setRemoteVideoStreamType(userId, streamType);
}

#if _WIN32 || __APPLE__
void TRTCCloud::snapshotVideo(const char* userId, TRTCVideoStreamType streamType, TRTCSnapshotSourceType sourceType) {
  trtc_cloud_->snapshotVideo(userId, streamType, sourceType);
}
#endif

void TRTCCloud::startLocalAudio(TRTCAudioQuality quality) {
  trtc_cloud_->startLocalAudio(quality);
}

void TRTCCloud::stopLocalAudio() {
  trtc_cloud_->stopLocalAudio();
}

void TRTCCloud::muteLocalAudio(bool mute) {
  trtc_cloud_->muteLocalAudio(mute);
}

void TRTCCloud::muteRemoteAudio(const char* userId, bool mute) {
  trtc_cloud_->muteRemoteAudio(userId, mute);
}

void TRTCCloud::muteAllRemoteAudio(bool mute) {
  trtc_cloud_->muteAllRemoteAudio(mute);
}

void TRTCCloud::setRemoteAudioVolume(const char* userId, int volume) {
  trtc_cloud_->setRemoteAudioVolume(userId, volume);
}

void TRTCCloud::setAudioCaptureVolume(int volume) {
  trtc_cloud_->setAudioCaptureVolume(volume);
}

int TRTCCloud::getAudioCaptureVolume() {
  return trtc_cloud_->getAudioCaptureVolume();
}

void TRTCCloud::setAudioPlayoutVolume(int volume) {
  trtc_cloud_->setAudioPlayoutVolume(volume);
}

int TRTCCloud::getAudioPlayoutVolume() {
  return trtc_cloud_->getAudioPlayoutVolume();
}

void TRTCCloud::enableAudioVolumeEvaluation(uint32_t interval, bool enable_vad) {
  trtc_cloud_->enableAudioVolumeEvaluation(interval, enable_vad);
}

int TRTCCloud::startAudioRecording(const TRTCAudioRecordingParams& param) {
  return trtc_cloud_->startAudioRecording(param);
}

void TRTCCloud::stopAudioRecording() {
  trtc_cloud_->stopAudioRecording();
}

#ifdef _WIN32
void TRTCCloud::startLocalRecording(const TRTCLocalRecordingParams& params) {
  trtc_cloud_->startLocalRecording(params);
}
#endif

#ifdef _WIN32
void TRTCCloud::stopLocalRecording() {
  trtc_cloud_->stopLocalRecording();
}
#endif

void TRTCCloud::setRemoteAudioParallelParams(const TRTCAudioParallelParams& params) {
  trtc_cloud_->setRemoteAudioParallelParams(params);
}

void TRTCCloud::enable3DSpatialAudioEffect(bool enabled) {
  trtc_cloud_->enable3DSpatialAudioEffect(enabled);
}

void TRTCCloud::updateSelf3DSpatialPosition(int position[3],
                                            float axisForward[3],
                                            float axisRight[3],
                                            float axisUp[3]) {
  trtc_cloud_->updateSelf3DSpatialPosition(position, axisForward, axisRight, axisUp);
}

ITXDeviceManager* TRTCCloud::getDeviceManager() {
  return trtc_cloud_->getDeviceManager();
}

void TRTCCloud::setBeautyStyle(TRTCBeautyStyle style,
                               uint32_t beautyLevel,
                               uint32_t whitenessLevel,
                               uint32_t ruddinessLevel) {
  trtc_cloud_->setBeautyStyle(style, beautyLevel, whitenessLevel, ruddinessLevel);
}

void TRTCCloud::setWaterMark(TRTCVideoStreamType streamType,
                             const char* srcData,
                             TRTCWaterMarkSrcType srcType,
                             uint32_t nWidth,
                             uint32_t nHeight,
                             float xOffset,
                             float yOffset,
                             float fWidthRatio,
                             bool isVisibleOnLocalPreview) {
  trtc_cloud_->setWaterMark(streamType, srcData, srcType, nWidth, nHeight, xOffset, yOffset, fWidthRatio,
                            isVisibleOnLocalPreview);
}

ITXAudioEffectManager* TRTCCloud::getAudioEffectManager() {
  return trtc_cloud_->getAudioEffectManager();
}

#if TARGET_PLATFORM_DESKTOP
void TRTCCloud::startSystemAudioLoopback(const char* deviceName) {
  trtc_cloud_->startSystemAudioLoopback(deviceName);
}
#endif

#if TARGET_PLATFORM_DESKTOP
void TRTCCloud::stopSystemAudioLoopback() {
  trtc_cloud_->stopSystemAudioLoopback();
}
#endif

#if TARGET_PLATFORM_DESKTOP || TARGET_OS_IPHONE
void TRTCCloud::setSystemAudioLoopbackVolume(uint32_t volume) {
  trtc_cloud_->setSystemAudioLoopbackVolume(volume);
}
#endif

void TRTCCloud::startScreenCapture(TXView view, TRTCVideoStreamType streamType, TRTCVideoEncParam* encParam) {
  trtc_cloud_->startScreenCapture(view, streamType, encParam);
}

void TRTCCloud::stopScreenCapture() {
  trtc_cloud_->stopScreenCapture();
}

void TRTCCloud::pauseScreenCapture() {
  trtc_cloud_->pauseScreenCapture();
}

void TRTCCloud::resumeScreenCapture() {
  trtc_cloud_->resumeScreenCapture();
}

#if TARGET_PLATFORM_DESKTOP
ITRTCScreenCaptureSourceList* TRTCCloud::getScreenCaptureSources(const SIZE& thumbnailSize, const SIZE& iconSize) {
  return trtc_cloud_->getScreenCaptureSources(thumbnailSize, iconSize);
}
#endif

#if TARGET_PLATFORM_DESKTOP
void TRTCCloud::selectScreenCaptureTarget(const TRTCScreenCaptureSourceInfo& source,
                                          const RECT& captureRect,
                                          const TRTCScreenCaptureProperty& property) {
  trtc_cloud_->selectScreenCaptureTarget(source, captureRect, property);
}
#endif

void TRTCCloud::setSubStreamEncoderParam(const TRTCVideoEncParam& param) {
  trtc_cloud_->setSubStreamEncoderParam(param);
}

#if TARGET_PLATFORM_DESKTOP
void TRTCCloud::setSubStreamMixVolume(uint32_t volume) {
  trtc_cloud_->setSubStreamMixVolume(volume);
}
#endif

#if TARGET_PLATFORM_DESKTOP
void TRTCCloud::addExcludedShareWindow(TXView windowID) {
  trtc_cloud_->addExcludedShareWindow(windowID);
}
#endif

#if TARGET_PLATFORM_DESKTOP
void TRTCCloud::removeExcludedShareWindow(TXView windowID) {
  trtc_cloud_->removeExcludedShareWindow(windowID);
}
#endif

#if TARGET_PLATFORM_DESKTOP
void TRTCCloud::removeAllExcludedShareWindow() {
  trtc_cloud_->removeAllExcludedShareWindow();
}
#endif

#if TARGET_PLATFORM_DESKTOP
void TRTCCloud::addIncludedShareWindow(TXView windowID) {
  trtc_cloud_->addIncludedShareWindow(windowID);
}
#endif

#if TARGET_PLATFORM_DESKTOP
void TRTCCloud::removeIncludedShareWindow(TXView windowID) {
  trtc_cloud_->removeIncludedShareWindow(windowID);
}
#endif

#if TARGET_PLATFORM_DESKTOP
void TRTCCloud::removeAllIncludedShareWindow() {
  trtc_cloud_->removeAllIncludedShareWindow();
}
#endif

void TRTCCloud::enableCustomVideoCapture(TRTCVideoStreamType streamType, bool enable) {
  trtc_cloud_->enableCustomVideoCapture(streamType, enable);
}

void TRTCCloud::sendCustomVideoData(TRTCVideoStreamType streamType, TRTCVideoFrame* frame) {
  trtc_cloud_->sendCustomVideoData(streamType, frame);
}

void TRTCCloud::enableCustomAudioCapture(bool enable) {
  trtc_cloud_->enableCustomAudioCapture(enable);
}

void TRTCCloud::sendCustomAudioData(TRTCAudioFrame* frame) {
  trtc_cloud_->sendCustomAudioData(frame);
}

void TRTCCloud::enableMixExternalAudioFrame(bool enablePublish, bool enablePlayout) {
  trtc_cloud_->enableMixExternalAudioFrame(enablePublish, enablePlayout);
}

int TRTCCloud::mixExternalAudioFrame(TRTCAudioFrame* frame) {
  return trtc_cloud_->mixExternalAudioFrame(frame);
}

void TRTCCloud::setMixExternalAudioVolume(int publishVolume, int playoutVolume) {
  trtc_cloud_->setMixExternalAudioVolume(publishVolume, playoutVolume);
}

uint64_t TRTCCloud::generateCustomPTS() {
  return trtc_cloud_->generateCustomPTS();
}

int TRTCCloud::setLocalVideoProcessCallback(TRTCVideoPixelFormat pixelFormat,
                                            TRTCVideoBufferType bufferType,
                                            ITRTCVideoFrameCallback* callback) {
  return trtc_cloud_->setLocalVideoProcessCallback(pixelFormat, bufferType, callback);
}

int TRTCCloud::setLocalVideoRenderCallback(TRTCVideoPixelFormat pixelFormat,
                                           TRTCVideoBufferType bufferType,
                                           ITRTCVideoRenderCallback* callback) {
  return trtc_cloud_->setLocalVideoRenderCallback(pixelFormat, bufferType, callback);
}

int TRTCCloud::setRemoteVideoRenderCallback(const char* userId,
                                            TRTCVideoPixelFormat pixelFormat,
                                            TRTCVideoBufferType bufferType,
                                            ITRTCVideoRenderCallback* callback) {
  return trtc_cloud_->setRemoteVideoRenderCallback(userId, pixelFormat, bufferType, callback);
}

int TRTCCloud::setAudioFrameCallback(ITRTCAudioFrameCallback* callback) {
  return trtc_cloud_->setAudioFrameCallback(callback);
}

int TRTCCloud::setCapturedRawAudioFrameCallbackFormat(TRTCAudioFrameCallbackFormat* format) {
  return trtc_cloud_->setCapturedRawAudioFrameCallbackFormat(format);
}

int TRTCCloud::setLocalProcessedAudioFrameCallbackFormat(TRTCAudioFrameCallbackFormat* format) {
  return trtc_cloud_->setLocalProcessedAudioFrameCallbackFormat(format);
}

int TRTCCloud::setMixedPlayAudioFrameCallbackFormat(TRTCAudioFrameCallbackFormat* format) {
  return trtc_cloud_->setMixedPlayAudioFrameCallbackFormat(format);
}

void TRTCCloud::enableCustomAudioRendering(bool enable) {
  trtc_cloud_->enableCustomAudioRendering(enable);
}

void TRTCCloud::getCustomAudioRenderingFrame(TRTCAudioFrame* audioFrame) {
  trtc_cloud_->getCustomAudioRenderingFrame(audioFrame);
}

bool TRTCCloud::sendCustomCmdMsg(uint32_t cmdId, const uint8_t* data, uint32_t dataSize, bool reliable, bool ordered) {
  return trtc_cloud_->sendCustomCmdMsg(cmdId, data, dataSize, reliable, ordered);
}

bool TRTCCloud::sendSEIMsg(const uint8_t* data, uint32_t dataSize, int32_t repeatCount) {
  return trtc_cloud_->sendSEIMsg(data, dataSize, repeatCount);
}

int TRTCCloud::startSpeedTest(const TRTCSpeedTestParams& params) {
  return trtc_cloud_->startSpeedTest(params);
}

void TRTCCloud::stopSpeedTest() {
  trtc_cloud_->stopSpeedTest();
}

const char* TRTCCloud::getSDKVersion() {
  return trtc_cloud_->getSDKVersion();
}

void TRTCCloud::setLogLevel(TRTCLogLevel level) {
  trtc_cloud_->setLogLevel(level);
}

void TRTCCloud::setConsoleEnabled(bool enabled) {
  trtc_cloud_->setConsoleEnabled(enabled);
}

void TRTCCloud::setLogCompressEnabled(bool enabled) {
  trtc_cloud_->setLogCompressEnabled(enabled);
}

void TRTCCloud::setLogDirPath(const char* path) {
  trtc_cloud_->setLogDirPath(path);
}

void TRTCCloud::setLogCallback(ITRTCLogCallback* callback) {
  trtc_cloud_->setLogCallback(callback);
}

void TRTCCloud::showDebugView(int showType) {
  trtc_cloud_->showDebugView(showType);
}

#ifdef _WIN32
const char* TRTCCloud::callExperimentalAPI(const char* jsonStr) {
  return trtc_cloud_->callExperimentalAPI(jsonStr);
}
#else
void TRTCCloud::callExperimentalAPI(const char* jsonStr) {
  trtc_cloud_->callExperimentalAPI(jsonStr);
}
#endif

}  // namespace ue
}  // namespace liteav
