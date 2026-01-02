
#ifndef _ARCLINKUTILITY_C
#define _ARCLINKUTILITY_C



UINT GetUINT(USINT *p){
USINT *s;
UINT m;

	 s = &m;

	*(s+1) = *p;
	*s = *(p+1);

	return m; 
}

UINT GetUDINT(USINT *p){
USINT *s;
UDINT m;

	 s = &m;

	*(s+3) 	= *p;
	*(s+2) 	= *(p+1);
	*(s+1) 	= *(p+2);
	*s 		= *(p+3);
	
	return m; 
}


void swapArcLinkHeaderBytes(Arc_Link_Header_typ *h) {
	swapBytes((USINT *) &h->Flags, sizeof(h->Flags));
	swapBytes((USINT *) &h->Sequence, sizeof(h->Sequence));
	swapBytes((USINT *) &h->LastError, sizeof(h->LastError));
	swapBytes((USINT *) &h->DataLength, sizeof(h->DataLength));
}

void swapArcLinkResponseBytes(Arc_Link_Response_typ *r) {
	swapBytes((USINT *) &r->ObjectCount, sizeof(r->ObjectCount));
	swapBytes((USINT *) &r->Reserved, sizeof(r->Reserved));
	swapBytes((USINT *) &r->Status, sizeof(r->Status));
	swapBytes((USINT *) &r->ObjectAddress, sizeof(r->ObjectAddress));
	swapBytes((USINT *) &r->ResponseSize, sizeof(r->ResponseSize));
	swapBytes((USINT *) &r->Reserved2, sizeof(r->Reserved2));
}

void swapArcLinkRequestBytes(Arc_Link_Request_typ *r) {
	swapBytes((USINT *) &r->TimeoutTime, sizeof(r->TimeoutTime));
	swapBytes((USINT *) &r->Flags, sizeof(r->Flags));
	swapBytes((USINT *) &r->Retries, sizeof(r->Retries));
	swapBytes((USINT *) &r->MessageCounter, sizeof(r->MessageCounter));
	swapBytes((USINT *) &r->AddressingMode, sizeof(r->AddressingMode));
	swapBytes((USINT *) &r->ConnectionID, sizeof(r->ConnectionID));
	swapBytes((USINT *) &r->MaximumObjs, sizeof(r->MaximumObjs));
	swapBytes((USINT *) &r->ServiceID.AttributeNumber,
		sizeof(r->ServiceID.AttributeNumber));
	swapBytes((USINT *) &r->ServiceID.StateNumber,
		sizeof(r->ServiceID.StateNumber));
	swapBytes((USINT *) &r->ServiceID.PropertyNumber,
		sizeof(r->ServiceID.PropertyNumber));
	swapBytes((USINT *) &r->SourceAddress, sizeof(r->SourceAddress));
	swapBytes((USINT *) &r->DestinationAddress, sizeof(r->DestinationAddress));
	swapBytes((USINT *) &r->Service, sizeof(r->Service));
	swapBytes((USINT *) &r->MaxResponseSize, sizeof(r->MaxResponseSize));
	swapBytes((USINT *) &r->RequestSize, sizeof(r->RequestSize));
	swapBytes((USINT *) &r->Reserved, sizeof(r->Reserved));
}

void swapArcLinkEventReportRequestBytes(Event_Report_Request_typ *er){
	swapBytes((USINT *) &er->AttributeIDNumber, sizeof(er->AttributeIDNumber));
	swapBytes((USINT *) &er->SequenceState, sizeof(er->SequenceState));	
	swapBytes((USINT *) &er->PropertyIDNumber, sizeof(er->PropertyIDNumber));	
	swapBytes((USINT *) &er->TriggerOptions, sizeof(er->TriggerOptions));	
	swapBytes((USINT *) &er->Reserved, sizeof(er->Reserved));	
}

/* swapWeldTableSearchRequest */
void swapWeldTableSearchRequest(WeldTableSearchRequest_typ *wtsr){
	swapBytes((USINT *) &wtsr->ModeDescriptor , 4);
	swapBytes((USINT *) &wtsr->ModeDescriptorCare , 4);
	swapBytes((USINT *) &wtsr->StartWeldModeIndex , 2);
	swapBytes((USINT *) &wtsr->MaxNumberMatches , 1);
	swapBytes((USINT *) &wtsr->ProcessType , 1);
	swapBytes((USINT *) &wtsr->WireType , 1);
	swapBytes((USINT *) &wtsr->WireSize , 1);
	swapBytes((USINT *) &wtsr->GasType , 1);
	swapBytes((USINT *) &wtsr->SearchMask , 1);
}


/* swapSetControlsActionRequest */
void swapSetControlsActionRequest(SetControlsActionRequest_typ *scar){
	swapBytes((USINT *) &scar->ParameterMask   , sizeof(scar->ParameterMask) );
	swapBytes((USINT *) &scar->WeldMode   , sizeof(scar->WeldMode));
	swapBytes((USINT *) &scar->WorkPoint  , sizeof(scar->WorkPoint));
	swapBytes((USINT *) &scar->Trim  , sizeof(scar->Trim));
	swapBytes((USINT *) &scar->WaveControl1  , sizeof(scar->WaveControl1));
	swapBytes((USINT *) &scar->WaveControl2  , sizeof(scar->WaveControl2));
	swapBytes((USINT *) &scar->WaveControl3  , sizeof(scar->WaveControl3));
	swapBytes((USINT *) &scar->WaveControl4  , sizeof(scar->WaveControl4));
}

/* SwapBytes acording to length provided */
void swapBytes(USINT* bytes, int length) {
	int i = 0;
	int z = 0;

	USINT *pByte;
	USINT tmp;

	int order = length;
	for (z = 0; z < length; z++) {
		for (i = 0; i < length; i++) {
			if (i + 1 >= order) {
				// Last Byte
				// Don't do anything
			} else {
				pByte = (bytes + i);
				tmp = *pByte;
				*pByte = *(bytes + i + 1);
				*(bytes + i + 1) = tmp;
			}
		}
		order--;
	}
}


#endif

