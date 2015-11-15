// InfraTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../FMInfra.h"

int _tmain(int argc, _TCHAR* argv[])
{
	UDPChannel* channel = new UDPChannel(false);
	channel->SetAddress(5050, "172.23.1.134", 6060, "172.23.1.138", "");

	channel->SendBuf("Hello", 5);

	char buf[10];
	channel->RcvBuffer(buf, 10);


	delete (channel);
	return 0;
}

