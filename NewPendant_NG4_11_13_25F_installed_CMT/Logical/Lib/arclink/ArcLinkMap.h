#ifndef _ARC_LINK_MAP_H
#define _ARC_LINK_MAP_H

#include <bur/plctypes.h>

#include <runtime.h>
#include "ArcLink.h"

void handleMessage(Arc_Link_typ *h);	// private void handleMessage(byte[] bytes) {

/** Handle incoming ArcLink Requests Messages */
void handleArcLinkRequests(Arc_Link_Header_typ *h, char *bytes);//private void handleArcLinkRequests(Header h, byte[] bytes)


typedef struct Arc_Link_Map_typ
{
	USINT state; // Current State
	

} Arc_Link_Map_typ;

#endif /* _ARC_LINK_MAP_H */
