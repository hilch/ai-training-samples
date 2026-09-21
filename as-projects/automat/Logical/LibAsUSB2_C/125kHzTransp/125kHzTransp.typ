(********************************************************************
 * COPYRIGHT -- Bernecker + Rainer 
 ********************************************************************
 * Program: 125kHzTransp
 * File: 125kHzTransp.typ
 * Author: Bernecker + Rainer
 * Created: September 15, 2009
 ********************************************************************
 * Local data types of program 125kHzTransp
 ********************************************************************)

TYPE
	Transponder_typ : 	STRUCT 
		step : Transponder_steps := STEP_GETNODELIST;
		usbNodeIx : USINT;
		usbAttachDetachCount : UDINT;
		usbNodeId : UDINT;
		UsbNodeGetFub : UsbNodeGet;
		UsbNodeListGetFub : UsbNodeListGet;
		Ident : UDINT;
		ReadBufferLength : UINT;
		SendBufferLength : UINT;
		pReadBuffer : REFERENCE TO USINT;
		pSendBuffer : REFERENCE TO USINT;
		StringDevice : STRING[20];
		ReadData : STRING[255];
		WriteData : STRING[255];
		usbNodeList : ARRAY[0..31]OF UDINT;
		usbDevice : usbNode_typ;
		FrameGetBufferFub : FRM_gbuf;
		FrameReadFub : FRM_read;
		FrameReleaseBufferFub : FRM_rbuf;
		FrameReleaseOutputBufferFub : FRM_robuf;
		FrameWriteFub : FRM_write;
		FrameXOpenFub : FRM_xopen;
		FrameCloseFub : FRM_close;
		UserStop : BOOL;
		UsbDisconnected : BOOL;
	END_STRUCT;
END_TYPE
