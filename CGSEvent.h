//
//  CGSEvent.h
//  CGSInternal
//
//  Created by Robert Widmann on 9/14/13.
//  Copyright (c) 2015 CodaFi. All rights reserved.
//  Released under the MIT license.
//

#pragma once

#include "CGSWindow.h"

typedef unsigned long long      CGSUInt64;
typedef long long               CGSInt64;
typedef unsigned long           CGSUInt32;
typedef long                    CGSInt32;
typedef unsigned short          CGSUInt16;
typedef short                   CGSInt16;
typedef unsigned char           CGSUInt8;
typedef char                    CGSInt8;
typedef float                   CGSFloat32;

typedef CGSUInt32 CGSByteCount;
typedef CGSUInt16 CGSEventRecordVersion;
typedef unsigned long CGSEventType;
typedef CGSUInt64 CGSEventRecordTime;  /* nanosecond timer */
typedef unsigned long CGSEventFlag;
typedef CGSUInt32  CGSError;

typedef struct {
	NXEventData eventData;
	SInt32 _padding[4];
} CGSEventRecordData;

struct _CGSEventRecord {
	CGSEventRecordVersion major;
	CGSEventRecordVersion minor;
	CGSByteCount length;        /* Length of complete event record */
	CGSEventType type;          /* An event type from above */
	CGPoint location;           /* Base coordinates (global), from upper-left */
	CGPoint windowLocation;     /* Coordinates relative to window */
	CGSEventRecordTime time;    /* nanoseconds since startup */
	CGSEventFlag flags;         /* key state flags */
	CGWindowID window;         /* window number of assigned window */
	CGSConnectionID connection; /* connection the event came from */
	CGSEventRecordData data;    /* type-dependent data: 40 bytes */
};

typedef struct _CGSEventRecord CGSEventRecord;

/*! Gets the event record for a given CGEvent.  For Carbon events, use GetEventPlatformEventRecord. */
CG_EXTERN CGError CGEventGetEventRecord(CGEventRef event, CGSEventRecord *outRecord);
