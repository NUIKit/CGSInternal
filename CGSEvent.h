//
//  CGSEvent.h
//  CGSInternal
//
//  Created by Robert Widmann on 9/14/13.
//  Copyright (c) 2015 CodaFi. All rights reserved.
//  Released under the MIT license.
//

#ifndef CGS_EVENT_INTERNAL_H
#define CGS_EVENT_INTERNAL_H

#include "CGSWindow.h"

typedef unsigned long CGSByteCount;
typedef unsigned short CGSEventRecordVersion;
typedef unsigned long long CGSEventRecordTime;  /* nanosecond timer */
typedef unsigned long CGSEventFlag;
typedef unsigned long  CGSError;

typedef enum : unsigned int {
	kCGSEventNotificationNullEvent = 0,

	kCGSEventNotificationLeftMouseDown,
	kCGSEventNotificationLeftMouseUp,
	kCGSEventNotificationRightMouseDown,
	kCGSEventNotificationRightMouseUp,
	kCGSEventNotificationMouseMoved,
	kCGSEventNotificationLeftMouseDragged,
	kCGSEventNotificationRightMouseDragged,

	kCGSEventNotificationMouseEntered,
	kCGSEventNotificationMouseExited,

	kCGSEventNotificationKeyDown,
	kCGSEventNotificationKeyUp,

	kCGSEventNotificationFlagsChanged,

	kCGSEventNotificationKitDefined,
	kCGSEventNotificationSystemDefined,
	kCGSEventNotificationApplicationDefined,

	kCGSEventNotificationTimer,

	kCGSEventNotificationCursorUpdate,

	kCGSEventNotificationSuspend,
	kCGSEventNotificationResume,

	kCGSEventNotificationNotification,

	kCGSEventNotificationScrollWheel,

	kCGSEventNotificationTabletPointer,
	kCGSEventNotificationTabletProximity,

	kCGSEventNotificationOtherMouseDown,
	kCGSEventNotificationOtherMouseUp,
	kCGSEventNotificationOtherMouseDragged,

	kCGSEventNotificationZoom,

	kCGSEventNotificationAppIsUnresponsive,
	kCGSEventNotificationAppIsNoLongerUnresponsive,

	kCGSEventSecureTextInputIsActive,
	kCGSEventSecureTextInputIsOff,

	kCGSEventNotificationSymbolicHotKeyChanged,
	kCGSEventNotificationSymbolicHotKeyDisabled,
	kCGSEventNotificationSymbolicHotKeyEnabled,
	kCGSEventNotificationHotKeysGloballyDisabled,
	kCGSEventNotificationHotKeysGloballyEnabled,
	kCGSEventNotificationHotKeysExceptUniversalAccessGloballyDisabled,
	kCGSEventNotificationHotKeysExceptUniversalAccessGloballyEnabled,

	kCGSWindowIsObscured,
	kCGSWindowIsUnobscured,
	kCGSWindowIsOrderedIn,
	kCGSWindowIsOrderedOut,
	kCGSWindowIsTerminated,
	kCGSWindowIsChangingScreens,
	kCGSWindowDidMove,
	kCGSWindowDidResize,
	kCGSWindowDidChangeOrder,
	kCGSWindowGeometryDidChange,
	kCGSWindowMonitorDataPending,
	kCGSWindowDidCreate,
	kCGSWindowRightsGrantOffered,
	kCGSWindowRightsGrantCompleted,
	kCGSWindowRecordForTermination,
	kCGSWindowIsVisible,
	kCGSWindowIsInvisible,

	kCGSPackagesWorkspacesDisabled,
	kCGSPackagesWorkspacesEnabled,
	kCGSPackagesStatusBarSpaceChanged,

	kCGSessionConsoleConnect,
	kCGSessionConsoleDisconnect,
	kCGSessionRemoteConnect,
	kCGSessionRemoteDisconnect,
	kCGSessionLoggedOn,
	kCGSessionLoggedOff,
	kCGSessionConsoleWillDisconnect,

	kCGSTransitionDidFinish,

	kCGSConnectionWindowsBecameVisible,
	kCGSConnectionWindowsBecameOccluded,
	kCGSConnectionWindowModificationsStarted,
	kCGSConnectionWindowModificationsStopped,

	kCGSLikelyUnbalancedDisableUpdateNotification,

	kCGSWindowBecameVisible,
	kCGSWindowBecameOccluded,

	kCGSWorkspaceWillChange,
	kCGSWorkspaceDidChange,

	kCGSWorkspaceWindowIsViewable,
	kCGSWorkspaceWindowIsNotViewable,

	kCGSWorkspaceWindowDidMove,

	kCGSWorkspacePrefsDidChange,

	kCGSWorkspacesWindowDidOrderOutInOtherSpace,
	kCGSWorkspacesWindowDidOrderInOtherSpace,
	kCGSWorkspacesWindowDidChangeOrder,

	kCGSWorkspacesWindowDragDidStart,
	kCGSWorkspacesWindowDragDidEnd,

	kCGSWorkspacesWindowDidOrderInUserSpace,

	kCGSWorkspacesShowSpaceForProcess,

	kCGSWorkspacesWindowDidOrderInOnNonCurrentManagedSpacesOnly,
	kCGSWorkspacesWindowDidOrderOutOnNonCurrentManagedSpaces,
} CGSEventType;

typedef struct {
	NXEventData eventData;
	SInt32 _padding[4];
} CGSEventRecordData;

typedef struct _CGSEventRecord {
	CGSEventRecordVersion major; /*0x0*/
	CGSEventRecordVersion minor; /*0x2*/
	CGSByteCount length;         /*0x4*/ /* Length of complete event record */
	CGSEventType type;           /*0x8*/ /* An event type from above */
	CGPoint location;            /*0x10*/ /* Base coordinates (global), from upper-left */
	CGPoint windowLocation;      /*0x20*/ /* Coordinates relative to window */
	CGSEventRecordTime time;     /*0x30*/ /* nanoseconds since startup */
	CGSEventFlag flags;         /* key state flags */
	CGWindowID window;         /* window number of assigned window */
	CGSConnectionID connection; /* connection the event came from */
	struct __CGEventSourceData {
		int source;
		unsigned int sourceUID;
		unsigned int sourceGID;
		unsigned int flags;
		unsigned long long userData;
		unsigned int sourceState;
		unsigned short localEventSuppressionInterval;
		unsigned char suppressionIntervalFlags;
		unsigned char remoteMouseDragFlags;
		unsigned long long serviceID;
	} eventSource;
	struct _CGEventProcess {
		int pid;
		unsigned int psnHi;
		unsigned int psnLo;
		unsigned int targetID;
		unsigned int flags;
	} eventProcess;
	CGSEventRecordData data;    /* type-dependent data: 40 bytes */
	void *ioEventData;
	/*
	 unsigned short windowHeight;
	 unsigned short mainDisplayHeight;
	 unsigned short *unicodePayload;
	 unsigned int eventOwner;
	 unsigned char passedThrough;
	 */
	unsigned int _field15[4];
	unsigned short _field16;
	unsigned short _field17;
	unsigned short *_field18;
} CGSEventRecord;

/// Gets the event record for a given `CGEventRef`.
///
/// For Carbon events, use `GetEventPlatformEventRecord`.
CG_EXTERN CGError CGEventGetEventRecord(CGEventRef event, CGSEventRecord *outRecord, size_t recSize);

/// Gets the main event port for the connection ID.
CG_EXTERN OSErr CGSGetEventPort(CGSConnectionID identifier, mach_port_t *port);

/// Getter and setter for the background event mask.
CG_EXTERN void CGSGetBackgroundEventMask(CGSConnectionID cid, int *outMask);
CG_EXTERN CGError CGSSetBackgroundEventMask(CGSConnectionID cid, int mask);

#endif CGS_EVENT_INTERNAL_H /* CGS_EVENT_INTERNAL_H */
