/*
 * ringbuffer.c
 *
 *  Created on: Feb 7, 2017
 *      Author: obed
 */


static void *br_malloc(UDINT size);
static void br_free(void* ptr);

USINT* encodeArcLinkRequest(Arc_Link_Header_typ *h, Arc_Link_Request_typ *r)
{
/*
	// At this point Arclink Request is ready to be sent based on prority
	// Create a Request Message and header
	Robot robot = Robot.getInstance();

	// Create a UDP Request Header with all the required info
	Request r = new Request();
	r.TimeoutTime = 10; // 10 ms
	r.Flags = 0;
	r.Retries = 1;
	r.MessageCounter = 0;
	r.AddressingMode = 6;
	r.ConnectionID = 15;
	r.MaximumObjs = 1;
	r.ServiceID.AttributeNumber = NetworkActions.CREATE_EVENT_REPORT_ACTION;
	r.ServiceID.StateNumber = 0;
	r.ServiceID.PropertyNumber = Attribute.PROPERTY_ID_VALUE;

	r.SourceAddress = robot.arclink.network.ModuleBaseAddress;
	r.DestinationAddress = WeldController.ARCLINK_NETWORK_ADDRESS;// Weld
																	// Controller
	r.Service = Request.NETWORK_SERVICE_ACTION;
	r.MaxResponseSize = 254;

	try {
		r.OutgoingData = er.encode();
		r.RequestSize = r.OutgoingData.length;
	} catch (IOException e1) {
		// TODO Auto-generated catch block
		e1.printStackTrace();
	}

	Random rand = new Random();
	Header h = new Header();
	h.Flags = 0b010;
	h.Sequence = rand.nextInt(0xFFFF);
	h.LastError = 0;
	h.DataLength = 18 + r.RequestSize; // 18 is the normal request size

	byte[] bytes = null;
	ByteArrayOutputStream baos = new ByteArrayOutputStream();
	try {
		baos.write(h.encode());
		baos.write(r.encode());
		bytes = baos.toByteArray(); // Create message
	} catch (IOException e) {
		// TODO Auto-generated catch block
		e.printStackTrace();
	}

	sendUDPMessage(udpCfg, bytes);
*/	
}

USINT* encodeHeader(Arc_Link_Header_typ *h)
{
	
	UDINT h_size = sizeof(Arc_Link_Header_typ); // header size
	
}

static void *br_malloc(UDINT size)
{
UDINT msize;
UDINT *mptr;
UINT mstatus;
/*
-always allocate multiple of UDINT size bytes
-reserve first 4 bytes for the size
*/


	msize = ((size / sizeof(UDINT)) + 2) * sizeof(UDINT);

	mstatus = TMP_alloc(msize, (void **)&mptr);
	if(mstatus){
		return 0;
	}
	
	*mptr = msize; //store the msize as first 4 bytes
	
	return ((void*)(mptr) + sizeof(UDINT));
}

static void br_free(void* ptr)
{
UDINT msize;
UDINT *mptr;
UINT mstatus;

	mptr = (UDINT*)(ptr - sizeof(UDINT));
	msize = *mptr;
	
	mstatus = TMP_free(msize, (void **)mptr);
}

