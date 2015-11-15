//---------------------------------------------------------------------------
#ifndef FMUDPCHANNEL
#define FMUDPCHANNEL

#include <winsock.h>
//---------------------------------------------------------------------------

   
class UDPChannel
{    
public:
        UDPChannel(bool IsMulticast);
        ~UDPChannel();


        int SetAddress (int to_port, const char *to_multicast_ip,  int from_port, const char *from_ip,  const char *from_multicast_ip );        
        void SendBuf( void *buf, int lng );
        int RcvBuffer( void *buf, int lng );
        void SetMulticast(bool IsMulticast) {m_IsMulticast = IsMulticast;};

		bool IsOpend(){return m_IsOpened;};
 
private:
        bool	m_IsMulticast;
		bool    m_IsOpened;
        int     m_sockfd;
        int     m_to_port;
        
        struct	sockaddr_in to_addr;
        int     m_from_port;
        struct	sockaddr_in from_addr;
        unsigned wrong_tx_lng_cnt;
        
        char	my_ip_addr[20];
        unsigned long my_ip;

		 void    Close();

};


#endif
