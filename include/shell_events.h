#pragma once

#include <windows.h>
#include <tchar.h>
#include "qvcontrol.h"

ULONG RunShellEventsThread(
);
ULONG StopShellEventsThread(
);
ULONG ProcessUpdatedWindows(
	BOOLEAN bUpdateEverything
);


ULONG send_window_create(
	PWATCHED_DC	pWatchedDC
);

void send_pixmap_mfns(
	PWATCHED_DC pWatchedDC
);

void send_window_damage_event(
	HWND window,
	int x,
	int y,
	int width,
	int height
);

ULONG send_window_configure(
	PWATCHED_DC pWatchedDC
);

ULONG send_window_destroy(
	HWND window
);

ULONG send_window_unmap(
	HWND window
);


#define InitializeListHead(ListHead) (\
    (ListHead)->Flink = (ListHead)->Blink = (ListHead))

#define RemoveEntryList(Entry) {\
    PLIST_ENTRY _EX_Blink;\
    PLIST_ENTRY _EX_Flink;\
    _EX_Flink = (Entry)->Flink;\
    _EX_Blink = (Entry)->Blink;\
    _EX_Blink->Flink = _EX_Flink;\
    _EX_Flink->Blink = _EX_Blink;\
    }

#define RemoveHeadList(ListHead) \
    (ListHead)->Flink;\
    {RemoveEntryList((ListHead)->Flink)}

#define IsListEmpty(ListHead) \
    ((ListHead)->Flink == (ListHead))

#define InsertHeadList(ListHead,Entry) {\
    PLIST_ENTRY _EX_Flink;\
    PLIST_ENTRY _EX_ListHead;\
    _EX_ListHead = (ListHead);\
    _EX_Flink = _EX_ListHead->Flink;\
    (Entry)->Flink = _EX_Flink;\
    (Entry)->Blink = _EX_ListHead;\
    _EX_Flink->Blink = (Entry);\
    _EX_ListHead->Flink = (Entry);\
    }

#define InsertTailList(ListHead,Entry) {\
    PLIST_ENTRY _EX_Blink;\
    PLIST_ENTRY _EX_ListHead;\
    _EX_ListHead = (ListHead);\
    _EX_Blink = _EX_ListHead->Blink;\
    (Entry)->Flink = _EX_ListHead;\
    (Entry)->Blink = _EX_Blink;\
    _EX_Blink->Flink = (Entry);\
    _EX_ListHead->Blink = (Entry);\
    }
