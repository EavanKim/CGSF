#pragma once
// ============================================================================
// SFMsgPackPacket 클래스
// author : pdpdds
// desc : MessagePack 프로토콜을 위한 패킷 클래스
// ============================================================================

#pragma warning( push )
#pragma warning (disable : 4244)
#pragma warning (disable : 4267)
#pragma warning (disable : 4100)
#include <msgpack.hpp>
#pragma warning( pop )

#include "BasePacket.h"

class SFMsgPackPacket : public BasePacket
{
	friend class SFMsgPackProtocol;

public:
	SFMsgPackPacket(USHORT usPacketId);
	virtual ~SFMsgPackPacket();

	SFPacketHeader* GetHeader() { return &m_packetHeader; }
	msgpack::unpacker&	GetData(){ return m_unpacker; }
	virtual void Release() override;

	virtual BasePacket* Clone() override;

protected:

private:

	SFMsgPackPacket();

	// 패킷 헤더: SFPacketHeader(12바이트)
	// 패킷 아이디(2바이트) + 패킷 옵션(4바이트) + CRC(4바이트) + 데이터 사이즈(2바이트)	
	SFPacketHeader m_packetHeader;

	// 패킷 데이터: msgpack::unpacker
	msgpack::unpacker m_unpacker;
};