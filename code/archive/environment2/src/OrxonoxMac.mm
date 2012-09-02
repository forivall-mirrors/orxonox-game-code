/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
 *                    > www.orxonox.net <
 *
 *
 *   License notice:
 *
 *   This program is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU General Public License
 *   as published by the Free Software Foundation; either version 2
 *   of the License, or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 *   Author:
 *      Kevin J. Young (youngk) 2011
 *   Co-authors:
 *      Fabian Landau (x3n)
 *
 */

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
