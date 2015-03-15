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

typedef enum : NSInteger {
	CGSCursorArrow			= 0,
	CGSCursorIBeam			= 1,
	CGSCursorIBeamXOR		= 2,
	CGSCursorAlias			= 3,
	CGSCursorCopy			= 4,
	CGSCursorMove			= 5,
	CGSCursorArrowContext	= 6,
	CGSCursorWait			= 7,
	CGSCursorEmpty			= 8,
} CGSCursorID;

/*! Registers a cursor with the system with a given set of images and hotspot.  Images must be an array of CGImageRefs. */
CG_EXTERN CGError CGSRegisterCursorWithImages(CGSConnectionID cid, const char *cursorName, bool setGlobally, bool instantly, NSUInteger frameCount, CFArrayRef imageArray, CGSize cursorSize, CGPoint hotspot, int *seed, CGRect bounds, CGFloat frameDuration, NSInteger repeatCount);

/*! Sets a cursor registered with a particular name. */
CG_EXTERN CGError CGSSetRegisteredCursor(CGSConnectionID cid, const char *cursorName, int *cursorSeed);

CG_EXTERN CGError CGSCopyRegisteredCursorImages(CGSConnectionID cid, const char *cursorName, CGSize *imageSize, CGPoint *hotSpot, NSUInteger *frameCount, CGFloat *frameDuration, CFArrayRef *imageArray);
CG_EXTERN CGError CGSGetRegisteredCursorDataSize(CGSConnectionID cid, const char *cursorName, size_t *outDataSize);

/*! Sets one of the cursors known to the window server. */
CG_EXTERN CGError CGSSetSystemDefinedCursor(CGSConnectionID cid, CGSCursorID cursor);
CG_EXTERN void CGSSetSystemDefinedCursorWithSeed(CGSConnectionID connection, CGSCursorID systemCursor, int *cursorSeed);

/*! Shows the cursor. */
CG_EXTERN CGError CGSShowCursor(CGSConnectionID cid);

/*! Hides the cursor. */
CG_EXTERN CGError CGSHideCursor(CGSConnectionID cid);

/*! Hides the cursor until the mouse is moved. */
CG_EXTERN CGError CGSObscureCursor(CGSConnectionID cid);
CG_EXTERN CGError CGSRevealCursor(CGSConnectionID cid);

/*! Gets the cursor location. */
CG_EXTERN CGError CGSGetCurrentCursorLocation(CGSConnectionID cid, CGPoint *outPos);

/*! Gets the name (in reverse DNS form) of a system cursor. */
CG_EXTERN char *CGSCursorNameForSystemCursor(CGSCursorID cursor);

/*! Gets the size of the data for the connection's cursor. */
CG_EXTERN CGError CGSGetCursorDataSize(CGSConnectionID cid, size_t *outDataSize);

/*! Gets the data for the connection's cursor. */
CG_EXTERN CGError CGSGetCursorData(CGSConnectionID cid, void *outData);

/*! Gets the size of the data for the current cursor. */
CG_EXTERN CGError CGSGetGlobalCursorDataSize(CGSConnectionID cid, size_t *outDataSize);

/*! Gets the data for the current cursor. */
CG_EXTERN CGError CGSGetGlobalCursorData(CGSConnectionID cid, void *outData, int *outDataSize, int *outRowBytes, CGRect *outRect, CGPoint *outHotSpot, int *outDepth, int *outComponents, int *outBitsPerComponent);

/*! Gets the size of data for a system-defined cursor. */
CG_EXTERN CGError CGSGetSystemDefinedCursorDataSize(CGSConnectionID cid, CGSCursorID cursor, size_t *outDataSize);

/*! Gets the data for a system-defined cursor. */
CG_EXTERN CGError CGSGetSystemDefinedCursorData(CGSConnectionID cid, CGSCursorID cursor, void *outData, int *outRowBytes, CGRect *outRect, CGPoint *outHotSpot, int *outDepth, int *outComponents, int *outBitsPerComponent);

CG_EXTERN CGError CGSSetCursorScale(CGSConnectionID cid, CGFloat scale);
CG_EXTERN CGFloat CGSGetCursorScale(CGSConnectionID cid);

/*! Gets the cursor 'seed'. Every time the cursor is updated, the seed changes. */
CG_EXTERN int CGSCurrentCursorSeed(void);

/*! Shows or hides the spinning beachball of death. */
CG_EXTERN CGError CGSForceWaitCursorActive(CGSConnectionID cid, bool showWaitCursor);
