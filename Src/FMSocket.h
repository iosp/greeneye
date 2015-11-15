#ifndef _FMSOCKET_H
#define _FMSOCKET_H


/* ----------------------------------------------------------- */
/* DstIpAddress - Destination IP address (usually remote host) */
/* RxIpAddress - Receiving IP address (usually local host)     */
/* ----------------------------------------------------------- */

int SocketInit( const char *DstIpAddress, int DstPort, const char *RxIpAddress, int RxPort );

void SocketSendData( void *Data, int DataSize );

int SocketGetData( void *Data, int DataSize );

#endif
