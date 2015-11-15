#include "FMSocket.h"
#include "oldFMEthernetPort.h"


static oldCEtherPort ComPort;


/* ----------------------------------------------------------- */
/* DstIpAddress - Destination IP address (usually remote host) */
/* RxIpAddress - Receiving IP address (usually local host)     */
/* ----------------------------------------------------------- */

int SocketInit( const char *DstIpAddress, int DstPort, 
     			const char *RxIpAddress, int RxPort )
{

	return ComPort.SetAddr(DstIpAddress, DstPort, RxIpAddress, RxPort);

}

void SocketSendData( void *Data, int DataSize )
{

	ComPort.SendBuf(Data, DataSize);
	return;
}

int SocketGetData( void *Data, int DataSize )
{

	return ComPort.GetBuf(Data, DataSize);
}
