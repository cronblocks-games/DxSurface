#ifndef _DXSURFACE_DXSTYPES_H_
#define _DXSURFACE_DXSTYPES_H_

#include <atomic>
#include <string>
#include <sstream>
#include <mutex>
#include <memory>
#include <vector>
#include <chrono>
#include <thread>

#include "DxsSettings.hpp"
#include "DxsWinApi.hpp"

namespace CB::DxSurface {

  using String = std::string;
  using WString = std::wstring;
  using StringStream = std::stringstream;
  using WStringStream = std::wstringstream;

#ifdef DxsUnicodeBuild
  using TChar = wchar_t;
  using TString = WString;
  using TStringStream = WStringStream;
#else
  using TChar = char;
  using TString = String;
  using TStringStream = StringStream;
#endif

  using TCCharPtr = const TChar*;
  using TCharPtr = TChar*;

  template <typename T>
  using PtrShared = std::shared_ptr<T>;
  template <typename T>
  using PtrUnique = std::unique_ptr<T>;
  template <typename T>
  using PtrCom = Microsoft::WRL::ComPtr<T>;
  template <typename T>
  using Ptr = PtrShared<T>;
  template <typename T>
  using Vector = std::vector<T>;
  
  using DxDevice = ID3D11Device;
  using DxSwapChain = IDXGISwapChain;
  using DxDeviceContext = ID3D11DeviceContext;
  using DxVertexShader = ID3D11VertexShader;
  using DxPixelShader = ID3D11PixelShader;
  using DxRenderTargetView = ID3D11RenderTargetView;
  using DxDepthStencilView = ID3D11DepthStencilView;
  using DxResource = ID3D11Resource;
  using DxBlob = ID3DBlob;
  using DxDXGIInfoQueue = IDXGIInfoQueue;
  enum class DxFeatureLevel : int
  {
    _11_1 = D3D_FEATURE_LEVEL_11_1,
    _11_0 = D3D_FEATURE_LEVEL_11_0,
    _10_1 = D3D_FEATURE_LEVEL_10_1,
    _10_0 = D3D_FEATURE_LEVEL_10_0,
    _9_3  = D3D_FEATURE_LEVEL_9_3,
    _9_2  = D3D_FEATURE_LEVEL_9_2,
    _9_1  = D3D_FEATURE_LEVEL_9_1
  };
  enum class DxShaderType
  {
    VertexShader, PixelShader
  };

  using Thread = std::thread;
  using Mutex = std::mutex;
  using MutexLock = const std::lock_guard<Mutex>;
  using Lock = MutexLock;
  template <typename T>
  using Atomic = std::atomic<T>;

  using Clock = std::chrono::high_resolution_clock;
  using Time = Clock;
  using TimePoint = std::chrono::time_point<Clock>;
  using TimeDurationMilli = std::chrono::duration<double, std::milli>;
  using TimeDurationSec = std::chrono::duration<double>;

  enum class WindowCreationState
  {
    NotInitiated = 0, Successful, Failed
  };
  enum class ExecutionCommand
  {
    Run, Pause, Exit,
  };
  enum class ExecutionState
  {
    NONE = 0, Init, Running, Paused, Exitted,
  };
  enum class ExecutionExitReason
  {
    Normal = 0, Exception
  };

  class Window;
  class Graphics;
  class Keyboard;
  class Mouse;
  class Icon;
  class Cursor;
  class Bitmap;

  struct WindowRect { long x = 0, y = 0, w = 0, h = 0; };
  struct ClientRect { long x = 0, y = 0, w = 0, h = 0; };

  using WindowCallbackPositionChanged = void(*)(Window&, WindowRect&, ClientRect&);
  using WindowCallbackSizeChanged = void(*)(Window&, WindowRect&, ClientRect&);

  using RenderingCallbackInit = void(*)(Window&, Graphics&);
  using RenderingCallbackRunning = void(*)(Window&, Graphics&, Keyboard&, Mouse&, double deltaTimeSec);
  using RenderingCallbackPaused = void(*)(Window&, Graphics&, Keyboard&, Mouse&, double deltaTimeSec);
  using RenderingCallbackExitted = void(*)(Window&, Graphics&, Keyboard&, Mouse&, ExecutionExitReason reason, const TString& message);
  using RenderingCallbackStateChanged = void(*)(Window&, Graphics&, Keyboard&, Mouse&, ExecutionState from, ExecutionState to);

  using ProcessingCallbackInit = void(*)(Window&);
  using ProcessingCallbackRunning = void(*)(Window&, Keyboard&, Mouse&, double deltaTimeSec);
  using ProcessingCallbackPaused = void(*)(Window&, Keyboard&, Mouse&, double deltaTimeSec);
  using ProcessingCallbackExitted = void(*)(Window&, Keyboard&, Mouse&, ExecutionExitReason reason, const TString& message);
  using ProcessingCallbackStateChanged = void(*)(Window&, Keyboard&, Mouse&, ExecutionState from, ExecutionState to);

  struct WindowCallbacks
  {
    WindowCallbacks(
      WindowCallbackPositionChanged windowCallbackPositionChanged = nullptr,
      WindowCallbackSizeChanged windowCallbackSizeChanged = nullptr,

      RenderingCallbackInit renderingCallbackInit = nullptr,
      RenderingCallbackRunning renderingCallbackRunning = nullptr,
      RenderingCallbackPaused renderingCallbackPaused = nullptr,
      RenderingCallbackExitted renderingCallbackExitted = nullptr,
      RenderingCallbackStateChanged renderingCallbackStateChanged = nullptr,

      ProcessingCallbackInit processingCallbackInit = nullptr,
      ProcessingCallbackRunning processingCallbackRunning = nullptr,
      ProcessingCallbackPaused processingCallbackPaused = nullptr,
      ProcessingCallbackExitted processingCallbackExitted = nullptr,
      ProcessingCallbackStateChanged processingCallbackStateChanged = nullptr)
    {
      this->windowCallbackPositionChanged.store(windowCallbackPositionChanged);
      this->windowCallbackSizeChanged.store(windowCallbackSizeChanged);

      this->renderingCallbackInit.store(renderingCallbackInit);
      this->renderingCallbackRunning.store(renderingCallbackRunning);
      this->renderingCallbackPaused.store(renderingCallbackPaused);
      this->renderingCallbackExitted.store(renderingCallbackExitted);
      this->renderingCallbackStateChanged.store(renderingCallbackStateChanged);

      this->processingCallbackInit.store(processingCallbackInit);
      this->processingCallbackRunning.store(processingCallbackRunning);
      this->processingCallbackPaused.store(processingCallbackPaused);
      this->processingCallbackExitted.store(processingCallbackExitted);
      this->processingCallbackStateChanged.store(processingCallbackStateChanged);
    }
    WindowCallbacks(const WindowCallbacks& o)
    {
      *this = o;
    }
    WindowCallbacks(WindowCallbacks&& o) noexcept
    {
      *this = std::move(o);
    }
    WindowCallbacks& operator=(const WindowCallbacks& o)
    {
      if (this != &o)
      {
        windowCallbackPositionChanged.store(o.windowCallbackPositionChanged.load());
        windowCallbackSizeChanged.store(o.windowCallbackSizeChanged.load());

        renderingCallbackInit.store(o.renderingCallbackInit.load());
        renderingCallbackRunning.store(o.renderingCallbackRunning.load());
        renderingCallbackPaused.store(o.renderingCallbackPaused.load());
        renderingCallbackExitted.store(o.renderingCallbackExitted.load());
        renderingCallbackStateChanged.store(o.renderingCallbackStateChanged.load());

        processingCallbackInit.store(o.processingCallbackInit.load());
        processingCallbackRunning.store(o.processingCallbackRunning.load());
        processingCallbackPaused.store(o.processingCallbackPaused.load());
        processingCallbackExitted.store(o.processingCallbackExitted.load());
        processingCallbackStateChanged.store(o.processingCallbackStateChanged.load());
      }
      return *this;
    }
    WindowCallbacks& operator=(WindowCallbacks&& o) noexcept
    {
      if (this != &o)
      {
        windowCallbackPositionChanged.store(o.windowCallbackPositionChanged.exchange(nullptr));
        windowCallbackSizeChanged.store(o.windowCallbackSizeChanged.exchange(nullptr));

        renderingCallbackInit.store(o.renderingCallbackInit.exchange(nullptr));
        renderingCallbackRunning.store(o.renderingCallbackRunning.exchange(nullptr));
        renderingCallbackPaused.store(o.renderingCallbackPaused.exchange(nullptr));
        renderingCallbackExitted.store(o.renderingCallbackExitted.exchange(nullptr));
        renderingCallbackStateChanged.store(o.renderingCallbackStateChanged.exchange(nullptr));

        processingCallbackInit.store(o.processingCallbackInit.exchange(nullptr));
        processingCallbackRunning.store(o.processingCallbackRunning.exchange(nullptr));
        processingCallbackPaused.store(o.processingCallbackPaused.exchange(nullptr));
        processingCallbackExitted.store(o.processingCallbackExitted.exchange(nullptr));
        processingCallbackStateChanged.store(o.processingCallbackStateChanged.exchange(nullptr));
      }
      return *this;
    }

    Atomic<WindowCallbackPositionChanged> windowCallbackPositionChanged;
    Atomic<WindowCallbackSizeChanged> windowCallbackSizeChanged;

    Atomic<RenderingCallbackInit> renderingCallbackInit;
    Atomic<RenderingCallbackRunning> renderingCallbackRunning;
    Atomic<RenderingCallbackPaused> renderingCallbackPaused;
    Atomic<RenderingCallbackExitted> renderingCallbackExitted;
    Atomic<RenderingCallbackStateChanged> renderingCallbackStateChanged;

    Atomic<ProcessingCallbackInit> processingCallbackInit;
    Atomic<ProcessingCallbackRunning> processingCallbackRunning;
    Atomic<ProcessingCallbackPaused> processingCallbackPaused;
    Atomic<ProcessingCallbackExitted> processingCallbackExitted;
    Atomic<ProcessingCallbackStateChanged> processingCallbackStateChanged;
  };
  struct WindowCreationOptions
  {
    TString      title                            = DxsT("DxSurface");
    bool         isPrimary                        = false;
    Ptr<Icon>    icon;
    Ptr<Icon>    iconSmall;
    Ptr<Cursor>  cursor;
    WindowRect   windowRectangle                  = WindowRect(CW_USEDEFAULT, CW_USEDEFAULT, 600, 450);
    DWORD        windowStyle                      = WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SIZEBOX;
    DWORD        windowExStyle                    = WS_EX_WINDOWEDGE;
    
    unsigned int maxRenderingThreadRefreshRateHz  = DxsDefaultThreadRefreshRateHz;
    unsigned int maxProcessingThreadRefreshRateHz = DxsDefaultThreadRefreshRateHz;

    WindowCallbacks callbacks = WindowCallbacks();
  };

  enum class KeyCode : unsigned char
  {
    MouseLeft = VK_LBUTTON,         // 0x01 - Left mouse button
    MouseRight = VK_RBUTTON,        // 0x02 - Right mouse button
    Cancel = VK_CANCEL,             // 0x03 - Control+break processing
    CtrlBreak = Cancel,             // 
    MouseMiddle = VK_MBUTTON,       // 0x04 - Middle mouse button
    MouseX1 = VK_XBUTTON1,          // 0x05 - X1 mouse button
    MouseX2 = VK_XBUTTON2,          // 0x06 - X2 mouse button
    RESERVED_001 = 0x07,            // 0x07 - Reserved
    BackSpace = VK_BACK,            // 0x08 - BACKSPACE key
    Tab = VK_TAB,                   // 0x09 - TAB key
    RESERVED_002 = 0x0A, RESERVED_003 = 0x0B,     // 0x0A,0x0B - Reserved
    Clear = VK_CLEAR,               // 0x0C - CLEAR key
    Enter = VK_RETURN,              // 0x0D - ENTER key
    Return = Enter,                 // 
    UNASSIGNED_001 = 0x0E, UNASSIGNED_002 = 0x0F, // 0x0E,0x0F - Unassigned
    Shift = VK_SHIFT,               // 0x10 - SHIFT key
    Control = VK_CONTROL,           // 0x11 - CTRL key
    Ctrl = Control,                 // 
    Alt = VK_MENU,                  // 0x12 - ALT key
    Pause = VK_PAUSE,               // 0x13 - PAUSE key
    CapsLock = VK_CAPITAL,          // 0x14 - CAPS LOCK key
    IME_Kana = VK_KANA,             // 0x15 - IME Kana mode
    IME_Hangul = VK_HANGUL,         // 0x15 - IME Hangul mode
    IME_ON = VK_IME_ON,             // 0x16 - IME On
    IME_Junja = VK_JUNJA,           // 0x17 - IME Junja mode
    IME_Final = VK_FINAL,           // 0x18 - IME final mode
    IME_Hanja = VK_HANJA,           // 0x19 - IME Hanja mode
    IME_Kanji = VK_KANJI,           // 0x19 - IME Kanji mode
    IME_OFF = VK_IME_OFF,           // 0x1A - IME Off
    Escape = VK_ESCAPE,             // 0x1B - ESC key
    Esc = Escape,                   // 
    IME_Convert = VK_CONVERT,       // 0x1C - IME convert
    IME_NonConvert = VK_NONCONVERT, // 0x1D - IME non-convert
    IME_Accept = VK_ACCEPT,         // 0x1E - IME accept
    IME_ChangeReq = VK_MODECHANGE,  // 0x1F - IME mode change request
    Space = VK_SPACE,               // 0x20 - SPACEBAR
    PageUp = VK_PRIOR,              // 0x21 - PAGE UP key
    PageDown = VK_NEXT,             // 0x22 - PAGE DOWN key
    End = VK_END,                   // 0x23 - END key
    Home = VK_HOME,                 // 0x24 - HOME key
    LeftArrow = VK_LEFT,            // 0x25 - LEFT ARROW key
    UpArrow = VK_UP,                // 0x26 - UP ARROW key
    RightArrow = VK_RIGHT,          // 0x27 - RIGHT ARROW key
    DownArrow = VK_DOWN,            // 0x28 - DOWN ARROW key
    Select = VK_SELECT,             // 0x29 - SELECT key
    Print = VK_PRINT,               // 0x2A - PRINT key
    Execute = VK_EXECUTE,           // 0x2B - EXECUTE key
    PrintScreen = VK_SNAPSHOT,      // 0x2C - PRINT SCREEN key
    Insert = VK_INSERT,             // 0x2D - INS key
    Delete = VK_DELETE,             // 0x2E - DEL key
    Del = Delete,                   // 
    Help = VK_HELP,                 // 0x2F - HELP key
    Num0 = 0x30, Digit0 = Num0,     // 0x30 - 0 key
    Num1 = 0x31, Digit1 = Num1,     // 0x31 - 1 key
    Num2 = 0x32, Digit2 = Num2,     // 0x32 - 2 key
    Num3 = 0x33, Digit3 = Num3,     // 0x33 - 3 key
    Num4 = 0x34, Digit4 = Num4,     // 0x34 - 4 key
    Num5 = 0x35, Digit5 = Num5,     // 0x35 - 5 key
    Num6 = 0x36, Digit6 = Num6,     // 0x36 - 6 key
    Num7 = 0x37, Digit7 = Num7,     // 0x37 - 7 key
    Num8 = 0x38, Digit8 = Num8,     // 0x38 - 8 key
    Num9 = 0x39, Digit9 = Num9,     // 0x39 - 9 key
    UNDEFINED_001 = 0x3A, UNDEFINED_002 = 0x3B, UNDEFINED_003 = 0x3C,      // 0x3A-0x40 - Undefined
    UNDEFINED_004 = 0x3D, UNDEFINED_005 = 0x3E, UNDEFINED_006 = 0x3F,      // 
    UNDEFINED_007 = 0x40,                                                  // 
    A = 0x41,                       // 0x41 - A key
    B = 0x42,                       // 0x42 - B key
    C = 0x43,                       // 0x43 - C key
    D = 0x44,                       // 0x44 - D key
    E = 0x45,                       // 0x45 - E key
    F = 0x46,                       // 0x46 - F key
    G = 0x47,                       // 0x47 - G key
    H = 0x48,                       // 0x48 - H key
    I = 0x49,                       // 0x49 - I key
    J = 0x4A,                       // 0x4A - J key
    K = 0x4B,                       // 0x4B - K key
    L = 0x4C,                       // 0x4C - L key
    M = 0x4D,                       // 0x4D - M key
    N = 0x4E,                       // 0x4E - N key
    O = 0x4F,                       // 0x4F - O key
    P = 0x50,                       // 0x50 - P key
    Q = 0x51,                       // 0x51 - Q key
    R = 0x52,                       // 0x52 - R key
    S = 0x53,                       // 0x53 - S key
    T = 0x54,                       // 0x54 - T key
    U = 0x55,                       // 0x55 - U key
    V = 0x56,                       // 0x56 - V key
    W = 0x57,                       // 0x57 - W key
    X = 0x58,                       // 0x58 - X key
    Y = 0x59,                       // 0x59 - Y key
    Z = 0x5A,                       // 0x5A - Z key
    WinLeft = VK_LWIN,              // 0x5B - Left Windows key
    WinRight = VK_RWIN,             // 0x5C - Right Windows key
    Applications = VK_APPS,         // 0x5D - Applications key
    RESERVED_004 = 0x5E,            // 0x5E - Reserved
    Sleep = VK_SLEEP,               // 0x5F - Computer Sleep key
    NumPad0 = VK_NUMPAD0,           // 0x60 - Numeric keypad 0 key
    NumPad1 = VK_NUMPAD1,           // 0x61 - Numeric keypad 1 key
    NumPad2 = VK_NUMPAD2,           // 0x62 - Numeric keypad 2 key
    NumPad3 = VK_NUMPAD3,           // 0x63 - Numeric keypad 3 key
    NumPad4 = VK_NUMPAD4,           // 0x64 - Numeric keypad 4 key
    NumPad5 = VK_NUMPAD5,           // 0x65 - Numeric keypad 5 key
    NumPad6 = VK_NUMPAD6,           // 0x66 - Numeric keypad 6 key
    NumPad7 = VK_NUMPAD7,           // 0x67 - Numeric keypad 7 key
    NumPad8 = VK_NUMPAD8,           // 0x68 - Numeric keypad 8 key
    NumPad9 = VK_NUMPAD9,           // 0x69 - Numeric keypad 9 key
    Asterisk = VK_MULTIPLY,         // 0x6A - Multiply key
    Multiply = Asterisk,            // 
    Plus = VK_ADD,                  // 0x6B - Add key
    Add = Plus,                     // 
    Separator = VK_SEPARATOR,       // 0x6C - Separator key
    Minus = VK_SUBTRACT,            // 0x6D - Subtract key
    Subtract = Minus,               // 
    DecimalPoint = VK_DECIMAL,      // 0x6E - Decimal key
    ForwardSlash = VK_DIVIDE,       // 0x6F - Divide key
    Divide = ForwardSlash,          // 
    F1 = VK_F1,                     // 0x70 - F1 key
    F2 = VK_F2,                     // 0x71 - F2 key
    F3 = VK_F3,                     // 0x72 - F3 key
    F4 = VK_F4,                     // 0x73 - F4 key
    F5 = VK_F5,                     // 0x74 - F5 key
    F6 = VK_F6,                     // 0x75 - F6 key
    F7 = VK_F7,                     // 0x76 - F7 key
    F8 = VK_F8,                     // 0x77 - F8 key
    F9 = VK_F9,                     // 0x78 - F9 key
    F10 = VK_F10,                   // 0x79 - F10 key
    F11 = VK_F11,                   // 0x7A - F11 key
    F12 = VK_F12,                   // 0x7B - F12 key
    F13 = VK_F13,                   // 0x7C - F13 key
    F14 = VK_F14,                   // 0x7D - F14 key
    F15 = VK_F15,                   // 0x7E - F15 key
    F16 = VK_F16,                   // 0x7F - F16 key
    F17 = VK_F17,                   // 0x80 - F17 key
    F18 = VK_F18,                   // 0x81 - F18 key
    F19 = VK_F19,                   // 0x82 - F19 key
    F20 = VK_F20,                   // 0x83 - F20 key
    F21 = VK_F21,                   // 0x84 - F21 key
    F22 = VK_F22,                   // 0x85 - F22 key
    F23 = VK_F23,                   // 0x86 - F23 key
    F24 = VK_F24,                   // 0x87 - F24 key
    RESERVED_005 = 0x88,            // 0x88-0x8F - Reserved
    RESERVED_006 = 0x89,            // 
    RESERVED_007 = 0x8A,            // 
    RESERVED_008 = 0x8B,            // 
    RESERVED_009 = 0x8C,            // 
    RESERVED_010 = 0x8D,            // 
    RESERVED_011 = 0x8E,            // 
    RESERVED_012 = 0x8F,            // 
    NumLock = VK_NUMLOCK,           // 0x90 - NUM LOCK key
    ScrollLock = VK_SCROLL,         // 0x91 - SCROLL LOCK key
    OEM_Specific_001 = 0x92,        // 0x92-0x96 - OEM specific
    OEM_Specific_002 = 0x93,        // 
    OEM_Specific_003 = 0x94,        // 
    OEM_Specific_004 = 0x95,        // 
    OEM_Specific_005 = 0x96,        // 
    UNASSIGNED_003 = 0x97,          // 0x97-0x9F - Unassigned
    UNASSIGNED_004 = 0x98,          // 
    UNASSIGNED_005 = 0x99,          // 
    UNASSIGNED_006 = 0x9A,          // 
    UNASSIGNED_007 = 0x9B,          // 
    UNASSIGNED_008 = 0x9C,          // 
    UNASSIGNED_009 = 0x9D,          // 
    UNASSIGNED_010 = 0x9E,          // 
    UNASSIGNED_011 = 0x9F,          // 
    ShiftLeft = VK_LSHIFT,          // 0xA0 - Left SHIFT key
    ShiftRight = VK_RSHIFT,         // 0xA1 - Right SHIFT key
    ControlLeft = VK_LCONTROL,      // 0xA2 - Left CONTROL key
    CtrlLeft = ControlLeft,         // 
    ControlRight = VK_RCONTROL,     // 0xA3 - Right CONTROL key
    CtrlRight = ControlRight,       // 
    AltLeft = VK_LMENU,             // 0xA4 - Left ALT key
    AltRight = VK_RMENU,            // 0xA5 - Right ALT key
    BrowserBack = VK_BROWSER_BACK,                   // 0xA6 - Browser Back key
    BrowserForward = VK_BROWSER_FORWARD,             // 0xA7 - Browser Forward key
    BrowserRefresh = VK_BROWSER_REFRESH,             // 0xA8 - Browser Refresh key
    BrowserStop = VK_BROWSER_STOP,                   // 0xA9 - Browser Stop key
    BrowserSearch = VK_BROWSER_SEARCH,               // 0xAA - Browser Search key
    BrowserFavorites = VK_BROWSER_FAVORITES,         // 0xAB - Browser Favorites key
    BrowserHome = VK_BROWSER_HOME,                   // 0xAC - Browser Start and Home key
    VolumeMute = VK_VOLUME_MUTE,                     // 0xAD - Volume Mute key
    VolumeDown = VK_VOLUME_DOWN,                     // 0xAE - Volume Down key
    VolumeUp = VK_VOLUME_UP,                         // 0xAF - Volume Up key
    MediaNextTrack = VK_MEDIA_NEXT_TRACK,            // 0xB0 - Next Track key
    MediaPreviousTrack = VK_MEDIA_PREV_TRACK,        // 0xB1 - Previous Track key
    MediaStop = VK_MEDIA_STOP,                       // 0xB2 - Stop Media key
    MediaPlayPause = VK_MEDIA_PLAY_PAUSE,            // 0xB3 - Play / Pause Media key
    LaunchMail = VK_LAUNCH_MAIL,                     // 0xB4 - Start Mail key
    LaunchMediaSelection = VK_LAUNCH_MEDIA_SELECT,   // 0xB5 - Select Media key
    LaunchApplication1 = VK_LAUNCH_APP1,             // 0xB6 - Start Application 1 key
    LaunchApplication2 = VK_LAUNCH_APP2,             // 0xB7 - Start Application 2 key
    RESERVED_013 = 0xB8,                             // 0xB8-0xB9 - Reserved
    RESERVED_014 = 0xB9,                             // 
    OEM_K1 = VK_OEM_1,                               // 0xBA - Used for miscellaneous characters. For the US standard keyboard  ;:  key
    OEM_Plus = VK_OEM_PLUS,                          // 0xBB - For any country / region, the + key
    OEM_Comma = VK_OEM_COMMA,                        // 0xBC - For any country / region, the , key
    OEM_Minus = VK_OEM_MINUS,                        // 0xBD - For any country / region, the - key
    OEM_Period = VK_OEM_PERIOD,                      // 0xBE - For any country / region, the . key
    OEM_K2 = VK_OEM_2,                               // 0xBF - Used for miscellaneous characters; For the US standard keyboard  /?  key
    OEM_K3 = VK_OEM_3,                               // 0xC0 - Used for miscellaneous characters; For the US standard keyboard  `~  key
    RESERVED_015 = 0xC1,                             // 0xC1-0xDA - Reserved
    RESERVED_016 = 0xC2,                             // 
    RESERVED_017 = 0xC3,                             // 
    RESERVED_018 = 0xC4,                             // 
    RESERVED_019 = 0xC5,                             // 
    RESERVED_020 = 0xC6,                             // 
    RESERVED_021 = 0xC7,                             // 
    RESERVED_022 = 0xC8,                             // 
    RESERVED_023 = 0xC9,                             // 
    RESERVED_024 = 0xCA,                             // 
    RESERVED_025 = 0xCB,                             // 
    RESERVED_026 = 0xCC,                             // 
    RESERVED_027 = 0xCD,                             // 
    RESERVED_028 = 0xCE,                             // 
    RESERVED_029 = 0xCF,                             // 
    RESERVED_030 = 0xD0,                             // 
    RESERVED_031 = 0xD1,                             // 
    RESERVED_032 = 0xD2,                             // 
    RESERVED_033 = 0xD3,                             // 
    RESERVED_034 = 0xD4,                             // 
    RESERVED_035 = 0xD5,                             // 
    RESERVED_036 = 0xD6,                             // 
    RESERVED_037 = 0xD7,                             // 
    RESERVED_038 = 0xD8,                             // 
    RESERVED_039 = 0xD9,                             // 
    RESERVED_040 = 0xDA,                             // 
    OEM_K4 = VK_OEM_4,                               // 0xDB - Used for miscellaneous characters; For the US standard keyboard  [{   key
    OEM_K5 = VK_OEM_5,                               // 0xDC - Used for miscellaneous characters; For the US standard keyboard  \\|  key
    OEM_K6 = VK_OEM_6,                               // 0xDD - Used for miscellaneous characters; For the US standard keyboard  ]}   key
    OEM_K7 = VK_OEM_7,                               // 0xDE - Used for miscellaneous characters; For the US standard keyboard  '"   key
    OEM_K8 = VK_OEM_8,                               // 0xDF - Used for miscellaneous characters;
    RESERVED_041 = 0xE0,                             // 0xE0 - Reserved
    OEM_Specific_006 = 0xE1,                         // 0xE1 - OEM specific
    OEM_K102 = VK_OEM_102,                           // 0xE2 - The  <>  keys on the US standard keyboard, or the  \\|  key on the non-US 102-key keyboard
    OEM_Specific_007 = 0xE3,                         // 0xE3-0xE4 - OEM specific
    OEM_Specific_008 = 0xE4,                         // 
    IME_Process = VK_PROCESSKEY,                     // 0xE5 - IME PROCESS key
    OEM_Specific_009 = 0xE6,                         // 0xE6 - OEM specific
    Packet = VK_PACKET,                              // 0xE7 - Used to pass Unicode characters as if they were keystrokes
    UNASSIGNED_012 = 0xE8,                           // 0xE8 - Unassigned
    OEM_Specific_010 = 0xE9,                         // 0xE9-0xF5 - OEM specific
    OEM_Specific_011 = 0xEA,                         // 
    OEM_Specific_012 = 0xEB,                         // 
    OEM_Specific_013 = 0xEC,                         // 
    OEM_Specific_014 = 0xED,                         // 
    OEM_Specific_015 = 0xEE,                         // 
    OEM_Specific_016 = 0xEF,                         // 
    OEM_Specific_017 = 0xF0,                         // 
    OEM_Specific_018 = 0xF1,                         // 
    OEM_Specific_019 = 0xF2,                         // 
    OEM_Specific_020 = 0xF3,                         // 
    OEM_Specific_021 = 0xF4,                         // 
    OEM_Specific_022 = 0xF5,                         // 
    Attn = VK_ATTN,                                  // 0xF6 - Attn key
    CrSel = VK_CRSEL,                                // 0xF7 - CrSel key
    ExSel = VK_EXSEL,                                // 0xF8 - ExSel key
    EraseEOF = VK_EREOF,                             // 0xF9 - Erase EOF key
    Play = VK_PLAY,                                  // 0xFA - Play key
    Zoom = VK_ZOOM,                                  // 0xFB - Zoom key
    NoName = VK_NONAME,                              // 0xFC - Reserved
    PA1 = VK_PA1,                                    // 0xFD - PA1 key
    OEM_Clear = VK_OEM_CLEAR,                        // 0xFE - Clear key
  };
  enum class KeyStatus
  {
    Pressed, Released
  };
  enum class MouseButton
  {
    Left, Right, Middle
  };
  enum class ButtonStatus
  {
    Pressed, Released
  };

  enum class SystemIcon : unsigned int
  {
    Sample = OIC_SAMPLE,
    Hand = OIC_HAND,
    Question = OIC_QUES,
    Bang = OIC_BANG,
    Note = OIC_NOTE,
    WinLogo = OIC_WINLOGO,
    Warning = Bang,
    Error = Hand,
    Information = Note,
    Shield = OIC_SHIELD,
  };
  enum class SystemCursor : unsigned int
  {
    Normal = OCR_NORMAL,
    IBeam = OCR_IBEAM,
    Wait = OCR_WAIT,
    Cross = OCR_CROSS,
    Up = OCR_UP,
    SizeNWSE = OCR_SIZENWSE,
    SizeNESW = OCR_SIZENESW,
    SizeWE = OCR_SIZEWE,
    SizeNS = OCR_SIZENS,
    SizeAll = OCR_SIZEALL,
    No = OCR_NO,
    Hand = OCR_HAND,
    AppStarting = OCR_APPSTARTING,
  };
  enum class SystemBitmap : unsigned int
  {
    Close = OBM_CLOSE,
    UpArrow = OBM_UPARROW,
    DownArrow = OBM_DNARROW,
    RightArrow = OBM_RGARROW,
    LeftArrow = OBM_LFARROW,
    Reduce = OBM_REDUCE,
    Zoom = OBM_ZOOM,
    Restore = OBM_RESTORE,
    Reduced = OBM_REDUCED,
    Zoomed = OBM_ZOOMD,
    Restored = OBM_RESTORED,
    UpArrowD = OBM_UPARROWD,
    DownArrowD = OBM_DNARROWD,
    RightArrowD = OBM_RGARROWD,
    LeftArrowD = OBM_LFARROWD,
    MNarrow = OBM_MNARROW,
    Combo = OBM_COMBO,
    UpArrowI = OBM_UPARROWI,
    DownArrowI = OBM_DNARROWI,
    RightArrowI = OBM_RGARROWI,
    LeftArrowI = OBM_LFARROWI,
    Size = OBM_SIZE,
    BtSize = OBM_BTSIZE,
    Check = OBM_CHECK,
    CheckBoxes = OBM_CHECKBOXES,
    ButtonCorners = OBM_BTNCORNERS,
    OldClose = OBM_OLD_CLOSE,
    OldUpArrow = OBM_OLD_UPARROW,
    OldDownArrow = OBM_OLD_DNARROW,
    OldRightArrow = OBM_OLD_RGARROW,
    OldLeftArrow = OBM_OLD_LFARROW,
    OldReduce = OBM_OLD_REDUCE,
    OldZoom = OBM_OLD_ZOOM,
    OldRestore = OBM_OLD_RESTORE,
  };




} //- namespace CB::DxSurface



#endif
