#include "ULTI_Gui.h"
#include "ULTI_Collision.h"
#include <iostream>

ULONG_PTR EnableVisualStyles(VOID){
    TCHAR dir[MAX_PATH];
    ULONG_PTR ulpActivationCookie = FALSE;
    ::ACTCTX actCtx =
    {
        sizeof(actCtx),
        ACTCTX_FLAG_RESOURCE_NAME_VALID
            | ACTCTX_FLAG_SET_PROCESS_DEFAULT
            | ACTCTX_FLAG_ASSEMBLY_DIRECTORY_VALID,
        TEXT("shell32.dll"), 0, 0, dir, (LPCTSTR)124
    };
    UINT cch = GetSystemDirectory(dir, sizeof(dir) / sizeof(*dir));
    if (cch >= sizeof(dir) / sizeof(*dir)) { return FALSE; /*shouldn't happen*/ }
    dir[cch] = TEXT('\0');
    ActivateActCtx(CreateActCtx(&actCtx), &ulpActivationCookie);
    return ulpActivationCookie;
}

std::string getEventName(long index){
    static std::map<long,std::string> eventMap = {{0,"WM_NULL"},{1,"WM_CREATE"},{2,"WM_DESTROY"},{3,"WM_MOVE"},{5,"WM_SIZE"},{6,"WM_ACTIVATE"},{7,"WM_SETFOCUS"},{8,"WM_KILLFOCUS"},{10,"WM_ENABLE"},{11,"WM_SETREDRAW"},{12,"WM_SETTEXT"},{13,"WM_GETTEXT"},{14,"WM_GETTEXTLENGTH"},{15,"WM_PAINT"},{16,"WM_CLOSE"},{17,"WM_QUERYENDSESSION"},{18,"WM_QUIT"},{19,"WM_QUERYOPEN"},{20,"WM_ERASEBKGND"},{21,"WM_SYSCOLORCHANGE"},{22,"WM_ENDSESSION"},{24,"WM_SHOWWINDOW"},{25,"WM_CTLCOLOR"},{26,"WM_WININICHANGE"},{27,"WM_DEVMODECHANGE"},{28,"WM_ACTIVATEAPP"},{29,"WM_FONTCHANGE"},{30,"WM_TIMECHANGE"},{31,"WM_CANCELMODE"},{32,"WM_SETCURSOR"},{33,"WM_MOUSEACTIVATE"},{34,"WM_CHILDACTIVATE"},{35,"WM_QUEUESYNC"},{36,"WM_GETMINMAXINFO"},{38,"WM_PAINTICON"},{39,"WM_ICONERASEBKGND"},{40,"WM_NEXTDLGCTL"},{42,"WM_SPOOLERSTATUS"},{43,"WM_DRAWITEM"},{44,"WM_MEASUREITEM"},{45,"WM_DELETEITEM"},{46,"WM_VKEYTOITEM"},{47,"WM_CHARTOITEM"},{48,"WM_SETFONT"},{49,"WM_GETFONT"},{50,"WM_SETHOTKEY"},{51,"WM_GETHOTKEY"},{55,"WM_QUERYDRAGICON"},{57,"WM_COMPAREITEM"},{61,"WM_GETOBJECT"},{65,"WM_COMPACTING"},{68,"WM_COMMNOTIFY"},{70,"WM_WINDOWPOSCHANGING"},{71,"WM_WINDOWPOSCHANGED"},{72,"WM_POWER"},{73,"WM_COPYGLOBALDATA"},{74,"WM_COPYDATA"},{75,"WM_CANCELJOURNAL"},{78,"WM_NOTIFY"},{80,"WM_INPUTLANGCHANGEREQUEST"},{81,"WM_INPUTLANGCHANGE"},{82,"WM_TCARD"},{83,"WM_HELP"},{84,"WM_USERCHANGED"},{85,"WM_NOTIFYFORMAT"},{123,"WM_CONTEXTMENU"},{124,"WM_STYLECHANGING"},{125,"WM_STYLECHANGED"},{126,"WM_DISPLAYCHANGE"},{127,"WM_GETICON"},{128,"WM_SETICON"},{129,"WM_NCCREATE"},{130,"WM_NCDESTROY"},{131,"WM_NCCALCSIZE"},{132,"WM_NCHITTEST"},{133,"WM_NCPAINT"},{134,"WM_NCACTIVATE"},{135,"WM_GETDLGCODE"},{136,"WM_SYNCPAINT"},{160,"WM_NCMOUSEMOVE"},{161,"WM_NCLBUTTONDOWN"},{162,"WM_NCLBUTTONUP"},{163,"WM_NCLBUTTONDBLCLK"},{164,"WM_NCRBUTTONDOWN"},{165,"WM_NCRBUTTONUP"},{166,"WM_NCRBUTTONDBLCLK"},{167,"WM_NCMBUTTONDOWN"},{168,"WM_NCMBUTTONUP"},{169,"WM_NCMBUTTONDBLCLK"},{171,"WM_NCXBUTTONDOWN"},{172,"WM_NCXBUTTONUP"},{173,"WM_NCXBUTTONDBLCLK"},{176,"EM_GETSEL"},{177,"EM_SETSEL"},{178,"EM_GETRECT"},{179,"EM_SETRECT"},{180,"EM_SETRECTNP"},{181,"EM_SCROLL"},{182,"EM_LINESCROLL"},{183,"EM_SCROLLCARET"},{185,"EM_GETMODIFY"},{187,"EM_SETMODIFY"},{188,"EM_GETLINECOUNT"},{189,"EM_LINEINDEX"},{190,"EM_SETHANDLE"},{191,"EM_GETHANDLE"},{192,"EM_GETTHUMB"},{193,"EM_LINELENGTH"},{194,"EM_REPLACESEL"},{195,"EM_SETFONT"},{196,"EM_GETLINE"},{197,"EM_LIMITTEXT"},{197,"EM_SETLIMITTEXT"},{198,"EM_CANUNDO"},{199,"EM_UNDO"},{200,"EM_FMTLINES"},{201,"EM_LINEFROMCHAR"},{202,"EM_SETWORDBREAK"},{203,"EM_SETTABSTOPS"},{204,"EM_SETPASSWORDCHAR"},{205,"EM_EMPTYUNDOBUFFER"},{206,"EM_GETFIRSTVISIBLELINE"},{207,"EM_SETREADONLY"},{209,"EM_SETWORDBREAKPROC"},{209,"EM_GETWORDBREAKPROC"},{210,"EM_GETPASSWORDCHAR"},{211,"EM_SETMARGINS"},{212,"EM_GETMARGINS"},{213,"EM_GETLIMITTEXT"},{214,"EM_POSFROMCHAR"},{215,"EM_CHARFROMPOS"},{216,"EM_SETIMESTATUS"},{217,"EM_GETIMESTATUS"},{224,"SBM_SETPOS"},{225,"SBM_GETPOS"},{226,"SBM_SETRANGE"},{227,"SBM_GETRANGE"},{228,"SBM_ENABLE_ARROWS"},{230,"SBM_SETRANGEREDRAW"},{233,"SBM_SETSCROLLINFO"},{234,"SBM_GETSCROLLINFO"},{235,"SBM_GETSCROLLBARINFO"},{240,"BM_GETCHECK"},{241,"BM_SETCHECK"},{242,"BM_GETSTATE"},{243,"BM_SETSTATE"},{244,"BM_SETSTYLE"},{245,"BM_CLICK"},{246,"BM_GETIMAGE"},{247,"BM_SETIMAGE"},{248,"BM_SETDONTCLICK"},{255,"WM_INPUT"},{256,"WM_KEYDOWN"},{256,"WM_KEYFIRST"},{257,"WM_KEYUP"},{258,"WM_CHAR"},{259,"WM_DEADCHAR"},{260,"WM_SYSKEYDOWN"},{261,"WM_SYSKEYUP"},{262,"WM_SYSCHAR"},{263,"WM_SYSDEADCHAR"},{264,"WM_KEYLAST"},{265,"WM_UNICHAR"},{265,"WM_WNT_CONVERTREQUESTEX"},{266,"WM_CONVERTREQUEST"},{267,"WM_CONVERTRESULT"},{268,"WM_INTERIM"},{269,"WM_IME_STARTCOMPOSITION"},{270,"WM_IME_ENDCOMPOSITION"},{271,"WM_IME_COMPOSITION"},{271,"WM_IME_KEYLAST"},{272,"WM_INITDIALOG"},{273,"WM_COMMAND"},{274,"WM_SYSCOMMAND"},{275,"WM_TIMER"},{276,"WM_HSCROLL"},{277,"WM_VSCROLL"},{278,"WM_INITMENU"},{279,"WM_INITMENUPOPUP"},{280,"WM_SYSTIMER"},{287,"WM_MENUSELECT"},{288,"WM_MENUCHAR"},{289,"WM_ENTERIDLE"},{290,"WM_MENURBUTTONUP"},{291,"WM_MENUDRAG"},{292,"WM_MENUGETOBJECT"},{293,"WM_UNINITMENUPOPUP"},{294,"WM_MENUCOMMAND"},{295,"WM_CHANGEUISTATE"},{296,"WM_UPDATEUISTATE"},{297,"WM_QUERYUISTATE"},{306,"WM_CTLCOLORMSGBOX"},{307,"WM_CTLCOLOREDIT"},{308,"WM_CTLCOLORLISTBOX"},{309,"WM_CTLCOLORBTN"},{310,"WM_CTLCOLORDLG"},{311,"WM_CTLCOLORSCROLLBAR"},{312,"WM_CTLCOLORSTATIC"},{512,"WM_MOUSEFIRST"},{512,"WM_MOUSEMOVE"},{513,"WM_LBUTTONDOWN"},{514,"WM_LBUTTONUP"},{515,"WM_LBUTTONDBLCLK"},{516,"WM_RBUTTONDOWN"},{517,"WM_RBUTTONUP"},{518,"WM_RBUTTONDBLCLK"},{519,"WM_MBUTTONDOWN"},{520,"WM_MBUTTONUP"},{521,"WM_MBUTTONDBLCLK"},{521,"WM_MOUSELAST"},{522,"WM_MOUSEWHEEL"},{523,"WM_XBUTTONDOWN"},{524,"WM_XBUTTONUP"},{525,"WM_XBUTTONDBLCLK"},{528,"WM_PARENTNOTIFY"},{529,"WM_ENTERMENULOOP"},{530,"WM_EXITMENULOOP"},{531,"WM_NEXTMENU"},{532,"WM_SIZING"},{533,"WM_CAPTURECHANGED"},{534,"WM_MOVING"},{536,"WM_POWERBROADCAST"},{537,"WM_DEVICECHANGE"},{544,"WM_MDICREATE"},{545,"WM_MDIDESTROY"},{546,"WM_MDIACTIVATE"},{547,"WM_MDIRESTORE"},{548,"WM_MDINEXT"},{549,"WM_MDIMAXIMIZE"},{550,"WM_MDITILE"},{551,"WM_MDICASCADE"},{552,"WM_MDIICONARRANGE"},{553,"WM_MDIGETACTIVE"},{560,"WM_MDISETMENU"},{561,"WM_ENTERSIZEMOVE"},{562,"WM_EXITSIZEMOVE"},{563,"WM_DROPFILES"},{564,"WM_MDIREFRESHMENU"},{640,"WM_IME_REPORT"},{641,"WM_IME_SETCONTEXT"},{642,"WM_IME_NOTIFY"},{643,"WM_IME_CONTROL"},{644,"WM_IME_COMPOSITIONFULL"},{645,"WM_IME_SELECT"},{646,"WM_IME_CHAR"},{648,"WM_IME_REQUEST"},{656,"WM_IMEKEYDOWN"},{656,"WM_IME_KEYDOWN"},{657,"WM_IMEKEYUP"},{657,"WM_IME_KEYUP"},{672,"WM_NCMOUSEHOVER"},{673,"WM_MOUSEHOVER"},{674,"WM_NCMOUSELEAVE"},{675,"WM_MOUSELEAVE"},{768,"WM_CUT"},{769,"WM_COPY"},{770,"WM_PASTE"},{771,"WM_CLEAR"},{772,"WM_UNDO"},{773,"WM_RENDERFORMAT"},{774,"WM_RENDERALLFORMATS"},{775,"WM_DESTROYCLIPBOARD"},{776,"WM_DRAWCLIPBOARD"},{777,"WM_PAINTCLIPBOARD"},{778,"WM_VSCROLLCLIPBOARD"},{779,"WM_SIZECLIPBOARD"},{780,"WM_ASKCBFORMATNAME"},{781,"WM_CHANGECBCHAIN"},{782,"WM_HSCROLLCLIPBOARD"},{783,"WM_QUERYNEWPALETTE"},{784,"WM_PALETTEISCHANGING"},{785,"WM_PALETTECHANGED"},{786,"WM_HOTKEY"},{791,"WM_PRINT"},{792,"WM_PRINTCLIENT"},{793,"WM_APPCOMMAND"},{856,"WM_HANDHELDFIRST"},{863,"WM_HANDHELDLAST"},{864,"WM_AFXFIRST"},{895,"WM_AFXLAST"},{896,"WM_PENWINFIRST"},{897,"WM_RCRESULT"},{898,"WM_HOOKRCRESULT"},{899,"WM_GLOBALRCCHANGE"},{899,"WM_PENMISCINFO"},{900,"WM_SKB"},{901,"WM_HEDITCTL"},{901,"WM_PENCTL"},{902,"WM_PENMISC"},{903,"WM_CTLINIT"},{904,"WM_PENEVENT"},{911,"WM_PENWINLAST"},{1024,"DDM_SETFMT"},{1024,"DM_GETDEFID"},{1024,"NIN_SELECT"},{1024,"TBM_GETPOS"},{1024,"WM_PSD_PAGESETUPDLG"},{1024,"WM_USER"},{1025,"CBEM_INSERTITEMA"},{1025,"DDM_DRAW"},{1025,"DM_SETDEFID"},{1025,"HKM_SETHOTKEY"},{1025,"PBM_SETRANGE"},{1025,"RB_INSERTBANDA"},{1025,"SB_SETTEXTA"},{1025,"TB_ENABLEBUTTON"},{1025,"TBM_GETRANGEMIN"},{1025,"TTM_ACTIVATE"},{1025,"WM_CHOOSEFONT_GETLOGFONT"},{1025,"WM_PSD_FULLPAGERECT"},{1026,"CBEM_SETIMAGELIST"},{1026,"DDM_CLOSE"},{1026,"DM_REPOSITION"},{1026,"HKM_GETHOTKEY"},{1026,"PBM_SETPOS"},{1026,"RB_DELETEBAND"},{1026,"SB_GETTEXTA"},{1026,"TB_CHECKBUTTON"},{1026,"TBM_GETRANGEMAX"},{1026,"WM_PSD_MINMARGINRECT"},{1027,"CBEM_GETIMAGELIST"},{1027,"DDM_BEGIN"},{1027,"HKM_SETRULES"},{1027,"PBM_DELTAPOS"},{1027,"RB_GETBARINFO"},{1027,"SB_GETTEXTLENGTHA"},{1027,"TBM_GETTIC"},{1027,"TB_PRESSBUTTON"},{1027,"TTM_SETDELAYTIME"},{1027,"WM_PSD_MARGINRECT"},{1028,"CBEM_GETITEMA"},{1028,"DDM_END"},{1028,"PBM_SETSTEP"},{1028,"RB_SETBARINFO"},{1028,"SB_SETPARTS"},{1028,"TB_HIDEBUTTON"},{1028,"TBM_SETTIC"},{1028,"TTM_ADDTOOLA"},{1028,"WM_PSD_GREEKTEXTRECT"},{1029,"CBEM_SETITEMA"},{1029,"PBM_STEPIT"},{1029,"TB_INDETERMINATE"},{1029,"TBM_SETPOS"},{1029,"TTM_DELTOOLA"},{1029,"WM_PSD_ENVSTAMPRECT"},{1030,"CBEM_GETCOMBOCONTROL"},{1030,"PBM_SETRANGE32"},{1030,"RB_SETBANDINFOA"},{1030,"SB_GETPARTS"},{1030,"TB_MARKBUTTON"},{1030,"TBM_SETRANGE"},{1030,"TTM_NEWTOOLRECTA"},{1030,"WM_PSD_YAFULLPAGERECT"},{1031,"CBEM_GETEDITCONTROL"},{1031,"PBM_GETRANGE"},{1031,"RB_SETPARENT"},{1031,"SB_GETBORDERS"},{1031,"TBM_SETRANGEMIN"},{1031,"TTM_RELAYEVENT"},{1032,"CBEM_SETEXSTYLE"},{1032,"PBM_GETPOS"},{1032,"RB_HITTEST"},{1032,"SB_SETMINHEIGHT"},{1032,"TBM_SETRANGEMAX"},{1032,"TTM_GETTOOLINFOA"},{1033,"CBEM_GETEXSTYLE"},{1033,"CBEM_GETEXTENDEDSTYLE"},{1033,"PBM_SETBARCOLOR"},{1033,"RB_GETRECT"},{1033,"SB_SIMPLE"},{1033,"TB_ISBUTTONENABLED"},{1033,"TBM_CLEARTICS"},{1033,"TTM_SETTOOLINFOA"},{1034,"CBEM_HASEDITCHANGED"},{1034,"RB_INSERTBANDW"},{1034,"SB_GETRECT"},{1034,"TB_ISBUTTONCHECKED"},{1034,"TBM_SETSEL"},{1034,"TTM_HITTESTA"},{1034,"WIZ_QUERYNUMPAGES"},{1035,"CBEM_INSERTITEMW"},{1035,"RB_SETBANDINFOW"},{1035,"SB_SETTEXTW"},{1035,"TB_ISBUTTONPRESSED"},{1035,"TBM_SETSELSTART"},{1035,"TTM_GETTEXTA"},{1035,"WIZ_NEXT"},{1036,"CBEM_SETITEMW"},{1036,"RB_GETBANDCOUNT"},{1036,"SB_GETTEXTLENGTHW"},{1036,"TB_ISBUTTONHIDDEN"},{1036,"TBM_SETSELEND"},{1036,"TTM_UPDATETIPTEXTA"},{1036,"WIZ_PREV"},{1037,"CBEM_GETITEMW"},{1037,"RB_GETROWCOUNT"},{1037,"SB_GETTEXTW"},{1037,"TB_ISBUTTONINDETERMINATE"},{1037,"TTM_GETTOOLCOUNT"},{1038,"CBEM_SETEXTENDEDSTYLE"},{1038,"RB_GETROWHEIGHT"},{1038,"SB_ISSIMPLE"},{1038,"TB_ISBUTTONHIGHLIGHTED"},{1038,"TBM_GETPTICS"},{1038,"TTM_ENUMTOOLSA"},{1039,"SB_SETICON"},{1039,"TBM_GETTICPOS"},{1039,"TTM_GETCURRENTTOOLA"},{1040,"RB_IDTOINDEX"},{1040,"SB_SETTIPTEXTA"},{1040,"TBM_GETNUMTICS"},{1040,"TTM_WINDOWFROMPOINT"},{1041,"RB_GETTOOLTIPS"},{1041,"SB_SETTIPTEXTW"},{1041,"TBM_GETSELSTART"},{1041,"TB_SETSTATE"},{1041,"TTM_TRACKACTIVATE"},{1042,"RB_SETTOOLTIPS"},{1042,"SB_GETTIPTEXTA"},{1042,"TB_GETSTATE"},{1042,"TBM_GETSELEND"},{1042,"TTM_TRACKPOSITION"},{1043,"RB_SETBKCOLOR"},{1043,"SB_GETTIPTEXTW"},{1043,"TB_ADDBITMAP"},{1043,"TBM_CLEARSEL"},{1043,"TTM_SETTIPBKCOLOR"},{1044,"RB_GETBKCOLOR"},{1044,"SB_GETICON"},{1044,"TB_ADDBUTTONSA"},{1044,"TBM_SETTICFREQ"},{1044,"TTM_SETTIPTEXTCOLOR"},{1045,"RB_SETTEXTCOLOR"},{1045,"TB_INSERTBUTTONA"},{1045,"TBM_SETPAGESIZE"},{1045,"TTM_GETDELAYTIME"},{1046,"RB_GETTEXTCOLOR"},{1046,"TB_DELETEBUTTON"},{1046,"TBM_GETPAGESIZE"},{1046,"TTM_GETTIPBKCOLOR"},{1047,"RB_SIZETORECT"},{1047,"TB_GETBUTTON"},{1047,"TBM_SETLINESIZE"},{1047,"TTM_GETTIPTEXTCOLOR"},{1048,"RB_BEGINDRAG"},{1048,"TB_BUTTONCOUNT"},{1048,"TBM_GETLINESIZE"},{1048,"TTM_SETMAXTIPWIDTH"},{1049,"RB_ENDDRAG"},{1049,"TB_COMMANDTOINDEX"},{1049,"TBM_GETTHUMBRECT"},{1049,"TTM_GETMAXTIPWIDTH"},{1050,"RB_DRAGMOVE"},{1050,"TBM_GETCHANNELRECT"},{1050,"TB_SAVERESTOREA"},{1050,"TTM_SETMARGIN"},{1051,"RB_GETBARHEIGHT"},{1051,"TB_CUSTOMIZE"},{1051,"TBM_SETTHUMBLENGTH"},{1051,"TTM_GETMARGIN"},{1052,"RB_GETBANDINFOW"},{1052,"TB_ADDSTRINGA"},{1052,"TBM_GETTHUMBLENGTH"},{1052,"TTM_POP"},{1053,"RB_GETBANDINFOA"},{1053,"TB_GETITEMRECT"},{1053,"TBM_SETTOOLTIPS"},{1053,"TTM_UPDATE"},{1054,"RB_MINIMIZEBAND"},{1054,"TB_BUTTONSTRUCTSIZE"},{1054,"TBM_GETTOOLTIPS"},{1054,"TTM_GETBUBBLESIZE"},{1055,"RB_MAXIMIZEBAND"},{1055,"TBM_SETTIPSIDE"},{1055,"TB_SETBUTTONSIZE"},{1055,"TTM_ADJUSTRECT"},{1056,"TBM_SETBUDDY"},{1056,"TB_SETBITMAPSIZE"},{1056,"TTM_SETTITLEA"},{1057,"MSG_FTS_JUMP_VA"},{1057,"TB_AUTOSIZE"},{1057,"TBM_GETBUDDY"},{1057,"TTM_SETTITLEW"},{1058,"RB_GETBANDBORDERS"},{1059,"MSG_FTS_JUMP_QWORD"},{1059,"RB_SHOWBAND"},{1059,"TB_GETTOOLTIPS"},{1060,"MSG_REINDEX_REQUEST"},{1060,"TB_SETTOOLTIPS"},{1061,"MSG_FTS_WHERE_IS_IT"},{1061,"RB_SETPALETTE"},{1061,"TB_SETPARENT"},{1062,"RB_GETPALETTE"},{1063,"RB_MOVEBAND"},{1063,"TB_SETROWS"},{1064,"TB_GETROWS"},{1065,"TB_GETBITMAPFLAGS"},{1066,"TB_SETCMDID"},{1067,"RB_PUSHCHEVRON"},{1067,"TB_CHANGEBITMAP"},{1068,"TB_GETBITMAP"},{1069,"MSG_GET_DEFFONT"},{1069,"TB_GETBUTTONTEXTA"},{1070,"TB_REPLACEBITMAP"},{1071,"TB_SETINDENT"},{1072,"TB_SETIMAGELIST"},{1073,"TB_GETIMAGELIST"},{1074,"TB_LOADIMAGES"},{1074,"EM_CANPASTE"},{1074,"TTM_ADDTOOLW"},{1075,"EM_DISPLAYBAND"},{1075,"TB_GETRECT"},{1075,"TTM_DELTOOLW"},{1076,"EM_EXGETSEL"},{1076,"TB_SETHOTIMAGELIST"},{1076,"TTM_NEWTOOLRECTW"},{1077,"EM_EXLIMITTEXT"},{1077,"TB_GETHOTIMAGELIST"},{1077,"TTM_GETTOOLINFOW"},{1078,"EM_EXLINEFROMCHAR"},{1078,"TB_SETDISABLEDIMAGELIST"},{1078,"TTM_SETTOOLINFOW"},{1079,"EM_EXSETSEL"},{1079,"TB_GETDISABLEDIMAGELIST"},{1079,"TTM_HITTESTW"},{1080,"EM_FINDTEXT"},{1080,"TB_SETSTYLE"},{1080,"TTM_GETTEXTW"},{1081,"EM_FORMATRANGE"},{1081,"TB_GETSTYLE"},{1081,"TTM_UPDATETIPTEXTW"},{1082,"EM_GETCHARFORMAT"},{1082,"TB_GETBUTTONSIZE"},{1082,"TTM_ENUMTOOLSW"},{1083,"EM_GETEVENTMASK"},{1083,"TB_SETBUTTONWIDTH"},{1083,"TTM_GETCURRENTTOOLW"},{1084,"EM_GETOLEINTERFACE"},{1084,"TB_SETMAXTEXTROWS"},{1085,"EM_GETPARAFORMAT"},{1085,"TB_GETTEXTROWS"},{1086,"EM_GETSELTEXT"},{1086,"TB_GETOBJECT"},{1087,"EM_HIDESELECTION"},{1087,"TB_GETBUTTONINFOW"},{1088,"EM_PASTESPECIAL"},{1088,"TB_SETBUTTONINFOW"},{1089,"EM_REQUESTRESIZE"},{1089,"TB_GETBUTTONINFOA"},{1090,"EM_SELECTIONTYPE"},{1090,"TB_SETBUTTONINFOA"},{1091,"EM_SETBKGNDCOLOR"},{1091,"TB_INSERTBUTTONW"},{1092,"EM_SETCHARFORMAT"},{1092,"TB_ADDBUTTONSW"},{1093,"EM_SETEVENTMASK"},{1093,"TB_HITTEST"},{1094,"EM_SETOLECALLBACK"},{1094,"TB_SETDRAWTEXTFLAGS"},{1095,"EM_SETPARAFORMAT"},{1095,"TB_GETHOTITEM"},{1096,"EM_SETTARGETDEVICE"},{1096,"TB_SETHOTITEM"},{1097,"EM_STREAMIN"},{1097,"TB_SETANCHORHIGHLIGHT"},{1098,"EM_STREAMOUT"},{1098,"TB_GETANCHORHIGHLIGHT"},{1099,"EM_GETTEXTRANGE"},{1099,"TB_GETBUTTONTEXTW"},{1100,"EM_FINDWORDBREAK"},{1100,"TB_SAVERESTOREW"},{1101,"EM_SETOPTIONS"},{1101,"TB_ADDSTRINGW"},{1102,"EM_GETOPTIONS"},{1102,"TB_MAPACCELERATORA"},{1103,"EM_FINDTEXTEX"},{1103,"TB_GETINSERTMARK"},{1104,"EM_GETWORDBREAKPROCEX"},{1104,"TB_SETINSERTMARK"},{1105,"EM_SETWORDBREAKPROCEX"},{1105,"TB_INSERTMARKHITTEST"},{1106,"EM_SETUNDOLIMIT"},{1106,"TB_MOVEBUTTON"},{1107,"TB_GETMAXSIZE"},{1108,"EM_REDO"},{1108,"TB_SETEXTENDEDSTYLE"},{1109,"EM_CANREDO"},{1109,"TB_GETEXTENDEDSTYLE"},{1110,"EM_GETUNDONAME"},{1110,"TB_GETPADDING"},{1111,"EM_GETREDONAME"},{1111,"TB_SETPADDING"},{1112,"EM_STOPGROUPTYPING"},{1112,"TB_SETINSERTMARKCOLOR"},{1113,"EM_SETTEXTMODE"},{1113,"TB_GETINSERTMARKCOLOR"},{1114,"EM_GETTEXTMODE"},{1114,"TB_MAPACCELERATORW"},{1115,"EM_AUTOURLDETECT"},{1115,"TB_GETSTRINGW"},{1116,"EM_GETAUTOURLDETECT"},{1116,"TB_GETSTRINGA"},{1117,"EM_SETPALETTE"},{1118,"EM_GETTEXTEX"},{1119,"EM_GETTEXTLENGTHEX"},{1120,"EM_SHOWSCROLLBAR"},{1121,"EM_SETTEXTEX"},{1123,"TAPI_REPLY"},{1124,"ACM_OPENA"},{1124,"BFFM_SETSTATUSTEXTA"},{1124,"CDM_FIRST"},{1124,"CDM_GETSPEC"},{1124,"EM_SETPUNCTUATION"},{1124,"IPM_CLEARADDRESS"},{1124,"WM_CAP_UNICODE_START"},{1125,"ACM_PLAY"},{1125,"BFFM_ENABLEOK"},{1125,"CDM_GETFILEPATH"},{1125,"EM_GETPUNCTUATION"},{1125,"IPM_SETADDRESS"},{1125,"PSM_SETCURSEL"},{1125,"UDM_SETRANGE"},{1125,"WM_CHOOSEFONT_SETLOGFONT"},{1126,"ACM_STOP"},{1126,"BFFM_SETSELECTIONA"},{1126,"CDM_GETFOLDERPATH"},{1126,"EM_SETWORDWRAPMODE"},{1126,"IPM_GETADDRESS"},{1126,"PSM_REMOVEPAGE"},{1126,"UDM_GETRANGE"},{1126,"WM_CAP_SET_CALLBACK_ERRORW"},{1126,"WM_CHOOSEFONT_SETFLAGS"},{1127,"ACM_OPENW"},{1127,"BFFM_SETSELECTIONW"},{1127,"CDM_GETFOLDERIDLIST"},{1127,"EM_GETWORDWRAPMODE"},{1127,"IPM_SETRANGE"},{1127,"PSM_ADDPAGE"},{1127,"UDM_SETPOS"},{1127,"WM_CAP_SET_CALLBACK_STATUSW"},{1128,"BFFM_SETSTATUSTEXTW"},{1128,"CDM_SETCONTROLTEXT"},{1128,"EM_SETIMECOLOR"},{1128,"IPM_SETFOCUS"},{1128,"PSM_CHANGED"},{1128,"UDM_GETPOS"},{1129,"CDM_HIDECONTROL"},{1129,"EM_GETIMECOLOR"},{1129,"IPM_ISBLANK"},{1129,"PSM_RESTARTWINDOWS"},{1129,"UDM_SETBUDDY"},{1130,"CDM_SETDEFEXT"},{1130,"EM_SETIMEOPTIONS"},{1130,"PSM_REBOOTSYSTEM"},{1130,"UDM_GETBUDDY"},{1131,"EM_GETIMEOPTIONS"},{1131,"PSM_CANCELTOCLOSE"},{1131,"UDM_SETACCEL"},{1132,"EM_CONVPOSITION"},{1132,"EM_CONVPOSITION"},{1132,"PSM_QUERYSIBLINGS"},{1132,"UDM_GETACCEL"},{1133,"MCIWNDM_GETZOOM"},{1133,"PSM_UNCHANGED"},{1133,"UDM_SETBASE"},{1134,"PSM_APPLY"},{1134,"UDM_GETBASE"},{1135,"PSM_SETTITLEA"},{1135,"UDM_SETRANGE32"},{1136,"PSM_SETWIZBUTTONS"},{1136,"UDM_GETRANGE32"},{1136,"WM_CAP_DRIVER_GET_NAMEW"},{1137,"PSM_PRESSBUTTON"},{1137,"UDM_SETPOS32"},{1137,"WM_CAP_DRIVER_GET_VERSIONW"},{1138,"PSM_SETCURSELID"},{1138,"UDM_GETPOS32"},{1139,"PSM_SETFINISHTEXTA"},{1140,"PSM_GETTABCONTROL"},{1141,"PSM_ISDIALOGMESSAGE"},{1142,"MCIWNDM_REALIZE"},{1142,"PSM_GETCURRENTPAGEHWND"},{1143,"MCIWNDM_SETTIMEFORMATA"},{1143,"PSM_INSERTPAGE"},{1144,"EM_SETLANGOPTIONS"},{1144,"MCIWNDM_GETTIMEFORMATA"},{1144,"PSM_SETTITLEW"},{1144,"WM_CAP_FILE_SET_CAPTURE_FILEW"},{1145,"EM_GETLANGOPTIONS"},{1145,"MCIWNDM_VALIDATEMEDIA"},{1145,"PSM_SETFINISHTEXTW"},{1145,"WM_CAP_FILE_GET_CAPTURE_FILEW"},{1146,"EM_GETIMECOMPMODE"},{1147,"EM_FINDTEXTW"},{1147,"MCIWNDM_PLAYTO"},{1147,"WM_CAP_FILE_SAVEASW"},{1148,"EM_FINDTEXTEXW"},{1148,"MCIWNDM_GETFILENAMEA"},{1149,"EM_RECONVERSION"},{1149,"MCIWNDM_GETDEVICEA"},{1149,"PSM_SETHEADERTITLEA"},{1149,"WM_CAP_FILE_SAVEDIBW"},{1150,"EM_SETIMEMODEBIAS"},{1150,"MCIWNDM_GETPALETTE"},{1150,"PSM_SETHEADERTITLEW"},{1151,"EM_GETIMEMODEBIAS"},{1151,"MCIWNDM_SETPALETTE"},{1151,"PSM_SETHEADERSUBTITLEA"},{1152,"MCIWNDM_GETERRORA"},{1152,"PSM_SETHEADERSUBTITLEW"},{1153,"PSM_HWNDTOINDEX"},{1154,"PSM_INDEXTOHWND"},{1155,"MCIWNDM_SETINACTIVETIMER"},{1155,"PSM_PAGETOINDEX"},{1156,"PSM_INDEXTOPAGE"},{1157,"DL_BEGINDRAG"},{1157,"MCIWNDM_GETINACTIVETIMER"},{1157,"PSM_IDTOINDEX"},{1158,"DL_DRAGGING"},{1158,"PSM_INDEXTOID"},{1159,"DL_DROPPED"},{1159,"PSM_GETRESULT"},{1160,"DL_CANCELDRAG"},{1160,"PSM_RECALCPAGESIZES"},{1164,"MCIWNDM_GET_SOURCE"},{1165,"MCIWNDM_PUT_SOURCE"},{1166,"MCIWNDM_GET_DEST"},{1167,"MCIWNDM_PUT_DEST"},{1168,"MCIWNDM_CAN_PLAY"},{1169,"MCIWNDM_CAN_WINDOW"},{1170,"MCIWNDM_CAN_RECORD"},{1171,"MCIWNDM_CAN_SAVE"},{1172,"MCIWNDM_CAN_EJECT"},{1173,"MCIWNDM_CAN_CONFIG"},{1174,"IE_GETINK"},{1174,"IE_MSGFIRST"},{1174,"MCIWNDM_PALETTEKICK"},{1175,"IE_SETINK"},{1176,"IE_GETPENTIP"},{1177,"IE_SETPENTIP"},{1178,"IE_GETERASERTIP"},{1179,"IE_SETERASERTIP"},{1180,"IE_GETBKGND"},{1181,"IE_SETBKGND"},{1182,"IE_GETGRIDORIGIN"},{1183,"IE_SETGRIDORIGIN"},{1184,"IE_GETGRIDPEN"},{1185,"IE_SETGRIDPEN"},{1186,"IE_GETGRIDSIZE"},{1187,"IE_SETGRIDSIZE"},{1188,"IE_GETMODE"},{1189,"IE_SETMODE"},{1190,"IE_GETINKRECT"},{1190,"WM_CAP_SET_MCI_DEVICEW"},{1191,"WM_CAP_GET_MCI_DEVICEW"},{1204,"WM_CAP_PAL_OPENW"},{1205,"WM_CAP_PAL_SAVEW"},{1208,"IE_GETAPPDATA"},{1209,"IE_SETAPPDATA"},{1210,"IE_GETDRAWOPTS"},{1211,"IE_SETDRAWOPTS"},{1212,"IE_GETFORMAT"},{1213,"IE_SETFORMAT"},{1214,"IE_GETINKINPUT"},{1215,"IE_SETINKINPUT"},{1216,"IE_GETNOTIFY"},{1217,"IE_SETNOTIFY"},{1218,"IE_GETRECOG"},{1219,"IE_SETRECOG"},{1220,"IE_GETSECURITY"},{1221,"IE_SETSECURITY"},{1222,"IE_GETSEL"},{1223,"IE_SETSEL"},{1224,"CDM_LAST"},{1224,"EM_SETBIDIOPTIONS"},{1224,"IE_DOCOMMAND"},{1224,"MCIWNDM_NOTIFYMODE"},{1225,"EM_GETBIDIOPTIONS"},{1225,"IE_GETCOMMAND"},{1226,"EM_SETTYPOGRAPHYOPTIONS"},{1226,"IE_GETCOUNT"},{1227,"EM_GETTYPOGRAPHYOPTIONS"},{1227,"IE_GETGESTURE"},{1227,"MCIWNDM_NOTIFYMEDIA"},{1228,"EM_SETEDITSTYLE"},{1228,"IE_GETMENU"},{1229,"EM_GETEDITSTYLE"},{1229,"IE_GETPAINTDC"},{1229,"MCIWNDM_NOTIFYERROR"},{1230,"IE_GETPDEVENT"},{1231,"IE_GETSELCOUNT"},{1232,"IE_GETSELITEMS"},{1233,"IE_GETSTYLE"},{1243,"MCIWNDM_SETTIMEFORMATW"},{1244,"EM_OUTLINE"},{1244,"EM_OUTLINE"},{1244,"MCIWNDM_GETTIMEFORMATW"},{1245,"EM_GETSCROLLPOS"},{1245,"EM_GETSCROLLPOS"},{1246,"EM_SETSCROLLPOS"},{1246,"EM_SETSCROLLPOS"},{1247,"EM_SETFONTSIZE"},{1247,"EM_SETFONTSIZE"},{1248,"EM_GETZOOM"},{1248,"MCIWNDM_GETFILENAMEW"},{1249,"EM_SETZOOM"},{1249,"MCIWNDM_GETDEVICEW"},{1250,"EM_GETVIEWKIND"},{1251,"EM_SETVIEWKIND"},{1252,"EM_GETPAGE"},{1252,"MCIWNDM_GETERRORW"},{1253,"EM_SETPAGE"},{1254,"EM_GETHYPHENATEINFO"},{1255,"EM_SETHYPHENATEINFO"},{1259,"EM_GETPAGEROTATE"},{1260,"EM_SETPAGEROTATE"},{1261,"EM_GETCTFMODEBIAS"},{1262,"EM_SETCTFMODEBIAS"},{1264,"EM_GETCTFOPENSTATUS"},{1265,"EM_SETCTFOPENSTATUS"},{1266,"EM_GETIMECOMPTEXT"},{1267,"EM_ISIME"},{1268,"EM_GETIMEPROPERTY"},{1293,"EM_GETQUERYRTFOBJ"},{1294,"EM_SETQUERYRTFOBJ"},{1536,"FM_GETFOCUS"},{1537,"FM_GETDRIVEINFOA"},{1538,"FM_GETSELCOUNT"},{1539,"FM_GETSELCOUNTLFN"},{1540,"FM_GETFILESELA"},{1541,"FM_GETFILESELLFNA"},{1542,"FM_REFRESH_WINDOWS"},{1543,"FM_RELOAD_EXTENSIONS"},{1553,"FM_GETDRIVEINFOW"},{1556,"FM_GETFILESELW"},{1557,"FM_GETFILESELLFNW"},{1625,"WLX_WM_SAS"},{2024,"SM_GETSELCOUNT"},{2024,"UM_GETSELCOUNT"},{2024,"WM_CPL_LAUNCH"},{2025,"SM_GETSERVERSELA"},{2025,"UM_GETUSERSELA"},{2025,"WM_CPL_LAUNCHED"},{2026,"SM_GETSERVERSELW"},{2026,"UM_GETUSERSELW"},{2027,"SM_GETCURFOCUSA"},{2027,"UM_GETGROUPSELA"},{2028,"SM_GETCURFOCUSW"},{2028,"UM_GETGROUPSELW"},{2029,"SM_GETOPTIONS"},{2029,"UM_GETCURFOCUSA"},{2030,"UM_GETCURFOCUSW"},{2031,"UM_GETOPTIONS"},{2032,"UM_GETOPTIONS2"},{4096,"LVM_FIRST"},{4096,"LVM_GETBKCOLOR"},{4097,"LVM_SETBKCOLOR"},{4098,"LVM_GETIMAGELIST"},{4099,"LVM_SETIMAGELIST"},{4100,"LVM_GETITEMCOUNT"},{4101,"LVM_GETITEMA"},{4102,"LVM_SETITEMA"},{4103,"LVM_INSERTITEMA"},{4104,"LVM_DELETEITEM"},{4105,"LVM_DELETEALLITEMS"},{4106,"LVM_GETCALLBACKMASK"},{4107,"LVM_SETCALLBACKMASK"},{4108,"LVM_GETNEXTITEM"},{4109,"LVM_FINDITEMA"},{4110,"LVM_GETITEMRECT"},{4111,"LVM_SETITEMPOSITION"},{4112,"LVM_GETITEMPOSITION"},{4113,"LVM_GETSTRINGWIDTHA"},{4114,"LVM_HITTEST"},{4115,"LVM_ENSUREVISIBLE"},{4116,"LVM_SCROLL"},{4117,"LVM_REDRAWITEMS"},{4118,"LVM_ARRANGE"},{4119,"LVM_EDITLABELA"},{4120,"LVM_GETEDITCONTROL"},{4121,"LVM_GETCOLUMNA"},{4122,"LVM_SETCOLUMNA"},{4123,"LVM_INSERTCOLUMNA"},{4124,"LVM_DELETECOLUMN"},{4125,"LVM_GETCOLUMNWIDTH"},{4126,"LVM_SETCOLUMNWIDTH"},{4127,"LVM_GETHEADER"},{4129,"LVM_CREATEDRAGIMAGE"},{4130,"LVM_GETVIEWRECT"},{4131,"LVM_GETTEXTCOLOR"},{4132,"LVM_SETTEXTCOLOR"},{4133,"LVM_GETTEXTBKCOLOR"},{4134,"LVM_SETTEXTBKCOLOR"},{4135,"LVM_GETTOPINDEX"},{4136,"LVM_GETCOUNTPERPAGE"},{4137,"LVM_GETORIGIN"},{4138,"LVM_UPDATE"},{4139,"LVM_SETITEMSTATE"},{4140,"LVM_GETITEMSTATE"},{4141,"LVM_GETITEMTEXTA"},{4142,"LVM_SETITEMTEXTA"},{4143,"LVM_SETITEMCOUNT"},{4144,"LVM_SORTITEMS"},{4145,"LVM_SETITEMPOSITION32"},{4146,"LVM_GETSELECTEDCOUNT"},{4147,"LVM_GETITEMSPACING"},{4148,"LVM_GETISEARCHSTRINGA"},{4149,"LVM_SETICONSPACING"},{4150,"LVM_SETEXTENDEDLISTVIEWSTYLE"},{4151,"LVM_GETEXTENDEDLISTVIEWSTYLE"},{4152,"LVM_GETSUBITEMRECT"},{4153,"LVM_SUBITEMHITTEST"},{4154,"LVM_SETCOLUMNORDERARRAY"},{4155,"LVM_GETCOLUMNORDERARRAY"},{4156,"LVM_SETHOTITEM"},{4157,"LVM_GETHOTITEM"},{4158,"LVM_SETHOTCURSOR"},{4159,"LVM_GETHOTCURSOR"},{4160,"LVM_APPROXIMATEVIEWRECT"},{4161,"LVM_SETWORKAREAS"},{4162,"LVM_GETSELECTIONMARK"},{4163,"LVM_SETSELECTIONMARK"},{4164,"LVM_SETBKIMAGEA"},{4165,"LVM_GETBKIMAGEA"},{4166,"LVM_GETWORKAREAS"},{4167,"LVM_SETHOVERTIME"},{4168,"LVM_GETHOVERTIME"},{4169,"LVM_GETNUMBEROFWORKAREAS"},{4170,"LVM_SETTOOLTIPS"},{4171,"LVM_GETITEMW"},{4172,"LVM_SETITEMW"},{4173,"LVM_INSERTITEMW"},{4174,"LVM_GETTOOLTIPS"},{4179,"LVM_FINDITEMW"},{4183,"LVM_GETSTRINGWIDTHW"},{4191,"LVM_GETCOLUMNW"},{4192,"LVM_SETCOLUMNW"},{4193,"LVM_INSERTCOLUMNW"},{4211,"LVM_GETITEMTEXTW"},{4212,"LVM_SETITEMTEXTW"},{4213,"LVM_GETISEARCHSTRINGW"},{4214,"LVM_EDITLABELW"},{4235,"LVM_GETBKIMAGEW"},{4236,"LVM_SETSELECTEDCOLUMN"},{4237,"LVM_SETTILEWIDTH"},{4238,"LVM_SETVIEW"},{4239,"LVM_GETVIEW"},{4241,"LVM_INSERTGROUP"},{4243,"LVM_SETGROUPINFO"},{4245,"LVM_GETGROUPINFO"},{4246,"LVM_REMOVEGROUP"},{4247,"LVM_MOVEGROUP"},{4250,"LVM_MOVEITEMTOGROUP"},{4251,"LVM_SETGROUPMETRICS"},{4252,"LVM_GETGROUPMETRICS"},{4253,"LVM_ENABLEGROUPVIEW"},{4254,"LVM_SORTGROUPS"},{4255,"LVM_INSERTGROUPSORTED"},{4256,"LVM_REMOVEALLGROUPS"},{4257,"LVM_HASGROUP"},{4258,"LVM_SETTILEVIEWINFO"},{4259,"LVM_GETTILEVIEWINFO"},{4260,"LVM_SETTILEINFO"},{4261,"LVM_GETTILEINFO"},{4262,"LVM_SETINSERTMARK"},{4263,"LVM_GETINSERTMARK"},{4264,"LVM_INSERTMARKHITTEST"},{4265,"LVM_GETINSERTMARKRECT"},{4266,"LVM_SETINSERTMARKCOLOR"},{4267,"LVM_GETINSERTMARKCOLOR"},{4269,"LVM_SETINFOTIP"},{4270,"LVM_GETSELECTEDCOLUMN"},{4271,"LVM_ISGROUPVIEWENABLED"},{4272,"LVM_GETOUTLINECOLOR"},{4273,"LVM_SETOUTLINECOLOR"},{4275,"LVM_CANCELEDITLABEL"},{4276,"LVM_MAPINDEXTOID"},{4277,"LVM_MAPIDTOINDEX"},{4278,"LVM_ISITEMVISIBLE"},{8192,"OCM__BASE"},{8197,"LVM_SETUNICODEFORMAT"},{8198,"LVM_GETUNICODEFORMAT"},{8217,"OCM_CTLCOLOR"},{8235,"OCM_DRAWITEM"},{8236,"OCM_MEASUREITEM"},{8237,"OCM_DELETEITEM"},{8238,"OCM_VKEYTOITEM"},{8239,"OCM_CHARTOITEM"},{8249,"OCM_COMPAREITEM"},{8270,"OCM_NOTIFY"},{8465,"OCM_COMMAND"},{8468,"OCM_HSCROLL"},{8469,"OCM_VSCROLL"},{8498,"OCM_CTLCOLORMSGBOX"},{8499,"OCM_CTLCOLOREDIT"},{8500,"OCM_CTLCOLORLISTBOX"},{8501,"OCM_CTLCOLORBTN"},{8502,"OCM_CTLCOLORDLG"},{8503,"OCM_CTLCOLORSCROLLBAR"},{8504,"OCM_CTLCOLORSTATIC"},{8720,"OCM_PARENTNOTIFY"},{32768,"WM_APP"},{52429,"WM_RASDIALEVENT"}};
    return eventMap[index];
}

namespace ulti{
    charset guichr;
    event tvnt;
    void guiobj::setfunc(int (*infnc)(void*)){
        fnc=infnc;
    }
    sdfptr guiobj::getfunc(){
        return fnc;
    }
    void guiobj::create(){

    }
    std::string guiobj::getname(){
        return guiname;
    }
    void guiobj::setname(const std::string& instr){
        guiname=instr;
    }
    std::string guiobj::getstring(){
        return str;
    }
    int guiobj::getvalue(){
        return value;
    }
    void guiobj::setstring(const std::string& instr){
        str=instr;
    }
    void guiobj::setvalue(int inval){
        value=inval;
    }
    void guiobj::apply(surface& sur){
        sfc.draw(sur,pos.x,pos.y);
    }
    int guiobj::submit_listens(eventSystem* manager){
        esys=manager;
        esys->listen_for_event(*this,"mousedown");
        esys->listen_for_event(*this,"mouseup");
		return 1;
    }
    int guiobj::handle_event(event* evnt){
        if (evnt->type=="mousedown"){
            if (collide(evnt->x,evnt->y,1,1,pos.x,pos.y,size.x,size.y)){
                str="down";
            }
			return 1;
        }
        else if (evnt->type=="mouseup"){
            if (str=="down"){
                if (collide(evnt->x,evnt->y,1,1,pos.x,pos.y,size.x,size.y)){
                    str="clicked";
                    tvnt.type="click";
                    tvnt.caller=this;
                    tvnt.x=evnt->x;tvnt.y=evnt->y;
                    tvnt.command=this->guiname;
                    if (esys!=NULL){
                        esys->send_event(tvnt);
                    }
                }
                else{
                    str="";
                }
            }
            else{
                str="";
            }
			return 1;
        }
        else if (evnt->type=="mousemove"){

        }
		return 0;
    }
    int guiobj::class_type(){
        return 7;
    }

    std::string button::getstring(){
        return str;
    }
    void button::setstring(const std::string& instr){
        str=instr;
    }
    void button::apply(surface& sur){
        sfc.draw(sur,pos.x,pos.y);
    }
    int button::submit_listens(eventSystem* manager){
        esys=manager;
        esys->listen_for_event(*this,"mousedown");
        esys->listen_for_event(*this,"mouseup");
		return 1;
    }

    int button::handle_event(event* evnt){
        if (evnt->type=="mousedown"){
            if (collide(evnt->x,evnt->y,1,1,pos.x,pos.y,size.x,size.y)){
                value=1;
                create();
            }
        }
        else if (evnt->type=="mouseup"){
            if (value==1){
                if (collide(evnt->x,evnt->y,1,1,pos.x,pos.y,size.x,size.y)){
                    value=2;
                    tvnt.type="click";
                    tvnt.caller=this;
                    tvnt.x=evnt->x;tvnt.y=evnt->y;
                    tvnt.command=this->guiname;
                    if (esys!=NULL){
                        if (fnc!=NULL){
                            tvnt.type="function";
                            tvnt.extra=(void*)fnc;
                            esys->send_event(tvnt);
                            create();
                            value=0;
                        }
                        else{
                            esys->send_event(tvnt);
                            create();
                            value=0;
                        }
                    }
                }
                else{
                    value=0;
                    create();
                }
            }
            else{
                value=0;
                create();
            }
        }
        else if (evnt->type=="mousemove"){

        }
        return 2;//pass along
    }
    int button::class_type(){
        return 7;
    }
    void button::create(){
        setname(guiname);
        float fsize = (width()-10)/(str.size());
        if (value==1){
             this->getsfc().clear(dnbcol);
             guichr.setfont(fsize-3,dnfcol,0,1);
        }
        else{
             this->getsfc().clear(bcol);
             guichr.setfont(fsize-3,fcol,0,1);
        }
        guichr.writemsg(str,5,height()/2-fsize/2,getsfc(),3,3);
    }
    button::button(std::string iname, std::string val, int isx, int isy, ui32 fcolor, ui32 bcolor){
        guiname=iname; str=val; resize(icoord(isx,isy)); fcol=fcolor; bcol=bcolor;
        dnfcol=invcol(fcol); dnbcol=invcol(bcol);
        fnc=NULL;
        create();
    }
    button::button(){
        guiname=""; str=""; resize(icoord(150,75)); fcol=0;bcol=0xffffffff;
        dnfcol=invcol(fcol); dnbcol=invcol(bcol);
        fnc=NULL;
        create();
    }
    void button::setdowncol(ui32 ifcol, ui32 ibcol){
        dnfcol=ifcol,dnbcol=ibcol;
    }
    void button::setupcol(ui32 ifcol, ui32 ibcol){
        fcol=ifcol,bcol=ibcol;
    }
    void label::create(){

        guichr.setfont(fsize,fcol,0,thick);
        this->resize(coord((fsize+3)*str.size()+10-3,fsize+10));getsfc().clear(0x0);
        guichr.writemsg(str,5,5,getsfc(),3,9);
    }
    int label::submit_listens(eventSystem* manager){
		return -1;
    }
    int label::handle_event(event* evnt){
		return -1;
    }
    int label::class_type(){
        return 7;
    }
    void label::setfont(int fontsize,ui32 icol,int ithick){
        fsize=fontsize;fcol=icol;thick=ithick;
    }
    label::label(){
        str=""; fsize=12;fcol=0xff000000;thick=1;
    }
    label::label(std::string instr, int fontsize, ui32 icol, int ithick){
        str=instr; fsize=fontsize;fcol=icol;thick=ithick;
        create();
    }

    bool pressed(int key){
        return (GetAsyncKeyState(key)!=0);
    }

    unsigned long getScanCode(int vkcode){
        return MapVirtualKeyA(vkcode,MAPVK_VK_TO_VSC);
    }
    unsigned long getVKCode(int scancode){
        return MapVirtualKeyA(scancode,MAPVK_VSC_TO_VK);
    }

    int sendKeyDown(HWND hwnd, int key){
        LPARAM lp = (GetAsyncKeyState(key)!=0);
        lp = lp << 8;//Previous State Before Press, moved over 14 so we can can add and shift for scan code
        lp += getScanCode(key)&0b11111111;
        lp = lp << 16;
        lp += 1;

        return PostMessage(hwnd,WM_KEYDOWN,(WPARAM)key,lp);
    }
    int sendKeyUp(HWND hwnd, int key){
        LPARAM lp = 1;
        lp = lp << 1;
        lp += 1;
        lp = lp << 8;
        lp += getScanCode(key)&0b11111111;
        lp = lp << 16;
        lp += 1;

        return PostMessage(hwnd,WM_KEYUP,(WPARAM)key,lp);
    }
    int captureJoy(HWND hwnd){
        return joySetCapture(hwnd, JOYSTICKID1, NULL, FALSE);
    }
    int captureRaw(HWND hwnd){
        RAWINPUTDEVICE rid;

        rid.usUsagePage = 1;
        rid.usUsage     = 4; // Joystick
        rid.dwFlags     = 0;
        rid.hwndTarget  = hwnd;

        if(!RegisterRawInputDevices(&rid, 1, sizeof(RAWINPUTDEVICE)))
            return -1;
    }
    std::string requestSaveDialog(const std::string& typeFilter, window* owner, ui32 flags){
        OPENFILENAMEA ofn;       // common dialog box structure
        char szFile[MAX_PATH];  // buffer for file name
        HWND hwnd = NULL;              // owner window
        if (owner!=NULL)
            hwnd=owner->veil.hwnd;
        HANDLE hf;              // file handle

        // Initialize OPENFILENAME
        ZeroMemory(&ofn, sizeof(ofn));
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = hwnd;
        ofn.lpstrFile = szFile;
        // Set lpstrFile[0] to '\0' so that GetOpenFileName does not
        // use the contents of szFile to initialize itself.
        ofn.lpstrFile[0] = '\0';
        ofn.nMaxFile = sizeof(szFile);
        ofn.lpstrFilter = typeFilter.c_str();
        ofn.nFilterIndex = 1;
        ofn.lpstrFileTitle = NULL;
        ofn.nMaxFileTitle = 0;
        ofn.lpstrInitialDir = NULL;
        ofn.Flags = flags;


        //OFN_OVERWRITEPROMPT

        // Display the Open dialog box.

        if (GetSaveFileNameA(&ofn)==TRUE){
            std::string ret = ofn.lpstrFile;
            return ret;
        }
        return "";
    }
    std::string requestLoadDialog(const std::string& typeFilter, window* owner, ui32 flags){
        OPENFILENAMEA ofn;       // common dialog box structure
        char szFile[MAX_PATH];  // buffer for file name
        HWND hwnd = NULL;              // owner window
        if (owner!=NULL)
            hwnd=owner->veil.hwnd;
        HANDLE hf;              // file handle

        // Initialize OPENFILENAME
        ZeroMemory(&ofn, sizeof(ofn));
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = hwnd;
        ofn.lpstrFile = szFile;
        // Set lpstrFile[0] to '\0' so that GetOpenFileName does not
        // use the contents of szFile to initialize itself.
        ofn.lpstrFile[0] = '\0';
        ofn.nMaxFile = sizeof(szFile);
        ofn.lpstrFilter = typeFilter.c_str();
        ofn.nFilterIndex = 1;
        ofn.lpstrFileTitle = NULL;
        ofn.nMaxFileTitle = 0;
        ofn.lpstrInitialDir = NULL;
        ofn.Flags = flags;

        // Display the Open dialog box.

        if (GetOpenFileNameA(&ofn)==TRUE){
            std::string ret = ofn.lpstrFile;
            return ret;
        }
        return "";
    }
    std::vector<std::string> requestMultiLoadDialog(){
        //OFN_ALLOWMULTISELECT
        //split by NULL
        return std::vector<std::string>();
    }

    std::string requestDirectoryDialog(window* owner){
        char buf[MAX_PATH];
        buf[0]=NULL;
        BROWSEINFOA binfo;
        binfo.hwndOwner=NULL;
        if (owner!=NULL)
            binfo.hwndOwner=owner->veil.hwnd;
        binfo.lParam=NULL;
        binfo.lpfn=NULL;
        binfo.lpszTitle="";
        binfo.pidlRoot=NULL;
        binfo.pszDisplayName=buf;
        binfo.ulFlags=BIF_USENEWUI;
        LPITEMIDLIST res = SHBrowseForFolderA(&binfo);
        if (res==NULL)
            return "";
        SHGetPathFromIDListA(res,buf);
        std::string ret = buf;
        return ret;
    }
VendorElements::VendorElements(){
    menu=NULL;
    hwnd=NULL;
    font=NULL;
}

    positioning::positioning(){x=y=minwidth=minheight=maxwidth=maxheight=xweight=yweight=row=col=calculatedHeight=calculatedWidth=calculatedX=calculatedY=marginLeft=marginRight=marginTop=marginBottom=0;}
positioning::positioning(const positioning& ipos){postype=ipos.postype;x=ipos.x;y=ipos.y;minwidth=ipos.minwidth;minheight=ipos.minheight;
maxwidth=ipos.maxwidth;maxheight=ipos.maxheight;xweight=ipos.xweight;yweight=ipos.yweight;row=ipos.row;col=ipos.col;
calculatedWidth=ipos.calculatedWidth;calculatedHeight=ipos.calculatedHeight;calculatedX=ipos.calculatedX;calculatedY=ipos.calculatedY;
marginBottom=ipos.marginBottom;marginLeft=ipos.marginLeft;marginRight=ipos.marginRight;marginTop=ipos.marginTop;}
positioning::positioning(int icol, int irow, int iwidth, int iheight, int ixweight, int iyweight, PositionType ipostype):positioning(){
    col=icol;
    row=irow;
    minwidth=iwidth;
    minheight=iheight;
    xweight=ixweight;
    yweight=iyweight;
    postype=ipostype;
}

positioning DEFAULT_POSITIONING = positioning(0,0,20,20,0,0,PositionType::POSITION_NORMAL);
unsigned long DEFAULT_FLAGS = WINDOW_UNKNOWN;
WindowsClasses WinClasses;


ClassType::ClassType(){
    origin=CLASS_NONE;
    flags=0;
}
ClassType::ClassType(ClassCreation howCreated,unsigned long defaultFlags){
    origin=howCreated;
    flags=defaultFlags;
}



WindowsClasses::WindowsClasses(){
    classes = {
        {"Animation",ClassType(CLASS_DEFAULT)},
        {"Button",ClassType(CLASS_DEFAULT)},
        {"ComboBox",ClassType(CLASS_DEFAULT)},
        {"ComboBoxEx",ClassType(CLASS_DEFAULT)},
        {"Date And Time Picker",ClassType(CLASS_DEFAULT)},
        {"Edit",ClassType(CLASS_DEFAULT)},
        {"Flat Scroll Bar",ClassType(CLASS_DEFAULT)},
        {"Header Control",ClassType(CLASS_DEFAULT)},
        {"Hot Key",ClassType(CLASS_DEFAULT)},
        {"Image List",ClassType(CLASS_DEFAULT)},
        {"IP Address Control",ClassType(CLASS_DEFAULT)},
        {"List Box",ClassType(CLASS_DEFAULT)},
        {"ListBox",ClassType(CLASS_DEFAULT)},
        {"List View",ClassType(CLASS_DEFAULT)},
        {"MdiClient",ClassType(CLASS_DEFAULT)},
        {"Month Calendar",ClassType(CLASS_DEFAULT)},
        {"Pager",ClassType(CLASS_DEFAULT)},
        {"Progress Bar",ClassType(CLASS_DEFAULT)},
        {"Property Sheet",ClassType(CLASS_DEFAULT)},
        {"Rebar",ClassType(CLASS_DEFAULT)},
        {"Rich Edit",ClassType(CLASS_DEFAULT)},
        {"RichEdit",ClassType(CLASS_DEFAULT)},
        {"RichEdit_Class",ClassType(CLASS_DEFAULT)},
        {"Scroll Bar",ClassType(CLASS_DEFAULT)},
        {"ScrollBar",ClassType(CLASS_DEFAULT)},
        {"Static Control",ClassType(CLASS_DEFAULT)},
        {"Static",ClassType(CLASS_DEFAULT)},
        {"Status Bar",ClassType(CLASS_DEFAULT)},
        {"SysLink",ClassType(CLASS_DEFAULT)},
        {"Tab",ClassType(CLASS_DEFAULT)},
        {"Task Dialog",ClassType(CLASS_DEFAULT)},
        {"Toolbar",ClassType(CLASS_DEFAULT)},
        {"Tooltip",ClassType(CLASS_DEFAULT)},
        {"Trackbar",ClassType(CLASS_DEFAULT)},
        {"Tree View",ClassType(CLASS_DEFAULT)},
        {"Up-Down Control",ClassType(CLASS_DEFAULT)}
        };
}
bool WindowsClasses::isClassCreated(const std::string& classname){
    if (classes[classname].origin==CLASS_NONE)
        return false;
    return true;
}
bool WindowsClasses::isClassDefault(const std::string& classname){
    if (classes[classname].origin==CLASS_DEFAULT)
        return true;
    return false;
}
int WindowsClasses::addCustom(const std::string& classname){
    if (isClassCreated(classname))
        return -1;
    classes[classname].origin=CLASS_CUSTOM;
    return 1;
}
unsigned long WindowsClasses::getDefaultFlags(const std::string& classname){
    return classes[classname].flags;
}

static const int OLD_CUSTOM_PROC=GWLP_USERDATA+2;



window::~window(){
    if(parent!=NULL)
        parent->remChild(this);
    if (veil.font!=NULL){
        DeleteObject(veil.font);
    }
    if (veil.hwnd!=NULL)
        DestroyWindow(veil.hwnd);
}
int window::getx(){
    #if defined(_WIN_32) || defined(_WIN32) || defined(WIN32)
    RECT rect;
    if(veil.hwnd!=NULL){
        GetWindowRect(veil.hwnd,&rect);
    }
    return rect.left;
    #endif
}
int window::gety(){
    #if defined(_WIN_32) || defined(_WIN32) || defined(WIN32)
    RECT rect;
    if(veil.hwnd!=NULL){
        GetWindowRect(veil.hwnd,&rect);
    }
    return rect.top;
    #endif
}
int window::clientWidth(){
    #if defined(_WIN_32) || defined(_WIN32) || defined(WIN32)
    RECT rect;
    if(veil.hwnd!=NULL){
        GetClientRect(veil.hwnd,&rect);
    }
    return rect.right-rect.left;
    #endif
    return 0;
}
int window::clientHeight(){
    #if defined(_WIN_32) || defined(_WIN32) || defined(WIN32)
    RECT rect;
    if(veil.hwnd!=NULL){
        GetClientRect(veil.hwnd,&rect);
    }
    return rect.bottom-rect.top;
    #endif
    return 0;
}
int window::row(){
    return wpos.row;
}
int window::col(){
    return wpos.col;
}
void window::setRow(int irow){
    wpos.row=irow;
}
void window::setCol(int icol){
    wpos.col=icol;
}
int window::width(){
    #if defined(_WIN_32) || defined(_WIN32) || defined(WIN32)
    RECT rect;
    if(veil.hwnd!=NULL){
        GetWindowRect(veil.hwnd,&rect);
    }
    return rect.right-rect.left;
    #endif
}
int window::height(){
    #if defined(_WIN_32) || defined(_WIN32) || defined(WIN32)
    RECT rect;
    if(veil.hwnd!=NULL){
        GetWindowRect(veil.hwnd,&rect);
    }
    return rect.bottom-rect.top;
    #endif
}
int window::xweight(){
    return wpos.xweight;
}
int window::yweight(){
    return wpos.yweight;
}
bool window::canBePositioned(){
    return ((!isHidden())&&(wpos.postype==PositionType::POSITION_NORMAL||wpos.postype==PositionType::POSITION_RELATIVE));
}
int window::positionType(){
    return wpos.postype;
}
void window::setPositionType(PositionType itype){
    wpos.postype=itype;
}
int window::marginLeft(){return wpos.marginLeft;}
int window::marginRight(){return wpos.marginRight;}
int window::marginTop(){return wpos.marginTop;}
int window::marginBottom(){return wpos.marginBottom;}
void window::setMarginLeft(int imargin){wpos.marginLeft=imargin;}
void window::setMarginRight(int imargin){wpos.marginRight=imargin;}
void window::setMarginTop(int imargin){wpos.marginTop=imargin;}
void window::setMarginBottom(int imargin){wpos.marginBottom=imargin;}
void window::setMargins(int xmargin, int ymargin){wpos.marginLeft=wpos.marginRight=xmargin;wpos.marginTop=wpos.marginBottom=ymargin;}
void window::setMargins(int left,int right,int top,int bottom){wpos.marginLeft=left;wpos.marginRight=right;wpos.marginTop=top;wpos.marginBottom=bottom;}
void window::setMargins(int imargin){wpos.marginLeft=wpos.marginRight=wpos.marginTop=wpos.marginBottom=imargin;}
unsigned long window::getFlags(){
    return createFlags;
}
void window::setPosition(int ix, int iy){
    wpos.x=ix;wpos.y=iy;
    SetWindowPos(veil.hwnd,NULL,ix,iy,0,0,SWP_NOZORDER|SWP_NOSIZE);
}
void window::setSize(int iwidth, int iheight){
    SetWindowPos(veil.hwnd,NULL,0,0,iwidth,iheight,SWP_NOZORDER|SWP_NOMOVE);
}
void window::reposition(const positioning& pos){
    wpos=pos;
}
int window::doResize(){
	return -1;
}
int window::getRelativeMouseX(){
    static POINT point;
    GetCursorPos(&point);
    ScreenToClient(veil.hwnd,&point);
    return point.x;
}
int window::getRelativeMouseY(){
    static POINT point;
    GetCursorPos(&point);
    ScreenToClient(veil.hwnd,&point);
    return point.y;
}
int window::getTextLength(){
    return GetWindowTextLength(veil.hwnd);
}
std::string window::getText(){
    int len = getTextLength()+1;
    std::string ret; ret.resize(len);
    GetWindowTextA(veil.hwnd,&ret[0],len);
    return ret;
}
void window::setText(const std::string& itext){
    #if defined(_WIN_32) || defined(_WIN32) || defined(WIN32)
        if (veil.hwnd!=NULL){
            SendMessageA(veil.hwnd,WM_SETTEXT,NULL,(LPARAM)itext.c_str());
        }
    #endif // _WIN32
}
void window::setHint(const std::string& title){
     if (veil.hwnd!=NULL){
         std::wstring stringy(title.begin(),title.end());
         SendMessageW(veil.hwnd,0x1501,false,(LPARAM)stringy.c_str());//EM_SETCUEBANNER = 0x1501
     }
 }
bool POINTER_COMPARE(window* a, window* b) { return (a < b); }
//left to right, top to bottom, pointer first to pointer last
bool POSITION_COMPARE(window* a, window* b) { return ((a->row() < b->row())||(a->row()==b->row() &&(a->col() < b->col()))||(a->row()==b->row()&&a->col()==b->col()&&(a < b)));}
int window::addChild(window* win){
    children.push_back(win);
    std::sort(children.begin(),children.end(),&POINTER_COMPARE);
    return 1;
}
int window::remChild(window* win){///TODO TEST TEST TEST TEST
    auto pr = std::equal_range(children.begin(),children.end(),win,&POINTER_COMPARE);
    if (pr.first==pr.second)
        return -1;
    children.erase(pr.first);//first object that is not < win where first and last are not equal, second is 1 beyond range
    return 1;
}
int window::childCount(){
    return children.size();
}
window* window::getChild(int index){
    return children[index];
}


static BITMAPINFO bitmapFormat = {
            sizeof(bitmapFormat.bmiHeader),
            0,
            0,
            1,
            32,
            BI_RGB,
        };
int window::drawRaw(unsigned int* data, int canvasX, int canvasY, int width, int height){
    bitmapFormat.bmiHeader.biWidth=width;
    bitmapFormat.bmiHeader.biHeight=-height;

    HDC dc = GetDC(veil.hwnd);

    int res = SetDIBitsToDevice(
        dc,
        canvasX,
        canvasY,
        width,
        height,
        0,
        0,
        0,
        height,
        data,
        &bitmapFormat,
        DIB_RGB_COLORS
    );
    if (res==0){//This is super slow, hopefully you don't get here!
        for(int a = 0; a < height; a++){
            for(int b = 0; b < width; b++){
                SetPixel(dc,canvasX+b,canvasY+a,data[b+a*width]);
            }
        }
    }


    ReleaseDC(veil.hwnd,dc);
    return res;
}

window::window(){
    created=false;
    isCustomDrawn=false;
    doAutoScroll=false;
    veil.hwnd=NULL;
    nextID=0;
    oldproc=NULL;
    yscroll=yscrollmax=yscrollmin=0;
}
window::window(window* iparent, const std::string& title, const std::string& type, unsigned long flags, positioning ipos):window(){
    create(iparent,title,type,flags,ipos);
}
int window::getNextChildID(){
    return (++nextID);
}
long window::getWindowStyle(){
    return GetWindowLong(veil.hwnd,GWL_STYLE);
}
long window::setWindowStyle(long istyle){
    return SetWindowLong(veil.hwnd,GWL_STYLE,istyle);
}
long window::setDefaultMenuBar(bool usemenu){
    if (usemenu)
        return setWindowStyle(getWindowStyle()|WS_OVERLAPPEDWINDOW);
    else
        return setWindowStyle(getWindowStyle()&(~WS_OVERLAPPEDWINDOW));
}
int window::reparent(window* nparent,bool titlebar){
    if (parent!=NULL){
        parent->remChild(this);
        parent->layout();
    }

    parent=nparent;
    HWND setter = NULL;
    if (nparent!=NULL){
        setter = nparent->veil.hwnd;
        nparent->addChild(this);
    }
    int sret = 0;
    if (setter==NULL){
        if (!titlebar)
            sret = setWindowStyle((getWindowStyle()&(~WS_CHILD))|WS_POPUP);//no longah a chuld breh
        else
            sret = setWindowStyle((getWindowStyle()&(~WS_CHILD))|WS_OVERLAPPEDWINDOW|WS_OVERLAPPED);//no longah a chuld breh
        if (sret==0){
            //err
        }
    }
    else{
        setWindowStyle((getWindowStyle()&(~(WS_OVERLAPPEDWINDOW)))|WS_CHILD);
    }
    HWND res = SetParent(veil.hwnd,setter);
    return (res!=NULL);
}
int window::popout(){
    return reparent(NULL,true);
}
int isClassDefault(const std::string type){
    WNDCLASS ii;
    BOOL ret = GetClassInfo(NULL,type.c_str(),&ii);
    return ret;
}
int window::create(window* iparent, const std::string& title, const std::string& type, unsigned long flags, positioning ipos){
    parent=iparent;
    createFlags=flags;
    createType=type;
    unsigned long winflags = WS_VISIBLE;
    if ((flags&WINDOW_REALTIME)==WINDOW_REALTIME){
        isCustomDrawn=true;
    }
    if ((flags&WINDOW_MULTILINE)==WINDOW_MULTILINE){
        winflags|=ES_MULTILINE|ES_AUTOVSCROLL;
    }
    if ((flags&WINDOW_SCROLLED)==WINDOW_SCROLLED){
        winflags|=WS_VSCROLL;
    }
    else if ((flags&WINDOW_AUTO_SCROLLED)==WINDOW_AUTO_SCROLLED){
        doAutoScroll=true;
    }
    if ((flags&WINDOW_BORDERED)==WINDOW_BORDERED){
        winflags|=WS_BORDER;
    }
    if (!WinClasses.isClassCreated(type)){
        WNDCLASSA wc = { 0 };
        wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
        wc.lpfnWndProc = &WindowProcedure;
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
        wc.lpszClassName = type.c_str();
        if (RegisterClassA(&wc)==0)
            return -1;
        WinClasses.addCustom(type);
    }
    HWND phwnd=NULL;
    int identifier = 0;
    HINSTANCE hInstance = GetModuleHandle(NULL);
    if(parent!=NULL){
        phwnd=parent->veil.hwnd;
        identifier=parent->getNextChildID();
        winflags|=WS_CHILD;
        parent->addChild(this);
    }
    else{
        winflags |= WS_OVERLAPPEDWINDOW;
    }
    veil.hwnd= CreateWindowA(type.c_str(), title.c_str(), winflags,
                                  ipos.col,ipos.row,ipos.minwidth,ipos.minheight, phwnd, (HMENU) identifier, hInstance, (LPVOID)this);

    ///BBBBBB

    if (isClassDefault(type)){//subclass any existing windows classes
        oldproc=reinterpret_cast<WNDPROC>(static_cast<LONG_PTR>(
             SetWindowLongPtrA(veil.hwnd,GWLP_WNDPROC,
             reinterpret_cast<LONG_PTR>(WindowProcedure))));
        //SetWindowLongPtrA(veil.hwnd,OLD_CUSTOM_PROC,reinterpret_cast<LONG_PTR>(oldproc));
    }
    //if (!WinClasses.isClassDefault(type)){
    SetWindowLongPtrA(veil.hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
    //}
    wpos=ipos;
    return 1;
}
std::string window::getType(){
    return createType;
}
void window::setFont(int fsize, int family, const std::string& face, int weight, bool italic, bool underline, int charset, int quality){
    if (veil.font != NULL){
        DeleteObject(veil.font);
    }
    veil.font = CreateFontA(fsize,0,0,0,weight,italic,underline,false,charset,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,quality,DEFAULT_PITCH|family,face.c_str());
    SendMessage(veil.hwnd,WM_SETFONT,(WPARAM)veil.font,true);
}
void window::fillSortedChildren(){
    positionSortedChildren = children;
    std::sort(positionSortedChildren.begin(),positionSortedChildren.end(),&POSITION_COMPARE);
    for(int a = 0; a < positionSortedChildren.size(); a++){
        positionSortedChildren[a]->fillSortedChildren();
    }

}
int window::calculateRowCount(){
    if (children.size()==0)
        return 0;
    int ret = 1;
    int prev = positionSortedChildren[0]->row();
    for(int a = 1; a < positionSortedChildren.size(); a++){
        if (positionSortedChildren[a]->row()!=prev){
            prev=positionSortedChildren[a]->row();
            ret++;
        }
    }
    return ret;
}
int window::calculateColCount(int irow){
    if (children.size()==0)
        return 0;
    int ret = 0;
    for(int a = 0; a < positionSortedChildren.size(); a++){
        if (positionSortedChildren[a]->row()==irow){
            ret++;
        }
    }
    return ret;
}
int window::calculateMinWidth(){
    if (children.size()==0){
        if (!canBePositioned())
            return 0;
        return wpos.minwidth+wpos.marginLeft+wpos.marginRight;
    }
    int curmin = 0;
    int currow = 0;
    int retmin = 0;
    for(int a = 0; a < positionSortedChildren.size(); a++){
        if (positionSortedChildren[a]->row()!=currow){
            if (curmin>retmin)
                retmin=curmin;
            curmin=0;
            currow=positionSortedChildren[a]->row();
        }
        curmin+=positionSortedChildren[a]->calculateMinWidth();
    }
    if (curmin>retmin)
        retmin=curmin;
    if (wpos.minwidth>retmin&&parent!=NULL)
        retmin=wpos.minwidth;
    retmin+=wpos.marginLeft+wpos.marginRight;
    return retmin;
}
int window::calculateRowMinWidth(int irow){
    if (children.size()==0)
        return 0;
    int retmin = 0;
    for(int a = 0; a < positionSortedChildren.size(); a++){
        if (positionSortedChildren[a]->row()==irow){
            retmin+=positionSortedChildren[a]->calculateMinWidth();
        }
    }
    return retmin;
}
int window::calculateRowMinHeight(int irow){
    if (children.size()==0)
        return 0;
    int retmin = 0;
    for(int a = 0; a < positionSortedChildren.size(); a++){
        if (positionSortedChildren[a]->row()==irow){
            int theight = positionSortedChildren[a]->calculateMinHeight();
            if (theight>retmin)
                retmin=theight;
        }
    }
    return retmin;
}
int window::calculateMinHeight(){
    if (children.size()==0){
        if (!canBePositioned())
            return 0;
        return wpos.minheight+wpos.marginTop+wpos.marginBottom;
    }
    int curmin = 0;
    int currow = 0;
    int retmin = 0;
    for(int a = 0; a < positionSortedChildren.size(); a++){
        if (positionSortedChildren[a]->row()!=currow){
            retmin+=curmin;
            curmin=0;
            currow=positionSortedChildren[a]->row();
        }
        int minh = positionSortedChildren[a]->calculateMinHeight();
        if (minh>curmin){
            curmin=minh;
        }
    }
    retmin+=curmin;
    if (wpos.minheight>retmin&&parent!=NULL)
        retmin=wpos.minheight;
    return retmin;
}
int window::calculateVerticalWeight(){
    if (children.size()==0)
        return 0;//is intended to get filled weight, not this object's weight

    int rowmaxweight = 0;
    int currow = 0;
    int retweight = 0;
    for(int a = 0; a < positionSortedChildren.size(); a++){
        if (positionSortedChildren[a]->row()!=currow){
            retweight+=rowmaxweight;
            currow=positionSortedChildren[a]->row();
            rowmaxweight=0;
        }
        if (positionSortedChildren[a]->yweight()>rowmaxweight)
            rowmaxweight=positionSortedChildren[a]->yweight();
    }
    retweight+=rowmaxweight;
    return retweight;
}
int window::calculateRowVerticalWeight(int inrow){
    if (children.size()==0)
        return 0;//is intended to get filled weight, not this object's weight
    int retweight = 0;
    for(int a = 0; a < positionSortedChildren.size(); a++){
        if (positionSortedChildren[a]->row()==inrow){
            if (positionSortedChildren[a]->yweight()>retweight)
                retweight=positionSortedChildren[a]->yweight();
        }
    }
    return retweight;
}
int window::calculateHorizontalWeight(int irow){
    if (children.size()==0)
        return 0;

    int retweight = 0;
    for(int a = 0; a < positionSortedChildren.size(); a++){
        if (positionSortedChildren[a]->row()==irow){
            retweight+=positionSortedChildren[a]->xweight();
        }
    }
    return retweight;
}
void window::layout(){
    if (children.size()>0){
        int temp = children.size();
        fillSortedChildren();
        temp = positionSortedChildren.size();
        //int rowcount = calculateRowCount();
        wpos.calculatedWidth = calculateMinWidth();
        wpos.calculatedHeight = calculateMinHeight();
        int leftoverheight = clientHeight() - wpos.calculatedHeight;
        int scrollx=0,scrolly=0;
        if (leftoverheight<0){
                scrolly=abs(leftoverheight);
                if (doAutoScroll){
                    if ((getWindowStyle()&WS_VSCROLL)!=WS_VSCROLL){
                        ShowScrollBar(veil.hwnd,SB_VERT,TRUE);
                        return layout();
                    }
                    yscrollmin=0;yscrollmax=scrolly;
                    SCROLLINFO sinfo;
                    sinfo.cbSize=sizeof(SCROLLINFO);
                    sinfo.fMask=SIF_POS|SIF_RANGE;//|SIF_PAGE;
                    sinfo.nPos=yscroll;
                    sinfo.nMin=yscrollmin;
                    sinfo.nMax=yscrollmax;
                    //sinfo.nPage=clientHeight();
                    SetScrollInfo(veil.hwnd,SB_VERT,&sinfo,TRUE);
                }

                //SCROLL HEIGHT = abs LEFTOVER HEIGHT
                leftoverheight=0;
        }
        else{
            if (doAutoScroll&&((getWindowStyle()&WS_VSCROLL)==WS_VSCROLL)){
                //setWindowStyle(getWindowStyle()&(~WS_VSCROLL));
                ShowScrollBar(veil.hwnd,SB_VERT,FALSE);
                yscroll=yscrollmin=yscrollmax=0;
                return layout();
            }
        }
        int vweight = calculateVerticalWeight();
        int cury = -yscroll;//Could set cury to the current scroll position of parent window, then use that to prevent unnecessary drawing
        int curx = 0;

        int rowWeightedHeight=0;
        int rowTotalWeight=0;
        int rowTotalYWeight=0;
        int rowLeftover=0;
        int currow=positionSortedChildren[0]->row()-1;
        for(int a = 0; a < positionSortedChildren.size(); a++){
            std::string tmp = positionSortedChildren[a]->getText();
            if (!positionSortedChildren[a]->canBePositioned())
                continue;
            if (currow!=positionSortedChildren[a]->row()){
                cury+=rowWeightedHeight;
                curx=0;
                currow=positionSortedChildren[a]->row();
                rowTotalWeight=calculateHorizontalWeight(currow);
                rowTotalYWeight=calculateRowVerticalWeight(currow);
                rowWeightedHeight=calculateRowMinHeight(currow);
                if (vweight>0){//no stinking dividing by zero ;)
                    double portion = (double)rowTotalYWeight / (double)vweight;
                    int add = (portion*(double)leftoverheight);
                    if (add < 0)
                        add=0;
                    rowWeightedHeight+=add;
                }
                unsigned long rmw = calculateRowMinWidth(currow);
                rowLeftover = clientWidth() - rmw;
                if (rowLeftover<0){
                    int tmpx = abs(rowLeftover);
                    if (tmpx > scrollx)
                        scrollx=tmpx;
                    //SCROLL WIDTH = MAX (CUR X SCROLL, rowleftover)
                    rowLeftover=0;
                }
            }
            int wid,hei;
            wid=positionSortedChildren[a]->calculateMinWidth();
            if (positionSortedChildren[a]->xweight()>0){
                double portion = (double)positionSortedChildren[a]->xweight() / (double)rowTotalWeight;
                int add = (portion*(double)rowLeftover);
                if (add<0){
                    add=0;
                }
                wid+=add;
            }
            if (positionSortedChildren[a]->yweight()>0||((positionSortedChildren[a]->getFlags()&WINDOW_FILL_Y)==WINDOW_FILL_Y)){
                std::string name = positionSortedChildren[a]->getText();
                hei=rowWeightedHeight;//everything or nothing, could be undesirable
            }
            else{
                hei=positionSortedChildren[a]->calculateMinHeight();
            }
            positionSortedChildren[a]->setPosition(curx+positionSortedChildren[a]->marginLeft(),cury+positionSortedChildren[a]->marginTop());
            positionSortedChildren[a]->setSize(wid-positionSortedChildren[a]->marginLeft()-positionSortedChildren[a]->marginRight(),hei-positionSortedChildren[a]->marginTop()-positionSortedChildren[a]->marginBottom());
            positionSortedChildren[a]->layout();
            curx+=wid;//+positionSortedChildren[a]->marginLeft()+positionSortedChildren[a]->marginRight();
        }
    }
    else{
        //reposition(wpos);
    }
}
void window::layout(const positioning& pos){
    reposition(pos);
}
int window::checkEvents(){
    static MSG msg;
    return PeekMessageA(&msg,veil.hwnd,0,0,PM_NOREMOVE);
}
int window::pauseForEvent(){
    static MSG msg;
    static int ret;
    ret = GetMessage(&msg, NULL, 0, 0);
    TranslateMessage(&msg);
    DispatchMessage(&msg);
    return ret;
}
int window::processEvents(){
    static MSG Msg;
    static int msgAvail;
    int ret = 1;
    msgAvail = PeekMessageA(&Msg,NULL,0,0,PM_NOREMOVE);
    if (msgAvail!=1)
        return 0;
    while(msgAvail){
        ret = (GetMessageA(&Msg, NULL, 0, 0) != 0);
        TranslateMessage(&Msg);
        DispatchMessageA(&Msg);
        msgAvail = PeekMessageA(&Msg,NULL,0,0,PM_NOREMOVE);
    }
    if (ret==0)
        return -1;//quit
    return 1;
}

int window::show(bool doLayout){
    if (doLayout)
        layout();
    return ShowWindow(veil.hwnd,SW_SHOW);
}
int window::hide(){
    return ShowWindow(veil.hwnd,SW_HIDE);
}
bool window::hasFocus(){
    return (GetFocus()==veil.hwnd);
}
bool window::isForeground(){
    return (GetForegroundWindow()==veil.hwnd);
}
bool window::isHidden(){
    return (!IsWindowVisible(veil.hwnd));
}

ulti::icoord window::getPosition(){
    #if defined(_WIN_32) || defined(_WIN32) || defined(WIN32)
    RECT rect;
    if(veil.hwnd!=NULL){
        GetWindowRect(veil.hwnd,&rect);
    }
    return ulti::icoord(rect.left,rect.top);
    #endif
    return ulti::icoord();
}
ulti::icoord window::getSize(){
    #if defined(_WIN_32) || defined(_WIN32) || defined(WIN32)
    RECT rect;
    if(veil.hwnd!=NULL){
        GetWindowRect(veil.hwnd,&rect);
    }
    else{
        return icoord(0,0);
    }
    return ulti::icoord(rect.right-rect.left,rect.bottom-rect.top);
    #endif
    return ulti::icoord();
}
int window::getHorizontalWeight(){
    return wpos.xweight;
}
int window::getVerticalWeight(){
    return wpos.yweight;
}

ulti::icoord window::getClientPosition(){
    #if defined(_WIN_32) || defined(_WIN32) || defined(WIN32)
    if(veil.hwnd!=NULL){
        POINT pnt;
        pnt.x=0;pnt.y=0;
        bool res = ClientToScreen(veil.hwnd,&pnt);
        if (res){
            return ulti::icoord(pnt.x,pnt.y);
        }
    }
    #endif
    return ulti::icoord(-1,-1);
}
ulti::icoord window::getClientSize(){
    #if defined(_WIN_32) || defined(_WIN32) || defined(WIN32)
    RECT rect;
    if(veil.hwnd!=NULL){
        GetClientRect(veil.hwnd,&rect);
    }
    return ulti::icoord(rect.right-rect.left,rect.bottom-rect.top);
    #endif
    return ulti::icoord();
}

int window::setPosition(ulti::icoord pos){
    if (veil.hwnd!=NULL){
        return SetWindowPos(veil.hwnd,NULL,pos.x,pos.y,0,0,SWP_NOZORDER|SWP_NOSIZE);
    }
    return -1;
}
int window::setSize(ulti::icoord siz){
    if (veil.hwnd!=NULL){
        return SetWindowPos(veil.hwnd,NULL,0,0,siz.x,siz.y,SWP_NOZORDER|SWP_NOMOVE);
    }
    return -1;
}
int window::setClientSize(ulti::icoord siz){
    ulti::icoord nc = getSize();
    ulti::icoord ic = getClientSize();
    nc.x-=ic.x;
    nc.y-=ic.y;
    siz.x+=nc.x;
    siz.y+=nc.y;
    return setSize(siz);
}
ulti::icoord window::getRelativeMousePos(){
    static POINT point;
    GetCursorPos(&point);
    ScreenToClient(veil.hwnd,&point);
    return icoord(point.x,point.y);
}
bool window::isMouseInside(){
    return false;
}
bool window::isMouseInClient(){
    icoord mpos = getRelativeMousePos();
    return(ulti::collide(mpos.x,mpos.y,1,1,0,0,clientWidth(),clientHeight()));
}

void window::on(const std::string& eventName, std::function<void(ulti::message& msg)> func){
    soloStation.listenFunc(func,eventName);
}
void window::on(ulti::ui32 eventIndex, std::function<void(ulti::message& msg)> func){
    soloStation.listenFunc(func,eventIndex);
}
HMENU recurseElementForMenu(HMENU menu, ulti::element& el, ulti::window* win, int depth = 0){
    ///std::string tstr = ulti::stringlayout(el);
    if (depth>2)
        return menu;
    bool created=false;
    if (menu==NULL){
        menu = CreateMenu();
        created=true;
    }
    UINT_PTR hsub = NULL;
    DWORD mflags = MF_STRING;
    std::string text = "UNKNOWN";
    if (el.getType()==valuetype::maptype){

        if (el.checkKey("text")){
            text = el["text"].getStr();
        }
        if (el.checkKey("children")){
            hsub = (UINT_PTR)CreatePopupMenu();
            mflags |= MF_POPUP;
        }
        else if (el.checkKey("id")){
            hsub = ulti::atol(el["id"].getStr());
        }
        if (el.checkKey("event")){

        }
        AppendMenuA(menu,mflags,hsub,text.c_str());
        if (el.checkKey("children")){
            for(int a = 0; a < el["children"].size(); a++){
                recurseElementForMenu((HMENU)hsub,el["children"][a],win,depth+1);
            }
        }
    }
    else if (el.getType()==valuetype::arraytype){
        ///int ttt = el.size();
        for(int a = 0; a < el.size(); a++){
            recurseElementForMenu(menu,el[a],win,depth+1);
        }
    }
    if (created)
        SetMenu(win->veil.hwnd,menu);

}
int window::attachMenu(const std::string& str){
    if (veil.menu!=NULL){
        DestroyMenu(veil.menu);
        veil.menu=NULL;
    }

    ulti::element el;
    el.setStr(str);

    recurseElementForMenu(veil.menu,el,this);
	return 1;
}
int window::setIcon(int resourceNumber){
    HICON hIconSmall =(HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(resourceNumber), IMAGE_ICON,16, 16, 0);
    return setIcon(hIconSmall);
}
int window::setIcon(const std::string& filename){
    HICON hIconSmall = (HICON)LoadImageA(NULL, filename.c_str(), IMAGE_ICON, 16, 16, LR_LOADFROMFILE );
    return setIcon(hIconSmall);
}
int window::setIcon(HICON ico){
    return SendMessageA(veil.hwnd,WM_SETICON,0,(LPARAM)ico);
}
int window::scrollTo(int scrollpos){
    if (scrollpos<yscrollmin)
        yscroll=yscrollmin;
    else if (scrollpos>yscrollmax)
        yscroll=yscrollmax;
    else
        yscroll=scrollpos;
    return 1;
}

static LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
    window* winn = reinterpret_cast<window*>(GetWindowLongPtrW(hwnd, GWLP_USERDATA));
    if (winn!=NULL&&winn->veil.hwnd==hwnd){
        return winn->windowCallback(hwnd,uMsg,wParam,lParam);
    }
    else{
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

    LRESULT window::windowCallback(HWND ihwnd, UINT msg, WPARAM wparam, LPARAM lparam){
        //ulti::element el(ulti::valuetype::maptype);
        std::string ename = getEventName(msg);
        message res;
        res.type=msg;
        res.value1=wparam;
        res.value2=lparam;
        res.parent=this;
        res.handle=(unsigned long)ihwnd;
        soloStation.send(res);
        if (res.preventDefault)
            return 0;

        //el["event"]=ename;
        //soloStation.send(ulti::message(el,soloStation.getMessageType("ui"),this));
        if (oldproc!=NULL){
                CallWindowProc(oldproc,ihwnd,msg,wparam,lparam);
        }
        static SCROLLINFO si = { sizeof(si), SIF_TRACKPOS };
        switch(msg) {
            case WM_SIZE:
                if (parent==NULL)//don't relayout children, the parent does that
                    layout();
                doResize();
                break;
            case WM_CREATE:
                this->created=true;
                break;
            case WM_CLOSE:
                PostQuitMessage(0);
                break;
            case WM_ERASEBKGND:
                if (this->isCustomDrawn){
                    return 1;
                }
                break;
            case WM_ACTIVATE:
            case WM_MOUSEACTIVATE:
                SetFocus(veil.hwnd);
                break;
            case WM_PAINT:
                if (this->isCustomDrawn){
                    static RECT rect;
                    GetClientRect(veil.hwnd,&rect);//use these two
                    ValidateRect(veil.hwnd,&rect);
                    return 0;
                }
                else{
                    BeginPaint(veil.hwnd, NULL);
                    EndPaint(veil.hwnd, NULL);
                    return 0;
                }
                break;
            case WM_VSCROLL:
                if (this->yscrollmax>this->yscrollmin){
                    switch(LOWORD(wparam)) {
                        case SB_TOP:
                            scrollTo(yscrollmin);
                            layout();
                            break;
                        case SB_BOTTOM:
                            scrollTo(yscrollmax);
                            layout();
                            break;

                        case SB_THUMBPOSITION: case SB_THUMBTRACK:
                            scrollTo(HIWORD(wparam));
                            layout();
                            break;
                        case SB_LINEUP:
                            scrollTo(yscroll-1);
                            layout();
                            break;

                        case SB_LINEDOWN:
                            scrollTo(yscroll+1);
                            layout();
                            break;
                    }

                    return 0;
                }
                return 1;
                break;

            case WM_MOUSEWHEEL:
                if (yscrollmax>yscrollmin){
                    scrollTo(yscroll+(((short)HIWORD(wparam)/20)*-1));
                    layout();
                }
                break;
            case WM_LBUTTONDOWN:case WM_LBUTTONUP:case WM_LBUTTONDBLCLK:
            case WM_RBUTTONDOWN:case WM_RBUTTONUP:case WM_RBUTTONDBLCLK:
            case WM_MOUSEFIRST:case WM_MOUSELAST:
            case WM_SETCURSOR:
            case WM_NCHITTEST:
            case WM_NCACTIVATE:
            case WM_NCPAINT:
            case WM_SETFOCUS:
            case WM_KILLFOCUS:
            case WM_KEYDOWN:
            case WM_KEYUP:
                break;
            case WM_DESTROY:
                if(parent!=NULL)
                    parent->remChild(this);
                break;
        }
        return DefWindowProc(ihwnd, msg, wparam, lparam);
    }


    whiteboard::whiteboard():window(){
        isCustomDrawn=true;
    }
    whiteboard::whiteboard(window* parent, const std::string& title, const std::string& type, unsigned long flags, positioning ipos):window(parent,title,type,flags|WINDOW_REALTIME,ipos){
        sur.resize(getSize().x,getSize().y);
        isCustomDrawn=true;
    }
    whiteboard::~whiteboard(){

    }
    int whiteboard::doResize(){
        //HDC dc = GetDC(hwnd);
        //clearScreen(hbrWhite);
        //layout(-1,-1,-1,-1,false);
        sur.resize(getClientSize().x,getClientSize().y);
        //ReleaseDC(hwnd,dc);
        //doPaint();
        return 1;
    }
    int whiteboard::drawSurface(){
        return this->drawRaw(&sur.pixel(0,0),0,0,sur.width(),sur.height());
    }

}
