#ifndef PACKETS_TS_CS_LOGIN_H
#define PACKETS_TS_CS_LOGIN_H

#pragma pack(push, 1)
struct TS_CS_LOGIN : public TS_MESSAGE
{
	char name[19];
	char race;
	static const int packetID = 1;
};
#pragma pack(pop)

#endif // PACKETS_TS_CS_LOGIN_H
