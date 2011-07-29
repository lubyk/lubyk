/*
  ==============================================================================

   This file is part of the LUBYK project (http://lubyk.org)
   Copyright (c) 2007-2011 by Gaspard Bucher (http://teti.ch).

  ------------------------------------------------------------------------------

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.

  ==============================================================================
*/
#ifndef LUBYK_INCLUDE_MIMAS_CONSTANTS_H_
#define LUBYK_INCLUDE_MIMAS_CONSTANTS_H_

#include "mimas/mimas.h"

#include "lubyk.h"

#include <string>

namespace mimas {

/** List of Brush styles.
 */
enum BrushStyles {
  NoBrush                 = Qt::NoBrush,
  SolidPattern            = Qt::SolidPattern,
  //Dense1Pattern           = Qt::Dense1Pattern
  //Dense2Pattern           = Qt::Dense2Pattern
  //Dense3Pattern           = Qt::Dense3Pattern
  //Dense4Pattern           = Qt::Dense4Pattern
  //Dense5Pattern           = Qt::Dense5Pattern
  //Dense6Pattern           = Qt::Dense6Pattern
  //Dense7Pattern           = Qt::Dense7Pattern
  //HorPattern              = Qt::HorPattern
  //VerPattern              = Qt::VerPattern
  //CrossPattern            = Qt::CrossPattern
  //BDiagPattern            = Qt::BDiagPattern
  //FDiagPattern            = Qt::FDiagPattern
  //DiagCrossPattern        = Qt::DiagCrossPattern
  LinearGradientPattern   = Qt::LinearGradientPattern,
  ConicalGradientPattern  = Qt::ConicalGradientPattern,
  RadialGradientPattern   = Qt::RadialGradientPattern,
  TexturePattern          = Qt::TexturePattern,
};

/** Pen styles.
 */
enum PenStyles {
  NoPen          = Qt::NoPen,
  SolidLine      = Qt::SolidLine,
  DashLine       = Qt::DashLine,
  DotLine        = Qt::DotLine,
  DashDotLine    = Qt::DashDotLine,
  DashDotDotLine = Qt::DashDotDotLine,
  CustomDashLine = Qt::CustomDashLine,
};

/** List of Pen cap styles.
 */
enum CapStyles {
  FlatCap   = Qt::FlatCap,
  SquareCap = Qt::SquareCap,
  RoundCap  = Qt::RoundCap,
};

/** List of Pen join styles.
 */
enum JoinStyles {
  MiterJoin    = Qt::MiterJoin,
  BevelJoin    = Qt::BevelJoin,
  RoundJoin    = Qt::RoundJoin,
  SvgMiterJoin = Qt::SvgMiterJoin,
};


enum AlignFlags {
  AlignLeft                 = Qt::AlignLeft,
  AlignRight                = Qt::AlignRight,
  AlignHCenter              = Qt::AlignHCenter,
  AlignJustify              = Qt::AlignJustify,
  AlignTop                  = Qt::AlignTop,
  AlignBottom               = Qt::AlignBottom,
  AlignVCenter              = Qt::AlignVCenter,
  AlignCenter               = Qt::AlignCenter,
};

enum TextFlags {
  TextDontClip              = Qt::TextDontClip,
  TextSingleLine            = Qt::TextSingleLine,
  TextExpandTabs            = Qt::TextExpandTabs,
  TextShowMnemonic          = Qt::TextShowMnemonic,
  TextWordWrap              = Qt::TextWordWrap,
  TextIncludeTrailingSpaces = Qt::TextIncludeTrailingSpaces,
};

enum SizePolicy {
  Fixed            = QSizePolicy::Fixed,
  Minimum          = QSizePolicy::Minimum,
  Maximum          = QSizePolicy::Maximum,
  Preferred        = QSizePolicy::Preferred,
  Expanding        = QSizePolicy::Expanding,
  MinimumExpanding = QSizePolicy::MinimumExpanding,
  Ignored          = QSizePolicy::Ignored,
};

// mouse
enum MouseButtons {
  NoButton     = Qt::NoButton,
  LeftButton   = Qt::LeftButton,
  RightButton  = Qt::RightButton,
  MidButton    = Qt::MidButton,
  MiddleButton = Qt::MiddleButton,
  XButton1     = Qt::XButton1,
  XButton2     = Qt::XButton2,
};

enum ClickTypes {
  MousePress   = 1,
  MouseRelease = 2,
  DoubleClick  = 3,
};

enum KeyboardModifiers {
  NoModifier          = Qt::NoModifier,
  ShiftModifier       = Qt::ShiftModifier,
  ControlModifier     = Qt::ControlModifier,
  AltModifier         = Qt::AltModifier,
  MetaModifier        = Qt::MetaModifier,
  KeypadModifier      = Qt::KeypadModifier,
  GroupSwitchModifier = Qt::GroupSwitchModifier,
};

enum Orientation {
  Horizontal = Qt::Horizontal,
  Vertical   = Qt::Vertical,
};

enum Key {
  Key_Escape = Qt::Key_Escape,
  Key_Tab = Qt::Key_Tab,
  Key_Backtab = Qt::Key_Backtab,
  Key_Backspace = Qt::Key_Backspace,
  Key_Return = Qt::Key_Return,
  Key_Enter = Qt::Key_Enter,
  Key_Insert = Qt::Key_Insert,
  Key_Delete = Qt::Key_Delete,
  Key_Pause = Qt::Key_Pause,
  Key_Print = Qt::Key_Print,
  Key_SysReq = Qt::Key_SysReq,
  Key_Clear = Qt::Key_Clear,
  Key_Home = Qt::Key_Home,
  Key_End = Qt::Key_End,
  Key_Left = Qt::Key_Left,
  Key_Up = Qt::Key_Up,
  Key_Right = Qt::Key_Right,
  Key_Down = Qt::Key_Down,
  Key_PageUp = Qt::Key_PageUp,
  Key_PageDown = Qt::Key_PageDown,
  Key_Shift = Qt::Key_Shift,
  Key_Control = Qt::Key_Control,
  Key_Meta = Qt::Key_Meta,
  Key_Alt = Qt::Key_Alt,
  Key_AltGr = Qt::Key_AltGr,
  Key_CapsLock = Qt::Key_CapsLock,
  Key_NumLock = Qt::Key_NumLock,
  Key_ScrollLock = Qt::Key_ScrollLock,
  Key_F1 = Qt::Key_F1,
  Key_F2 = Qt::Key_F2,
  Key_F3 = Qt::Key_F3,
  Key_F4 = Qt::Key_F4,
  Key_F5 = Qt::Key_F5,
  Key_F6 = Qt::Key_F6,
  Key_F7 = Qt::Key_F7,
  Key_F8 = Qt::Key_F8,
  Key_F9 = Qt::Key_F9,
  Key_F10 = Qt::Key_F10,
  Key_F11 = Qt::Key_F11,
  Key_F12 = Qt::Key_F12,
  Key_F13 = Qt::Key_F13,
  Key_F14 = Qt::Key_F14,
  Key_F15 = Qt::Key_F15,
  Key_F16 = Qt::Key_F16,
  Key_F17 = Qt::Key_F17,
  Key_F18 = Qt::Key_F18,
  Key_F19 = Qt::Key_F19,
  Key_F20 = Qt::Key_F20,
  Key_F21 = Qt::Key_F21,
  Key_F22 = Qt::Key_F22,
  Key_F23 = Qt::Key_F23,
  Key_F24 = Qt::Key_F24,
  Key_F25 = Qt::Key_F25,
  Key_F26 = Qt::Key_F26,
  Key_F27 = Qt::Key_F27,
  Key_F28 = Qt::Key_F28,
  Key_F29 = Qt::Key_F29,
  Key_F30 = Qt::Key_F30,
  Key_F31 = Qt::Key_F31,
  Key_F32 = Qt::Key_F32,
  Key_F33 = Qt::Key_F33,
  Key_F34 = Qt::Key_F34,
  Key_F35 = Qt::Key_F35,
  Key_Super_L = Qt::Key_Super_L,
  Key_Super_R = Qt::Key_Super_R,
  Key_Menu = Qt::Key_Menu,
  Key_Hyper_L = Qt::Key_Hyper_L,
  Key_Hyper_R = Qt::Key_Hyper_R,
  Key_Help = Qt::Key_Help,
  Key_Direction_L = Qt::Key_Direction_L,
  Key_Direction_R = Qt::Key_Direction_R,
  Key_Space = Qt::Key_Space,
  Key_Any = Qt::Key_Any,
  Key_Exclam = Qt::Key_Exclam,
  Key_QuoteDbl = Qt::Key_QuoteDbl,
  Key_NumberSign = Qt::Key_NumberSign,
  Key_Dollar = Qt::Key_Dollar,
  Key_Percent = Qt::Key_Percent,
  Key_Ampersand = Qt::Key_Ampersand,
  Key_Apostrophe = Qt::Key_Apostrophe,
  Key_ParenLeft = Qt::Key_ParenLeft,
  Key_ParenRight = Qt::Key_ParenRight,
  Key_Asterisk = Qt::Key_Asterisk,
  Key_Plus = Qt::Key_Plus,
  Key_Comma = Qt::Key_Comma,
  Key_Minus = Qt::Key_Minus,
  Key_Period = Qt::Key_Period,
  Key_Slash = Qt::Key_Slash,
  Key_0 = Qt::Key_0,
  Key_1 = Qt::Key_1,
  Key_2 = Qt::Key_2,
  Key_3 = Qt::Key_3,
  Key_4 = Qt::Key_4,
  Key_5 = Qt::Key_5,
  Key_6 = Qt::Key_6,
  Key_7 = Qt::Key_7,
  Key_8 = Qt::Key_8,
  Key_9 = Qt::Key_9,
  Key_Colon = Qt::Key_Colon,
  Key_Semicolon = Qt::Key_Semicolon,
  Key_Less = Qt::Key_Less,
  Key_Equal = Qt::Key_Equal,
  Key_Greater = Qt::Key_Greater,
  Key_Question = Qt::Key_Question,
  Key_At = Qt::Key_At,
  Key_A = Qt::Key_A,
  Key_B = Qt::Key_B,
  Key_C = Qt::Key_C,
  Key_D = Qt::Key_D,
  Key_E = Qt::Key_E,
  Key_F = Qt::Key_F,
  Key_G = Qt::Key_G,
  Key_H = Qt::Key_H,
  Key_I = Qt::Key_I,
  Key_J = Qt::Key_J,
  Key_K = Qt::Key_K,
  Key_L = Qt::Key_L,
  Key_M = Qt::Key_M,
  Key_N = Qt::Key_N,
  Key_O = Qt::Key_O,
  Key_P = Qt::Key_P,
  Key_Q = Qt::Key_Q,
  Key_R = Qt::Key_R,
  Key_S = Qt::Key_S,
  Key_T = Qt::Key_T,
  Key_U = Qt::Key_U,
  Key_V = Qt::Key_V,
  Key_W = Qt::Key_W,
  Key_X = Qt::Key_X,
  Key_Y = Qt::Key_Y,
  Key_Z = Qt::Key_Z,
  Key_BracketLeft = Qt::Key_BracketLeft,
  Key_Backslash = Qt::Key_Backslash,
  Key_BracketRight = Qt::Key_BracketRight,
  Key_AsciiCircum = Qt::Key_AsciiCircum,
  Key_Underscore = Qt::Key_Underscore,
  Key_QuoteLeft = Qt::Key_QuoteLeft,
  Key_BraceLeft = Qt::Key_BraceLeft,
  Key_Bar = Qt::Key_Bar,
  Key_BraceRight = Qt::Key_BraceRight,
  Key_AsciiTilde = Qt::Key_AsciiTilde,
  Key_nobreakspace = Qt::Key_nobreakspace,
  Key_exclamdown = Qt::Key_exclamdown,
  Key_cent = Qt::Key_cent,
  Key_sterling = Qt::Key_sterling,
  Key_currency = Qt::Key_currency,
  Key_yen = Qt::Key_yen,
  Key_brokenbar = Qt::Key_brokenbar,
  Key_section = Qt::Key_section,
  Key_diaeresis = Qt::Key_diaeresis,
  Key_copyright = Qt::Key_copyright,
  Key_ordfeminine = Qt::Key_ordfeminine,
  Key_guillemotleft = Qt::Key_guillemotleft,
  Key_notsign = Qt::Key_notsign,
  Key_hyphen = Qt::Key_hyphen,
  Key_registered = Qt::Key_registered,
  Key_macron = Qt::Key_macron,
  Key_degree = Qt::Key_degree,
  Key_plusminus = Qt::Key_plusminus,
  Key_twosuperior = Qt::Key_twosuperior,
  Key_threesuperior = Qt::Key_threesuperior,
  Key_acute = Qt::Key_acute,
  Key_mu = Qt::Key_mu,
  Key_paragraph = Qt::Key_paragraph,
  Key_periodcentered = Qt::Key_periodcentered,
  Key_cedilla = Qt::Key_cedilla,
  Key_onesuperior = Qt::Key_onesuperior,
  Key_masculine = Qt::Key_masculine,
  Key_guillemotright = Qt::Key_guillemotright,
  Key_onequarter = Qt::Key_onequarter,
  Key_onehalf = Qt::Key_onehalf,
  Key_threequarters = Qt::Key_threequarters,
  Key_questiondown = Qt::Key_questiondown,
  Key_Agrave = Qt::Key_Agrave,
  Key_Aacute = Qt::Key_Aacute,
  Key_Acircumflex = Qt::Key_Acircumflex,
  Key_Atilde = Qt::Key_Atilde,
  Key_Adiaeresis = Qt::Key_Adiaeresis,
  Key_Aring = Qt::Key_Aring,
  Key_AE = Qt::Key_AE,
  Key_Ccedilla = Qt::Key_Ccedilla,
  Key_Egrave = Qt::Key_Egrave,
  Key_Eacute = Qt::Key_Eacute,
  Key_Ecircumflex = Qt::Key_Ecircumflex,
  Key_Ediaeresis = Qt::Key_Ediaeresis,
  Key_Igrave = Qt::Key_Igrave,
  Key_Iacute = Qt::Key_Iacute,
  Key_Icircumflex = Qt::Key_Icircumflex,
  Key_Idiaeresis = Qt::Key_Idiaeresis,
  Key_ETH = Qt::Key_ETH,
  Key_Ntilde = Qt::Key_Ntilde,
  Key_Ograve = Qt::Key_Ograve,
  Key_Oacute = Qt::Key_Oacute,
  Key_Ocircumflex = Qt::Key_Ocircumflex,
  Key_Otilde = Qt::Key_Otilde,
  Key_Odiaeresis = Qt::Key_Odiaeresis,
  Key_multiply = Qt::Key_multiply,
  Key_Ooblique = Qt::Key_Ooblique,
  Key_Ugrave = Qt::Key_Ugrave,
  Key_Uacute = Qt::Key_Uacute,
  Key_Ucircumflex = Qt::Key_Ucircumflex,
  Key_Udiaeresis = Qt::Key_Udiaeresis,
  Key_Yacute = Qt::Key_Yacute,
  Key_THORN = Qt::Key_THORN,
  Key_ssharp = Qt::Key_ssharp,
  Key_division = Qt::Key_division,
  Key_ydiaeresis = Qt::Key_ydiaeresis,
  Key_Multi_key = Qt::Key_Multi_key,
  Key_Codeinput = Qt::Key_Codeinput,
  Key_SingleCandidate = Qt::Key_SingleCandidate,
  Key_MultipleCandidate = Qt::Key_MultipleCandidate,
  Key_PreviousCandidate = Qt::Key_PreviousCandidate,
  Key_Mode_switch = Qt::Key_Mode_switch,
  Key_Kanji = Qt::Key_Kanji,
  Key_Muhenkan = Qt::Key_Muhenkan,
  Key_Henkan = Qt::Key_Henkan,
  Key_Romaji = Qt::Key_Romaji,
  Key_Hiragana = Qt::Key_Hiragana,
  Key_Katakana = Qt::Key_Katakana,
  Key_Hiragana_Katakana = Qt::Key_Hiragana_Katakana,
  Key_Zenkaku = Qt::Key_Zenkaku,
  Key_Hankaku = Qt::Key_Hankaku,
  Key_Zenkaku_Hankaku = Qt::Key_Zenkaku_Hankaku,
  Key_Touroku = Qt::Key_Touroku,
  Key_Massyo = Qt::Key_Massyo,
  Key_Kana_Lock = Qt::Key_Kana_Lock,
  Key_Kana_Shift = Qt::Key_Kana_Shift,
  Key_Eisu_Shift = Qt::Key_Eisu_Shift,
  Key_Eisu_toggle = Qt::Key_Eisu_toggle,
  Key_Hangul = Qt::Key_Hangul,
  Key_Hangul_Start = Qt::Key_Hangul_Start,
  Key_Hangul_End = Qt::Key_Hangul_End,
  Key_Hangul_Hanja = Qt::Key_Hangul_Hanja,
  Key_Hangul_Jamo = Qt::Key_Hangul_Jamo,
  Key_Hangul_Romaja = Qt::Key_Hangul_Romaja,
  Key_Hangul_Jeonja = Qt::Key_Hangul_Jeonja,
  Key_Hangul_Banja = Qt::Key_Hangul_Banja,
  Key_Hangul_PreHanja = Qt::Key_Hangul_PreHanja,
  Key_Hangul_PostHanja = Qt::Key_Hangul_PostHanja,
  Key_Hangul_Special = Qt::Key_Hangul_Special,
  Key_Dead_Grave = Qt::Key_Dead_Grave,
  Key_Dead_Acute = Qt::Key_Dead_Acute,
  Key_Dead_Circumflex = Qt::Key_Dead_Circumflex,
  Key_Dead_Tilde = Qt::Key_Dead_Tilde,
  Key_Dead_Macron = Qt::Key_Dead_Macron,
  Key_Dead_Breve = Qt::Key_Dead_Breve,
  Key_Dead_Abovedot = Qt::Key_Dead_Abovedot,
  Key_Dead_Diaeresis = Qt::Key_Dead_Diaeresis,
  Key_Dead_Abovering = Qt::Key_Dead_Abovering,
  Key_Dead_Doubleacute = Qt::Key_Dead_Doubleacute,
  Key_Dead_Caron = Qt::Key_Dead_Caron,
  Key_Dead_Cedilla = Qt::Key_Dead_Cedilla,
  Key_Dead_Ogonek = Qt::Key_Dead_Ogonek,
  Key_Dead_Iota = Qt::Key_Dead_Iota,
  Key_Dead_Voiced_Sound = Qt::Key_Dead_Voiced_Sound,
  Key_Dead_Semivoiced_Sound = Qt::Key_Dead_Semivoiced_Sound,
  Key_Dead_Belowdot = Qt::Key_Dead_Belowdot,
  Key_Dead_Hook = Qt::Key_Dead_Hook,
  Key_Dead_Horn = Qt::Key_Dead_Horn,
  Key_Back = Qt::Key_Back,
  Key_Forward = Qt::Key_Forward,
  Key_Stop = Qt::Key_Stop,
  Key_Refresh = Qt::Key_Refresh,
  Key_VolumeDown = Qt::Key_VolumeDown,
  Key_VolumeMute = Qt::Key_VolumeMute,
  Key_VolumeUp = Qt::Key_VolumeUp,
  Key_BassBoost = Qt::Key_BassBoost,
  Key_BassUp = Qt::Key_BassUp,
  Key_BassDown = Qt::Key_BassDown,
  Key_TrebleUp = Qt::Key_TrebleUp,
  Key_TrebleDown = Qt::Key_TrebleDown,
  Key_MediaPlay = Qt::Key_MediaPlay,
  Key_MediaStop = Qt::Key_MediaStop,
  Key_MediaPrevious = Qt::Key_MediaPrevious,
  Key_MediaNext = Qt::Key_MediaNext,
  Key_MediaRecord = Qt::Key_MediaRecord,
  Key_MediaPause = Qt::Key_MediaPause,
  Key_MediaTogglePlayPause = Qt::Key_MediaTogglePlayPause,
  Key_HomePage = Qt::Key_HomePage,
  Key_Favorites = Qt::Key_Favorites,
  Key_Search = Qt::Key_Search,
  Key_Standby = Qt::Key_Standby,
  Key_OpenUrl = Qt::Key_OpenUrl,
  Key_LaunchMail = Qt::Key_LaunchMail,
  Key_LaunchMedia = Qt::Key_LaunchMedia,
  Key_Launch0 = Qt::Key_Launch0,
  Key_Launch1 = Qt::Key_Launch1,
  Key_Launch2 = Qt::Key_Launch2,
  Key_Launch3 = Qt::Key_Launch3,
  Key_Launch4 = Qt::Key_Launch4,
  Key_Launch5 = Qt::Key_Launch5,
  Key_Launch6 = Qt::Key_Launch6,
  Key_Launch7 = Qt::Key_Launch7,
  Key_Launch8 = Qt::Key_Launch8,
  Key_Launch9 = Qt::Key_Launch9,
  Key_LaunchA = Qt::Key_LaunchA,
  Key_LaunchB = Qt::Key_LaunchB,
  Key_LaunchC = Qt::Key_LaunchC,
  Key_LaunchD = Qt::Key_LaunchD,
  Key_LaunchE = Qt::Key_LaunchE,
  Key_LaunchF = Qt::Key_LaunchF,
  Key_LaunchG = Qt::Key_LaunchG,
  Key_LaunchH = Qt::Key_LaunchH,
  Key_MonBrightnessUp = Qt::Key_MonBrightnessUp,
  Key_MonBrightnessDown = Qt::Key_MonBrightnessDown,
  Key_KeyboardLightOnOff = Qt::Key_KeyboardLightOnOff,
  Key_KeyboardBrightnessUp = Qt::Key_KeyboardBrightnessUp,
  Key_KeyboardBrightnessDown = Qt::Key_KeyboardBrightnessDown,
  Key_PowerOff = Qt::Key_PowerOff,
  Key_WakeUp = Qt::Key_WakeUp,
  Key_Eject = Qt::Key_Eject,
  Key_ScreenSaver = Qt::Key_ScreenSaver,
  Key_WWW = Qt::Key_WWW,
  Key_Memo = Qt::Key_Memo,
  Key_LightBulb = Qt::Key_LightBulb,
  Key_Shop = Qt::Key_Shop,
  Key_History = Qt::Key_History,
  Key_AddFavorite = Qt::Key_AddFavorite,
  Key_HotLinks = Qt::Key_HotLinks,
  Key_BrightnessAdjust = Qt::Key_BrightnessAdjust,
  Key_Finance = Qt::Key_Finance,
  Key_Community = Qt::Key_Community,
  Key_AudioRewind = Qt::Key_AudioRewind,
  Key_BackForward = Qt::Key_BackForward,
  Key_ApplicationLeft = Qt::Key_ApplicationLeft,
  Key_ApplicationRight = Qt::Key_ApplicationRight,
  Key_Book = Qt::Key_Book,
  Key_CD = Qt::Key_CD,
  Key_Calculator = Qt::Key_Calculator,
  Key_ToDoList = Qt::Key_ToDoList,
  Key_ClearGrab = Qt::Key_ClearGrab,
  Key_Close = Qt::Key_Close,
  Key_Copy = Qt::Key_Copy,
  Key_Cut = Qt::Key_Cut,
  Key_Display = Qt::Key_Display,
  Key_DOS = Qt::Key_DOS,
  Key_Documents = Qt::Key_Documents,
  Key_Excel = Qt::Key_Excel,
  Key_Explorer = Qt::Key_Explorer,
  Key_Game = Qt::Key_Game,
  Key_Go = Qt::Key_Go,
  Key_iTouch = Qt::Key_iTouch,
  Key_LogOff = Qt::Key_LogOff,
  Key_Market = Qt::Key_Market,
  Key_Meeting = Qt::Key_Meeting,
  Key_MenuKB = Qt::Key_MenuKB,
  Key_MenuPB = Qt::Key_MenuPB,
  Key_MySites = Qt::Key_MySites,
  Key_News = Qt::Key_News,
  Key_OfficeHome = Qt::Key_OfficeHome,
  Key_Option = Qt::Key_Option,
  Key_Paste = Qt::Key_Paste,
  Key_Phone = Qt::Key_Phone,
  Key_Calendar = Qt::Key_Calendar,
  Key_Reply = Qt::Key_Reply,
  Key_Reload = Qt::Key_Reload,
  Key_RotateWindows = Qt::Key_RotateWindows,
  Key_RotationPB = Qt::Key_RotationPB,
  Key_RotationKB = Qt::Key_RotationKB,
  Key_Save = Qt::Key_Save,
  Key_Send = Qt::Key_Send,
  Key_Spell = Qt::Key_Spell,
  Key_SplitScreen = Qt::Key_SplitScreen,
  Key_Support = Qt::Key_Support,
  Key_TaskPane = Qt::Key_TaskPane,
  Key_Terminal = Qt::Key_Terminal,
  Key_Tools = Qt::Key_Tools,
  Key_Travel = Qt::Key_Travel,
  Key_Video = Qt::Key_Video,
  Key_Word = Qt::Key_Word,
  Key_Xfer = Qt::Key_Xfer,
  Key_ZoomIn = Qt::Key_ZoomIn,
  Key_ZoomOut = Qt::Key_ZoomOut,
  Key_Away = Qt::Key_Away,
  Key_Messenger = Qt::Key_Messenger,
  Key_WebCam = Qt::Key_WebCam,
  Key_MailForward = Qt::Key_MailForward,
  Key_Pictures = Qt::Key_Pictures,
  Key_Music = Qt::Key_Music,
  Key_Battery = Qt::Key_Battery,
  Key_Bluetooth = Qt::Key_Bluetooth,
  Key_WLAN = Qt::Key_WLAN,
  Key_UWB = Qt::Key_UWB,
  Key_AudioForward = Qt::Key_AudioForward,
  Key_AudioRepeat = Qt::Key_AudioRepeat,
  Key_AudioRandomPlay = Qt::Key_AudioRandomPlay,
  Key_Subtitle = Qt::Key_Subtitle,
  Key_AudioCycleTrack = Qt::Key_AudioCycleTrack,
  Key_Time = Qt::Key_Time,
  Key_Hibernate = Qt::Key_Hibernate,
  Key_View = Qt::Key_View,
  Key_TopMenu = Qt::Key_TopMenu,
  Key_PowerDown = Qt::Key_PowerDown,
  Key_Suspend = Qt::Key_Suspend,
  Key_ContrastAdjust = Qt::Key_ContrastAdjust,
  Key_MediaLast = Qt::Key_MediaLast,
  Key_unknown = Qt::Key_unknown,
  Key_Call = Qt::Key_Call,
  Key_Camera = Qt::Key_Camera,
  Key_CameraFocus = Qt::Key_CameraFocus,
  Key_Context1 = Qt::Key_Context1,
  Key_Context2 = Qt::Key_Context2,
  Key_Context3 = Qt::Key_Context3,
  Key_Context4 = Qt::Key_Context4,
  Key_Flip = Qt::Key_Flip,
  Key_Hangup = Qt::Key_Hangup,
  Key_No = Qt::Key_No,
  Key_Select = Qt::Key_Select,
  Key_Yes = Qt::Key_Yes,
  Key_ToggleCallHangup = Qt::Key_ToggleCallHangup,
  Key_VoiceDial = Qt::Key_VoiceDial,
  Key_LastNumberRedial = Qt::Key_LastNumberRedial,
  Key_Execute = Qt::Key_Execute,
  Key_Printer = Qt::Key_Printer,
  Key_Play = Qt::Key_Play,
  Key_Sleep = Qt::Key_Sleep,
  Key_Zoom = Qt::Key_Zoom,
  Key_Cancel = Qt::Key_Cancel,
};
} // mimas

#endif // LUBYK_INCLUDE_MIMAS_CONSTANTS_H_