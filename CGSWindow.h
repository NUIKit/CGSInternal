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
#include "CGSConnection.h"
#include "CGSRegion.h"

typedef CFTypeRef CGSAnimationRef;
typedef CFTypeRef CGSWindowBackdropRef;
typedef struct CGSWarpPoint CGSWarpPoint;

#define kCGSRealMaximumTagSize (sizeof(void *) * 8)

typedef enum {
	kCGSSharingNone,
	kCGSSharingReadOnly,
	kCGSSharingReadWrite
} CGSSharingState;

typedef enum {
	kCGSOrderBelow = -1,
	kCGSOrderOut, /* hides the window */
	kCGSOrderAbove,
	kCGSOrderIn /* shows the window */
} CGSWindowOrderingMode;

typedef enum {
	kCGSBackingNonRetianed,
	kCGSBackingRetained,
	kCGSBackingBuffered,
} CGSBackingType;

typedef enum : int {
	// Lo bits
	
	/// The window appears in the default style of OS X windows.  "Document" is most likely a
	/// historical name.
	kCGSDocumentWindowTagBit						= 1 << 0,
	/// The window appears floating over other windows.  This mask is often combined with other
	/// non-activating bits to enable floating panels.
	kCGSFloatingWindowTagBit						= 1 << 1,
	
	/// Disables the window's badging when it is minimized into its Dock Tile.
	kCGSDoNotShowBadgeInDockTagBit					= 1 << 2,
	
	/// The window will be displayed without a shadow, and will ignore any given shadow parameters.
	kCGSDisableShadowTagBit							= 1 << 3,
	
	/// Causes the window server to resample the window at a higher rate.  While this may lead to an
	/// improvement in the look of the window, it can lead to performance issues.
	kCGSHighQualityResamplingTagBit					= 1 << 4,
	
	/// The window may set the cursor when the application is not active.  Useful for windows that
	/// present controls like editable text fields.
	kCGSSetsCursorInBackgroundTagBit				= 1 << 5,
	
	/// The window continues to operate while a modal run loop has been pushed.
	kCGSWorksWhenModalTagBit						= 1 << 6,
	
	/// The window is anchored to another window.
	kCGSAttachedWindowTagBit						= 1 << 7,

	/// When dragging, the window will ignore any alpha and appear 100% opaque.
	kCGSIgnoreAlphaForDraggingTagBit				= 1 << 8,
	
	/// The window appears transparent to events.  Mouse events will pass through it to the next
	/// eligible responder.  This bit or kCGSOpaqueForEventsTagBit must be exclusively set.
	kCGSIgnoreForEventsTagBit						= 1 << 9,
	/// The window appears opaque to events.  Mouse events will be intercepted by the window when
	/// necessary.  This bit or kCGSIgnoreForEventsTagBit must be exclusively set.
	kCGSOpaqueForEventsTagBit						= 1 << 10,
	
	/// The window appears on all workspaces regardless of where it was created.  This bit is used
	/// for QuickLook panels.
	kCGSOnAllWorkspacesTagBit						= 1 << 11,

	///
	kCGSPointerEventsAvoidCPSTagBit					= 1 << 12,
	
	///
	kCGSKitVisibleTagBit							= 1 << 13,
	
	/// On application deactivation the window disappears from the window list.
	kCGSHideOnDeactivateTagBit						= 1 << 14,
	
	/// When the window appears it will not bring the application to the forefront.
	kCGSAvoidsActivationTagBit						= 1 << 15,
	/// When the window is selected it will not bring the application to the forefront.
	kCGSPreventsActivationTagBit					= 1 << 16,
	
	///
	kCGSIgnoresOptionTagBit							= 1 << 17,
	
	/// The window ignores the window cycling mechanism.
	kCGSIgnoresCycleTagBit							= 1 << 18,
 
	///
	kCGSDefersOrderingTagBit						= 1 << 19,
	
	///
	kCGSDefersActivationTagBit						= 1 << 20,
	
	/// WindowServer will ignore all requests to order this window front.
	kCGSIgnoreAsFrontWindowTagBit					= 1 << 21,
	
	/// The WindowServer will control the movement of the window on the screen using its given
	/// dragging rects.  This enables windows to be movable even when the application stalls.
	kCGSEnableServerSideDragTagBit					= 1 << 22,
	
	///
	kCGSMouseDownEventsGrabbedTagBit				= 1 << 23,
	
	/// The window ignores all requests to hide.
	kCGSDontHideTagBit								= 1 << 24,
	
	///
	kCGSDontDimWindowDisplayTagBit					= 1 << 25,
	
	/// The window converts all pointers, no matter if they are mice or tablet pens, to its pointer
	/// type when they enter the window.
	kCGSInstantMouserWindowTagBit					= 1 << 26,
	
	/// The window appears only on active spaces, and will follow when the user changes said active
	/// space.
	kCGSWindowOwnerFollowsForegroundTagBit			= 1 << 27,
	
	///
	kCGSActivationWindowLevelTagBit					= 1 << 28,
	
	/// The window brings its owning application to the forefront when it is selected.
	kCGSBringOwningApplicationForwardTagBit			= 1 << 29,
	
	/// The window is allowed to appear when over login screen.
	kCGSPermittedBeforeLoginTagBit					= 1 << 30,
	
	/// The window is modal.
	kCGSModalWindowTagBit							= 1 << 31,

	// Hi bits
	
	/// The window draws itself like the dock -the "Magic Mirror".
	kCGSWindowIsMagicMirrorTagBit					= 1 << 1,
	
	///
	kCGSFollowsUserTagBit							= 1 << 2,
	
	///
	kCGSWindowDoesNotCastMirrorReflectionTagBit		= 1 << 3,
	
	///
	kCGSMeshedWindowTagBit							= 1 << 4,
	
	/// Bit is set when CoreDrag has dragged something to the window.
	kCGSCoreDragIsDraggingWindowTagBit				= 1 << 5,
	
	///
	kCGSAvoidsCaptureTagBit							= 1 << 6,
	
	/// The window is ignored for expose and does not change its appearance in any way when it is
	/// activated.
	kCGSIgnoreForExposeTagBit						= 1 << 7,
	
	/// The window is hidden.
	kCGSHiddenTagBit								= 1 << 8,
	
	/// The window is explicitly included in the window cycling mechanism.
	kCGSIncludeInCycleTagBit						= 1 << 9,
	
	/// The window captures gesture events even when the application is not in the foreground.
	kCGSWantGesturesInBackgroundTagBit				= 1 << 10,
	
	/// The window is fullscreen.
	kCGSFullScreenTagBit							= 1 << 11,
	
	///
	kCGSWindowIsMagicZoomTagBit						= 1 << 12,
	
	///
	kCGSSuperStickyTagBit							= 1 << 13,
	
	/// The window is attached to the menu bar.  This is used for NSMenus presented by menu bar
	/// apps.
	kCGSAttachesToMenuBarTagBit						= 1 << 14,
	
	/// The window appears on the menu bar.  This is used for all menu bar items.
	kCGSMergesWithMenuBarTagBit						= 1 << 15,
	
	///
	kCGSNeverStickyTagBit							= 1 << 16,
	
	/// The window appears at the level of the desktop picture.
	kCGSDesktopPictureTagBit						= 1 << 17,
	
	/// When the window is redrawn it moves forward.  Useful for debugging, annoying in practice.
	kCGSOrdersForwardWhenSurfaceFlushedTagBit		= 1 << 18,
	
	/// 
	kCGSDragsMovementGroupParentTagBit				= 1 << 19,
} CGSWindowTagBit;

struct CGSWarpPoint {
	CGPoint localPoint;
	CGPoint globalPoint;
};

/*! Switches to the next (or previous) window in the global list. */
CG_EXTERN CGError CGSCycleWindows(CGSConnectionID cid, CGSWindowOrderingMode order);

/*! Gets and sets the desktop window. Not sure what happens if more than one process sets the desktop window. */
CG_EXTERN CGError CGSDesktopWindow(CGSConnectionID cid, CGWindowID *outWID);
CG_EXTERN CGError CGSSetDesktopWindow(CGSConnectionID cid, CGWindowID wid);

/*! Sets the window's title. Internally this simply calls `CGSSetWindowProperty(cid, wid, kCGSWindowTitle, title)`. */
CG_EXTERN CGError CGSSetWindowTitle(CGSConnectionID cid, CGWindowID wid, CFStringRef title);

/*! Gets and sets a property for a window. */
CG_EXTERN CGError CGSGetWindowProperty(CGSConnectionID cid, CGWindowID wid, CFStringRef key, CFTypeRef *outValue);
CG_EXTERN CGError CGSSetWindowProperty(CGSConnectionID cid, CGWindowID wid, CFStringRef key, CFTypeRef value);

/*! Gets and sets whether a window is opaque. */
CG_EXTERN CGError CGSGetWindowOpacity(CGSConnectionID cid, CGWindowID wid, bool *isOpaque);
CG_EXTERN CGError CGSSetWindowOpacity(CGSConnectionID cid, CGWindowID wid, bool isOpaque);

/*! Gets and sets the window's transparency. */
CG_EXTERN CGError CGSGetWindowAlpha(CGSConnectionID cid, CGWindowID wid, CGFloat *outAlpha);
CG_EXTERN CGError CGSSetWindowAlpha(CGSConnectionID cid, CGWindowID wid, CGFloat alpha);

/*! Gets and sets the window's event mask. */
CG_EXTERN CGError CGSGetWindowEventMask(CGSConnectionID cid, CGWindowID wid, CGEventMask *mask);
CG_EXTERN CGError CGSSetWindowEventMask(CGSConnectionID cid, CGWindowID wid, CGEventMask mask);

/*! Gets and sets the window's clip shape. */
CG_EXTERN CGError CGSCopyWindowClipShape(CGSConnectionID cid, CGWindowID wid, CGSRegionRef *outRegion);
CG_EXTERN CGError CGSSetWindowClipShape(CGWindowID wid, CGSRegionRef shape);

/*! Gets and sets the window's transform. 
 
	Severe restrictions are placed on transformation:
 - Transformation Matrices may only include a singular transform.
 - Transformations involving scale may not scale upwards past the window's frame.
 - Transformations involving rotation must be followed by translation or the window will fall offscreen.
 */
CG_EXTERN CGError CGSGetWindowTransform(CGSConnectionID cid, CGWindowID wid, const CGAffineTransform *outTransform);
CG_EXTERN CGError CGSSetWindowTransform(CGSConnectionID cid, CGWindowID wid, CGAffineTransform transform);

/*! Gets and sets the window's transform in place. 
	
	Severe restrictions are placed on transformation:
 - Transformation Matrices may only include a singular transform.
 - Transformations involving scale may not scale upwards past the window's frame.
 - Transformations involving rotation must be followed by translation or the window will fall offscreen.
 */
CG_EXTERN CGError CGSGetWindowTransformAtPlacement(CGSConnectionID cid, CGWindowID wid, const CGAffineTransform *outTransform);
CG_EXTERN CGError CGSSetWindowTransformAtPlacement(CGSConnectionID cid, CGWindowID wid, CGAffineTransform transform);

/*! Sets the alpha of a group of windows over a period of time. Note that `duration` is in seconds. */
CG_EXTERN CGError CGSSetWindowListAlpha(CGSConnectionID cid, const CGWindowID *widList, int widCount, CGFloat alpha, CGFloat duration);

/*! Gets and sets the `CGConnectionID` that owns this window. Only the owner can change most properties of the window. */
CG_EXTERN CGError CGSGetWindowOwner(CGSConnectionID cid, CGWindowID wid, CGSConnectionID *outOwner);
CG_EXTERN CGError CGSSetWindowOwner(CGSConnectionID cid, CGWindowID wid, CGSConnectionID owner);

/*! Sets the background color of the window. */
CG_EXTERN CGError CGSSetWindowAutofillColor(CGSConnectionID cid, CGWindowID wid, CGFloat red, CGFloat green, CGFloat blue);

/*! Sets the warp for the window. The mesh maps a local (window) point to a point on screen. */
CG_EXTERN CGError CGSSetWindowWarp(CGSConnectionID cid, CGWindowID wid, int warpWidth, int warpHeight, const CGSWarpPoint *warp);

/*! Gets or sets whether the window server should auto-fill the window's background. */
CG_EXTERN CGError CGSGetWindowAutofill(CGSConnectionID cid, CGWindowID wid, bool *outShouldAutoFill);
CG_EXTERN CGError CGSSetWindowAutofill(CGSConnectionID cid, CGWindowID wid, bool shouldAutoFill);

/*! Sets whether a window can recieve mouse events.  If no, events will pass to the next window that can receive the event. */
CG_EXTERN CGError CGSSetMouseEventEnableFlags(CGSConnectionID cid, CGWindowID wid, bool shouldEnable);

/*! Forces a window to acquire key window status. */
CG_EXTERN CGError CGSSetMouseFocusWindow(CGSConnectionID cid, CGWindowID wid);

/*! Gets the screen rect for a window. */
CG_EXTERN CGError CGSGetScreenRectForWindow(CGSConnectionID cid, CGWindowID wid, CGRect *outRect);

/*! Gets and sets the window level for a window. */
CG_EXTERN CGError CGSGetWindowLevel(CGSConnectionID cid, CGWindowID wid, CGWindowLevel *outLevel);
CG_EXTERN CGError CGSSetWindowLevel(CGSConnectionID cid, CGWindowID wid, CGWindowLevel level);

/*! Gets and sets the sharing state. This determines the level of access other applications have over this window. */
CG_EXTERN CGError CGSGetWindowSharingState(CGSConnectionID cid, CGWindowID wid, CGSSharingState *outState);
CG_EXTERN CGError CGSSetWindowSharingState(CGSConnectionID cid, CGWindowID wid, CGSSharingState state);

/*! Sets whether this window is ignored in the global window cycle (Control-F4 by default). There is no Get version? */
CG_EXTERN CGError CGSSetIgnoresCycle(CGSConnectionID cid, CGWindowID wid, bool ignoresCycle);

CG_EXTERN CGError CGSSetWindowHasKeyAppearance(CGSConnectionID cid, CGWindowID wid, bool hasKeyAppearance);

/*! Creates a graphics context for the window. 
 
 Acceptable keys options:
 
 - CGWindowContextShouldUseCA : CFBooleanRef
 */
CG_EXTERN CGContextRef CGWindowContextCreate(CGSConnectionID cid, CGWindowID wid, CFDictionaryRef options);

/*! Sets the order of a window */
CG_EXTERN CGError CGSOrderWindow(CGSConnectionID cid, CGWindowID wid, CGSWindowOrderingMode mode, CGWindowID relativeToWID);

CG_EXTERN CGError CGSOrderFrontConditionally(CGSConnectionID cid, CGWindowID wid, bool force);

/*! Sets the origin (top-left) of a window */
CG_EXTERN CGError CGSMoveWindow(CGSConnectionID cid, CGWindowID wid, const CGPoint *origin);

/*! Sets the origin (top-left) of a window relative to another window's origin. */
CG_EXTERN CGError CGSSetWindowOriginRelativeToWindow(CGSConnectionID cid, CGWindowID wid, CGWindowID relativeToWID, CGFloat offsetX, CGFloat offsetY);

/*! Sets the frame and position of a window.  Updates are grouped for the sake of animation. */
CG_EXTERN CGError CGSMoveWindowWithGroup(CGSConnectionID cid, CGWindowID wid, CGRect *newFrame);

/* Flushes a window's buffer to the screen. */
CG_EXTERN CGError CGSFlushWindow(CGSConnectionID cid, CGWindowID wid, CGSRegionRef flushRegion);

/* Gets the mouse's current location inside the bounds rectangle of the window. */
CG_EXTERN CGError CGSGetWindowMouseLocation(CGSConnectionID cid, CGWindowID wid, CGPoint *outPos);

#pragma mark shadows

/*! Gets and sets the shadow information for a window. Values for `flags` are unknown. */
CG_EXTERN CGError CGSSetWindowShadowAndRimParameters(CGSConnectionID cid, CGWindowID wid, CGFloat standardDeviation, CGFloat density, int offsetX, int offsetY, int flags);
CG_EXTERN CGError CGSGetWindowShadowAndRimParameters(CGSConnectionID cid, CGWindowID wid, CGFloat *outStandardDeviation, CGFloat *outDensity, int *outOffsetX, int *outOffsetY, int *outFlags);

/*! Sets the shadow information for a window. Simply calls through to `CGSSetWindowShadowAndRimParameters` passing 1 for `flags`. */
CG_EXTERN CGError CGSSetWindowShadowParameters(CGSConnectionID cid, CGWindowID wid, CGFloat standardDeviation, CGFloat density, int offsetX, int offsetY);

/*! Invalidates a window's shadow. */
CG_EXTERN CGError CGSInvalidateWindowShadow(CGSConnectionID cid, CGWindowID wid);

/*!  
 Sets a window's shadow properties.
 
 Acceptable keys:
 
 - com.apple.WindowShadowDensity		- (0.0 - 1.0) Opacity of the window's shadow.
 - com.apple.WindowShadowRadius			- The radius of the shadow around the window's corners.
 - com.apple.WindowShadowVerticalOffset	- Vertical offset of the shadow.
 - com.apple.WindowShadowRimDensity		- (0.0 - 1.0) Opacity of the black rim around the window.
 - com.apple.WindowShadowRimStyleHard	- Sets a hard black rim around the window.
 */
CG_EXTERN CGError CGSWindowSetShadowProperties(CGWindowID wid, CFDictionaryRef properties);

#pragma mark window lists

/*! Gets the number of windows the `targetCID` owns. */
CG_EXTERN CGError CGSGetWindowCount(CGSConnectionID cid, CGSConnectionID targetCID, int *outCount);

/*! Gets a list of windows owned by `targetCID`. */
CG_EXTERN CGError CGSGetWindowList(CGSConnectionID cid, CGSConnectionID targetCID, int count, CGWindowID *list, int *outCount);

/*! Gets the number of windows owned by `targetCID` that are on screen. */
CG_EXTERN CGError CGSGetOnScreenWindowCount(CGSConnectionID cid, CGSConnectionID targetCID, int *outCount);

/*! Gets a list of windows oned by `targetCID` that are on screen. */
CG_EXTERN CGError CGSGetOnScreenWindowList(CGSConnectionID cid, CGSConnectionID targetCID, int count, CGWindowID *list, int *outCount);


#pragma mark window management

/*! Creates a new CGSWindow. The real window top/left is the sum of the region's top/left and the top/left parameters. */
CG_EXTERN CGError CGSNewWindow(CGSConnectionID cid, CGSBackingType backingType, CGFloat left, CGFloat top, CGSRegionRef region, CGWindowID *outWID);

/*! Creates a new CGSWindow. The real window top/left is the sum of the region's top/left and the top/left parameters. */
CG_EXTERN CGError CGSNewWindowWithOpaqueShape(CGSConnectionID cid, CGSBackingType backingType, CGFloat left, CGFloat top, CGSRegionRef region, CGSRegionRef opaqueShape, int unknown, CGSWindowTagBit *tags, int tagSize, CGWindowID *outWID);

/*! Releases a CGSWindow. */
CG_EXTERN CGError CGSReleaseWindow(CGSConnectionID cid, CGWindowID wid);

/*! DEPRECATED: Sets the shape over which the window can capture events in its frame rectangle. */
CG_EXTERN CGError CGSSetWindowEventShape(CGSConnectionID cid, CGSBackingType backingType, CGSRegionRef *shape);

/*! Sets the shape over which the window can capture events in its frame rectangle. */
CG_EXTERN CGError CGSAddActivationRegion(CGSConnectionID cid, CGWindowID wid, CGSRegionRef region);

/*! Sets the shape over which the window can recieve mouse drag events. */
CG_EXTERN CGError CGSAddDragRegion(CGSConnectionID cid, CGWindowID wid, CGSRegionRef region);

/*! Removes any shapes over which the window can be dragged. */
CG_EXTERN CGError CGSClearDragRegion(CGSConnectionID cid, CGWindowID wid);

CG_EXTERN CGError CGSDragWindowRelativeToMouse(CGSConnectionID cid, CGWindowID wid, CGPoint point);

#pragma mark animations

/*! Creates a Dock-style genie animation that goes from `wid` to `destinationWID`. */
CG_EXTERN CGError CGSCreateGenieWindowAnimation(CGSConnectionID cid, CGWindowID wid, CGWindowID destinationWID, CGSAnimationRef *outAnimation);

/*! Creates a sheet animation that's used when the parent window is brushed metal. Oddly enough, seems to be the only one used, even if the parent window isn't metal. */
CG_EXTERN CGError CGSCreateMetalSheetWindowAnimationWithParent(CGSConnectionID cid, CGWindowID wid, CGWindowID parentWID, CGSAnimationRef *outAnimation);

/*! Sets the progress of an animation. */
CG_EXTERN CGError CGSSetWindowAnimationProgress(CGSAnimationRef animation, CGFloat progress);

/*! DOCUMENTATION PENDING */
CG_EXTERN CGError CGSWindowAnimationChangeLevel(CGSAnimationRef animation, CGWindowLevel level);

/*! DOCUMENTATION PENDING */
CG_EXTERN CGError CGSWindowAnimationSetParent(CGSAnimationRef animation, CGWindowID parent) AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;

/*! Releases a window animation. */
CG_EXTERN CGError CGSReleaseWindowAnimation(CGSAnimationRef animation);


#pragma mark window accelleration

/*! Gets the state of accelleration for the window. */
CG_EXTERN CGError CGSWindowIsAccelerated(CGSConnectionID cid, CGWindowID wid, bool *outIsAccelerated);

/*! Gets and sets if this window can be accellerated. I don't know if playing with this is safe. */
CG_EXTERN CGError CGSWindowCanAccelerate(CGSConnectionID cid, CGWindowID wid, bool *outCanAccelerate);
CG_EXTERN CGError CGSWindowSetCanAccelerate(CGSConnectionID cid, CGWindowID wid, bool canAccelerate);


#pragma mark system status items

/*! Registers or unregisters a window as a global status item (see `NSStatusItem`, `NSMenuExtra`). Once a window is registered, the window server takes care of placing it in the apropriate location. */
CG_EXTERN CGError CGSSystemStatusBarRegisterWindow(CGSConnectionID cid, CGWindowID wid, int priority);
CG_EXTERN CGError CGSUnregisterWindowWithSystemStatusBar(CGSConnectionID cid, CGWindowID wid);

/*! Rearranges items in the system status bar. You should call this after registering or unregistering a status item or changing the window's width. */
CG_EXTERN CGError CGSAdjustSystemStatusBarWindows(CGSConnectionID cid);

#pragma mark window tags

/*! Get the given tags for a window.  Pass kCGSRealMaximumTagSize to maxTagSize.
 
    Tags are represented server-side as 64-bit integers, but CoreGraphics maintains compatibility
    with 32-bit clients by requiring 2 32-bit options tags to be specified.  The first entry in the
    options array populates the lower 32 bits, the last populates the upper 32 bits.
 */
CG_EXTERN CGError CGSGetWindowTags(CGSConnectionID cid, CGWindowID wid, const CGSWindowTagBit tags[2], size_t maxTagSize);

/*! Set the given tags for a window.  Pass kCGSRealMaximumTagSize to maxTagSize.
 
    Tags are represented server-side as 64-bit integers, but CoreGraphics maintains compatibility
    with 32-bit clients by requiring 2 32-bit options tags to be specified.  The first entry in the
    options array populates the lower 32 bits, the last populates the upper 32 bits.
 */
CG_EXTERN CGError CGSSetWindowTags(CGSConnectionID cid, CGWindowID wid, const CGSWindowTagBit tags[2], size_t maxTagSize);

/*! Clear the given tags for a window.  Pass kCGSRealMaximumTagSize to maxTagSize. 
 
    Tags are represented server-side as 64-bit integers, but CoreGraphics maintains compatibility
    with 32-bit clients by requiring 2 32-bit options tags to be specified.  The first entry in the
    options array populates the lower 32 bits, the last populates the upper 32 bits.
 */
CG_EXTERN CGError CGSClearWindowTags(CGSConnectionID cid, CGWindowID wid, CGSWindowTagBit *tags, size_t maxTagSize);

#pragma mark window backdrop

/*! Creates a new window backdrop with a given material and frame.  The window server will apply the backdrop's material effect to the window using the default CGConnection. */
CG_EXTERN CGSWindowBackdropRef CGSWindowBackdropCreateWithLevel(CGWindowID wid, CFStringRef materialName, CGWindowLevel level, CGRect frame) AVAILABLE_MAC_OS_X_VERSION_10_10_AND_LATER;
CG_EXTERN void CGSWindowBackdropRelease(CGSWindowBackdropRef backdrop) AVAILABLE_MAC_OS_X_VERSION_10_10_AND_LATER;

/*! Activates the backdrop's effect.  OS X currently only makes the key window's backdrop active. */
CG_EXTERN void CGSWindowBackdropActivate(CGSWindowBackdropRef backdrop) AVAILABLE_MAC_OS_X_VERSION_10_10_AND_LATER;
CG_EXTERN void CGSWindowBackdropDeactivate(CGSWindowBackdropRef backdrop) AVAILABLE_MAC_OS_X_VERSION_10_10_AND_LATER;

/*! Sets the saturation of the backdrop.  For certain material types this can imitate the "vibrancy" effect in AppKit. */
CG_EXTERN void CGSWindowBackdropSetSaturation(CGSWindowBackdropRef backdrop, CGFloat saturation) AVAILABLE_MAC_OS_X_VERSION_10_10_AND_LATER;

/*! Sets the bleed for the window's backdrop effect.  Vibrant NSWindows use ~0.2. */
CG_EXTERN void CGSWindowSetBackdropBackgroundBleed(CGWindowID wid, CGFloat bleedAmount) AVAILABLE_MAC_OS_X_VERSION_10_10_AND_LATER;
