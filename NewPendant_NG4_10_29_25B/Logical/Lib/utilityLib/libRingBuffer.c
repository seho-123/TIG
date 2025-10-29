#ifndef RINGBUFFERLIB_C_
#define RINGBUFFERLIB_C_

#include <bur/plctypes.h>
#ifdef _DEFAULT_INCLUDES
 #include <AsDefault.h>
#endif
 



 int RingBuffer_Init(RingBuffer *buffer , char *buf , UDINT RecNum , UDINT RecLen){

 	if(!buffer)
		return 0 ;

	memset(buffer , 0 , sizeof(RingBuffer));

	if(RecNum && RecLen && buf  ){

		buffer->buffer = buf;
		buffer->recNum = RecNum;
		
		buffer->length = RecNum * (RecLen+4);

		buffer->recordLength = RecLen;

		buffer->cnt = 0;
		return  1;

	}	
	else
		return -1;

	
 }

UDINT RingBuffer_readRecord(RingBuffer *buffer, char *target ){
UDINT len = 0;
UDINT *pHead;

	if(!buffer || !target)
		return 0;

	if(RingBuffer_isEmpty(buffer) )
		return 0;


	pHead = (UDINT)buffer->buffer + (UDINT)buffer->start;
	len =  *pHead;

	void *result = memcpy(target, (UDINT)pHead +4, len);
	
	if (result == 0) {
		
		return 0;
	}

	buffer->cnt --;

	buffer->start = (buffer->start + buffer->recordLength +4) % (buffer->length );

	if (buffer->end == buffer->start) 
		buffer->start = buffer->end = 0;

	return len;
	
}
int RingBuffer_writeRecord(RingBuffer *buffer, char *data, UDINT length){
UDINT *pHead;

	if(!buffer || !data || !length)
		return 0;

	if(RingBuffer_isFull(buffer))
		return 0;

	pHead = (UDINT)buffer->buffer + (UDINT)buffer->end;
	*pHead = length;	

	void *result = memcpy((UDINT)pHead +4, data, length);
	
	if (result == 0) {
		
		return -1;
	}

	buffer->cnt ++;

	buffer->end = (buffer->end + buffer->recordLength +4) % (buffer->length );

	return 1;

return 1;
}

plcbit RingBuffer_isEmpty(RingBuffer *buffer ){		

	return (buffer->cnt <= 0) ;
}

plcbit RingBuffer_isFull(RingBuffer *buffer ){
	

	return (buffer->cnt >= buffer->recNum);
}

#endif

