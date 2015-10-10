//
//  CGSTile.h
//  NUIKit
//
//  Created by Robert Widmann on 10/9/15.
//  Copyright © 2015 CodaFi. All rights reserved.
//

#pragma once
#include "CGSSurface.h"

typedef size_t CGSTileID;

/// Returns true if the space ID and connection admit the creation of a new tile.
CG_EXTERN bool CGSSpaceCanCreateTile(CGSConnectionID cid, CGSSpaceID sid) AVAILABLE_MAC_OS_X_VERSION_10_11_AND_LATER;

CG_EXTERN CGError CGSSpaceGetSizeForProposedTile(CGSConnectionID cid, CGSSpaceID sid, CGSize *outSize) AVAILABLE_MAC_OS_X_VERSION_10_11_AND_LATER;

/// Creates a new tile ID in the given space.
CG_EXTERN CGError CGSSpaceCreateTile(CGSConnectionID cid, CGSSpaceID sid, CGSTileID *outTID) AVAILABLE_MAC_OS_X_VERSION_10_11_AND_LATER;

/// Returns an array of CFNumberRefs of CGSSpaceIDs.
CG_EXTERN CFArrayRef CGSSpaceCopyTileSpaces(CGSConnectionID cid, CGSSpaceID sid) AVAILABLE_MAC_OS_X_VERSION_10_11_AND_LATER;

/// Returns the size of the inter-tile spacing between tiles in the given space ID.
CG_EXTERN CGFloat CGSSpaceGetInterTileSpacing(CGSConnectionID cid, CGSSpaceID sid) AVAILABLE_MAC_OS_X_VERSION_10_11_AND_LATER;
/// Sets the size of the inter-tile spacing for the given space ID.
CG_EXTERN CGError CGSSpaceSetInterTileSpacing(CGSConnectionID cid, CGSSpaceID sid, CGFloat spacing) AVAILABLE_MAC_OS_X_VERSION_10_11_AND_LATER;

/// Gets the space ID for the given tile space.
CG_EXTERN CGSSpaceID CGSTileSpaceResizeRecordGetSpaceID(CGSSpaceID sid) AVAILABLE_MAC_OS_X_VERSION_10_11_AND_LATER;
/// Gets the space ID for the parent of the given tile space.
CG_EXTERN CGSSpaceID CGSTileSpaceResizeRecordGetParentSpaceID(CGSSpaceID sid) AVAILABLE_MAC_OS_X_VERSION_10_11_AND_LATER;

/// Returns whether the current tile space is being resized.
CG_EXTERN bool CGSTileSpaceResizeRecordIsLiveResizing(CGSSpaceID sid) AVAILABLE_MAC_OS_X_VERSION_10_11_AND_LATER;

///
CG_EXTERN CGSTileID CGSTileOwnerChangeRecordGetTileID(CGSConnectionID ownerID) AVAILABLE_MAC_OS_X_VERSION_10_11_AND_LATER;
///
CG_EXTERN CGSSpaceID CGSTileOwnerChangeRecordGetManagedSpaceID(CGSConnectionID ownerID) AVAILABLE_MAC_OS_X_VERSION_10_11_AND_LATER;

///
CG_EXTERN CGSTileID CGSTileEvictionRecordGetTileID(CGSConnectionID ownerID) AVAILABLE_MAC_OS_X_VERSION_10_11_AND_LATER;
///
CG_EXTERN CGSSpaceID CGSTileEvictionRecordGetManagedSpaceID(CGSConnectionID ownerID) AVAILABLE_MAC_OS_X_VERSION_10_11_AND_LATER;

///
CG_EXTERN CGSSpaceID CGSTileOwnerChangeRecordGetNewOwner(CGSConnectionID ownerID) AVAILABLE_MAC_OS_X_VERSION_10_11_AND_LATER;
///
CG_EXTERN CGSSpaceID CGSTileOwnerChangeRecordGetOldOwner(CGSConnectionID ownerID) AVAILABLE_MAC_OS_X_VERSION_10_11_AND_LATER;
