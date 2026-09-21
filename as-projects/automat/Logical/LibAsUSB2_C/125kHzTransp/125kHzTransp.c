/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer 
 ********************************************************************
 * Program: 125kHzTransp
 * File: 125kHzTransp.c
 * Author: Bernecker + Rainer
 * Created: September 15, 2009
 ********************************************************************
 * Description: Supports 5E9000.29 (125kHz) transponder reader
 ********************************************************************/


/********************************************************************
 * INCLUDES
 ********************************************************************/
#include <bur/plctypes.h>
#ifdef _DEFAULT_INCLUDES
 #include <AsDefault.h>
#endif
#include <string.h>
#include <DVFrame.h>
#include <AsUsb.h>


/********************************************************************
 * CONSTANTS
 ********************************************************************/


/********************************************************************
* INIT UP
 ********************************************************************/
void _INIT kHzTranspINIT( void )
{
	Transponder.usbAttachDetachCount = 0;
    Transponder.usbNodeIx = 0;
}


/********************************************************************
* CYCLIC TASK
 ********************************************************************/
_CYCLIC void kHzTranspCYCLIC(void)
{
	/* check for unplugged used USB device */	
	if ((Transponder.usbNodeList[Transponder.usbNodeIx] > 0)
		&& ((Transponder.step == 6) || (Transponder.step == 7)))
	{
		Transponder.UsbNodeGetFub.enable = 1;
		Transponder.UsbNodeGetFub.nodeId = Transponder.usbNodeList[Transponder.usbNodeIx];
		Transponder.UsbNodeGetFub.pBuffer = (UDINT)&Transponder.usbDevice;
		Transponder.UsbNodeGetFub.bufferSize = sizeof(Transponder.usbDevice);
	
		UsbNodeGet(&Transponder.UsbNodeGetFub);
		
		if (Transponder.UsbNodeGetFub.status == asusbERR_USB_NOTFOUND) 
		{
			Transponder.UsbDisconnected = 1;
			Transponder.usbNodeList[Transponder.usbNodeIx] = 0;
		}
	}
	
	
	switch (Transponder.step)
	{
		
		case 1:
			/* Wait */
			break;
		
		case 2:
			/* get list of connected USB devices */
			Transponder.UsbNodeListGetFub.enable = 1;
			Transponder.UsbNodeListGetFub.pBuffer = (UDINT)&Transponder.usbNodeList;
			Transponder.UsbNodeListGetFub.bufferSize = sizeof(Transponder.usbNodeList);
			Transponder.UsbNodeListGetFub.filterInterfaceClass = 0;
			Transponder.UsbNodeListGetFub.filterInterfaceSubClass = 0;
			
			UsbNodeListGet(&Transponder.UsbNodeListGetFub);
				
			
			if (Transponder.UsbNodeListGetFub.status == ERR_OK)
			{
				Transponder.step = 3;	/* FUB worked correctly => next step */
				Transponder.usbNodeIx = 0;
				Transponder.usbAttachDetachCount = Transponder.UsbNodeListGetFub.attachDetachCount;			
			}
			else if (Transponder.UsbNodeListGetFub.status == ERR_FUB_BUSY)
			{
				Transponder.step = 2;	/* FUB work asynchron => called until status isn't BUSY */ 
			}
			else if (Transponder.UsbNodeListGetFub.status == asusbERR_USB_NOTFOUND)
			{
				Transponder.step = 2;	/* no USB device connected */
			}
			else
			{
				Transponder.step = 11;			/* error occured */
			}
			break;
		
		case 3:		
			/* search for USB device */
			Transponder.UsbNodeGetFub.enable = 1;
			Transponder.UsbNodeGetFub.nodeId = Transponder.usbNodeList[Transponder.usbNodeIx];
			Transponder.UsbNodeGetFub.pBuffer = (UDINT)&Transponder.usbDevice;
			Transponder.UsbNodeGetFub.bufferSize = sizeof(Transponder.usbDevice);
				
			UsbNodeGet(&Transponder.UsbNodeGetFub);
			
			if (Transponder.UsbNodeGetFub.status == ERR_OK )
			{
				/* USB Prolific Transponder ? */
				if (Transponder.usbDevice.vendorId == TRANSPONDER_PROLIFIC_VENDOR_ID
					&& Transponder.usbDevice.productId == TRANSPONDER_PROLIFIC_PRODUCT_ID
					&& Transponder.usbDevice.bcdDevice == TRANSPONDER_PROLIFIC_BCD )
				{
					/* USB Prolific Transponder found */
					strcpy((char*)Transponder.StringDevice, Transponder.usbDevice.ifName);
					Transponder.usbNodeId = Transponder.usbNodeList[Transponder.usbNodeIx];
					Transponder.step = 4;
				}
				else
				{
					Transponder.usbNodeIx++;
					if (Transponder.usbNodeIx >= Transponder.UsbNodeListGetFub.allNodes)
					{						
						Transponder.step = 2;	/* USB Device not found */
					}
				}
			}
			else if (Transponder.UsbNodeGetFub.status == ERR_FUB_BUSY)
			{				
				Transponder.step = 3;		/* FUB work asynchron => called until status isn't BUSY */
			}
			else if (Transponder.UsbNodeGetFub.status == asusbERR_USB_NOTFOUND)
			{				
				Transponder.step = 2;		/* USB Device not found */
			}
			else
			{
				Transponder.step = 11;	
			}
			
			break;
		
		case 4:
			/* initialize open structure */
			Transponder.FrameXOpenFub.enable = 1;
			Transponder.FrameXOpenFub.device = (UDINT) Transponder.StringDevice;
			Transponder.FrameXOpenFub.mode = (UDINT) NULL;
			Transponder.FrameXOpenFub.config = (UDINT) NULL;
			
			FRM_xopen(&Transponder.FrameXOpenFub); /* open an interface */
			
			if (Transponder.FrameXOpenFub.status == ERR_OK)
			{
				Transponder.step = 5;
				Transponder.Ident = Transponder.FrameXOpenFub.ident; /* get ident */
			}
			else
			{
				Transponder.step = 11;
			}
			
			break;
		
		
		case 5:
			/* request a buffer for sending data */
			Transponder.FrameGetBufferFub.enable = 1;
			Transponder.FrameGetBufferFub.ident = Transponder.Ident;
			
			FRM_gbuf(&Transponder.FrameGetBufferFub);		
			
			if (Transponder.FrameGetBufferFub.status == ERR_OK)
			{
				Transponder.step = 6;
				Transponder.pSendBuffer = (USINT*)Transponder.FrameGetBufferFub.buffer; /* get adress OF send buffer */
				Transponder.SendBufferLength = Transponder.FrameGetBufferFub.buflng; /* get length OF send buffer */
			}
			else
			{			
				Transponder.step = 11;		
			}
			break;
		
		
		case 6:
			/* prepare data for sending */
			strcpy((char*)Transponder.WriteData, "tag,0,0,#crc\r\n"); /* read tag */
			memcpy(Transponder.pSendBuffer, Transponder.WriteData, strlen((char*)Transponder.WriteData)); /* copy write data into send buffer */
			
			/* write message for reading data */
			Transponder.FrameWriteFub.enable = 1;
			Transponder.FrameWriteFub.ident = Transponder.Ident;
			Transponder.FrameWriteFub.buffer = (UDINT) Transponder.pSendBuffer;
			Transponder.FrameWriteFub.buflng = strlen((char*)Transponder.WriteData);			
			
			FRM_write(&Transponder.FrameWriteFub); /* write data to interface */
			
			if (Transponder.FrameWriteFub.status == ERR_OK) /* check status */
			{
				Transponder.step = 7;		
			}
			else
			{
				Transponder.step = 11;
			}
			
			if (Transponder.UserStop || Transponder.UsbDisconnected)
			{
				Transponder.UserStop = 0;
				Transponder.UsbDisconnected = 0;
				Transponder.step = 8;
			}			
		break;
		
		case 7:
			/* read the data */
			Transponder.FrameReadFub.enable = 1;
			Transponder.FrameReadFub.ident = Transponder.Ident;
			
			FRM_read(&Transponder.FrameReadFub); /* read data form reader */
			
			Transponder.pReadBuffer = (USINT*) Transponder.FrameReadFub.buffer; /* get adress of read buffer */
			Transponder.ReadBufferLength = Transponder.FrameReadFub.buflng; /* get length of read buffer */
			
			if (Transponder.FrameReadFub.status == ERR_OK) /* check status */
			{
				memset(Transponder.ReadData, 0, sizeof(Transponder.ReadData));
				if (Transponder.ReadBufferLength <= sizeof(Transponder.ReadData))
				{
					memcpy(Transponder.ReadData, Transponder.pReadBuffer, Transponder.ReadBufferLength); /* copy read data into array */
				}
				
Transponder.step = 6;
			
				/* release read buffer */	
				Transponder.FrameReleaseBufferFub.enable = 1;
				Transponder.FrameReleaseBufferFub.ident = Transponder.Ident;
				Transponder.FrameReleaseBufferFub.buffer = (UDINT) Transponder.pReadBuffer;				
			
				FRM_rbuf(&Transponder.FrameReleaseBufferFub); 		
			
				if (Transponder.FrameReleaseBufferFub.status != ERR_OK)
				{
					Transponder.step = 11;
				}
			
			}
			else if (Transponder.FrameReadFub.status == frmERR_NOINPUT)		/* nothing received */
			{
				/* wait for valid data */
			}
			else
			{				
				Transponder.step = 11;
			}
						
			if (Transponder.UserStop || Transponder.UsbDisconnected)
			{
				Transponder.UserStop = 0;
				Transponder.UsbDisconnected = 0;
				Transponder.step = 8;
			}				
					
			break;
		
		
		
		case 8:
		/* shutdown sequence */
		case 9:
			/* release output buffer */
			Transponder.FrameReleaseOutputBufferFub.enable = 1;
			Transponder.FrameReleaseOutputBufferFub.ident = Transponder.Ident;
			Transponder.FrameReleaseOutputBufferFub.buffer = (UDINT)Transponder.pSendBuffer;
			Transponder.FrameReleaseOutputBufferFub.buflng = strlen((char*)&Transponder.WriteData);
				
			FRM_robuf(&Transponder.FrameReleaseOutputBufferFub);
			
			if (Transponder.FrameReleaseOutputBufferFub.status == ERR_OK)
			{
				Transponder.step = 10;	
			}
			else
			{
				Transponder.step = 11;
			}
			break;
			
			
		case 10:
			/* close the frame */
			Transponder.FrameCloseFub.enable = 1;
			Transponder.FrameCloseFub.ident = Transponder.Ident;
			
			FRM_close(&Transponder.FrameCloseFub);
			
			if (Transponder.FrameCloseFub.status == ERR_OK)
			{
				Transponder.step = 1;
			}
			else
			{
				Transponder.step = 11;
			}
			break;
		
		case 11:
			/* error-handling */
			break;
		
	}
}
