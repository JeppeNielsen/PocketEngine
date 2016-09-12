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

       /**
     * Callback function of the OS X FSEvent API.
     */
static void fileSystemEventCallback(ConstFSEventStreamRef streamRef, void *clientCallBackInfo, size_t numEvents, void *eventPaths, const FSEventStreamEventFlags eventFlags[], const FSEventStreamEventId eventIds[]) {

     char **paths = (char **)eventPaths;
    
     if (numEvents>0) {
        FileSystemWatcher *watcher = (FileSystemWatcher *)clientCallBackInfo;
        watcher->Changed();
    }
    return;
    
    for (size_t i=0; i<numEvents; i++) {
        // When a file is created we receive first a kFSEventStreamEventFlagItemCreated and second a (kFSEventStreamEventFlagItemCreated & kFSEventStreamEventFlagItemModified)
        // when the file is finally copied. Catch this second event.
        
        
        if (eventFlags[i] & kFSEventStreamEventFlagItemCreated
                && !(eventFlags[i] & kFSEventStreamEventFlagItemIsDir)
                && !(eventFlags[i] & kFSEventStreamEventFlagItemIsSymlink)
                && !(eventFlags[i] & kFSEventStreamEventFlagItemFinderInfoMod)) {
            std::cout << "FILE Created: " << paths[i]<<std::endl;
        }
        
        if (eventFlags[i] & kFSEventStreamEventFlagItemModified
                && !(eventFlags[i] & kFSEventStreamEventFlagItemIsDir)
                && !(eventFlags[i] & kFSEventStreamEventFlagItemIsSymlink)
                && !(eventFlags[i] & kFSEventStreamEventFlagItemFinderInfoMod)) {
            std::cout << "FILE Modified: " << paths[i]<<std::endl;
        }
        
        if (eventFlags[i] & kFSEventStreamEventFlagItemRemoved
                && !(eventFlags[i] & kFSEventStreamEventFlagItemIsDir)
                && !(eventFlags[i] & kFSEventStreamEventFlagItemIsSymlink)
                && !(eventFlags[i] & kFSEventStreamEventFlagItemFinderInfoMod)) {
            std::cout << "FILE Removed: " << paths[i]<<std::endl;
        }
        
        if (eventFlags[i] & kFSEventStreamEventFlagItemRenamed
                && !(eventFlags[i] & kFSEventStreamEventFlagItemIsDir)
                && !(eventFlags[i] & kFSEventStreamEventFlagItemIsSymlink)
                && !(eventFlags[i] & kFSEventStreamEventFlagItemFinderInfoMod)) {
            std::cout << "FILE Renamed: " << paths[i]<<std::endl;
        }
            
        /*
        if (eventFlags[i] & kFSEventStreamEventFlagItemCreated
                && eventFlags[i] & kFSEventStreamEventFlagItemModified
                && !(eventFlags[i] & kFSEventStreamEventFlagItemIsDir)
                && !(eventFlags[i] & kFSEventStreamEventFlagItemIsSymlink)
                && !(eventFlags[i] & kFSEventStreamEventFlagItemFinderInfoMod)) {

            FileSystemWatcher *watcher = (FileSystemWatcher *)clientCallBackInfo;
            //if (watcher->FileValidator(paths[i]))
            //    emit watcher->yourSignalHere();
            
            std::cout << "FILE changed: " << paths[i]<<std::endl;
            
        }
        */
    }
    
    
}

void FileSystemWatcher::Start(const std::string& pathToWatch) {
    CFStringRef pathToWatchCF = CFStringCreateWithCString(NULL, pathToWatch.c_str(), kCFStringEncodingUTF8);
    CFArrayRef pathsToWatch = CFArrayCreate(NULL, (const void **)&pathToWatchCF, 1, NULL);

    FSEventStreamContext context;
    context.version = 0;
    context.info = this;
    context.retain = NULL;
    context.release = NULL;
    context.copyDescription = NULL;
    
    FSEventStreamRef stream;


    stream = FSEventStreamCreate(NULL, &fileSystemEventCallback, &context, pathsToWatch, kFSEventStreamEventIdSinceNow, 3.0, kFSEventStreamCreateFlagFileEvents);
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
