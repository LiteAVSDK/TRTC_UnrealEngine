# TRTC Unreal Engine 5 Plugin

## Overview
Leveraging Tencent's many years of experience in network and audio/video technologies, Tencent Real-Time Communication (TRTC) offers solutions for group audio/video calls and low-latency interactive live streaming. With TRTC, you can quickly develop cost-effective, low-latency, and high-quality interactive audio/video services. [Learn more](https://www.tencentcloud.com/document/product/647/35078)...

## Contents
This TRTC Unreal Plugin help you to integrate the real-time interaction services with high quality and availability into your UE5 projects.
This directory contains the TRTC UE Demo for iOS/Android/macOS/Windows.
```
.
├── Config                      // Demo Project Files
├── Content                     // Demo Project Files
├── Source                      // Demo Project Files
├── TRTCPluginDemo.uproject     // Demo Project Files
├── Plugins
│   └── TRTCPlugin              // TRTC Unreal Engine 5 Plugin with TRTC SDK(10.3)
│       ├── Resources
│       ├── Source
│       │   ├── TRTCPlugin
│       │   ├── TRTCSDK	        // Manually downloaded latest LiteAVSDK
│       │   └── UserSig	        // Manually downloaded UserSig generation lib
│       └── TRTCPlugin.uplugin
├── Doc                         // Readme file assets
├── CopyFrameworkAndUpdateInfoPlistForMac.sh
├── DemoQuickStart_en.md
├── SDKQuickIntegration_en.md
└── README.en.md                // 💻 Start from here
```

## SDK Download
[![Download](https://img.shields.io/badge/LiteAVSDK--UE-10.3-green)](https://liteav.sdk.qcloud.com/customer/ue5/LiteAVSDK_10.3_UE5.zip)

## Related Documents
- [Demo Quick Start](./DemoQuickStart_en.md)
- [SDK Quick Integration](./SDKQuickIntegration_en.md)