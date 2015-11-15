//---------------------------------------------------------------------------
//#include <vcl.h>
#pragma hdrstop

#include <iostream>
#include "FMUdpChannel.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

UDPChannel::UDPChannel(bool IsMulticast )
{
	m_IsOpened = false;
	m_IsMulticast = IsMulticast;
	wrong_tx_lng_cnt = 0;
}

//---------------------------------------------------------------------------
UDPChannel::~UDPChannel()
{
    Close();
}

//---------------------------------------------------------------------------
int UDPChannel::SetAddress(int to_port, const char *to_multicast_ip,  int from_port, const char *from_ip,  const char *from_multicast_ip )
{

    unsigned long from_ad;			
	unsigned long Mc_tx_Ip = inet_addr(to_multicast_ip); //( "228.1.101.2");
	

	WSADATA wsaData;
	if ( from_ip == NULL )
	{
		from_ad = INADDR_ANY;
	}
	else
	{
		from_ad = inet_addr( from_ip );
	}
    int sock_err = WSAStartup( MAKEWORD(1,1), &wsaData );
    if ( sock_err == -1 )
	{
		printf ("Unable Open MultiCast Ethernet Port\n");
		return -1;
		//throw Exception( "Unable Open Ethernet Port" );
	}    

    m_IsOpened = true;

    m_sockfd = socket( AF_INET, SOCK_DGRAM, 0 );
    if ( m_sockfd == INVALID_SOCKET )
	{
		printf ("Unable Open MultiCast Ethernet Port\n");
		return -1;
        //throw Exception( "Unable Open Ethernet Port" );
	}
	
    to_addr.sin_family = AF_INET;
    to_addr.sin_port = htons( to_port );
    to_addr.sin_addr.S_un.S_addr = Mc_tx_Ip;
    memset( &(to_addr.sin_zero), 0, 8 );

    from_addr.sin_family = AF_INET;
    from_addr.sin_port = htons( from_port );
    from_addr.sin_addr.S_un.S_addr = from_ad;
    memset( &(from_addr.sin_zero), 0, 8 );
    int err = bind( m_sockfd, ( struct sockaddr * ) &from_addr, sizeof( struct sockaddr ) );
    if ( err != 0 )
	{
		printf ("Unable Open MultiCast Ethernet Port\n");
		int d = WSAGetLastError();
		return -1;
		//throw Exception( "Unable Open Ethernet Port" );
	}
        
   if (m_IsMulticast)
   {
     struct ip_mreq mcRequest;
     char ipopt = 5;
	 unsigned long Mc_rx_Ip = inet_addr(from_multicast_ip);//( "228.1.100.3");
     mcRequest.imr_multiaddr.s_addr = Mc_rx_Ip;//htonl(MULTICAST_GROUP_ADDRESS1);
		//Mc_rx_Ip = inet_addr( "228.1.100.3");

	 mcRequest.imr_interface.s_addr = from_ad;//my_ip;
     if (setsockopt(m_sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&mcRequest,sizeof(mcRequest)) < 0)
     {
		 printf ("Unable Open MultiCast Ethernet Port\n");
		 return -1;
        //throw Exception( "Unable Open MultiCust Ethernet Port" );
     }
     if (setsockopt(m_sockfd, IPPROTO_IP, IP_MULTICAST_TTL, &ipopt, sizeof(ipopt)) < 0)
     {
		 printf ("Unable Open MultiCast Ethernet Port\n");
		 return -1;
        //throw Exception( "Unable Open MultiCust Ethernet Port" );
     }
   }

   return 0;
}

//---------------------------------------------------------------------------
void UDPChannel::Close()
{
    if ( m_IsOpened )
    {
        closesocket( m_sockfd );
        WSACleanup();
    }
	m_IsOpened = false;
}




//---------------------------------------------------------------------------
void UDPChannel::SendBuf( void *buf, int lng )
{
    int sent_lng = sendto( m_sockfd, ( const char * ) buf, lng, 0,
                    ( struct sockaddr * ) &to_addr,
                    sizeof( struct sockaddr ) );
    if ( sent_lng != lng )
	{
        int error = WSAGetLastError();
		wrong_tx_lng_cnt++;

	}
}

//---------------------------------------------------------------------------
int UDPChannel::RcvBuffer( void *buf, int lng )
{
    fd_set readfds;
    struct timeval tv;
    int rcv_lng = 0;
    struct sockaddr_in from;
    int fromlng = sizeof( struct sockaddr );

	// DF: time to wait when listen to channel
	// values of 0 <=> polling
	// when tv = null <=> blocking
    tv.tv_sec = 1;
    tv.tv_usec = 0;

    FD_ZERO( &readfds );
    FD_SET( m_sockfd, &readfds );
    //int sock_err = select( m_sockfd + 1, &readfds, NULL, NULL, &tv );
	
	int sock_err = select( m_sockfd + 1, &readfds, NULL, NULL, NULL );		// <- blocking

    if ( sock_err == SOCKET_ERROR )
	{
		int error = WSAGetLastError();
		printf ("Unable Listen to Ethernet Port" );
		return -1;
        //throw Exception( "Unable Listen to Ethernet Port" );
	}

    if ( FD_ISSET( m_sockfd, &readfds ) )
    {
        rcv_lng = recvfrom( m_sockfd, ( char * ) buf, lng, 0,
                    ( struct sockaddr * ) &from, &fromlng );
        char *addr = inet_ntoa( from.sin_addr );
        
// ###Removed by DF at 1/6/15
		//AnsiString fr = addr;
        //fr = fr + ':';
        //fr = fr + ntohs( from.sin_port );
    }
    return rcv_lng;
}
//---------------------------------------------------------------------------

