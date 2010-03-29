#ifndef _ChatBox_h_
#define _ChatBox_h_

#include "CEGuiSample.h"
#include "CEGUI/CEGUI.h"
#include "CEGUI/CEGUIXMLAttributes.h"

class ChatBox : public CEGuiSample
{
  public:
    bool initialiseSample();
    void cleanupSample(void);
    bool Event_HistorySizeChange(const CEGUI::EventArgs& args);
    bool Event_ChatTextAdded(const CEGUI::EventArgs& args);
    bool Event_FontChange(const CEGUI::EventArgs& args);
    void setHistorySize(const size_t& pSize);
    void addChatText(const CEGUI::String& pText);
    void registerFont(const CEGUI::String& pLogicalName, const CEGUI::String& pFileName);

  protected:
    CEGUI::Font* makeFont(const CEGUI::String& pFontName, const CEGUI::String& pFileName, const CEGUI::String& pSize);
    void changeFont(const CEGUI::String& pFontLogicalName, const CEGUI::String& pFontSize);

  private:
    // Type of list for registered fonts
    typedef std::map<CEGUI::String, CEGUI::String> FontList;

    // List of registered fonts
    FontList mFontList;

    // Maximal number of entries to retain within the Listbox
    size_t mHistorySize;

    // Logical font name dedicated to the chat box
    // This allows us to modify the properties of that font and not affect the fonts used elsewhere
    CEGUI::String mChatFontName;

    // Default font size
    CEGUI::String mDefaultFontSize;
};


#endif