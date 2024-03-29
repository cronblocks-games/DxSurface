#include "pch.hpp"
#include "DxsWinDebug.hpp"

using namespace std;
using namespace CB::DxSurface;

#define ID_NAME_PAIR(msg)     { msg, DxsT(#msg) }
#define KEY_NAME_PAIR(k)      { KeyCode::k, DxsT(#k)   }

using MMap = map<UINT, TString>;
using KMap = map<KeyCode, TString>;

static MMap _winMessagesMap(
	{
		ID_NAME_PAIR(WM_ACTIVATE),
		ID_NAME_PAIR(WM_ACTIVATEAPP),
		ID_NAME_PAIR(WM_ASKCBFORMATNAME),
		
		ID_NAME_PAIR(WM_CANCELJOURNAL),
		ID_NAME_PAIR(WM_CANCELMODE),
		ID_NAME_PAIR(WM_CAPTURECHANGED),
		ID_NAME_PAIR(WM_CHANGECBCHAIN),
		ID_NAME_PAIR(WM_CHAR),
		ID_NAME_PAIR(WM_CHARTOITEM),
		ID_NAME_PAIR(WM_CHILDACTIVATE),
		ID_NAME_PAIR(WM_CLEAR),
		ID_NAME_PAIR(WM_CLOSE),
		ID_NAME_PAIR(WM_COMMAND),
		ID_NAME_PAIR(WM_COMPACTING),
		ID_NAME_PAIR(WM_COMPAREITEM),
		ID_NAME_PAIR(WM_CONTEXTMENU),
		ID_NAME_PAIR(WM_COPY),
		ID_NAME_PAIR(WM_COPYDATA),
		ID_NAME_PAIR(WM_CREATE),
		ID_NAME_PAIR(WM_CTLCOLORBTN),
		ID_NAME_PAIR(WM_CTLCOLORDLG),
		ID_NAME_PAIR(WM_CTLCOLOREDIT),
		ID_NAME_PAIR(WM_CTLCOLORLISTBOX),
		ID_NAME_PAIR(WM_CTLCOLORMSGBOX),
		ID_NAME_PAIR(WM_CTLCOLORSCROLLBAR),
		ID_NAME_PAIR(WM_CTLCOLORSTATIC),
		ID_NAME_PAIR(WM_CUT),
		
		ID_NAME_PAIR(WM_DEADCHAR),
		ID_NAME_PAIR(WM_DELETEITEM),
		ID_NAME_PAIR(WM_DESTROY),
		ID_NAME_PAIR(WM_DESTROYCLIPBOARD),
		ID_NAME_PAIR(WM_DEVICECHANGE),
		ID_NAME_PAIR(WM_DEVMODECHANGE),
		ID_NAME_PAIR(WM_DISPLAYCHANGE),
		ID_NAME_PAIR(WM_DRAWCLIPBOARD),
		ID_NAME_PAIR(WM_DRAWITEM),
		ID_NAME_PAIR(WM_DROPFILES),
		ID_NAME_PAIR(WM_DWMNCRENDERINGCHANGED),
		
		ID_NAME_PAIR(WM_ENABLE),
		ID_NAME_PAIR(WM_ENDSESSION),
		ID_NAME_PAIR(WM_ENTERIDLE),
		ID_NAME_PAIR(WM_ENTERMENULOOP),
		ID_NAME_PAIR(WM_ENTERSIZEMOVE),
		ID_NAME_PAIR(WM_ERASEBKGND),
		ID_NAME_PAIR(WM_EXITMENULOOP),
		ID_NAME_PAIR(WM_EXITSIZEMOVE),
		
		ID_NAME_PAIR(WM_FONTCHANGE),

		ID_NAME_PAIR(WM_GETDLGCODE),
		ID_NAME_PAIR(WM_GETFONT),
		ID_NAME_PAIR(WM_GETICON),
		ID_NAME_PAIR(WM_GETMINMAXINFO),
		ID_NAME_PAIR(WM_GETTEXT),
		ID_NAME_PAIR(WM_GETTEXTLENGTH),

		ID_NAME_PAIR(WM_HELP),
		ID_NAME_PAIR(WM_HOTKEY),
		ID_NAME_PAIR(WM_HSCROLL),
		ID_NAME_PAIR(WM_HSCROLLCLIPBOARD),

		ID_NAME_PAIR(WM_ICONERASEBKGND),
		ID_NAME_PAIR(WM_IME_NOTIFY),
		ID_NAME_PAIR(WM_IME_SETCONTEXT),
		ID_NAME_PAIR(WM_INITDIALOG),
		ID_NAME_PAIR(WM_INITMENU),
		ID_NAME_PAIR(WM_INITMENUPOPUP),

		ID_NAME_PAIR(WM_KEYDOWN),
		ID_NAME_PAIR(WM_KEYLAST),
		ID_NAME_PAIR(WM_KEYUP),
		ID_NAME_PAIR(WM_KILLFOCUS),

		ID_NAME_PAIR(WM_LBUTTONDBLCLK),
		ID_NAME_PAIR(WM_LBUTTONDOWN),
		ID_NAME_PAIR(WM_LBUTTONUP),

		ID_NAME_PAIR(WM_MBUTTONDBLCLK),
		ID_NAME_PAIR(WM_MBUTTONDOWN),
		ID_NAME_PAIR(WM_MBUTTONUP),
		ID_NAME_PAIR(WM_MDIACTIVATE),
		ID_NAME_PAIR(WM_MDICASCADE),
		ID_NAME_PAIR(WM_MDICREATE),
		ID_NAME_PAIR(WM_MDIDESTROY),
		ID_NAME_PAIR(WM_MDIGETACTIVE),
		ID_NAME_PAIR(WM_MDIICONARRANGE),
		ID_NAME_PAIR(WM_MDIMAXIMIZE),
		ID_NAME_PAIR(WM_MDINEXT),
		ID_NAME_PAIR(WM_MDIREFRESHMENU),
		ID_NAME_PAIR(WM_MDIRESTORE),
		ID_NAME_PAIR(WM_MDISETMENU),
		ID_NAME_PAIR(WM_MDITILE),
		ID_NAME_PAIR(WM_MEASUREITEM),
		ID_NAME_PAIR(WM_MENUCHAR),
		ID_NAME_PAIR(WM_MENUSELECT),
		ID_NAME_PAIR(WM_MOUSEACTIVATE),
		ID_NAME_PAIR(WM_MOUSEMOVE),
		ID_NAME_PAIR(WM_MOUSEWHEEL),
		ID_NAME_PAIR(WM_MOVE),
		ID_NAME_PAIR(WM_MOVING),

		ID_NAME_PAIR(WM_NCACTIVATE),
		ID_NAME_PAIR(WM_NCCALCSIZE),
		ID_NAME_PAIR(WM_NCCREATE),
		ID_NAME_PAIR(WM_NCDESTROY),
		ID_NAME_PAIR(WM_NCHITTEST),
		ID_NAME_PAIR(WM_NCLBUTTONDBLCLK),
		ID_NAME_PAIR(WM_NCLBUTTONDOWN),
		ID_NAME_PAIR(WM_NCLBUTTONUP),
		ID_NAME_PAIR(WM_NCMBUTTONDBLCLK),
		ID_NAME_PAIR(WM_NCMBUTTONDOWN),
		ID_NAME_PAIR(WM_NCMBUTTONUP),
		ID_NAME_PAIR(WM_NCMOUSELEAVE),
		ID_NAME_PAIR(WM_NCMOUSEMOVE),
		ID_NAME_PAIR(WM_NCPAINT),
		ID_NAME_PAIR(WM_NCRBUTTONDBLCLK),
		ID_NAME_PAIR(WM_NCRBUTTONDOWN),
		ID_NAME_PAIR(WM_NCRBUTTONUP),
		ID_NAME_PAIR(WM_NEXTDLGCTL),
		ID_NAME_PAIR(WM_NOTIFY),
		ID_NAME_PAIR(WM_NULL),

		ID_NAME_PAIR(WM_PAINT),
		ID_NAME_PAIR(WM_PAINTCLIPBOARD),
		ID_NAME_PAIR(WM_PALETTECHANGED),
		ID_NAME_PAIR(WM_PALETTEISCHANGING),
		ID_NAME_PAIR(WM_PARENTNOTIFY),
		ID_NAME_PAIR(WM_PASTE),
		ID_NAME_PAIR(WM_POWER),
		ID_NAME_PAIR(WM_POWERBROADCAST),
		ID_NAME_PAIR(WM_PRINT),
		ID_NAME_PAIR(WM_PRINTCLIENT),

		ID_NAME_PAIR(WM_QUERYDRAGICON),
		ID_NAME_PAIR(WM_QUERYENDSESSION),
		ID_NAME_PAIR(WM_QUERYNEWPALETTE),
		ID_NAME_PAIR(WM_QUERYOPEN),
		ID_NAME_PAIR(WM_QUEUESYNC),
		ID_NAME_PAIR(WM_QUIT),

		ID_NAME_PAIR(WM_RBUTTONDBLCLK),
		ID_NAME_PAIR(WM_RBUTTONDOWN),
		ID_NAME_PAIR(WM_RBUTTONUP),
		ID_NAME_PAIR(WM_RENDERALLFORMATS),
		ID_NAME_PAIR(WM_RENDERFORMAT),

		ID_NAME_PAIR(WM_SETCURSOR),
		ID_NAME_PAIR(WM_SETFOCUS),
		ID_NAME_PAIR(WM_SETFONT),
		ID_NAME_PAIR(WM_SETHOTKEY),
		ID_NAME_PAIR(WM_SETICON),
		ID_NAME_PAIR(WM_SETREDRAW),
		ID_NAME_PAIR(WM_SETTEXT),
		ID_NAME_PAIR(WM_SETTINGCHANGE),
		ID_NAME_PAIR(WM_SHOWWINDOW),
		ID_NAME_PAIR(WM_SIZE),
		ID_NAME_PAIR(WM_SIZECLIPBOARD),
		ID_NAME_PAIR(WM_SIZING),
		ID_NAME_PAIR(WM_SPOOLERSTATUS),
		ID_NAME_PAIR(WM_STYLECHANGED),
		ID_NAME_PAIR(WM_STYLECHANGING),
		ID_NAME_PAIR(WM_SYSCHAR),
		ID_NAME_PAIR(WM_SYSDEADCHAR),
		ID_NAME_PAIR(WM_SYSCOLORCHANGE),
		ID_NAME_PAIR(WM_SYSCOMMAND),
		ID_NAME_PAIR(WM_SYSKEYDOWN),
		ID_NAME_PAIR(WM_SYSKEYUP),

		ID_NAME_PAIR(WM_TCARD),
		ID_NAME_PAIR(WM_TIMECHANGE),
		ID_NAME_PAIR(WM_TIMER),

		ID_NAME_PAIR(WM_UNDO),

		ID_NAME_PAIR(WM_VKEYTOITEM),
		ID_NAME_PAIR(WM_VSCROLL),
		ID_NAME_PAIR(WM_VSCROLLCLIPBOARD),

		ID_NAME_PAIR(WM_WINDOWPOSCHANGED),
		ID_NAME_PAIR(WM_WINDOWPOSCHANGING),
		ID_NAME_PAIR(WM_WININICHANGE),
});

static KMap _keyCodesMap(
	{
    KEY_NAME_PAIR(MouseLeft),
    KEY_NAME_PAIR(MouseRight),
    KEY_NAME_PAIR(Cancel),
    KEY_NAME_PAIR(CtrlBreak),
    KEY_NAME_PAIR(MouseMiddle),
    KEY_NAME_PAIR(MouseX1),
    KEY_NAME_PAIR(MouseX2),
    KEY_NAME_PAIR(__Reserved_001),
    KEY_NAME_PAIR(BackSpace),
    KEY_NAME_PAIR(Tab),
    KEY_NAME_PAIR(__Reserved_002),
    KEY_NAME_PAIR(__Reserved_003),
    KEY_NAME_PAIR(Clear),
    KEY_NAME_PAIR(Enter),
    KEY_NAME_PAIR(Return),
    KEY_NAME_PAIR(__Unassigned_001),
    KEY_NAME_PAIR(__Unassigned_002),
    KEY_NAME_PAIR(Shift),
    KEY_NAME_PAIR(Control),
    KEY_NAME_PAIR(Ctrl),
    KEY_NAME_PAIR(Alt),
    KEY_NAME_PAIR(Pause),
    KEY_NAME_PAIR(CapsLock),
    KEY_NAME_PAIR(IME_Kana),
    KEY_NAME_PAIR(IME_Hangul),
    KEY_NAME_PAIR(IME_ON),
    KEY_NAME_PAIR(IME_Junja),
    KEY_NAME_PAIR(IME_Final),
    KEY_NAME_PAIR(IME_Hanja),
    KEY_NAME_PAIR(IME_Kanji),
    KEY_NAME_PAIR(IME_OFF),
    KEY_NAME_PAIR(Escape),
    KEY_NAME_PAIR(Esc),
    KEY_NAME_PAIR(IME_Convert),
    KEY_NAME_PAIR(IME_NonConvert),
    KEY_NAME_PAIR(IME_Accept),
    KEY_NAME_PAIR(IME_ChangeReq),
    KEY_NAME_PAIR(Space),
    KEY_NAME_PAIR(PageUp),
    KEY_NAME_PAIR(PageDown),
    KEY_NAME_PAIR(End),
    KEY_NAME_PAIR(Home),
    KEY_NAME_PAIR(LeftArrow),
    KEY_NAME_PAIR(UpArrow),
    KEY_NAME_PAIR(RightArrow),
    KEY_NAME_PAIR(DownArrow),
    KEY_NAME_PAIR(Select),
    KEY_NAME_PAIR(Print),
    KEY_NAME_PAIR(Execute),
    KEY_NAME_PAIR(PrintScreen),
    KEY_NAME_PAIR(Insert),
    KEY_NAME_PAIR(Delete),
    KEY_NAME_PAIR(Del),
    KEY_NAME_PAIR(Help),
    KEY_NAME_PAIR(Num0),
    KEY_NAME_PAIR(Digit0),
    KEY_NAME_PAIR(Num1),
    KEY_NAME_PAIR(Digit1),
    KEY_NAME_PAIR(Num2),
    KEY_NAME_PAIR(Digit2),
    KEY_NAME_PAIR(Num3),
    KEY_NAME_PAIR(Digit3),
    KEY_NAME_PAIR(Num4),
    KEY_NAME_PAIR(Digit4),
    KEY_NAME_PAIR(Num5),
    KEY_NAME_PAIR(Digit5),
    KEY_NAME_PAIR(Num6),
    KEY_NAME_PAIR(Digit6),
    KEY_NAME_PAIR(Num7),
    KEY_NAME_PAIR(Digit7),
    KEY_NAME_PAIR(Num8),
    KEY_NAME_PAIR(Digit8),
    KEY_NAME_PAIR(Num9),
    KEY_NAME_PAIR(Digit9),
    KEY_NAME_PAIR(__Undefined_001),
    KEY_NAME_PAIR(__Undefined_002),
    KEY_NAME_PAIR(__Undefined_003),
    KEY_NAME_PAIR(__Undefined_004),
    KEY_NAME_PAIR(__Undefined_005),
    KEY_NAME_PAIR(__Undefined_006),
    KEY_NAME_PAIR(__Undefined_007),
    KEY_NAME_PAIR(A),
    KEY_NAME_PAIR(B),
    KEY_NAME_PAIR(C),
    KEY_NAME_PAIR(D),
    KEY_NAME_PAIR(E),
    KEY_NAME_PAIR(F),
    KEY_NAME_PAIR(G),
    KEY_NAME_PAIR(H),
    KEY_NAME_PAIR(I),
    KEY_NAME_PAIR(J),
    KEY_NAME_PAIR(K),
    KEY_NAME_PAIR(L),
    KEY_NAME_PAIR(M),
    KEY_NAME_PAIR(N),
    KEY_NAME_PAIR(O),
    KEY_NAME_PAIR(P),
    KEY_NAME_PAIR(Q),
    KEY_NAME_PAIR(R),
    KEY_NAME_PAIR(S),
    KEY_NAME_PAIR(T),
    KEY_NAME_PAIR(U),
    KEY_NAME_PAIR(V),
    KEY_NAME_PAIR(W),
    KEY_NAME_PAIR(X),
    KEY_NAME_PAIR(Y),
    KEY_NAME_PAIR(Z),
    KEY_NAME_PAIR(WinLeft),
    KEY_NAME_PAIR(WinRight),
    KEY_NAME_PAIR(Applications),
    KEY_NAME_PAIR(__Reserved_004),
    KEY_NAME_PAIR(Sleep),
    KEY_NAME_PAIR(NumPad0),
    KEY_NAME_PAIR(NumPad1),
    KEY_NAME_PAIR(NumPad2),
    KEY_NAME_PAIR(NumPad3),
    KEY_NAME_PAIR(NumPad4),
    KEY_NAME_PAIR(NumPad5),
    KEY_NAME_PAIR(NumPad6),
    KEY_NAME_PAIR(NumPad7),
    KEY_NAME_PAIR(NumPad8),
    KEY_NAME_PAIR(NumPad9),
    KEY_NAME_PAIR(Asterisk),
    KEY_NAME_PAIR(Multiply),
    KEY_NAME_PAIR(Plus),
    KEY_NAME_PAIR(Add),
    KEY_NAME_PAIR(Separator),
    KEY_NAME_PAIR(Minus),
    KEY_NAME_PAIR(Subtract),
    KEY_NAME_PAIR(DecimalPoint),
    KEY_NAME_PAIR(ForwardSlash),
    KEY_NAME_PAIR(Divide),
    KEY_NAME_PAIR(F1),
    KEY_NAME_PAIR(F2),
    KEY_NAME_PAIR(F3),
    KEY_NAME_PAIR(F4),
    KEY_NAME_PAIR(F5),
    KEY_NAME_PAIR(F6),
    KEY_NAME_PAIR(F7),
    KEY_NAME_PAIR(F8),
    KEY_NAME_PAIR(F9),
    KEY_NAME_PAIR(F10),
    KEY_NAME_PAIR(F11),
    KEY_NAME_PAIR(F12),
    KEY_NAME_PAIR(F13),
    KEY_NAME_PAIR(F14),
    KEY_NAME_PAIR(F15),
    KEY_NAME_PAIR(F16),
    KEY_NAME_PAIR(F17),
    KEY_NAME_PAIR(F18),
    KEY_NAME_PAIR(F19),
    KEY_NAME_PAIR(F20),
    KEY_NAME_PAIR(F21),
    KEY_NAME_PAIR(F22),
    KEY_NAME_PAIR(F23),
    KEY_NAME_PAIR(F24),
    KEY_NAME_PAIR(__Reserved_005),
    KEY_NAME_PAIR(__Reserved_006),
    KEY_NAME_PAIR(__Reserved_007),
    KEY_NAME_PAIR(__Reserved_008),
    KEY_NAME_PAIR(__Reserved_009),
    KEY_NAME_PAIR(__Reserved_010),
    KEY_NAME_PAIR(__Reserved_011),
    KEY_NAME_PAIR(__Reserved_012),
    KEY_NAME_PAIR(NumLock),
    KEY_NAME_PAIR(ScrollLock),
    KEY_NAME_PAIR(OEM_Specific_001),
    KEY_NAME_PAIR(OEM_Specific_002),
    KEY_NAME_PAIR(OEM_Specific_003),
    KEY_NAME_PAIR(OEM_Specific_004),
    KEY_NAME_PAIR(OEM_Specific_005),
    KEY_NAME_PAIR(__Unassigned_003),
    KEY_NAME_PAIR(__Unassigned_004),
    KEY_NAME_PAIR(__Unassigned_005),
    KEY_NAME_PAIR(__Unassigned_006),
    KEY_NAME_PAIR(__Unassigned_007),
    KEY_NAME_PAIR(__Unassigned_008),
    KEY_NAME_PAIR(__Unassigned_009),
    KEY_NAME_PAIR(__Unassigned_010),
    KEY_NAME_PAIR(__Unassigned_011),
    KEY_NAME_PAIR(ShiftLeft),
    KEY_NAME_PAIR(ShiftRight),
    KEY_NAME_PAIR(ControlLeft),
    KEY_NAME_PAIR(CtrlLeft),
    KEY_NAME_PAIR(ControlRight),
    KEY_NAME_PAIR(CtrlRight),
    KEY_NAME_PAIR(AltLeft),
    KEY_NAME_PAIR(AltRight),
    KEY_NAME_PAIR(BrowserBack),
    KEY_NAME_PAIR(BrowserForward),
    KEY_NAME_PAIR(BrowserRefresh),
    KEY_NAME_PAIR(BrowserStop),
    KEY_NAME_PAIR(BrowserSearch),
    KEY_NAME_PAIR(BrowserFavorites),
    KEY_NAME_PAIR(BrowserHome),
    KEY_NAME_PAIR(VolumeMute),
    KEY_NAME_PAIR(VolumeDown),
    KEY_NAME_PAIR(VolumeUp),
    KEY_NAME_PAIR(MediaNextTrack),
    KEY_NAME_PAIR(MediaPreviousTrack),
    KEY_NAME_PAIR(MediaStop),
    KEY_NAME_PAIR(MediaPlayPause),
    KEY_NAME_PAIR(LaunchMail),
    KEY_NAME_PAIR(LaunchMediaSelection),
    KEY_NAME_PAIR(LaunchApplication1),
    KEY_NAME_PAIR(LaunchApplication2),
    KEY_NAME_PAIR(__Reserved_013),
    KEY_NAME_PAIR(__Reserved_014),
    KEY_NAME_PAIR(OEM_K1),
    KEY_NAME_PAIR(OEM_Plus),
    KEY_NAME_PAIR(OEM_Comma),
    KEY_NAME_PAIR(OEM_Minus),
    KEY_NAME_PAIR(OEM_Period),
    KEY_NAME_PAIR(OEM_K2),
    KEY_NAME_PAIR(OEM_K3),
    KEY_NAME_PAIR(__Reserved_015),
    KEY_NAME_PAIR(__Reserved_016),
    KEY_NAME_PAIR(__Reserved_017),
    KEY_NAME_PAIR(__Reserved_018),
    KEY_NAME_PAIR(__Reserved_019),
    KEY_NAME_PAIR(__Reserved_020),
    KEY_NAME_PAIR(__Reserved_021),
    KEY_NAME_PAIR(__Reserved_022),
    KEY_NAME_PAIR(__Reserved_023),
    KEY_NAME_PAIR(__Reserved_024),
    KEY_NAME_PAIR(__Reserved_025),
    KEY_NAME_PAIR(__Reserved_026),
    KEY_NAME_PAIR(__Reserved_027),
    KEY_NAME_PAIR(__Reserved_028),
    KEY_NAME_PAIR(__Reserved_029),
    KEY_NAME_PAIR(__Reserved_030),
    KEY_NAME_PAIR(__Reserved_031),
    KEY_NAME_PAIR(__Reserved_032),
    KEY_NAME_PAIR(__Reserved_033),
    KEY_NAME_PAIR(__Reserved_034),
    KEY_NAME_PAIR(__Reserved_035),
    KEY_NAME_PAIR(__Reserved_036),
    KEY_NAME_PAIR(__Reserved_037),
    KEY_NAME_PAIR(__Reserved_038),
    KEY_NAME_PAIR(__Reserved_039),
    KEY_NAME_PAIR(__Reserved_040),
    KEY_NAME_PAIR(OEM_K4),
    KEY_NAME_PAIR(OEM_K5),
    KEY_NAME_PAIR(OEM_K6),
    KEY_NAME_PAIR(OEM_K7),
    KEY_NAME_PAIR(OEM_K8),
    KEY_NAME_PAIR(__Reserved_041),
    KEY_NAME_PAIR(OEM_Specific_006),
    KEY_NAME_PAIR(OEM_K102),
    KEY_NAME_PAIR(OEM_Specific_007),
    KEY_NAME_PAIR(OEM_Specific_008),
    KEY_NAME_PAIR(IME_Process),
    KEY_NAME_PAIR(OEM_Specific_009),
    KEY_NAME_PAIR(Packet),
    KEY_NAME_PAIR(__Unassigned_012),
    KEY_NAME_PAIR(OEM_Specific_010),
    KEY_NAME_PAIR(OEM_Specific_011),
    KEY_NAME_PAIR(OEM_Specific_012),
    KEY_NAME_PAIR(OEM_Specific_013),
    KEY_NAME_PAIR(OEM_Specific_014),
    KEY_NAME_PAIR(OEM_Specific_015),
    KEY_NAME_PAIR(OEM_Specific_016),
    KEY_NAME_PAIR(OEM_Specific_017),
    KEY_NAME_PAIR(OEM_Specific_018),
    KEY_NAME_PAIR(OEM_Specific_019),
    KEY_NAME_PAIR(OEM_Specific_020),
    KEY_NAME_PAIR(OEM_Specific_021),
    KEY_NAME_PAIR(OEM_Specific_022),
    KEY_NAME_PAIR(Attn),
    KEY_NAME_PAIR(CrSel),
    KEY_NAME_PAIR(ExSel),
    KEY_NAME_PAIR(EraseEOF),
    KEY_NAME_PAIR(Play),
    KEY_NAME_PAIR(Zoom),
    KEY_NAME_PAIR(NoName),
    KEY_NAME_PAIR(PA1),
    KEY_NAME_PAIR(OEM_Clear),
});


void WinDebug::PrintWindowsMessage(UINT msg, WPARAM wParam, LPARAM lParam)
{
#if defined(_UNICODE) || defined(UNICODE)
	wstringstream ss, tmpss;
#else
	stringstream ss, tmpss;
#endif

	MMap::const_iterator i = _winMessagesMap.find(msg);

	ss << left << setw(30);

	if (i != _winMessagesMap.end())
	{
		ss << i->second;
	}
	else
	{
		tmpss << "Unknown (0x" << hex << setfill(DxsT('0')) << setw(4) << msg << ")";
		ss << tmpss.str();
	}

	ss << right;
	ss << " - wParam (0x" << hex << setfill(DxsT('0')) << setw(8) << wParam << ")";
	ss << ", lParam (0x" << hex << setfill(DxsT('0')) << setw(8) << lParam << ")" << endl;

	OutputDebugString(ss.str().c_str());
}

void WinDebug::PrintKeyStatus(KeyCode key, KeyStatus status)
{
#if defined(_UNICODE) || defined(UNICODE)
	wstringstream ss;
#else
	stringstream ss;
#endif

	KMap::const_iterator i = _keyCodesMap.find(key);

	if (i != _keyCodesMap.end())
	{
		ss << i->second;
	}
	else
	{
    ss << "Unknown key";
	}

  switch (status)
  {
  case KeyStatus::Pressed:  ss << " - Pressed" << endl; break;
  case KeyStatus::Released: ss << " - Released" << endl; break;
  }

	OutputDebugString(ss.str().c_str());
}

void WinDebug::PrintCharReceived(TChar ch)
{
#if defined(_UNICODE) || defined(UNICODE)
  wstringstream ss;
#else
  stringstream ss;
#endif

  ss << "Keyboard: Char '" << ch << "'" << endl;

  OutputDebugString(ss.str().c_str());
}

void WinDebug::PrintMouseClientPosition(POINT pt)
{
#if defined(_UNICODE) || defined(UNICODE)
  wstringstream ss;
#else
  stringstream ss;
#endif

  ss << "Mouse: (Client - " << pt.x << ", " << pt.y << ")" << endl;

  OutputDebugString(ss.str().c_str());
}

void WinDebug::PrintMouseClientButtonStatus(MouseButton b, KeyStatus s, POINT pt)
{
#if defined(_UNICODE) || defined(UNICODE)
  wstringstream ss;
#else
  stringstream ss;
#endif

  ss << "Mouse: ";

  switch (b)
  {
  case MouseButton::Left:   ss << "Left";    break;
  case MouseButton::Right:  ss << "Right";   break;
  case MouseButton::Middle: ss << "Middle";  break;
  default:                  ss << "Unknown"; break;
  }

  ss << " Button ";

  switch (s)
  {
  case KeyStatus::Pressed:  ss << "Pressed"; break;
  case KeyStatus::Released: ss << "Released"; break;
  default: ss << "Unknown Status"; break;
  }

  ss << " (" << pt.x << ", " << pt.y << ")" << endl;

  OutputDebugString(ss.str().c_str());
}

void WinDebug::PrintMouseWheelStatus(short delta, POINT pt)
{
#if defined(_UNICODE) || defined(UNICODE)
  wstringstream ss;
#else
  stringstream ss;
#endif

  ss << "Mouse: Wheel " << delta << " (" << pt.x << ", " << pt.y << ")" << endl;

  OutputDebugString(ss.str().c_str());
}
