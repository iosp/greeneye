#include "oldFMEthernetPort.h"

oldCEtherPort::oldCEtherPort( ) :
    WSOpened(false),
    WrongTxLenCnt(0)
{
}

/* ----------------------------------------------------------- */
/* DstIpAddress - Destination IP address (usually remote host) */
/* RxIpAddress - Receiving IP address (usually local host)     */
/* ----------------------------------------------------------- */
int oldCEtherPort::SetAddr( const char *DstIpAddress, int a_DstPort, 
                         const char *RxIpAddress,  int a_RxPort )
{
    unsigned long RxAddress;
	int ReturnCode; 
    
    m_DstPort = a_DstPort; 	
    m_RxPort  = a_RxPort;   

	WSADATA wsaData;
	if ( RxIpAddress == NULL )
	{
		RxAddress = INADDR_ANY;
	}
	else
	{
		RxAddress = inet_addr( RxIpAddress );
	}
    
    // Initialization of the Winsock DLL
    int SocketError = WSAStartup( MAKEWORD(1,1), &wsaData );

    if ( SocketError == -1 )
    {
        return 	WSAGetLastError ();
    }


    WSOpened = true;

    // Creating a socket that is bound to a specific transport service provider
    SocketFd = socket( AF_INET, SOCK_DGRAM, 0 );

    if ( SocketFd == INVALID_SOCKET )
    {
        return 	WSAGetLastError ();
    }


    DstAddr.sin_family = AF_INET;
    DstAddr.sin_port = htons( m_DstPort );
    DstAddr.sin_addr.S_un.S_addr = inet_addr( DstIpAddress );
    memset( &(DstAddr.sin_zero), 0, 8 );

    RxAddr.sin_family = AF_INET;
    RxAddr.sin_port = htons( m_RxPort );
    RxAddr.sin_addr.S_un.S_addr = RxAddress;
    memset( &(RxAddr.sin_zero), 0, 8 );

    // Associating a local address with a socket
    ReturnCode = bind( SocketFd, ( struct sockaddr * ) &RxAddr, sizeof( struct sockaddr ) );
    
    if ( ReturnCode != 0 )
    {
        return 	WSAGetLastError ();
    }
    
     return 0;
}

//---------------------------------------------------------------------------
void oldCEtherPort::Close()
{
    if ( WSOpened )
    {
        closesocket( SocketFd );
        WSACleanup();
    }
}

//---------------------------------------------------------------------------
oldCEtherPort::~oldCEtherPort()
{
    Close();
}

//---------------------------------------------------------------------------
void oldCEtherPort::SendBuf( void *buf, int lng )
{
    int SentLen;
	
	SentLen= sendto( SocketFd, ( const char * ) buf, lng, 0,
                     ( struct sockaddr * ) &DstAddr,
                      sizeof( struct sockaddr ) );

    if ( SentLen != lng )
    {
        WrongTxLenCnt++;
    }
}

//---------------------------------------------------------------------------
int oldCEtherPort::GetBuf( void *buf, int lng )
{
    fd_set readfds;
    struct timeval tv;
    int rcv_lng = 0;
    struct sockaddr_in from;
    int fromlng = sizeof( struct sockaddr );

    tv.tv_sec = 0;
    tv.tv_usec = 0;

    FD_ZERO( &readfds );
    FD_SET( SocketFd, &readfds );
    int SocketError = select( SocketFd + 1, &readfds, NULL, NULL, &tv );
    /*EBH
    if ( SocketError == SOCKET_ERROR )
        throw Exception( "Unable Listen to Ethernet Port" );
        */

    if ( FD_ISSET( SocketFd, &readfds ) )
    {
        rcv_lng = recvfrom( SocketFd, ( char * ) buf, lng, 0,
							( struct sockaddr * ) &from, &fromlng );
        char *addr = inet_ntoa( from.sin_addr );

    }
    return rcv_lng;
}
//---------------------------------------------------------------------------

