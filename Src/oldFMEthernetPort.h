#ifndef _oldFMETHERNETPORT_H
#define _oldFMETHERNETPORT_H

#include <winsock.h>

class oldCEtherPort
{
    public:
        oldCEtherPort(  );
        ~oldCEtherPort(  );		
        int SetAddr( const char *to_ip, int ToPort, 
                     const char *from_ip, int from_por );
        void SendBuf( void *buf, int lng );
        int  GetBuf( void *buf, int lng );
    
    private:
        void Close();

    private:
        struct sockaddr_in  RxAddr;
        struct sockaddr_in  DstAddr;
        bool                WSOpened;   
        int                 SocketFd;
        int                 m_DstPort;
        int                 m_RxPort;
        unsigned            WrongTxLenCnt;
};


#endif
