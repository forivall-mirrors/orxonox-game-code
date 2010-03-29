#include "ChatBox.h"

#include "CEGuiSample.h"
#include "CEGUI/CEGUI.h"
#include "CEGUI/CEGUIXMLAttributes.h"

bool ChatBox::initialiseSample()
{
  using namespace CEGUI;
  try
  {
    // Retrieve the window manager
    WindowManager& winMgr = WindowManager::getSingleton();

    // Load the TaharezLook scheme and set up the default mouse cursor and font
    SchemeManager::getSingleton().loadScheme("TaharezLook.scheme");
    System::getSingleton().setDefaultMouseCursor("TaharezLook", "MouseArrow");
    if(!FontManager::getSingleton().isFontPresent("Commonwealth-10"))
      FontManager::getSingleton().createFont("Commonwealth-10.font");

    // Set the GUI Sheet
    Window* sheet = winMgr.createWindow("DefaultWindow", "root_wnd");
    System::getSingleton().setGUISheet(sheet);

    // Load a layout
    Window* guiLayout = winMgr.loadWindowLayout("ChatBox.layout");
    sheet->addChildWindow(guiLayout);

    // Obtain the handles of some widgets
    Window* historySize = winMgr.getWindow("/ChatBox/History");
    Window* fontName = winMgr.getWindow("/ChatBox/FontName");
    Spinner* fontSize = static_cast<Spinner*>(winMgr.getWindow("/ChatBox/FontSize"));
    Window* chatText = winMgr.getWindow("/ChatBox/Text");

    // Disable widgets until a valid font is registered
    fontName->setEnabled(false);
    fontSize->setEnabled(false);
    chatText->setEnabled(false);

    // Retrieve the design-specified values
    mHistorySize = static_cast<size_t>(PropertyHelper::stringToUint(historySize->getText()));
    mDefaultFontSize = fontSize->getText();
    mChatFontName = fontName->getText();
    setHistorySize(mHistorySize);
    fontName->setText("");

    // Configure the history size
    // Pressing <ENTER> changes the maximal number of entries within the history Listbox
    historySize->subscribeEvent(Editbox::EventTextAccepted,  Event::Subscriber(&ChatBox::Event_HistorySizeChange, this)); 

    // Configure the text Editbox
    // Pressing <ENTER> puts the text into the history Listbox
    chatText->subscribeEvent(Editbox::EventTextAccepted,  Event::Subscriber(&ChatBox::Event_ChatTextAdded,  this)); 

    // Configure the font name Combobox
    // Selecting a name changes the font used in the history Listbox and the text Editbox
    fontName->subscribeEvent(Combobox::EventTextChanged,  Event::Subscriber(&ChatBox::Event_FontChange,  this)); 

    // Configure the font size Spinner
    // Selecting a size changes the font size used in the history Listbox and the text Editbox
    fontSize->subscribeEvent(Spinner::EventValueChanged,  Event::Subscriber(&ChatBox::Event_FontChange,  this)); 
    fontSize->setTextInputMode(Spinner::Integer);
    fontSize->setMinimumValue(4.0f);
    fontSize->setMaximumValue(72.0f);
    fontSize->setStepSize(1.0f);
    fontSize->setCurrentValue(PropertyHelper::stringToFloat(mDefaultFontSize));

    // Initialize the list of fonts
    // The first registered font becomes the active font
    registerFont("Commonwealth",  "Commonv2c.ttf");
    registerFont("DejaVuSans",    "DejaVuSans.ttf");
    registerFont("Iconified",    "Iconiv2.ttf");
    registerFont("MissingFile",    "MissingFile.ttf");  // What happens if a font is missing?
    registerFont("Pixmap Font",    "FairChar-30.font"); // And what about a non-Freetype font?
  }
  catch(Exception &e)
  {
    #if defined( __WIN32__ ) || defined( _WIN32 )
      MessageBox(NULL, e.getMessage().c_str(), "Error initializing the demo", MB_OK | MB_ICONERROR | MB_TASKMODAL);
    #else
      //std::cout << "Error initializing the demo:" << e.getMessage().c_str() << "\n";
    #endif
  }

  return true;
}

void ChatBox::cleanupSample(void)
{
}

bool ChatBox::Event_HistorySizeChange(const CEGUI::EventArgs& args)
{
  using namespace CEGUI;

  WindowManager& winMgr = WindowManager::getSingleton();
  CEGUI::Window* historySize = winMgr.getWindow("/ChatBox/History");
  int size = PropertyHelper::stringToInt( historySize->getText() );
  setHistorySize(size);
  return true;
}

bool ChatBox::Event_ChatTextAdded(const CEGUI::EventArgs& args)
{
  using namespace CEGUI;

  WindowManager& winMgr = WindowManager::getSingleton();
  Editbox* chatText = static_cast<Editbox*> (winMgr.getWindow("/ChatBox/Text"));
  addChatText(chatText->getText());

  // Clear the text in the Editbox
  chatText->setText("");
  return true;
}

bool ChatBox::Event_FontChange(const CEGUI::EventArgs& args)
{
  using namespace CEGUI;
  WindowManager& winMgr = WindowManager::getSingleton();
  Window* fontName = winMgr.getWindow("/ChatBox/FontName");
  String name = fontName->getText();

  Spinner* fontSize = static_cast<Spinner*>(winMgr.getWindow("/ChatBox/FontSize"));
  String size = PropertyHelper::floatToString(fontSize->getCurrentValue());

  Window* chatText = winMgr.getWindow("/ChatBox/Text");
  chatText->setText(name + " - " + size);

  changeFont(name, size);
  return true;
}

void ChatBox::setHistorySize(const size_t& pSize)
{
  using namespace CEGUI;

  if(pSize > 0)
  {
    // A better validation would be to enforce a minimal and a maximal size
    mHistorySize = pSize;

    WindowManager& winMgr = WindowManager::getSingleton();
    Listbox* chatHistory = static_cast<Listbox*> (winMgr.getWindow("/ChatBox/List"));
    ListboxItem* chatItem;
    while(chatHistory->getItemCount() > mHistorySize)
    {
      // There are too many items within the history Listbox, purging them one at a time
      chatItem = chatHistory->getListboxItemFromIndex(0);
      chatHistory->removeItem(chatItem);
    }
  }
}

void ChatBox::addChatText(const CEGUI::String& pText)
{
  using namespace CEGUI;

  WindowManager& winMgr = WindowManager::getSingleton();
  Listbox* chatHistory = static_cast<Listbox*> (winMgr.getWindow("/ChatBox/List"));

  // If there's text then add it
  if(pText.size())
  {
    // Add the Editbox text to the history Listbox
    ListboxTextItem* chatItem;
    if(chatHistory->getItemCount() == mHistorySize)
    {
      /* We have reached the capacity of the Listbox so re-use the first Listbox item.
         This code is a little crafty.  By default the ListboxTextItem is created with
         the auto-delete flag set to true, which results in its automatic deletion when
         removed from the Listbox.  So we change that flag to false, extract the item
         from the Listbox, change its text, put the auto-delete flag back to true, and
         finally put the item back into the Listbox. */
      chatItem = static_cast<ListboxTextItem*>(chatHistory->getListboxItemFromIndex(0));
      chatItem->setAutoDeleted(false);
      chatHistory->removeItem(chatItem);
      chatItem->setAutoDeleted(true);
      chatItem->setText(pText);
    }
    else
    {
      // Create a new listbox item
      chatItem = new ListboxTextItem(pText);
    }
    chatHistory->addItem(chatItem);
    chatHistory->ensureItemIsVisible(chatHistory->getItemCount());
  }
}

void ChatBox::registerFont(const CEGUI::String& pLogicalName, const CEGUI::String& pFileName)
{
  using namespace CEGUI;

  // Ensure that font names are registered only once
  if(mFontList.find(pLogicalName) == mFontList.end())
  {
    // Test the font so that only valid fonts are available
    String testFont = mChatFontName;
    if(mFontList.size() != 0)
    {
      // If the list is empty then attempt to create the font using the "real" font name
      // Otherwise use a "test" font name so as not to corrupt the "real" one
      testFont += "__test_font__";
    }
    Font* font = makeFont(testFont, pFileName, mDefaultFontSize);
    if(mFontList.size() != 0 
      && FontManager::getSingleton().isFontPresent(testFont))
    {
      // Since this was only a test font we destroy it
      FontManager::getSingleton().destroyFont(testFont);
    }
    if(!font)
    {
      // This font is invalid
      if(FontManager::getSingleton().isFontPresent(testFont))
        return;
      else
      return;
    }

    WindowManager& winMgr = WindowManager::getSingleton();
    Combobox* fontName = static_cast<Combobox*>(winMgr.getWindow("/ChatBox/FontName"));
    mFontList[pLogicalName] = pFileName;
    ListboxTextItem* fontNameItem = new ListboxTextItem(pLogicalName);
    fontNameItem->setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush");
    fontName->addItem(fontNameItem);
    if(fontName->getItemCount() == 1)
    {
      // Enable widgets now that at least one valid font has been found
      Spinner* fontSize = static_cast<Spinner*>(winMgr.getWindow("/ChatBox/FontSize"));
      Window* chatText = winMgr.getWindow("/ChatBox/Text");
      fontName->setEnabled(true);
      fontSize->setEnabled(true);
      chatText->setEnabled(true);

      // The first registered font becomes the active font
      fontName->setText(pLogicalName); // This triggers a call to changeFont
      fontName->setItemSelectState(fontNameItem, true);
    }
  }
}

CEGUI::Font* makeFont(const CEGUI::String& pFontName, const CEGUI::String& pFileName, const CEGUI::String& pSize)
{
  using namespace CEGUI;

  Font* font;
  try
  {
    if(FontManager::getSingleton().isFontPresent(pFontName))
    {
      // The chat font is reused rather than deleted and recreated
      // every time an attribute changes.  For this reason it is
      // important to use a unique logical name for the font.
      font = FontManager::getSingleton().getFont(pFontName);
      font->setProperty("FileName", pFileName);
      font->setProperty("PointSize", pSize);
    }
    else
    {
      // This is the first time we make the chat font so we need to create it
      XMLAttributes xmlAttributes;

      // CEGUIFont.cpp
      xmlAttributes.add("Name", pFontName);
      xmlAttributes.add("Filename", pFileName);
      xmlAttributes.add("ResourceGroup", "");
      xmlAttributes.add("AutoScaled", "true");
      xmlAttributes.add("NativeHorzRes", "800");
      xmlAttributes.add("NativeVertRes", "600");

      // CEGUIXMLAttributes.cpp
      xmlAttributes.add("Size", pSize);
      xmlAttributes.add("AntiAlias", "true");

      font = FontManager::getSingleton().createFont("FreeType", xmlAttributes);
    }
    font->load();
  }
  catch(Exception& e)
  {
    // Display the error message in the chat window
    //addChatText(e.getMessage());
    font = 0;
  }

  return font;
}

void changeFont(const CEGUI::String& pFontLogicalName, const CEGUI::String& pFontSize)
{
  using namespace CEGUI;
  WindowManager& winMgr = WindowManager::getSingleton();

  if(!FontManager::getSingleton().isFontPresent(mChatFontName))
  {
    addChatText("You must call registerFont() at least once with a valid font");
    return;
  }

  FontList::iterator itFontList = mFontList.find(pFontLogicalName);
  if(itFontList == mFontList.end())
  {
    addChatText(pFontLogicalName + " has not been registered");
    return;
  }

  // Measure the height of the selected font
  Font* currentFont = makeFont(mChatFontName, (*itFontList).second, pFontSize);
  float fontHeight = currentFont->getFontHeight();

  /* Alter the area of the Editbox.  The original value is {{0.01,0},{1,-30},{0.99,0},{1,-5}}
     The value we are altering is the "-30" within the second couplet, defining the position of
     the upper y coordinate of the Editbox.  We base the new value on the position of the lower
     y coordinate, which is "-5", and the height of the font.  To this we add some space "10" to
     account for the Editbox's border. */
  Editbox* editBox = static_cast<Editbox*> (winMgr.getWindow("/ChatBox/Text"));
  URect chatTextArea = editBox->getArea();
  chatTextArea.d_min.d_y.d_offset = chatTextArea.d_max.d_y.d_offset
                  - fontHeight
                  - 10;
  editBox->setArea(chatTextArea);
  editBox->setFont(currentFont);

  /* Alther the area of the Listbox.  Here we only need the lower y coordinate.  Since this
     value is the same as the upper y coordinate of the Editbox we do not need to calculate
     it.  We also change the font of the Listbox and call upon handleUpdatedItemData() to
     update the current Listbox items.  Finally we ensure that the last entry is still
     visible. */
  Listbox* listBox = static_cast<Listbox*> (winMgr.getWindow("/ChatBox/List"));
  URect listTextArea = listBox->getArea();
  listTextArea.d_max.d_y.d_offset = chatTextArea.d_min.d_y.d_offset;
  listBox->setArea(listTextArea);
  listBox->setFont(currentFont);
  listBox->handleUpdatedItemData();
  size_t itemCount = listBox->getItemCount();
  if(itemCount)
  {
    ListboxItem* currentItem = listBox->getListboxItemFromIndex(itemCount - 1);
    listBox->ensureItemIsVisible(currentItem);
  }
}

