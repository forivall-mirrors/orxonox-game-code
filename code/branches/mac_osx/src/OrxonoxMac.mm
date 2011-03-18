//
//  OrxonoxMac.mm
//  Orxonox
//
//  Created by Kevin Jonas Young on 14.03.11.
//  Copyright 2011 -. All rights reserved.
//

#import "OrxonoxMac.h"

static int argc_s = 0;
static char** argv_s = 0;

int main(int argc, char** argv)
{
    argc_s = argc;
    argv_s = argv;
    
    int retVal = NSApplicationMain(argc, (const char**)argv);
    
    return retVal;
}

@implementation OrxonoxAppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)notification
{
    exit(main_mac(argc_s, argv_s));
}

@end
