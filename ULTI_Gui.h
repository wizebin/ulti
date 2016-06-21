#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#endif // _WIN32_WINNT 501==XP

#include <string>
#include <map>
#include <functional>
#include <windows.h>
#include <commctrl.h>
#include <sddl.h>
#include <shlobj.h>
//#include <shobjidl.h>



#include "ULTI_Text.h"
#include "ULTI_Types.h"
#include "ULTI_Structs.h"
#include "ULTI_Graphics.h"
#include "ULTI_Thread.h"
#include "ULTI_Utility.h"

/** In windows, if compiled with GCC, this will allow you to bypass the requirement for a manifest file
    For the newer visual features, if you don't have a manifest file and you haven't used this function
    your window will appear archaic

    To enable visual styles normally:

    1. Create and Include a .rc file in your c++ project
    2. Create and Include a .manifest file in your c++ project
    3. Inside the .rc file include the following line
        1 24 ".manifest"
        Where .manifest is the actual name of your manifest file, in my case main.manifest
    4. Inside the .manifest file include something like the following
        <?xml version="1.0" encoding="UTF-8" standalone="yes"?>
        <assembly xmlns="urn:schemas-microsoft-com:asm.v1" manifestVersion="1.0">
          <trustInfo xmlns="urn:schemas-microsoft-com:asm.v2">
            <security>
              <requestedPrivileges>
                <requestedExecutionLevel level="asInvoker" uiAccess="false"/>
              </requestedPrivileges>
            </security>
          </trustInfo>
          <dependency>
            <dependentAssembly>
              <assemblyIdentity type="Win32" name="Microsoft.Windows.Common-Controls" version="6.0.0.0"
                                processorArchitecture="*" publicKeyToken="6595b64144ccf1df" language="*"/>
            </dependentAssembly>
          </dependency>
        </assembly>

    Notes. This manifest file can do much more than simply enable visual styles, for more information check
    out https://msdn.microsoft.com/en-us/library/windows/desktop/bb773175(v=vs.85).aspx#using_manifests
*/
ULONG_PTR EnableVisualStyles(VOID);

namespace ulti{

class window;

//Request window's standard file dialogs for saving/loading, returns a blank string (or vector) if cancelled

std::string requestDirectoryDialog(window* parent = NULL);
std::string requestSaveDialog(const std::string& typeFilter = "All Files\0*.*\0\0", window* parent = NULL, ui32 flags = OFN_OVERWRITEPROMPT);
std::string requestLoadDialog(const std::string& typeFilter = "All Files\0*.*\0\0", window* parent = NULL, ui32 flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST);
std::vector<std::string> requestMultiLoadDialog();

//positioning of GUI elements, similar to HTML style
enum PositionType{
    POSITION_NORMAL,
    POSITION_ABSOLUTE,
    POSITION_FIXED,
    POSITION_RELATIVE
};
//Not all of these are implemented, most are, will document once this component is finished
enum WindowFlags{
    WINDOW_UNKNOWN = 0,
    WINDOW_PARENT = 1,
    WINDOW_CHILD = 2,
    WINDOW_STYLED = 4,
    WINDOW_REALTIME = 8,
    WINDOW_TOPMOST = 16,
    WINDOW_SCROLLED = 32,
    WINDOW_X_SCROLLED = 64,
    WINDOW_DOUBLE_BUFFERED = 128,
    WINDOW_HIDDEN = 256,
    WINDOW_FULLSCREEN = 512,
    WINDOW_FILL_Y = 1024,
    WINDOW_MULTILINE = 2048,
    WINDOW_BORDERED = 4096,
    WINDOW_AUTO_SCROLLED = 8192,
};
//convenience definitions for common event types, may be removed
enum WindowEvents{
    EVENT_CREATE,
    EVENT_EXIT,
    EVENT_TIMER,
    EVENT_NEW,
    EVENT_OPEN,
    EVENT_CLOSE,
    EVENT_SAVE,
    EVENT_DELETE,
    EVENT_SETTINGS,
    EVENT_HELP,
    EVENT_ABOUT,
    EVENT_RUN,
    EVENT_STOP,
    EVENT_STEP,
    EVENT_NEXT,
    EVENT_PREV,
    EVENT_FIRST,
    EVENT_LAST,
    EVENT_FRONT,
    EVENT_BACK,
    EVENT_SEARCH
};

//key definitions, may change based on platform using preprocessor macros in the future
enum keys{
    KEY_A = 'A',
    KEY_B = 'B',
    KEY_C = 'C',
    KEY_D = 'D',
    KEY_E = 'E',
    KEY_F = 'F',
    KEY_G = 'G',
    KEY_H = 'H',
    KEY_I = 'I',
    KEY_J = 'J',
    KEY_K = 'K',
    KEY_L = 'L',
    KEY_M = 'M',
    KEY_N = 'N',
    KEY_O = 'O',
    KEY_P = 'P',
    KEY_Q = 'Q',
    KEY_R = 'R',
    KEY_S = 'S',
    KEY_T = 'T',
    KEY_U = 'U',
    KEY_V = 'V',
    KEY_W = 'W',
    KEY_X = 'X',
    KEY_Y = 'Y',
    KEY_Z = 'Z',
    KEY_SPACE = ' ',
    KEY_UP = VK_UP,
    KEY_RIGHT = VK_RIGHT,
    KEY_LEFT = VK_LEFT,
    KEY_DOWN = VK_DOWN
};

//check to see if a key is currently depressed, you can use the keys enum to determine which key is which
bool pressed(int key);

//send a WM_KEYDOWN event to a window, currently non-functioning!
int sendKeyDown(HWND hwnd, int key);
int sendKeyUp(HWND hwnd, int key);

//Returns a windows scan code from a vkcode, useful for some very specific functions
unsigned long getScanCode(int vkcode);
unsigned long getVKCode(int scancode);

//Sends a message to windows that you do want joypad input, this will be modified in the future
//possibly to use xinput
int captureJoy(HWND win);


//The structure that holds all of the information about the position of a gui object, windows and child windows
//This structure is what allows the library to automate all GUI layouts
struct positioning{
    PositionType postype;
    int x,y;
    int minwidth,minheight,maxwidth,maxheight;
    int xweight,yweight;
    int row,col;
    int calculatedWidth,calculatedHeight,calculatedX,calculatedY;
    int marginLeft,marginRight,marginTop,marginBottom;
    positioning();
    positioning(const positioning& ipos);
    positioning(int icol, int irow, int iwidth, int iheight, int ixweight = 0, int iyweight = 0, PositionType ipostype = PositionType::POSITION_NORMAL);
};

//Internal use, causes a window to be CUSTOM if it doesn't exist yet
enum ClassCreation{
    CLASS_NONE,
    CLASS_DEFAULT,
    CLASS_EXTENDED,
    CLASS_CUSTOM
};
struct ClassType{
    ClassCreation origin;
    unsigned long flags;
    ClassType();
    ClassType(ClassCreation howCreated,unsigned long defaultFlags=0);
};

//This allows you to use any classname without having to worry about whether you are subclassing,
//creating a new class, or anything else- that whole process is all automated with this class
class WindowsClasses{
    public:
    std::map<std::string,ClassType> classes;
    bool isClassCreated(const std::string& classname);
    bool isClassDefault(const std::string& classname);
    unsigned long getDefaultFlags(const std::string& classname);
    int addCustom(const std::string& classname);
    WindowsClasses();
};

//Not a singleton, could very easily be made into one
extern WindowsClasses WinClasses;

//This is the vendor specific data class, this may well be included directly in the window class in the future
struct VendorElements{
    int createClass();

    #if defined(_WIN_32) || defined(_WIN32) || defined(WIN32)
        HWND hwnd;
        HMENU menu;
        HFONT font;
    #elif defined(_UNIX)
        //nothing
    #endif // defined
    VendorElements();
};

//The generic window procedure needed for all basic interaction with windows events
static LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//Not static, probably should be
extern positioning DEFAULT_POSITIONING;
extern unsigned long DEFAULT_FLAGS;

//Unused
enum EventCompletion{
    stopEvent,
    passEvent
};

//Unused
struct callbackData{
    int returncode;
    EventCompletion propogation;
};


/**
    The heart of the GUI library, this window class does it all, creates windows, positions child windows,
    sends and receives events, allows you to create menus, and perform any function you would expect from
    a GUI library
**/
class window: public responder{
protected:
    //ulti::element data;

    std::vector<window*> children;//always sorted by pointer value
    std::vector<window*> positionSortedChildren;

    bool created;
    WNDPROC oldproc;
    int nextID;
    window* parent;
    bool isCustomDrawn;
    bool doAutoScroll;
    positioning wpos;
    unsigned long createFlags;
    ulti::element eventMap;
    std::string createType;
    int yscroll,yscrollmin,yscrollmax;

public:

    VendorElements veil; //contains vendor specific structures

    window(window* parent, const std::string& title, const std::string& type = "DEFAULT_TYPE", unsigned long flags = DEFAULT_FLAGS, positioning ipos = DEFAULT_POSITIONING);
    //window* createChild();//This window will be responsible to clean up the mess
    //window(window* parent, const ulti::element& el);
    window();
    virtual int create(window* parent, const std::string& title, const std::string& type = "DEFAULT_TYPE", unsigned long flags = DEFAULT_FLAGS, positioning ipos = DEFAULT_POSITIONING);

    //Normally called internally- you don't need to add or remove children manually unless you're doing
    //something fancy
    virtual int addChild(window* win);
    virtual int remChild(window* win);
    virtual int childCount();
    virtual window* getChild(int index);

    long getWindowStyle();
    long setWindowStyle(long istyle);

    //This is a bit tricky, because when you reparent your styles change around a bunch, specifically
    //The styles for child window and parent window are not a single bit flag, be cautious with this
    //because reparenting to NULL (which makes you a new desktop window) then reparenting to the original
    //causes your window to lose some potentially important flags, including borders
    virtual int reparent(window* nparent, bool titlebar = true);

    //Reparent NULL, become a solo window
    virtual int popout();

    //use or don't use the menu bar that has the minimize, maximize and close buttons
    long setDefaultMenuBar(bool usemenu=true);

    //Layout finds the minimum size of the element by requesting the same information from all children
    //recursively (Be very careful of cyclical parenthood) and uses that information to distribute sizes
    //based on weight to all children
    virtual void layout();
    virtual void layout(const positioning& pos);//positioning row and column are x and y for parent window or absolute positioned window

    //Performs a resize event, may either be augmented by many subfunctions or removed at a future date
    virtual int doResize();

    //Non-Blocking, Returns a 0 if there are no events, and a non-zero if there are (uses peekmessage)
    virtual int checkEvents();

    //Blocking, waits until there is an event available, then processes that event
    virtual int pauseForEvent();
    //virtual int pauseForAnyEvent();//This way you only need one window to process events for the thread
    //Non-Blocking, returns -1 if quitting, 0 if no events, 1 if events were processed
    virtual int processEvents();

    //Internal use, increments the child id variable so all children have unique ids
    virtual int getNextChildID();

    //Draw directly on top of the window, should use a whiteboard if you want to use this function
    //Otherwise you will probably have flashing
    virtual int drawRaw(unsigned int* data, int canvasX, int canvasY, int width, int height);


    int getx();
    int gety();
    int row();
    int col();
    int width();
    int height();
    int xweight();
    int yweight();
    //Returns internal size vs window size, this means if the window is 400x400 px, the area that doesn't
    //include the scrollbar, top menu bar, and borders is a bit smaller- the smaller area is what the client
    //area is
    int clientWidth();
    int clientHeight();

    int marginLeft();
    int marginRight();
    int marginTop();
    int marginBottom();

    ulti::icoord getPosition();
    ulti::icoord getClientPosition();
    ulti::icoord getSize();
    ulti::icoord getClientSize();

    ulti::icoord getMinSize();
    ulti::icoord getMaxSize();
    ulti::icoord getWeights();

    //Returns the relative mouse position regardless of focus and regardless of the mouse being inside the window
    //This is relative to the client position, not the window position
    int getRelativeMouseX();
    int getRelativeMouseY();

    ulti::icoord getRelativeMousePos();
    bool isMouseInside();
    bool isMouseInClient();

    int setPosition(ulti::icoord pos);
    int setSize(ulti::icoord siz);

    //resize window based on required client area
    int setClientSize(ulti::icoord siz);

    int scrollTo(int scrollpos);

    int getHorizontalWeight();
    int getVerticalWeight();

    //returns false if the positiontype is absolute or fixed, or if the object is hidden
    bool canBePositioned();
    int positionType();
    void setPositionType(PositionType itype);


    //fills internal structure of children, then sorts it top->bottom,left->right,low pointer->high pointer
    void fillSortedChildren();
    int calculateRowCount();
    int calculateColCount(int irow);

    //currently irrelevant for colminheight- there is no overlap of rows
    int calculateRowMinWidth(int irow);
    int calculateRowMinHeight(int irow);
    int calculateMinWidth();
    int calculateMinHeight();
    int calculateVerticalWeight();
    int calculateRowVerticalWeight(int irow);
    int calculateHorizontalWeight(int irow);

    //Shows then lays out the window, the layout is important because the window will be wonky otherwise
    virtual int show(bool doLayout=true);
    virtual int hide();
    virtual bool isHidden();

    virtual bool hasFocus();
    virtual bool isForeground();


    //These text functions are relevant for anything with a title or a body, so a textbox or a button would
    //both use these functions to modify/retrieve their text
    int getTextLength();
    std::string getText();
    void setText(const std::string& itext);

    //Will not work on every control, generally just edit boxes
    void setHint(const std::string& title);


    //These do not take immediate effect, must call layout on the parent to cause a change
    void setPosition(int ix, int iy);
    void setSize(int iwidth, int iheight);
    void setRow(int irow);
    void setCol(int icol);
    void setxWeight(int ix);
    void setyWeight(int ix);
    void setMarginLeft(int imargin);
    void setMarginRight(int imargin);
    void setMarginTop(int imargin);
    void setMarginBottom(int imargin);
    void setMargins(int xmargin, int ymargin);
    void setMargins(int left,int right,int top,int bottom);
    void setMargins(int imargin);

    unsigned long getFlags();

    //returns windows class
    std::string getType();

    void setFont(int fsize, int family = FF_MODERN, const std::string& face="", int weight = FW_DONTCARE, bool italic = false, bool underline = false, int charset = DEFAULT_CHARSET, int quality = PROOF_QUALITY);


    //On str event perform func function, or send a new event named trigger
    void on(const std::string& str, const std::string& trigger);
    void on(const std::string& str, std::function<void(ulti::message& msg)> func);
    void on(ulti::ui32 eventIndex, std::function<void(ulti::message& msg)> func);

    /**
        This parses JSON into a menu, here's an example string:

        string menstring = (ulti::stringBuilder()<<"[{\"text\":\"file\",\"children\":"
                        << "["
                        << "{\"text\":\"new\",\"event\":\"new\",\"id\":" << ID_NEW << "},"
                        << "{\"text\":\"open\",\"event\":\"open\",\"id\":" << ID_OPEN << "},"
                        << "{\"text\":\"step\",\"event\":\"step\",\"id\":" << EVENT_STEP << "}]},"
                        << "{\"text\":\"command\",\"children\":"
                        << "[{\"text\":\"clear\",\"event\":\"clear\",\"id\":" << ID_CLEAR << "}]}]").str();

        children is an array of further menu items, it will not go past two children

    **/
    virtual int attachMenu(const std::string& str);

    virtual int setIcon(int resourceNumber);
    virtual int setIcon(const std::string& filename);
    virtual int setIcon(HICON ico);

    //Remove the previous ON function event trigger
    void clearTrigger(const std::string& str);

    //Change position and layout
    void reposition(const positioning& pos);
    #if defined(_WIN_32) || defined(_WIN32) || defined(WIN32)
        LRESULT CALLBACK windowCallback(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
    #endif // defined _WIN_32

    virtual ~window();
};

class menu: public responder{
    menu(window* parent);
    menu(window* parent, const element& el);
};

/**
    A whiteboard is nothing but a custom drawn window, this class allows you to draw a pixel buffer directly
    to the screen, no fluff, no meddling
**/
class whiteboard: public window{
    public:
    whiteboard();
    whiteboard(window* parent, const std::string& title, const std::string& type = "DEFAULT_TYPE", unsigned long flags = DEFAULT_FLAGS, positioning ipos = DEFAULT_POSITIONING);
    virtual ~whiteboard();
    surface sur;
    virtual int doResize();
    int drawSurface();
};


//DEPRACATED
extern charset guichr;
typedef int (*sdfptr)(void*);
//DEPRACATED
class guiobj: public sprite{
protected:
    std::string guiname;
    std::string str;
    int value;
    int (*fnc)(void*);
public:
    std::map<std::string,std::function<void(void*)> > funcMap;
    void setfunc(int (*infnc)(void*));
    sdfptr getfunc();
    std::string getname();
    void setname(const std::string& instr);
    int getvalue();
    void setvalue(int inval);

    virtual void create();
    virtual std::string getstring(); //returns a state-like string (on, off, "string", etc)
    virtual void setstring(const std::string& instr);
    virtual void apply(surface& sur);
    virtual int submit_listens(eventSystem* manager);
    virtual int handle_event(event* evnt);
    virtual int class_type();
};
//DEPRACATED
class button: public guiobj{
protected:
    ui32 fcol,bcol;
    ui32 dnfcol,dnbcol;
public:

    virtual std::string getstring(); //returns a state-like string (on, off, "string", etc)
    virtual void setstring(const std::string& instr);
    virtual void apply(surface& sur);
    virtual int submit_listens(eventSystem* manager);
    virtual int handle_event(event* evnt);
    virtual int class_type();

    virtual void setdowncol(ui32 ifcol, ui32 ibcol);
    virtual void setupcol(ui32 ifcol,ui32 ibcol);

    void create();

    button(std::string iname, std::string val, int isx, int isy, ui32 fcolor, ui32 bcolor);
    button();

};
//DEPRACATED
class toggle: public guiobj{
protected:

public:
    int submit_listens(eventSystem* manager);
    int handle_event(event* evnt);
    int class_type();
};
//DEPRACATED
class textinput: public guiobj{
protected:

public:

};
//DEPRACATED
class slider: public guiobj{
protected:

public:
    int submit_listens(eventSystem* manager);
    int handle_event(event* evnt);
    int class_type();
};
class label: public guiobj{
protected:
    int fsize,thick;
    ui32 fcol;
public:
    void create();
    int submit_listens(eventSystem* manager);
    int handle_event(event* evnt);
    int class_type();
    void setfont(int fontsize, ui32 icol, int ithick);
    label();
    label(std::string instr, int fontsize, ui32 icol, int ithick);
};


}
