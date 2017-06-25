//
//  ProjectBuilder.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 24/06/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include "ProjectBuilder.hpp"
#include "FileHelper.hpp"
#include <iostream>
#include <fstream>
#include "StringHelper.hpp"
#include "FileReader.hpp"

using namespace Pocket;

ProjectBuilder::ProjectBuilder() {
    IOS.project = this;
}

void ProjectBuilder::Initialize(const std::string &pocketEngineIncludePath, const std::string &workingDirectory) {
    this->pocketEngineIncludePath = pocketEngineIncludePath;
    this->workingDirectory = workingDirectory;
}

void ProjectBuilder::SetSourceFiles(const std::vector<std::string> &sourceFiles, const std::vector<std::string> &headerFiles) {
    this->sourceFiles = sourceFiles;
    this->headerFiles = headerFiles;
}

std::vector<std::string> ProjectBuilder::GetEngineHeaders() {
    return {
    "ComponentSystem",
    "ComponentSystem/Meta",
    "OpenGL",
    "Math",
    "Data",
    "Core",
    "Platform/OSX",
    "Serialization",
    
    "Logic/Animation",
    "Logic/Audio",
    "Logic/Common",
    "Logic/Editor",
    "Logic/Effects",
    "Logic/Gui",
    "Logic/Gui/Layout",
    "Logic/Gui/Menu",
    
    "Logic/Interaction",
    
    "Logic/Movement",
    "Logic/Physics",
    "Logic/Rendering",
    "Logic/Selection",
    "Logic/Spatial",
    "Logic/Cloning",
    "Logic/Switching",
    "Logic/Assets",
    "Logic/Input",
    "Logic/Switching",
    "Logic/Triggering",
    "Logic/Scenes",
    "Logic/Spawning",
    
    "Rendering/",
    "Libs/Zip",
    "Libs/STBImage",
    "Libs/FreeType/Include",
    "Libs/TinyXml",
    };
}


//
//void ProjectBuilder::BuildIOS(const std::string &outputPath, const std::string &pocketEngineLibPath) {
//
//    const std::string GameName = "Game";
//    std::string gameDir = workingDirectory+"/"+GameName;
//    
//    if (!FileHelper::CreateFolder(gameDir)) {
//    //    return;
//    }
//
//    //plist
//    {
//        std::string plistFile = gameDir+"/Info.plist";
//        std::ofstream file;
//        file.open(plistFile);
//        file << GetXCodePlistTemplate();
//        file.close();
//    }
//    //png
//    {
//        std::string pngFile = gameDir+"/Default-568h@2x.png";
//        std::ofstream file;
//        file.open(pngFile);
//        file << GetSplashPngData();
//        file.close();
//    }
//    //main
//    {
//        std::string cppFile = gameDir+"/main.cpp";
//        std::ofstream file;
//        file.open(cppFile);
//        file << GetMainFile();
//        file.close();
//    }
//
//    std::string xcodeProjectPath = workingDirectory+"/"+GameName+".xcodeproj";
//
//    CreateXCodeProject(xcodeProjectPath);
//    
//    FileReader::RunCommmand("xcodebuild -project "+xcodeProjectPath);
//    
//    std::string builtGamePath = workingDirectory+"/build/Release-iphoneos/"+GameName+".app";
//    
//    std::string payloadPath = workingDirectory +"/Payload";
//    
//    FileHelper::CreateFolder(payloadPath);
//    
//    FileReader::RunCommmand("cp -r " + builtGamePath+" "+ payloadPath);
//    
//    std::string outputIPAPath = outputPath + "/"+GameName+".ipa";
//    
//    FileReader::RunCommmand("cd "+workingDirectory+" && zip -r "+ outputIPAPath+" Payload");
//    
//}
//
//void ProjectBuilder::CreateXCodeProject(const std::string &outputFile) {
//    if (!FileHelper::CreateFolder(outputFile)) {
//    //   return;
//    }
//
//    std::ofstream file;
//    file.open(outputFile+"/project.pbxproj");
//
//    file << GetXCodeProjectTemplate() << std::endl;
//
//    file.close();
//}
//
//
//#define FILE_SOURCE(...) #__VA_ARGS__
//
//
//std::string ProjectBuilder::GetXCodeProjectTemplate() {
//
//std::string file = FILE_SOURCE(
//
//// !$*UTF8*$!
//{
//	archiveVersion = 1;
//	classes = {
//	};
//	objectVersion = 46;
//	objects = {
//
///* Begin PBXBuildFile section */
//		7214E02C1EFAFD6900F61526 /* Default-568h@2x.png in Resources */ = {isa = PBXBuildFile; fileRef = 7214E02B1EFAFD6900F61526 /* Default-568h@2x.png */; };
//		7214E0311EFB035600F61526 /* main.cpp in Sources */ = {isa = PBXBuildFile; fileRef = 7214E0301EFB035600F61526 /* main.cpp */; };
//		7214E0331EFB040500F61526 /* OpenGLES.framework in Frameworks */ = {isa = PBXBuildFile; fileRef = 7214E0321EFB040500F61526 /* OpenGLES.framework */; };
//		7214E03D1EFB083700F61526 /* Foundation.framework in Frameworks */ = {isa = PBXBuildFile; fileRef = 7214E03C1EFB083700F61526 /* Foundation.framework */; };
//		7214E03F1EFB086000F61526 /* /Projects/PocketEngine/Projects/Libraries/iOS/PocketEngine/Build/Build/Products/Debug-iphoneos/libPocketEngine.a in Frameworks */ = {isa = PBXBuildFile; fileRef = 7214E03E1EFB086000F61526 /* /Projects/PocketEngine/Projects/Libraries/iOS/PocketEngine/Build/Build/Products/Debug-iphoneos/libPocketEngine.a */; };
//		7214E0411EFB087C00F61526 /* UIKit.framework in Frameworks */ = {isa = PBXBuildFile; fileRef = 7214E0401EFB087C00F61526 /* UIKit.framework */; };
//		7214E0431EFB08BD00F61526 /* QuartzCore.framework in Frameworks */ = {isa = PBXBuildFile; fileRef = 7214E0421EFB08BD00F61526 /* QuartzCore.framework */; };
//		7214E0451EFB08DC00F61526 /* CoreGraphics.framework in Frameworks */ = {isa = PBXBuildFile; fileRef = 7214E0441EFB08DC00F61526 /* CoreGraphics.framework */; };
//		7214E0471EFB091B00F61526 /* OpenAL.framework in Frameworks */ = {isa = PBXBuildFile; fileRef = 7214E0461EFB091B00F61526 /* OpenAL.framework */; };
///* End PBXBuildFile section */
//
///* Begin PBXFileReference section */
//		7214E0111EFAFC8000F61526 /* Game.app */ = {isa = PBXFileReference; explicitFileType = wrapper.application; includeInIndex = 0; path = Game.app; sourceTree = BUILT_PRODUCTS_DIR; };
//		7214E0251EFAFC8000F61526 /* Info.plist */ = {isa = PBXFileReference; lastKnownFileType = text.plist.xml; path = Info.plist; sourceTree = "<group>"; };
//		7214E02B1EFAFD6900F61526 /* Default-568h@2x.png */ = {isa = PBXFileReference; lastKnownFileType = image.png; path = "Default-568h@2x.png"; sourceTree = "<group>"; };
//		7214E0301EFB035600F61526 /* main.cpp */ = {isa = PBXFileReference; fileEncoding = 4; lastKnownFileType = sourcecode.cpp.cpp; path = main.cpp; sourceTree = "<group>"; };
//		7214E0321EFB040500F61526 /* OpenGLES.framework */ = {isa = PBXFileReference; lastKnownFileType = wrapper.framework; name = OpenGLES.framework; path = System/Library/Frameworks/OpenGLES.framework; sourceTree = SDKROOT; };
//		7214E03C1EFB083700F61526 /* Foundation.framework */ = {isa = PBXFileReference; lastKnownFileType = wrapper.framework; name = Foundation.framework; path = System/Library/Frameworks/Foundation.framework; sourceTree = SDKROOT; };
//		7214E03E1EFB086000F61526 /* /Projects/PocketEngine/Projects/Libraries/iOS/PocketEngine/Build/Build/Products/Debug-iphoneos/libPocketEngine.a */ = {isa = PBXFileReference; lastKnownFileType = archive.ar; path = "/Projects/PocketEngine/Projects/Libraries/iOS/PocketEngine/Build/Build/Products/Debug-iphoneos/libPocketEngine.a"; sourceTree = "<group>"; };
//		7214E0401EFB087C00F61526 /* UIKit.framework */ = {isa = PBXFileReference; lastKnownFileType = wrapper.framework; name = UIKit.framework; path = System/Library/Frameworks/UIKit.framework; sourceTree = SDKROOT; };
//		7214E0421EFB08BD00F61526 /* QuartzCore.framework */ = {isa = PBXFileReference; lastKnownFileType = wrapper.framework; name = QuartzCore.framework; path = System/Library/Frameworks/QuartzCore.framework; sourceTree = SDKROOT; };
//		7214E0441EFB08DC00F61526 /* CoreGraphics.framework */ = {isa = PBXFileReference; lastKnownFileType = wrapper.framework; name = CoreGraphics.framework; path = System/Library/Frameworks/CoreGraphics.framework; sourceTree = SDKROOT; };
//		7214E0461EFB091B00F61526 /* OpenAL.framework */ = {isa = PBXFileReference; lastKnownFileType = wrapper.framework; name = OpenAL.framework; path = System/Library/Frameworks/OpenAL.framework; sourceTree = SDKROOT; };
///* End PBXFileReference section */
//
///* Begin PBXFrameworksBuildPhase section */
//		7214E00E1EFAFC8000F61526 /* Frameworks */ = {
//			isa = PBXFrameworksBuildPhase;
//			buildActionMask = 2147483647;
//			files = (
//				7214E0471EFB091B00F61526 /* OpenAL.framework in Frameworks */,
//				7214E0451EFB08DC00F61526 /* CoreGraphics.framework in Frameworks */,
//				7214E0431EFB08BD00F61526 /* QuartzCore.framework in Frameworks */,
//				7214E0411EFB087C00F61526 /* UIKit.framework in Frameworks */,
//				7214E03D1EFB083700F61526 /* Foundation.framework in Frameworks */,
//				7214E03F1EFB086000F61526 /* /Projects/PocketEngine/Projects/Libraries/iOS/PocketEngine/Build/Build/Products/Debug-iphoneos/libPocketEngine.a in Frameworks */,
//				7214E0331EFB040500F61526 /* OpenGLES.framework in Frameworks */,
//			);
//			runOnlyForDeploymentPostprocessing = 0;
//		};
///* End PBXFrameworksBuildPhase section */
//
///* Begin PBXGroup section */
//		7214E0081EFAFC8000F61526 = {
//			isa = PBXGroup;
//			children = (
//				7214E0131EFAFC8000F61526 /* Game */,
//				7214E0121EFAFC8000F61526 /* Products */,
//				7214E02D1EFB022700F61526 /* Frameworks */,
//			);
//			sourceTree = "<group>";
//		};
//		7214E0121EFAFC8000F61526 /* Products */ = {
//			isa = PBXGroup;
//			children = (
//				7214E0111EFAFC8000F61526 /* Game.app */,
//			);
//			name = Products;
//			sourceTree = "<group>";
//		};
//		7214E0131EFAFC8000F61526 /* Game */ = {
//			isa = PBXGroup;
//			children = (
//				7214E02B1EFAFD6900F61526 /* Default-568h@2x.png */,
//				7214E0251EFAFC8000F61526 /* Info.plist */,
//				7214E0301EFB035600F61526 /* main.cpp */,
//			);
//			path = Game;
//			sourceTree = "<group>";
//		};
//		7214E02D1EFB022700F61526 /* Frameworks */ = {
//			isa = PBXGroup;
//			children = (
//				7214E0461EFB091B00F61526 /* OpenAL.framework */,
//				7214E0441EFB08DC00F61526 /* CoreGraphics.framework */,
//				7214E0421EFB08BD00F61526 /* QuartzCore.framework */,
//				7214E0401EFB087C00F61526 /* UIKit.framework */,
//				7214E03E1EFB086000F61526 /* /Projects/PocketEngine/Projects/Libraries/iOS/PocketEngine/Build/Build/Products/Debug-iphoneos/libPocketEngine.a */,
//				7214E03C1EFB083700F61526 /* Foundation.framework */,
//				7214E0321EFB040500F61526 /* OpenGLES.framework */,
//			);
//			name = Frameworks;
//			sourceTree = "<group>";
//		};
///* End PBXGroup section */
//
///* Begin PBXNativeTarget section */
//		7214E0101EFAFC8000F61526 /* Game */ = {
//			isa = PBXNativeTarget;
//			buildConfigurationList = 7214E0281EFAFC8000F61526 /* Build configuration list for PBXNativeTarget "Game" */;
//			buildPhases = (
//				7214E00D1EFAFC8000F61526 /* Sources */,
//				7214E00E1EFAFC8000F61526 /* Frameworks */,
//				7214E00F1EFAFC8000F61526 /* Resources */,
//			);
//			buildRules = (
//			);
//			dependencies = (
//			);
//			name = Game;
//			productName = Game;
//			productReference = 7214E0111EFAFC8000F61526 /* Game.app */;
//			productType = "com.apple.product-type.application";
//		};
///* End PBXNativeTarget section */
//
///* Begin PBXProject section */
//		7214E0091EFAFC8000F61526 /* Project object */ = {
//			isa = PBXProject;
//			attributes = {
//				LastUpgradeCheck = 0820;
//				ORGANIZATIONNAME = "Jeppe Nielsen";
//				TargetAttributes = {
//					7214E0101EFAFC8000F61526 = {
//						CreatedOnToolsVersion = 8.2.1;
//						DevelopmentTeam = 24H2455352;
//						ProvisioningStyle = Automatic;
//					};
//				};
//			};
//			buildConfigurationList = 7214E00C1EFAFC8000F61526 /* Build configuration list for PBXProject "Game" */;
//			compatibilityVersion = "Xcode 3.2";
//			developmentRegion = English;
//			hasScannedForEncodings = 0;
//			knownRegions = (
//				en,
//				Base,
//			);
//			mainGroup = 7214E0081EFAFC8000F61526;
//			productRefGroup = 7214E0121EFAFC8000F61526 /* Products */;
//			projectDirPath = "";
//			projectRoot = "";
//			targets = (
//				7214E0101EFAFC8000F61526 /* Game */,
//			);
//		};
///* End PBXProject section */
//
///* Begin PBXResourcesBuildPhase section */
//		7214E00F1EFAFC8000F61526 /* Resources */ = {
//			isa = PBXResourcesBuildPhase;
//			buildActionMask = 2147483647;
//			files = (
//				7214E02C1EFAFD6900F61526 /* Default-568h@2x.png in Resources */,
//			);
//			runOnlyForDeploymentPostprocessing = 0;
//		};
///* End PBXResourcesBuildPhase section */
//
///* Begin PBXSourcesBuildPhase section */
//		7214E00D1EFAFC8000F61526 /* Sources */ = {
//			isa = PBXSourcesBuildPhase;
//			buildActionMask = 2147483647;
//			files = (
//				7214E0311EFB035600F61526 /* main.cpp in Sources */,
//			);
//			runOnlyForDeploymentPostprocessing = 0;
//		};
///* End PBXSourcesBuildPhase section */
//
///* Begin XCBuildConfiguration section */
//		7214E0261EFAFC8000F61526 /* Debug */ = {
//			isa = XCBuildConfiguration;
//			buildSettings = {
//				ALWAYS_SEARCH_USER_PATHS = NO;
//				CLANG_ANALYZER_NONNULL = YES;
//				CLANG_CXX_LANGUAGE_STANDARD = "gnu++0x";
//				CLANG_CXX_LIBRARY = "libc++";
//				CLANG_ENABLE_MODULES = YES;
//				CLANG_ENABLE_OBJC_ARC = YES;
//				CLANG_WARN_BOOL_CONVERSION = YES;
//				CLANG_WARN_CONSTANT_CONVERSION = YES;
//				CLANG_WARN_DIRECT_OBJC_ISA_USAGE = YES_ERROR;
//				CLANG_WARN_DOCUMENTATION_COMMENTS = YES;
//				CLANG_WARN_EMPTY_BODY = YES;
//				CLANG_WARN_ENUM_CONVERSION = YES;
//				CLANG_WARN_INFINITE_RECURSION = YES;
//				CLANG_WARN_INT_CONVERSION = YES;
//				CLANG_WARN_OBJC_ROOT_CLASS = YES_ERROR;
//				CLANG_WARN_SUSPICIOUS_MOVE = YES;
//				CLANG_WARN_UNREACHABLE_CODE = YES;
//				CLANG_WARN__DUPLICATE_METHOD_MATCH = YES;
//				"CODE_SIGN_IDENTITY[sdk=iphoneos*]" = "iPhone Developer";
//				COPY_PHASE_STRIP = NO;
//				DEBUG_INFORMATION_FORMAT = dwarf;
//				ENABLE_STRICT_OBJC_MSGSEND = YES;
//				ENABLE_TESTABILITY = YES;
//				GCC_C_LANGUAGE_STANDARD = gnu99;
//				GCC_DYNAMIC_NO_PIC = NO;
//				GCC_NO_COMMON_BLOCKS = YES;
//				GCC_OPTIMIZATION_LEVEL = 0;
//				GCC_PREPROCESSOR_DEFINITIONS = (
//					"DEBUG=1",
//					"$(inherited)",
//				);
//				GCC_WARN_64_TO_32_BIT_CONVERSION = YES;
//				GCC_WARN_ABOUT_RETURN_TYPE = YES_ERROR;
//				GCC_WARN_UNDECLARED_SELECTOR = YES;
//				GCC_WARN_UNINITIALIZED_AUTOS = YES_AGGRESSIVE;
//				GCC_WARN_UNUSED_FUNCTION = YES;
//				GCC_WARN_UNUSED_VARIABLE = YES;
//				IPHONEOS_DEPLOYMENT_TARGET = 8.0;
//				MTL_ENABLE_DEBUG_INFO = YES;
//				ONLY_ACTIVE_ARCH = YES;
//				SDKROOT = iphoneos;
//				TARGETED_DEVICE_FAMILY = "1,2";
//			};
//			name = Debug;
//		};
//		7214E0271EFAFC8000F61526 /* Release */ = {
//			isa = XCBuildConfiguration;
//			buildSettings = {
//				ALWAYS_SEARCH_USER_PATHS = NO;
//				CLANG_ANALYZER_NONNULL = YES;
//				CLANG_CXX_LANGUAGE_STANDARD = "gnu++0x";
//				CLANG_CXX_LIBRARY = "libc++";
//				CLANG_ENABLE_MODULES = YES;
//				CLANG_ENABLE_OBJC_ARC = YES;
//				CLANG_WARN_BOOL_CONVERSION = YES;
//				CLANG_WARN_CONSTANT_CONVERSION = YES;
//				CLANG_WARN_DIRECT_OBJC_ISA_USAGE = YES_ERROR;
//				CLANG_WARN_DOCUMENTATION_COMMENTS = YES;
//				CLANG_WARN_EMPTY_BODY = YES;
//				CLANG_WARN_ENUM_CONVERSION = YES;
//				CLANG_WARN_INFINITE_RECURSION = YES;
//				CLANG_WARN_INT_CONVERSION = YES;
//				CLANG_WARN_OBJC_ROOT_CLASS = YES_ERROR;
//				CLANG_WARN_SUSPICIOUS_MOVE = YES;
//				CLANG_WARN_UNREACHABLE_CODE = YES;
//				CLANG_WARN__DUPLICATE_METHOD_MATCH = YES;
//				"CODE_SIGN_IDENTITY[sdk=iphoneos*]" = "iPhone Developer";
//				COPY_PHASE_STRIP = NO;
//				DEBUG_INFORMATION_FORMAT = "dwarf-with-dsym";
//				ENABLE_NS_ASSERTIONS = NO;
//				ENABLE_STRICT_OBJC_MSGSEND = YES;
//				GCC_C_LANGUAGE_STANDARD = gnu99;
//				GCC_NO_COMMON_BLOCKS = YES;
//				GCC_WARN_64_TO_32_BIT_CONVERSION = YES;
//				GCC_WARN_ABOUT_RETURN_TYPE = YES_ERROR;
//				GCC_WARN_UNDECLARED_SELECTOR = YES;
//				GCC_WARN_UNINITIALIZED_AUTOS = YES_AGGRESSIVE;
//				GCC_WARN_UNUSED_FUNCTION = YES;
//				GCC_WARN_UNUSED_VARIABLE = YES;
//				IPHONEOS_DEPLOYMENT_TARGET = 8.0;
//				MTL_ENABLE_DEBUG_INFO = NO;
//				SDKROOT = iphoneos;
//				TARGETED_DEVICE_FAMILY = "1,2";
//				VALIDATE_PRODUCT = YES;
//			};
//			name = Release;
//		};
//		7214E0291EFAFC8000F61526 /* Debug */ = {
//			isa = XCBuildConfiguration;
//			buildSettings = {
//				ARCHS = armv7;
//				ASSETCATALOG_COMPILER_APPICON_NAME = AppIcon;
//				CLANG_CXX_LANGUAGE_STANDARD = "gnu++14";
//				DEVELOPMENT_TEAM = 24H2455352;
//				GCC_PREPROCESSOR_DEFINITIONS = (
//					"DEBUG=1",
//					"IPHONE=1",
//				);
//				HEADER_SEARCH_PATHS = (
//					POCKET_ENGINE_HEADERS
//				);
//				INFOPLIST_FILE = Game/Info.plist;
//				LD_RUNPATH_SEARCH_PATHS = "$(inherited) @executable_path/Frameworks";
//				LIBRARY_SEARCH_PATHS = (
//					"$(inherited)",
//					"$(PROJECT_DIR)",
//					"/Projects/PocketEngine/Projects/Libraries/iOS/PocketEngine/Build/Build/Products/Debug-iphoneos",
//				);
//				PRODUCT_BUNDLE_IDENTIFIER = dk.jeppenielsen.Game;
//				PRODUCT_NAME = "$(TARGET_NAME)";
//				VALID_ARCHS = armv7;
//			};
//			name = Debug;
//		};
//		7214E02A1EFAFC8000F61526 /* Release */ = {
//			isa = XCBuildConfiguration;
//			buildSettings = {
//				ARCHS = armv7;
//				ASSETCATALOG_COMPILER_APPICON_NAME = AppIcon;
//				CLANG_CXX_LANGUAGE_STANDARD = "gnu++14";
//				DEVELOPMENT_TEAM = 24H2455352;
//				GCC_PREPROCESSOR_DEFINITIONS = "IPHONE=1";
//				HEADER_SEARCH_PATHS = (
//					POCKET_ENGINE_HEADERS
//				);
//				INFOPLIST_FILE = Game/Info.plist;
//				LD_RUNPATH_SEARCH_PATHS = "$(inherited) @executable_path/Frameworks";
//				LIBRARY_SEARCH_PATHS = (
//					"$(inherited)",
//					"$(PROJECT_DIR)",
//					"/Projects/PocketEngine/Projects/Libraries/iOS/PocketEngine/Build/Build/Products/Debug-iphoneos",
//				);
//				PRODUCT_BUNDLE_IDENTIFIER = dk.jeppenielsen.Game;
//				PRODUCT_NAME = "$(TARGET_NAME)";
//				VALID_ARCHS = armv7;
//			};
//			name = Release;
//		};
///* End XCBuildConfiguration section */
//
///* Begin XCConfigurationList section */
//		7214E00C1EFAFC8000F61526 /* Build configuration list for PBXProject "Game" */ = {
//			isa = XCConfigurationList;
//			buildConfigurations = (
//				7214E0261EFAFC8000F61526 /* Debug */,
//				7214E0271EFAFC8000F61526 /* Release */,
//			);
//			defaultConfigurationIsVisible = 0;
//			defaultConfigurationName = Release;
//		};
//		7214E0281EFAFC8000F61526 /* Build configuration list for PBXNativeTarget "Game" */ = {
//			isa = XCConfigurationList;
//			buildConfigurations = (
//				7214E0291EFAFC8000F61526 /* Debug */,
//				7214E02A1EFAFC8000F61526 /* Release */,
//			);
//			defaultConfigurationIsVisible = 0;
//			defaultConfigurationName = Release;
//		};
///* End XCConfigurationList section */
//	};
//	rootObject = 7214E0091EFAFC8000F61526 /* Project object */;
//}
//
//
//);
//    
//    std::string headerPathStr = "";
//    auto headerPaths = GetEngineHeaders();
//    
//    std::string startQuote = StringHelper::base64_decode("Ilwi");
//    std::string endQuote = StringHelper::base64_decode("XCIi");
//    
//    for(auto& h : headerPaths) {
//        headerPathStr += (startQuote +
//            pocketEngineIncludePath + "/" + h + endQuote + ",");
//    }
//    
//    file = StringHelper::FindAndReplaceAll(file, "POCKET_ENGINE_HEADERS", headerPathStr);
//
//    return file;
//}
//
//std::string ProjectBuilder::GetXCodePlistTemplate() {
//    std::string plist = FILE_SOURCE(
//        <?xml version="1.0" encoding="UTF-8"?>
//<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
//<plist version="1.0">
//<dict>
//	<key>UIViewControllerBasedStatusBarAppearance</key>
//	<false/>
//	<key>UIStatusBar</key>
//	<string></string>
//	<key>UIStatusBarHidden</key>
//	<true/>
//	<key>CFBundleDevelopmentRegion</key>
//	<string>en</string>
//	<key>CFBundleExecutable</key>
//	<string>$(EXECUTABLE_NAME)</string>
//	<key>CFBundleIdentifier</key>
//	<string>dk.jeppenielsen.nano</string>
//	<key>CFBundleInfoDictionaryVersion</key>
//	<string>6.0</string>
//	<key>CFBundleName</key>
//	<string>$(PRODUCT_NAME)</string>
//	<key>CFBundlePackageType</key>
//	<string>APPL</string>
//	<key>CFBundleShortVersionString</key>
//	<string>1.0</string>
//	<key>CFBundleVersion</key>
//	<string>1</string>
//	<key>LSRequiresIPhoneOS</key>
//	<true/>
//	<key>UILaunchStoryboardName</key>
//	<string>LaunchScreen</string>
//	<key>UIMainStoryboardFile</key>
//	<string></string>
//	<key>UIRequiredDeviceCapabilities</key>
//	<array>
//		<string>armv7</string>
//	</array>
//	<key>UISupportedInterfaceOrientations</key>
//	<array>
//		<string>UIInterfaceOrientationPortrait</string>
//		<string>UIInterfaceOrientationLandscapeLeft</string>
//		<string>UIInterfaceOrientationLandscapeRight</string>
//	</array>
//	<key>UISupportedInterfaceOrientations~ipad</key>
//	<array>
//		<string>UIInterfaceOrientationPortrait</string>
//		<string>UIInterfaceOrientationPortraitUpsideDown</string>
//		<string>UIInterfaceOrientationLandscapeLeft</string>
//		<string>UIInterfaceOrientationLandscapeRight</string>
//	</array>
//</dict>
//</plist>
//    );
//    return plist;
//}
//
//std::string ProjectBuilder::GetSplashPngData() {
//    std::string pngData = "iVBORw0KGgoAAAANSUhEUgAAAoAAAARwAQMAAABAQXdDAAAAA1BMVEX39/dv1UhKAAAAfElEQVR42uzBgQAAAACAoP2pF6kCAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAOD24IAEAAAAQND/1+0IVAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAYCJnfwABjvgOPgAAAABJRU5ErkJggg==";
//    return StringHelper::base64_decode(pngData);
//}
//
//std::string ProjectBuilder::GetMainFile() {
//    std::string file = "";
//    
//   file+="#include \"Engine.hpp\" \n";
//   file+="#include \"GameWorld.hpp\" \n";
//   file+="#include \"RenderSystem.hpp\" \n";
//   file+="#include \"TouchSystem.hpp\" \n";
//   file+="#include \"DraggableSystem.hpp\" \n";
//
//   file+= FILE_SOURCE(
//
//
//using namespace Pocket;
//
//class Game : public GameState<Game> {
//public:
//    GameWorld world;
//    GameObject* root;
//    GameObject* camera;
//    GameObject* cube;
//    float rotation;
//    
//    void Initialize() {
//    
//        root = world.CreateRoot();
//        
//        root->CreateSystem<RenderSystem>();
//        root->CreateSystem<TouchSystem>();
//        root->CreateSystem<DraggableSystem>();
//        
//        camera = root->CreateObject();
//        camera->AddComponent<Camera>();
//        camera->AddComponent<Transform>()->Position = { 0, 0, 10 };
//        camera->GetComponent<Camera>()->FieldOfView = 40;
//        
//        cube = root->CreateObject();
//        cube->AddComponent<Transform>();
//        cube->AddComponent<Mesh>()->GetMesh<Vertex>().AddCube(0, 1);
//        cube->AddComponent<Renderable>();
//        
//        auto& verts = cube->GetComponent<Mesh>()->GetMesh<Vertex>().vertices;
//        
//        for (int i=0; i<verts.size(); i++) {
//            verts[i].Color = Colour::HslToRgb(i * 10, 1, 1, 1);
//        }
//        
//        cube->AddComponent<Draggable>();
//        cube->AddComponent<Touchable>();
//        
//        rotation = 0;
//    }
//    
//    void Update(float dt) {
//        Context().InputDevice().UpdateInputManager(&world.Input());
//        cube->GetComponent<Transform>()->Rotation = Quaternion(rotation, Vector3(1,0.2f,0.5f).Normalized());
//        rotation += dt;
//        world.Update(dt);
//    }
//    
//    void Render() {
//        world.Render();
//    }
//};
//
//int main() {
//    Engine e;
//    e.Start<Game>();
//	return 0;
//}
//    );
//    
//    return file;
//}


