//
//  CGSSurface.h
//	CGSInternal
//
//  Created by Robert Widmann on 9/14/13.
//  Copyright (c) 2015 CodaFi. All rights reserved.
//  Released under the MIT license.
//

#pragma once
#include "Compatability.h"
#include "CGSWindow.h"

CG_EXTERN_C_BEGIN

/*! Adds a drawable surface to a window. */
CG_EXTERN CGError CGSAddSurface(CGSConnectionID cid, CGWindowID wid, CGSSurfaceID *outSID);

/*! Removes a drawable surface from a window. */
CG_EXTERN CGError CGSRemoveSurface(CGSConnectionID cid, CGWindowID wid, CGSSurfaceID sid);

/*! Sets the bounds of a surface. */
CG_EXTERN CGError CGSSetSurfaceBounds(CGSConnectionID cid, CGWindowID wid, CGSSurfaceID sid, CGFloat xOrg, CGFloat yOrg, CGFloat width, CGFloat height);

/*! Gets the smallest rectangle a surface's frame fits in. */
CG_EXTERN CGError CGSGetSurfaceBounds(CGSConnectionID cid, CGWindowID wid, CGSSurfaceID sid, CGFloat *bounds);

/*! Gets the count of all drawable surfaces on a window. */
CG_EXTERN CGError CGSGetSurfaceCount(CGSConnectionID cid, CGWindowID wid, int *outCount);

/*! Gets a list of surfaces owned by a window. */
CG_EXTERN CGError CGSGetSurfaceList(CGSConnectionID cid, CGWindowID wid, int countIds, CGSSurfaceID *ids, int *outCount);

/*! Sets the order of a surface relative to another surface. */
CG_EXTERN CGError CGSOrderSurface(CGSConnectionID cid, CGWindowID wid, CGSSurfaceID surface, CGSSurfaceID otherSurface, int place);

/*! Flushes a surface to its window. */
CG_EXTERN CGError CGSFlushSurface(CGSConnectionID cid, CGWindowID wid, CGSSurfaceID, int param);

CG_EXTERN_C_END


