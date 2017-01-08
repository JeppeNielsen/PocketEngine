//
//  FileSystemWatcher.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 12/09/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "FileSystemWatcher.hpp"
#include <CoreServices/CoreServices.h>
#include <iostream>

using namespace Pocket;
       /**
     * Callback function of the OS X FSEvent API.
     */
static void fileSystemEventCallback(ConstFSEventStreamRef streamRef, void *clientCallBackInfo, size_t numEvents, void *eventPaths, const FSEventStreamEventFlags eventFlags[], const FSEventStreamEventId eventIds[]) {

    char **paths = (char **)eventPaths;
    FileSystemWatcher *watcher = (FileSystemWatcher *)clientCallBackInfo;

    if (numEvents>0) {
        watcher->Changed();
    }

    for (size_t i=0; i<numEvents; i++) {
        // When a file is created we receive first a kFSEventStreamEventFlagItemCreated and second a (kFSEventStreamEventFlagItemCreated & kFSEventStreamEventFlagItemModified)
        // when the file is finally copied. Catch this second event.
/*
        std::cout << "File event start: " <<paths[i] <<std::endl;
        if (eventFlags[i] & kFSEventStreamEventFlagMustScanSubDirs) {
            std::cout << "kFSEventStreamEventFlagMustScanSubDirs"<<std::endl;
        }
        
        if (eventFlags[i] & kFSEventStreamEventFlagUserDropped) {
            std::cout << "kFSEventStreamEventFlagUserDropped"<<std::endl;
        }
        
        if (eventFlags[i] & kFSEventStreamEventFlagKernelDropped) {
            std::cout << "kFSEventStreamEventFlagKernelDropped"<<std::endl;
        }
        
        if (eventFlags[i] & kFSEventStreamEventFlagEventIdsWrapped) {
            std::cout << "kFSEventStreamEventFlagEventIdsWrapped"<<std::endl;
        }
        
        if (eventFlags[i] & kFSEventStreamEventFlagHistoryDone) {
            std::cout << "kFSEventStreamEventFlagHistoryDone"<<std::endl;
        }
        
        if (eventFlags[i] & kFSEventStreamEventFlagRootChanged) {
            std::cout << "kFSEventStreamEventFlagRootChanged"<<std::endl;
        }
        
        if (eventFlags[i] & kFSEventStreamEventFlagMount) {
            std::cout << "kFSEventStreamEventFlagMount"<<std::endl;
        }
        
        if (eventFlags[i] & kFSEventStreamEventFlagUnmount) {
            std::cout << "kFSEventStreamEventFlagUnmount"<<std::endl;
        }
        
        if (eventFlags[i] & kFSEventStreamEventFlagItemCreated) {
            std::cout << "kFSEventStreamEventFlagItemCreated"<<std::endl;
        }
        
        if (eventFlags[i] & kFSEventStreamEventFlagItemRemoved) {
            std::cout << "kFSEventStreamEventFlagItemRemoved"<<std::endl;
        }
        
        if (eventFlags[i] & kFSEventStreamEventFlagItemInodeMetaMod) {
            std::cout << "kFSEventStreamEventFlagItemInodeMetaMod"<<std::endl;
        }
        
        if (eventFlags[i] & kFSEventStreamEventFlagItemRenamed) {
            std::cout << "kFSEventStreamEventFlagItemRenamed"<<std::endl;
        }
        
        if (eventFlags[i] & kFSEventStreamEventFlagItemModified) {
            std::cout << "kFSEventStreamEventFlagItemModified"<<std::endl;
        }
        
        if (eventFlags[i] & kFSEventStreamEventFlagItemFinderInfoMod) {
            std::cout << "kFSEventStreamEventFlagItemFinderInfoMod"<<std::endl;
        }
        
        if (eventFlags[i] & kFSEventStreamEventFlagItemChangeOwner) {
            std::cout << "kFSEventStreamEventFlagItemChangeOwner"<<std::endl;
        }
        
        if (eventFlags[i] & kFSEventStreamEventFlagItemXattrMod) {
            std::cout << "kFSEventStreamEventFlagItemXattrMod"<<std::endl;
        }
        
        if (eventFlags[i] & kFSEventStreamEventFlagItemIsFile) {
            std::cout << "kFSEventStreamEventFlagItemIsFile"<<std::endl;
        }
        
        
        
        if (eventFlags[i] & kFSEventStreamEventFlagItemIsDir) {
            std::cout << "kFSEventStreamEventFlagItemIsDir"<<std::endl;
        }


        if (eventFlags[i] & kFSEventStreamEventFlagItemIsSymlink) {
            std::cout << "kFSEventStreamEventFlagItemIsSymlink"<<std::endl;
        }
        
        if (eventFlags[i] & kFSEventStreamEventFlagOwnEvent) {
            std::cout << "kFSEventStreamEventFlagOwnEvent"<<std::endl;
        }

        if (eventFlags[i] & kFSEventStreamEventFlagItemIsHardlink) {
            std::cout << "kFSEventStreamEventFlagItemIsHardlink"<<std::endl;
        }
        
        if (eventFlags[i] & kFSEventStreamEventFlagItemIsLastHardlink) {
            std::cout << "kFSEventStreamEventFlagItemIsLastHardlink"<<std::endl;
        }
        
         std::cout << "----end----"<<std::endl;
*/

        if (eventFlags[i] & kFSEventStreamEventFlagItemModified
            && eventFlags[i] & kFSEventStreamEventFlagItemIsFile
                && !(eventFlags[i] & kFSEventStreamEventFlagItemIsDir)
                && !(eventFlags[i] & kFSEventStreamEventFlagItemIsSymlink)) {
            //std::cout << "FILE Modified: " << paths[i]<<std::endl;
            watcher->FileModified(paths[i]);
        }
    }
    
    
        
//        if (eventFlags[i] & kFSEventStreamEventFlagItemCreated
//                && !(eventFlags[i] & kFSEventStreamEventFlagItemIsDir)
//                && !(eventFlags[i] & kFSEventStreamEventFlagItemIsSymlink)
//                && !(eventFlags[i] & kFSEventStreamEventFlagItemFinderInfoMod)) {
//            std::cout << "FILE Created: " << paths[i]<<std::endl;
//        }
    
    //}
        
//        if (eventFlags[i] & kFSEventStreamEventFlagItemModified
//                && !(eventFlags[i] & kFSEventStreamEventFlagItemIsDir)
//                && !(eventFlags[i] & kFSEventStreamEventFlagItemIsSymlink)
//                && !(eventFlags[i] & kFSEventStreamEventFlagItemFinderInfoMod)) {
//            std::cout << "FILE Modified: " << paths[i]<<std::endl;
//        }
//        
//        if (eventFlags[i] & kFSEventStreamEventFlagItemRemoved
//                && !(eventFlags[i] & kFSEventStreamEventFlagItemIsDir)
//                && !(eventFlags[i] & kFSEventStreamEventFlagItemIsSymlink)
//                && !(eventFlags[i] & kFSEventStreamEventFlagItemFinderInfoMod)) {
//            std::cout << "FILE Removed: " << paths[i]<<std::endl;
//        }
//        
//        if (eventFlags[i] & kFSEventStreamEventFlagItemRenamed
//                && !(eventFlags[i] & kFSEventStreamEventFlagItemIsDir)
//                && !(eventFlags[i] & kFSEventStreamEventFlagItemIsSymlink)
//                && !(eventFlags[i] & kFSEventStreamEventFlagItemFinderInfoMod)) {
//            std::cout << "FILE Renamed: " << paths[i]<<std::endl;
//        }
//            
//        /*
//        if (eventFlags[i] & kFSEventStreamEventFlagItemCreated
//                && eventFlags[i] & kFSEventStreamEventFlagItemModified
//                && !(eventFlags[i] & kFSEventStreamEventFlagItemIsDir)
//                && !(eventFlags[i] & kFSEventStreamEventFlagItemIsSymlink)
//                && !(eventFlags[i] & kFSEventStreamEventFlagItemFinderInfoMod)) {
//
//            FileSystemWatcher *watcher = (FileSystemWatcher *)clientCallBackInfo;
//            //if (watcher->FileValidator(paths[i]))
//            //    emit watcher->yourSignalHere();
//            
//            std::cout << "FILE changed: " << paths[i]<<std::endl;
//            
//        }
//        */
//    }
    
    
}

void FileSystemWatcher::Start(const std::string& pathToWatch) {
    path = pathToWatch;
    CFStringRef pathToWatchCF = CFStringCreateWithCString(NULL, pathToWatch.c_str(), kCFStringEncodingUTF8);
    CFArrayRef pathsToWatch = CFArrayCreate(NULL, (const void **)&pathToWatchCF, 1, NULL);

    FSEventStreamContext context;
    context.version = 0;
    context.info = this;
    context.retain = NULL;
    context.release = NULL;
    context.copyDescription = NULL;
    
    FSEventStreamRef stream;


    stream = FSEventStreamCreate(NULL, &fileSystemEventCallback, &context, pathsToWatch, kFSEventStreamEventIdSinceNow, 1.0, kFSEventStreamCreateFlagFileEvents);
    FSEventStreamScheduleWithRunLoop(stream, CFRunLoopGetCurrent(), kCFRunLoopDefaultMode);
    FSEventStreamStart(stream);

    CFRelease(pathToWatchCF);
    
    streamPtr = stream;

    // Read the folder content to protect any unprotected or pending file
    //ReadFolderContent();
}

void FileSystemWatcher::Stop() {
    FSEventStreamRef stream = (FSEventStreamRef)streamPtr;

    FSEventStreamStop(stream);
    FSEventStreamInvalidate(stream);
    FSEventStreamRelease(stream);
}

const std::string& FileSystemWatcher::Path() {
    return path;
}
