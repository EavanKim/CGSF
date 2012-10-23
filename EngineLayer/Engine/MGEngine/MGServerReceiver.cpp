#include "MGServerReceiver.h"
#include "INetworkEngine.h"
#include "INetworkEngineCallback.h"

MGServerReceiver::MGServerReceiver(INetworkEngine* pOwner)
	: m_pOwner(pOwner)
{
}


MGServerReceiver::~MGServerReceiver(void)
{
}

void MGServerReceiver::notifyRegisterSocket(ASSOCKDESCEX& sockdesc, SOCKADDR_IN& ip)
{
	printf(" Connected %d\n", sockdesc.assockUid);

	m_pOwner->GetCallback()->OnConnect(sockdesc.assockUid);

	Synchronized es(&m_SessionLock);
	m_SessionMap.insert(std::make_pair(sockdesc.assockUid, sockdesc));
}

void MGServerReceiver::notifyReleaseSocket(ASSOCKDESCEX& sockdesc)
{
	printf("Disconnected %d\n", sockdesc.assockUid);

	m_pOwner->GetCallback()->OnDisconnect(sockdesc.assockUid);

	Synchronized es(&m_SessionLock);
           
	m_SessionMap.erase(sockdesc.assockUid);

	sockdesc.psender->releaseSocketUniqueId(sockdesc.assockUid);
}

void MGServerReceiver::notifyMessage(ASSOCKDESCEX& sockdesc, size_t length, char* data)
{
	if(false == m_pOwner->GetCallback()->OnData(sockdesc.assockUid, data, length))
	{
		Synchronized es(&m_SessionLock);
           
		m_SessionMap.erase(sockdesc.assockUid);

		sockdesc.psender->releaseSocketUniqueId(sockdesc.assockUid);
	}
}

void MGServerReceiver::notifyConnectingResult(INT32 requestID, ASSOCKDESCEX& sockdesc, DWORD error)
{

}

BOOL MGServerReceiver::Send(int Serial, char* pMessage, int BufSize )
{
	Synchronized es(&m_SessionLock);

	SessionMap::iterator iter = m_SessionMap.find(Serial);

	if(iter == m_SessionMap.end())
	{
		return FALSE;
	}

	iter->second.psender->postingSend(iter->second, BufSize, pMessage);

	return TRUE;
}

////////////////////////////////////////////////////////
//����� �����ϴ��� Ȯ���� �� �ʿ䰡 �ִ�...
////////////////////////////////////////////////////////
bool MGServerReceiver::Disconnect(int Serial)
{
	Synchronized es(&m_SessionLock);

	SessionMap::iterator iter = m_SessionMap.find(Serial);

	if(iter == m_SessionMap.end())
	{
		return FALSE;
	}

	iter->second.psender->releaseSocketUniqueId(Serial);
           
	m_SessionMap.erase(Serial);

	return true;

}
