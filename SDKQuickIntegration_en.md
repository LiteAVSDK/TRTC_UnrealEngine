This document describes how to quickly integrate the TRTC SDK for Unreal Engine into your project.

## Environment Requirements
- Unreal Engine 5.0.3 or above
- **Developing for Android:**
  - Android Studio 4.0 or above
  - NDK r21e
  - A real device for testing
- **Developing for iOS and macOS:**
  - Xcode 12.4 or above
  - macOS Monterey or above
  - A valid developer signature for your project
- **Developing for Windows:**
	- OS: Windows 10 64-bit version 1909 revision .1350 or higher, or versions 2004 and 20H2 revision .789 or higher.
    - [Visual Studio 2019](https://visualstudio.microsoft.com/zh-hans/downloads/)

## Integrating the SDK with TRTCPlugin
1. Download the latest LiteAVSDK for UE from [Here](https://liteav.sdk.qcloud.com/customer/ue5/LiteAVSDK_10.3_UE5.zip). And copy the unzipped files into **Plugins/TRTCPlugin/Source**.
2. Copy the `Plugins/TRTCPlugin` folder to the **Source/[project_name]/Plugins** directory of your project.
3. Add the following code to the **[project_name].Build.cs** file in your project.
```
public class YourProject : ModuleRules
{
    public YourProject(ReadOnlyTargetRules Target) : base(Target)
    {
        //...

        // TRTCPlugin
        PrivateDependencyModuleNames.AddRange(new string[] { "TRTCPlugin" });
        bEnableUndefinedIdentifierWarnings = false;
    }
}
```
3. You have integrated the TRTCPlugin into your project and can now use it in the CPP file.`#include "TRTCCloud.h"`. For more API usage examples, check out the demo code(	`BtnTRTCUserWidget.cpp`).
4. If you want to deploy on iOS, add the following code to the **[project_name].Target.cs**
```
public class YourProjectTarget : TargetRules
{
    public YourProjectTarget(TargetInfo Target) : base(Target)
    {
        //...

        // TRTCPlugin
        if (Target.Platform == UnrealTargetPlatform.IOS)
        {
            bOverrideBuildEnvironment = true;
            GlobalDefinitions.Add("FORCE_ANSI_ALLOCATOR=1");
        }
    }
}

```
## Packaging

#### macOS
1. Go to **File > Package Project > Mac**.
2. Configure permissions. Right-click the `xxx.app` file compiled in the previous step and select **Show Package Contents**. 
![](https://qcloudimg.tencent-cloud.cn/raw/3eb106ee3307c206dff5314a43920132.png)
3. Go to **Contents > Info.plist**.
4. Select **Information Property List** and add the following two permissions:
```
<key>NSCameraUsageDescription</key>
<string>Video calls are possible only with camera permission.</string>
<key>NSMicrophoneUsageDescription</key>
<string>Audio calls are possible only with mic access.</string>
```
5. If you use Unreal Editor, add the above permissions to the **UnrealEditor.app** file.

#### Windows
1. Go to **File > Package Project > Windows > Windows(64-bit)**.
![](https://imgcache.qq.com/operation/dianshi/other/win.ba79ccce59ae58718e6c35c16cdef55531456a70.png)

#### iOS
1. The following permissions are required on iOS.
```
Privacy - Camera Usage Description
Privacy - Microphone Usage Description
```
To add the permissions to `info.plist`, go to **Edit > Project Settings > Platforms: iOS** and add  
```
<key>NSCameraUsageDescription</key>
<string>Video calls are possible only with camera permission.</string>
<key>NSMicrophoneUsageDescription</key>
<string>Audio calls are possible only with mic access.</string>
```
to `Additional Plist Data`.
2. Go to **File > Package Project > iOS** to package your project.

####  Android
1. For development and testing, see [Android Quick Start](https://docs.unrealengine.com/4.27/en-US/SharingAndReleasing/Mobile/Android/GettingStarted/).
2. For packaging, see [Packaging Android Projects](https://docs.unrealengine.com/4.27/en-US/SharingAndReleasing/Mobile/Android/PackagingAndroidProject/).

## TRTC Cross-Platform (C++) APIs
[API Documentation (Chinese)](https://liteav.sdk.qcloud.com/doc/api/zh-cn/md_introduction_trtc_zh_Cplusplus_Brief.html)

[API Documentation (English)](https://liteav.sdk.qcloud.com/doc/api/en/md_introduction_trtc_en_Cplusplus_Brief.html)
