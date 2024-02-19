#ifndef _DXSURFACE_DXSWINSERVICES_H_
#define _DXSURFACE_DXSWINSERVICES_H_



//-    0x0400 -> Windows NT 4.0
//-    0x0500 -> Windows 2000
//-    0x0501 -> Windows XP
//-    0x0502 -> Windows Server 2003
//-    0x0600 -> Windows Vista
//-    0x0600 -> Windows Vista
//-    0x0600 -> Windows Server 2008
//-    0x0600 -> Windows Vista
//-    0x0601 -> Windows 7
//-    0x0602 -> Windows 8
//-    0x0603 -> Windows 8.1
//-    0x0A00 -> Windows 10
//-    0x0A00 -> Windows 10
#define _WIN32_WINNT 0x0601
#include <sdkddkver.h>


#ifndef NO_WIN_DISABLING
#  define WIN32_LEAN_AND_MEAN
#  define NOGDICAPMASKS
#  define NOSYSMETRICS
#  define NOMENUS
#  define NOICONS
#  define NOSYSCOMMANDS
#  define NORASTEROPS
#  define OEMRESOURCE
#  define NOATOM
#  define NOCLIPBOARD
#  define NOCOLOR
#  define NOCTLMGR
#  define NODRAWTEXT
#  define NOKERNEL
#  define NONLS
#  define NOMEMMGR
#  define NOMETAFILE
#  define NOOPENFILE
#  define NOSCROLL
#  define NOSERVICE
#  define NOSOUND
#  define NOTEXTMETRIC
#  define NOWH
#  define NOCOMM
#  define NOKANJI
#  define NOHELP
#  define NOPROFILER
#  define NODEFERWINDOWPOS
#  define NOMCX
#  define NORPC
#  define NOPROXYSTUB
#  define NOIMAGE
#  define NOTAPE
#  define STRICT
#  define NOMINMAX
#endif


#include <Windows.h>
#include <d3d11.h>



#endif
