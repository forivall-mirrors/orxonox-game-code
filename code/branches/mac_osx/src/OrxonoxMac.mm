//
//  OrxonoxMac.mm
//  Orxonox
//
//  Created by Kevin Jonas Young on 14.03.11.
//  Copyright 2011 -. All rights reserved.
//

#import "OrxonoxMac.h"

//#import <Foundation/Foundation.h>

static int argc_s = 0;
static char** argv_s = 0;

int main(int argc, char** argv)
{
    //NSLog(@"main");
    argc_s = argc;
    argv_s = argv;
    
    //NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
    //[[NSApplication sharedApplication] setDelegate:[[[OrxonoxAppDelegate alloc] init] autorelease]];
    int retVal = NSApplicationMain(argc, (const char**)argv);
    //[pool drain];
    
    return retVal;
}

@implementation OrxonoxAppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)notification
{
    NSLog(@"applicationDidFinishLaunching");
    exit(main_mac(argc_s, argv_s));
}

@end
