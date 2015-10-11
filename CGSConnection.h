/*
 * Copyright (C) 2007-2008 Alacatia Labs
 * 
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 * 
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 * 
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 * 
 * Joe Ranieri joe@alacatia.com
 *
 */

//
//  Updated by Robert Widmann.
//  Copyright © 2015 CodaFi. All rights reserved.
//  Released under the MIT license.
//

#ifndef CGS_CONNECTION_INTERNAL_H
#define CGS_CONNECTION_INTERNAL_H

/// The type of connections to the Window Server.
///
/// Every application is given a singular connection ID through which it can receieve and manipulate
/// values, state, notifications, events, etc. in the Window Server.  It
typedef int CGSConnectionID;

typedef void *CGSNotificationData;
typedef void *CGSNotificationArg;
typedef int CGSTransitionID;


#pragma mark - Connection Lifecycle


/// Gets the default connection for this process.
CG_EXTERN CGSConnectionID CGSMainConnectionID(void);

/// Creates a new connection to the Window Server.
CG_EXTERN CGError CGSNewConnection(int unused, CGSConnectionID *outConnection);

/// Releases a CGSConnection and all CGSWindows owned by it.
CG_EXTERN CGError CGSReleaseConnection(CGSConnectionID cid);

/// Gets the default connection for the current thread.
CG_EXTERN CGSConnectionID CGSDefaultConnectionForThread(void);

/// Gets the pid of the process that owns this connection to the Window Server.
CG_EXTERN CGError CGSConnectionGetPID(CGSConnectionID cid, pid_t *outPID);

/// Gets the connection for the given process serial number.
CG_EXTERN CGError CGSGetConnectionIDForPSN(CGSConnectionID cid, const ProcessSerialNumber *psn, CGSConnectionID *outOwnerCID);

/// Returns whether the menu bar exists for the given connection ID.
///
/// For the majority of applications, this function should return true.  But at system updates,
/// initialization, and shutdown, the menu bar will be either initially gone then created or
/// hidden and then destroyed.
CG_EXTERN bool CGSMenuBarExists(CGSConnectionID cid);

/// Closes ALL connections to the Window Server by the current application.
///
/// The application is effectively turned into a Console-based application after the invocation of
/// this method.
CG_EXTERN CGError CGSShutdownServerConnections(void);


#pragma mark - Connection Properties


/// Retrieves the value associated with the given key for the given connection.
///
/// This method is structured so processes can send values through the Window Server to other
/// processes - assuming they know each others connection IDs.  The recommended use case for this
/// function appears to be keeping state around for application-level sub-connections.
CG_EXTERN CGError CGSCopyConnectionProperty(CGSConnectionID cid, CGSConnectionID targetCID, CFStringRef key, CFTypeRef *outValue);

/// Associates a value for the given key on the given connection.
CG_EXTERN CGError CGSSetConnectionProperty(CGSConnectionID cid, CGSConnectionID targetCID, CFStringRef key, CFTypeRef value);


#pragma mark - Connection Updates


/// Disables updates on a connection
///
/// Calls to disable updates nest much like `-beginUpdates`/`-endUpdates`.  the Window Server will
/// forcibly reenable updates after 1 second if you fail to invoke `CGSReenableUpdate`.
CG_EXTERN CGError CGSDisableUpdate(CGSConnectionID cid);

/// Re-enables updates on a connection.
///
/// Calls to enable updates nest much like `-beginUpdates`/`-endUpdates`.
CG_EXTERN CGError CGSReenableUpdate(CGSConnectionID cid);


#pragma mark - System-Level Notification Registration


typedef enum {
	kCGSNotificationDebugOptionsChanged = 200,

	kCGSNotificationMouseMoved = 715,

	kCGSNotificationTrackingRegionEntered = 718,
	kCGSNotificationTrackingRegionExited = 719,

	// 724 - keyboard preferences changed

	// 729, 730 seem to be process deactivated / activated - but only for this process
	// 731 seems to be this process hidden or shown

	//	kCGSNotificationAppUnresponsive = 750,
	//	kCGSNotificationAppResponsive = 751,

	// 761 - hotkey disabled
	// 762 - hotkey enabled (do these two fire twice?)

	// 763 - hotkey begins editing
	// 764 - hotkey ends editing

	// 765, 766 seem to be about the hotkey state (all disabled, etc)

	kCGSNotificationWorkspaceChanged = 1401,

	kCGSNotificationTransitionEnded = 1700,
} CGSNotificationType;

typedef void (*CGSNotifyProcPtr)(CGSNotificationType type, void *data, unsigned int dataLength, void *userData);

/// Registers a function to receive notifications for system-wide events.
CG_EXTERN CGError CGSRegisterNotifyProc(CGSNotifyProcPtr proc, CGSNotificationType type, void *userData);

/// Unregisters a function that was registered to receive notifications for system-wide events.
CG_EXTERN CGError CGSRemoveNotifyProc(CGSNotifyProcPtr proc, CGSNotificationType type, void *userData);


#pragma mark - Application-Level Notification Registration


typedef enum {
	kCGSScreenResolutionChangedEvent		= 100,
	kCGSScreenDisplayParametersChangedEvent	= 101,
	kCGSClientEnterFullscreen				= 106,
	kCGSClientExitFullscreen				= 107,
	kCGSScreenAcceleratorChangedEvent		= 121,
	kCGSNotificationAppUnresponsive			= 750,
	kCGSNotificationAppResponsive			= 751,
	kCGSWorkspaceConfigurationDisabledEvent	= 761,
	kCGSWorkspaceConfigurationEnabledEvent	= 762,
	kCGSWindowDidBecomeUnoccludedEvent		= 912,
	kCGSWindowDidBecomeOccludedEvent		= 913,
	kCGSWindowWasMovedByDockEvent			= 1205,
	kCGSWindowWasResizedByDockEvent			= 1207,
	kCGSWindowDidBecomeManagedByDockEvent	= 1208,
	kCGSWindowTileSpaceBeganLiveResize		= 1312,
	kCGSWindowTileSpaceEndedLiveResize		= 1313,
	kCGSWindowTileSpaceDidResize			= 1314,
	kCGSWorkspaceChangedEvent				= 1401,
} CGSConnectionNotifyEvent;

typedef void (*CGConnectionNotifyProc)(CGSNotificationType type, CGSNotificationData notificationData, size_t dataLength, CGSNotificationArg userParameter, CGSConnectionID);

/// Registers a function to receive notifications for connection-level events.
CG_EXTERN CGError CGSRegisterConnectionNotifyProc(CGSConnectionID cid, CGConnectionNotifyProc function, CGSConnectionNotifyEvent event, void *userData);

/// Unregisters a function that was registered to receive notifications for connection-level events.
CG_EXTERN CGError CGSRemoveConnectionNotifyProc(CGSConnectionID cid, CGConnectionNotifyProc function, CGSConnectionNotifyEvent event, void *userData);


typedef void (*CGSNewConnectionNotificationProc)(CGSConnectionID cid);

/// Registers a function that gets invoked when the application's connection ID is created by the
/// Window Server.
CG_EXTERN CGError CGSRegisterForNewConnectionNotification(CGSNewConnectionNotificationProc proc);

/// Removes a function that was registered to receive notifications for the creation of the
/// application's connection to the Window Server.
CG_EXTERN CGError CGSRemoveNewConnectionNotification(CGSNewConnectionNotificationProc proc);

typedef void (*CGSConnectionDeathNotificationProc)(CGSConnectionID cid);

/// Registers a function that gets invoked when the application's connection ID is destroyed -
/// ideally by the Window Server.
///
/// Connection death is supposed to be a fatal event that is only triggered when the application
/// terminates or when you have explicitly destroyed a sub-connection to the Window Server.
CG_EXTERN CGError CGSRegisterForConnectionDeathNotification(CGSConnectionDeathNotificationProc proc);

/// Removes a function that was registered to receive notifications for the destruction of the
/// application's connection to the Window Server.
CG_EXTERN CGError CGSRemoveConnectionDeathNotification(CGSConnectionDeathNotificationProc proc);


#pragma mark - Miscellaneous Security Holes

/// Sets a "Universal Owner" for the connection ID.  Currently, that owner is Dock.app, which needs
/// control over the window to provide system features like hiding and showing windows, moving them
/// around, etc.
///
/// Because the Universal Owner owns every window under this connection, it can manipulate them
/// all as it sees fit.  If you can beat the dock, you have total control over the process'
/// connection.
CG_EXTERN CGError CGSSetUniversalOwner(CGSConnectionID cid);

/// Assuming you have the connection ID of the current universal owner, or are said universal owner,
/// allows you to specify another connection that has total control over the application's windows.
CG_EXTERN CGError CGSSetOtherUniversalConnection(CGSConnectionID cid, CGSConnectionID otherConnection);

/// Sets the given connection ID as the login window connection ID.  Windows for the application are
/// then brought to the fore when the computer logs off or goes to sleep.
///
/// Why this is still here, I have no idea.  Window Server only accepts one process calling this
/// ever.  If you attempt to invoke this after loginwindow does you will be yelled at and nothing
/// will happen.  If you can manage to beat loginwindow, however, you know what they say:
///
///    When you teach a man to phish...
CG_EXTERN CGError CGSSetLoginwindowConnection(CGSConnectionID cid) AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;

//! The data sent with kCGSNotificationAppUnresponsive and kCGSNotificationAppResponsive.
typedef struct {
#if __BIG_ENDIAN__
	uint16_t majorVersion;
	uint16_t minorVersion;
#else
	uint16_t minorVersion;
	uint16_t majorVersion;
#endif

	//! The length of the entire notification.
	uint32_t length;

	CGSConnectionID cid;
	pid_t pid;
	ProcessSerialNumber psn;
} CGSProcessNotificationData;

//! The data sent with kCGSNotificationDebugOptionsChanged.
typedef struct {
	int newOptions;
	int unknown[2]; // these two seem to be zero
} CGSDebugNotificationData;

//! The data sent with kCGSNotificationTransitionEnded
typedef struct {
	CGSTransitionID transition;
} CGSTransitionNotificationData;

#endif /* CGS_CONNECTION_INTERNAL_H */
