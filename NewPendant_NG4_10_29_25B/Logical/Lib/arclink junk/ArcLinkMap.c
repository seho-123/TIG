/*
 * ringbuffer.c
 *
 *  Created on: Feb 7, 2017
 *      Author: obed
 */
//#include <../../Lib/ringbuffer.h>
#include "ArcLinkMap.h"

	/**
	 * Handle incoming ArcLink messages
	 * 
	 * @param bytes
	 * @return Header representation of Arclink Message
	 */
//	private void handleMessage(byte[] bytes) {
//				////////////////////////////////////////////////////////////////////////////////////////////////////
//				/// Flags
//		boolean isAResponse = false; // Whether message is a request or response
//				////////////////////////////////////////////////////////////////////////////////////////////////////
//				/*
//				 * (1) Dissect arclink header to determine if its an Arclink Request or
//				 * Response
//				 */
//		Header h = new Header();
//				h.decode(bytes); // Dissect the header bytes
//		
//				/*
//				 * (2) Determine if message is a Request or Response, 0 (LSB) If not
//				 * set, then the accompanying message is a request. If set, then the
//				 * message is a response.<br>
//				 */
//		isAResponse = (h.Flags == 2) ? false : true;
//		
//				if (isAResponse) {
//					// processArcLinkResponse(h, bytes);
//				} else {
//					handleArcLinkRequests(h, bytes);
//				}
//}
	/**
	 * Handle incoming ArcLink messages
	 * 
	 * @param bytes
	 * @return Header representation of Arclink Message
	 */
	private void handleMessage(byte[] bytes) {
		Arc_Link_Header_typ *h;
		
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// Flags
		BOOL isAResponse = 0; // Whether message is a request or response
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
		 * (1) Dissect arclink header to determine if its an Arclink Request or
		 * Response
		 */
		//Header h = new Header(); [TODO:Get Header Reference]
		//h.decode(bytes); // Dissect the header bytes [TODO: Decode the header]

		/*
		 * (2) Determine if message is a Request or Response, 0 (LSB) If not
		 * set, then the accompanying message is a request. If set, then the
		 * message is a response.<br>
		 */
		// isAResponse = (h.Flags == 2) ? false : true;
		isAResponse = (h->Flags == 2) ? 0 : 1;
				
		
		
		if (isAResponse) {
			// processArcLinkResponse(h, bytes);
		} else {
			handleArcLinkRequests(h, bytes);
		}
		
}