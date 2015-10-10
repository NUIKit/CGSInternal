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


#pragma once

typedef int CGSConnectionID;
static const CGSConnectionID kCGSNullConnectionID = 0;
typedef void * CGSNotificationData;
typedef void * CGSNotificationArg;
typedef int CGSTransitionID;

/*! DOCUMENTATION PENDING - verify this is Leopard only! */
CG_EXTERN CGError CGSSetLoginwindowConnection(CGSConnectionID cid) AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;
CG_EXTERN CGError CGSSetLoginwindowConnectionWithOptions(CGSConnectionID cid, CFDictionaryRef options) AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;

/*! Enables or disables updates on a connection. The WindowServer will forcibly reenable updates after 1 second. */
CG_EXTERN CGError CGSDisableUpdate(CGSConnectionID cid);
CG_EXTERN CGError CGSReenableUpdate(CGSConnectionID cid);

/*! Is there a menubar associated with this connection? */
CG_EXTERN bool CGSMenuBarExists(CGSConnectionID cid);


#pragma mark notifications

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


typedef void (*CGSNotifyProcPtr)(CGSNotificationType type, void *data, unsigned int dataLength, void *userData);

//! Registers a function to receive notifications.
CG_EXTERN CGError CGSRegisterNotifyProc(CGSNotifyProcPtr proc, CGSNotificationType type, void *userData);

//! Unregisters a function that was registered to receive notifications.
CG_EXTERN CGError CGSRemoveNotifyProc(CGSNotifyProcPtr proc, CGSNotificationType type, void *userData);

typedef void (*CGConnectionNotifyProc)(CGSNotificationType type, CGSNotificationData notificationData, size_t dataLength, CGSNotificationArg userParameter, CGSConnectionID);

CG_EXTERN CGError CGSRegisterConnectionNotifyProc(CGSConnectionID cid, CGConnectionNotifyProc function, CGSConnectionNotifyEvent event, void *userData);
CG_EXTERN CGError CGSRemoveConnectionNotifyProc(CGSConnectionID cid, CGConnectionNotifyProc function, CGSConnectionNotifyEvent event, void *userData);

/*! Registers or removes a function to get notified when a connection is created. Only gets notified for connections created in the current application. */
typedef void (*CGSNewConnectionNotificationProc)(CGSConnectionID cid);
CG_EXTERN CGError CGSRegisterForNewConnectionNotification(CGSNewConnectionNotificationProc proc);
CG_EXTERN CGError CGSRemoveNewConnectionNotification(CGSNewConnectionNotificationProc proc);

/*! Registers or removes a function to get notified when a connection is released. Only gets notified for connections created in the current application. */
typedef void (*CGSConnectionDeathNotificationProc)(CGSConnectionID cid);
CG_EXTERN CGError CGSRegisterForConnectionDeathNotification(CGSConnectionDeathNotificationProc proc);
CG_EXTERN CGError CGSRemoveConnectionDeathNotification(CGSConnectionDeathNotificationProc proc);

/*! Creates a new connection to the window server. */
CG_EXTERN CGError CGSNewConnection(int unused, CGSConnectionID *outConnection);

/*! Releases a CGSConnection and all CGSWindows owned by it. */
CG_EXTERN CGError CGSReleaseConnection(CGSConnectionID cid);

/*! Gets the default connection for this process. `CGSMainConnectionID` is just a more modern name. */
CG_EXTERN CGSConnectionID _CGSDefaultConnection(void);
CG_EXTERN CGSConnectionID CGSMainConnectionID(void);

/*! Gets the default connection for the current thread. */
CG_EXTERN CGSConnectionID CGSDefaultConnectionForThread(void);

/* Gets the `pid` that owns this CGSConnection. */ 
CG_EXTERN CGError CGSConnectionGetPID(CGSConnectionID cid, pid_t *outPID);

/*! Gets the CGSConnection for the PSN. */
CG_EXTERN CGError CGSGetConnectionIDForPSN(CGSConnectionID cid, const ProcessSerialNumber *psn, CGSConnectionID *outOwnerCID);

/*! Gets and sets a connection's property. */
CG_EXTERN CGError CGSGetConnectionProperty(CGSConnectionID cid, CGSConnectionID targetCID, CFStringRef key, CFTypeRef *outValue);
CG_EXTERN CGError CGSSetConnectionProperty(CGSConnectionID cid, CGSConnectionID targetCID, CFStringRef key, CFTypeRef value);

/*! Closes ALL connections used by the current application. Essentially, it turns it into a console application. */
CG_EXTERN CGError CGSShutdownServerConnections(void);

/*! Only the owner of a window can manipulate it. So, Apple has the concept of a universal owner that owns all windows and can manipulate them all. There can only be one universal owner at a time (the Dock). */
CG_EXTERN CGError CGSSetUniversalOwner(CGSConnectionID cid);

/*! Sets a connection to be a universal owner. This call requires `cid` be a universal connection. */
CG_EXTERN CGError CGSSetOtherUniversalConnection(CGSConnectionID cid, CGSConnectionID otherConnection);
