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
 *      Felix Schulthess
 *   Co-authors:
 *      ...
 *
 */

#include "OrxonoxStableHeaders.h"

#include "InGameConsole.h"

#include <string>
#include <OgreOverlay.h>
#include <OgreOverlayElement.h>
#include <OgreOverlayManager.h>
#include <OgreOverlayContainer.h>
#include <OgreStringConverter.h>

#include "core/Debug.h"
#include "core/CoreIncludes.h"
#include "core/ConsoleCommand.h"
#include "GraphicsEngine.h"

#define LINES 20

namespace orxonox
{
    using namespace Ogre;

    const float REL_WIDTH = 0.8;
    const float REL_HEIGHT = 0.4;
    const float BLINK = 0.25;

    InGameConsole::InGameConsole(InputBuffer* ib){
        //RegisterObject(InGameConsole);
        ib_ = ib;
        active = false;
        cursor = 0.0;
        init();
    }

    InGameConsole::~InGameConsole(void){
        for(int i=0; i<LINES; i++) delete consoleOverlayTextAreas[i];
        delete consoleOverlayTextAreas;
    }

    void InGameConsole::listen(){
        if(!active) activate();
        print(convert2UTF(this->ib_->get()));
    }

    void InGameConsole::execute(){
        newline();
        if (!CommandExecutor::execute(this->ib_->get())){
            print("Error");
            newline();
        }
        this->ib_->clear();
    }

    void InGameConsole::hintandcomplete(){
        print(CommandExecutor::hint(this->ib_->get()));
        newline();
        this->ib_->set(CommandExecutor::complete(this->ib_->get()));
        print(convert2UTF(this->ib_->get()));
    }

    void InGameConsole::clear(){
        this->ib_->clear();
    }

    void InGameConsole::removeLast(){
        this->ib_->removeLast();
    }

    void InGameConsole::exit(){
        clear();
        deactivate();
        CommandExecutor::execute("setInputMode 2");
    }

    /**
    @brief called once by constructor
    */
    void InGameConsole::init(){
        // for the beginning, don't scroll
        scroll = 0;
        scrollTimer = 0;
        cursor = 0;

        // create overlay and elements
        om = &Ogre::OverlayManager::getSingleton();

        // create a container
        consoleOverlayContainer = static_cast<OverlayContainer*>(om->createOverlayElement("Panel", "container"));
        consoleOverlayContainer->setMetricsMode(Ogre::GMM_RELATIVE);
        consoleOverlayContainer->setPosition((1-REL_WIDTH)/2, 0);
        consoleOverlayContainer->setDimensions(REL_WIDTH, REL_HEIGHT);

        // create BorderPanel
        consoleOverlayBorder = static_cast<BorderPanelOverlayElement*>(om->createOverlayElement("BorderPanel", "borderPanel"));
        consoleOverlayBorder->setMetricsMode(Ogre::GMM_PIXELS);
        consoleOverlayBorder->setMaterialName("ConsoleCenter");
        // set parameters for border
        consoleOverlayBorder->setBorderSize(16, 16, 0, 16);
        consoleOverlayBorder->setBorderMaterialName("ConsoleBorder");
        consoleOverlayBorder->setLeftBorderUV(0.0, 0.49, 0.5, 0.51);
        consoleOverlayBorder->setRightBorderUV(0.5, 0.49, 1.0, 0.5);
        consoleOverlayBorder->setBottomBorderUV(0.49, 0.5, 0.51, 1.0);
        consoleOverlayBorder->setBottomLeftBorderUV(0.0, 0.5, 0.5, 1.0);
        consoleOverlayBorder->setBottomRightBorderUV(0.5, 0.5, 1.0, 1.0);

        // create the text lines
        consoleOverlayTextAreas = new TextAreaOverlayElement*[LINES];
        for(int i = 0; i<LINES; i++){
            consoleOverlayTextAreas[i] = static_cast<TextAreaOverlayElement*>(om->createOverlayElement("TextArea", "textArea"+Ogre::StringConverter::toString(i)));
            consoleOverlayTextAreas[i]->setMetricsMode(Ogre::GMM_PIXELS);
            consoleOverlayTextAreas[i]->setFontName("Console");
            consoleOverlayTextAreas[i]->setCharHeight(20);
            consoleOverlayTextAreas[i]->setParameter("colour_top", "0.21 0.69 0.21");
            consoleOverlayTextAreas[i]->setLeft(8);
            consoleOverlayTextAreas[i]->setCaption("");
        }

        // create noise
        consoleOverlayNoise = static_cast<PanelOverlayElement*>(om->createOverlayElement("Panel", "noise"));
        consoleOverlayNoise->setMetricsMode(Ogre::GMM_PIXELS);
        consoleOverlayNoise->setPosition(5,0);
        consoleOverlayNoise->setMaterialName("ConsoleNoise");

        consoleOverlay = om->create("Console");
        consoleOverlay->add2D(consoleOverlayContainer);
        consoleOverlayContainer->addChild(consoleOverlayBorder);
//comment following line to disable noise
        consoleOverlayContainer->addChild(consoleOverlayNoise);
        for(int i = 0; i<LINES; i++) consoleOverlayContainer->addChild(consoleOverlayTextAreas[i]);
        resize();

        // move overlay "above" the top edge of the screen
        // we take -1.2 because the border mkes the panel bigger
        consoleOverlayContainer->setTop(-1.2*REL_HEIGHT);
        // show overlay
        consoleOverlay->show();

        COUT(3) << "Info: InGameConsole initialized" << std::endl;
    }

    /**
    @brief used to control the actual scrolling and cursor
    */
    void InGameConsole::tick(float dt){
        scrollTimer += dt;
        if(scrollTimer >= 0.01){
            float top = consoleOverlayContainer->getTop();
            scrollTimer = 0;
            if(scroll!=0){
                // scroll
                top = top + 0.02*scroll;
                consoleOverlayContainer->setTop(top);
            }
            if(top <= -1.2*REL_HEIGHT){
                // window has completely scrolled up
                scroll = 0;
                consoleOverlay->hide();
                active = false;
            }
            if(top >= 0){
                // window has completely scrolled down
                scroll = 0;
                consoleOverlayContainer->setTop(0);
                active = true;
            }
        }

        cursor += dt;
        if(cursor >= 2*BLINK) cursor = 0;
        print(convert2UTF(this->ib_->get()));

// this creates a flickering effect
        consoleOverlayNoise->setTiling(1, rand()%5+1);
    }

    /**
    @brief resizes the console elements. call if window size changes
    */
    void InGameConsole::resize(){
        windowW = GraphicsEngine::getSingleton().getWindowWidth();
        windowH = GraphicsEngine::getSingleton().getWindowHeight();
        consoleOverlayBorder->setWidth((int) windowW*REL_WIDTH);
        consoleOverlayBorder->setHeight((int) windowH*REL_HEIGHT);
        consoleOverlayNoise->setWidth((int) windowW*REL_WIDTH - 10);
        consoleOverlayNoise->setHeight((int) windowH*REL_HEIGHT - 5);
        // now adjust the text lines...
        for(int i = 0; i<LINES; i++){
            consoleOverlayTextAreas[i]->setWidth(windowW*REL_WIDTH);
            consoleOverlayTextAreas[i]->setTop((int)windowH*REL_HEIGHT - 24 - 16*i);
        }
    }

    /**
    @brief shows console
    */
    void InGameConsole::activate(){
        consoleOverlay->show();
        // just in case window size has changed...
        resize();
        // scroll down
        scroll = 1;
        // the rest is done by tick
    }

    /**
    @brief hides console
    */
    void InGameConsole::deactivate(){
        // scroll up
        scroll = -1;
        // the rest is done by tick
    }

    /**
    @brief prints string to bottom line
    @param s string to be printed
    */
    void InGameConsole::print(Ogre::UTFString s){
        if(cursor>BLINK) consoleOverlayTextAreas[0]->setCaption(">" + s);
        else consoleOverlayTextAreas[0]->setCaption(">" + s + "_");
    }

    /**
    @brief shifts all lines up and clears the bottom line
    */
    void InGameConsole::newline(){
        Ogre::UTFString line;
        for(int i = LINES-1; i>=1; i--){
            line = consoleOverlayTextAreas[i-1]->getCaption();
            // don't copy the cursor...
            int l = line.length();
            if(!line.empty() && line.substr(l-1) == "_") line.erase(l-1);
            consoleOverlayTextAreas[i]->setCaption(line);
        }
        consoleOverlayTextAreas[0]->setCaption(">");
    }

    Ogre::UTFString InGameConsole::convert2UTF(std::string s){
        Ogre::UTFString utf;
        int i;
        Ogre::UTFString::code_point cp;
        for (i=0; i<(int)s.size(); ++i){
          cp = s[i];
          cp &= 0xFF;
          utf.append(1, cp);
        }
        return utf;
    }
}
