#ifndef _NETWORK_BLOCKS_H
#define _NETWORK_BLOCKS_H

#include <bur/plctypes.h>

#include <runtime.h>
//////////////////////////////////////////////////////////////////////
// Network Blocks
//////////////////////////////////////////////////////////////////////
/**
 * <pre>
Configuration Data Block
Block ID	0
Description	Contains a list of information the master needs from each object for system configuration.
Required	Yes
Block Structure Definition
Attribute Number (name)	Byte Offset	Bit Offset
4 (Class ID)	0	0
10 (Group Assignments)	2	0
11 (Instance Assignments)	3	0
7 (Class Type)	4	0
Table 19 1 Configuration Data Block (0)
The recommended class type for a robot is 2 for ?configuration? type.
 * </pre>
 */
#define CONFIGURATION_DATA_BLOCK 0

#endif /* _NETWORK_BLOCKS_H */