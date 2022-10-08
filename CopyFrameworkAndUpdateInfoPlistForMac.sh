#/bin/sh

echo $1

# Insert Camera/Micophone UsageDescription
plist_file="$1/Contents/info.plist"
echo $plist_file
sed -i '' "/<key>CFBundleExecutable<\/key>/ i \\ 
	<key>NSCameraUsageDescription</key><string>Video calls are possible only with camera permission.</string><key>NSMicrophoneUsageDescription</key><string>Audio calls are possible only with mic access.</string>" $plist_file

# Copy Mac Frameworks
binary_dir="$1/Contents/MacOS/"
cp -R ./Plugins/TRTCPlugin/Source/TRTCSDK/Mac/*.framework $binary_dir