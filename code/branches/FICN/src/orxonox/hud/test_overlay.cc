/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
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
 *      HUD design: Yuning Chai
 *      Implementation: Yuning Chai
 *   Co-authors:
 *      Implementation: Reto Grieder
 *
 */

//#include "OgreMath.h"
//#include "OgreVector3.h"
#include "OgreStringConverter.h"
#include "OgreOverlayManager.h"
#include "OgreOverlay.h"
#include "OgreOverlayElement.h"
#include "OgreRenderWindow.h"
#include "OgreRenderTarget.h"

#include "test_overlay.h"


namespace orxonox {
namespace hud {
  using namespace Ogre;

  TestOverlay::TestOverlay(RenderWindow *window)
    :window_(window)
  {
    //TODO: write safe guard
    overlay_ = OverlayManager::getSingleton().getByName("Core/DebugOverlay");
  }


  TestOverlay::~TestOverlay()
  {
  }


  void TestOverlay::show()
  {
    //TODO: safe guard
    overlay_->show();
  }


  void TestOverlay::hide()
  {
    //TODO: safe guard
    overlay_->hide();
  }


  void TestOverlay::setDebugText(Ogre::String &str)
  {
    debugText_ = str;
  }


  bool TestOverlay::tick(unsigned long, Ogre::Real)
  {
    static String currFps = "Current FPS: ";
    static String avgFps = "Average FPS: ";
    static String bestFps = "Best FPS: ";
    static String worstFps = "Worst FPS: ";
    static String tris = "Triangle Count: ";
    static String batches = "Batch Count: ";

    // update stats when necessary
    try {
      OverlayElement* guiAvg = OverlayManager::getSingleton()
        .getOverlayElement("Core/AverageFps");
      OverlayElement* guiCurr = OverlayManager::getSingleton()
        .getOverlayElement("Core/CurrFps");
      OverlayElement* guiBest = OverlayManager::getSingleton()
        .getOverlayElement("Core/BestFps");
      OverlayElement* guiWorst = OverlayManager::getSingleton()
        .getOverlayElement("Core/WorstFps");

      const RenderTarget::FrameStats& stats = window_->getStatistics();
      guiAvg->setCaption(avgFps + StringConverter::toString(stats.avgFPS));
      guiCurr->setCaption(currFps + StringConverter::toString(stats.lastFPS));
      guiBest->setCaption(bestFps + StringConverter::toString(stats.bestFPS)
        +" "+StringConverter::toString(stats.bestFrameTime)+" ms");
      guiWorst->setCaption(worstFps + StringConverter::toString(stats.worstFPS)
        +" "+StringConverter::toString(stats.worstFrameTime)+" ms");

      OverlayElement* guiTris = OverlayManager::getSingleton()
        .getOverlayElement("Core/NumTris");
      guiTris->setCaption(tris + StringConverter::toString(stats.triangleCount));

      OverlayElement* guiBatches = OverlayManager::getSingleton()
        .getOverlayElement("Core/NumBatches");
      guiBatches->setCaption(batches
        + StringConverter::toString(stats.batchCount));

      OverlayElement* guiDbg = OverlayManager::getSingleton()
        .getOverlayElement("Core/DebugText");
      guiDbg->setCaption(debugText_);
    }
    catch(...) { /* ignore */ }

    return true;
  }
}
}
