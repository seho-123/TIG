#ifndef RINGBUFFERLIB_H_
#define RINGBUFFERLIB_H_


#include <bur/plctypes.h>
#ifdef _DEFAULT_INCLUDES
 #include <AsDefault.h>
#endif

#ifndef _IEC_CONST
#define _IEC_CONST _WEAK const
#endif



typedef struct{
	char *buffer;
	int recordLength;
	int recNum;
	int length;
	int start;
	int end;
	int cnt;
} RingBuffer;

int RingBuffer_Init(RingBuffer *buffer , char *buf , UDINT RecNum , UDINT RecLen);

UDINT RingBuffer_readRecord(RingBuffer *buffer, char *target );
int RingBuffer_writeRecord(RingBuffer *buffer, char *data, UDINT length);

plcbit RingBuffer_isEmpty(RingBuffer *buffer );

plcbit RingBuffer_isFull(RingBuffer *buffer );







#endif
