
#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

#include <..\..\Lib\utilityLib\bgdebug.h>
#include <..\..\Lib\ringbuffer\ringbuffer.h>
#include <..\..\Lib\ringbuffer\ringbuffer.c>

#include <..\..\Lib\arclink\arclink.h>
#include <..\..\Lib\arclink\arclink.c>

_LOCAL Arc_Link_typ *arcLink;
_LOCAL UDP_typ UDP;

void _INIT ProgramCyclic(void)
{UDP_typ *udp;

	udp = &UDP;
	
	udp->send.iBufferSize = RingBuffer_readRecord(arcLink->bufOUT, udp->send.iBuffer);
	
	Udp_cyclic(&UDP);
	DBG("Dupa");
	arcLink = 0;
}
