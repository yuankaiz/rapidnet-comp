/* A RapidNet application. Generated by RapidNet compiler. */

#ifndef PKTFWDSDNPROVCOMPSTATICCHECK_H
#define PKTFWDSDNPROVCOMPSTATICCHECK_H

#include <string>
#include <iostream>
#include "ns3/ptr.h"
#include "ns3/event-id.h"
#include "ns3/ipv4-address.h"
#include "ns3/rapidnet-header.h"
#include "ns3/relation-base.h"
#include "ns3/database.h"
#include "ns3/rapidnet-application-base.h"
#include "ns3/aggregator.h"
#include "ns3/aggwrap.h"

using namespace std;
using namespace ns3;

namespace ns3 {

class Socket;
class Packet;

namespace rapidnet {
namespace pktfwdsdnprovcompstaticcheck {

class PktfwdSdnProvCompStaticCheck : public RapidNetApplicationBase
{
public:
  static const string DEVICE;
  static const string EMATCHINGPACKET;
  static const string EMATCHINGPACKETCOUNT;
  static const string EMATCHINGPACKETTEMP;
  static const string EPACKET;
  static const string EPACKETTEMP;
  static const string EPAKETCOUNT;
  static const string ERECVPACKET;
  static const string ERECVPACKETCOUNT;
  static const string ERECVPACKETTEMP;
  static const string EQUIHASHTABLE;
  static const string FLOWENTRY;
  static const string INITPACKET;
  static const string INITPACKETCOUNT;
  static const string INITPACKETHASH;
  static const string LINK;
  static const string LINKHR;
  static const string MATCHINGPACKET;
  static const string MATCHINGPACKETNONPROV;
  static const string MAXPRIORITY;
  static const string PACKETNONPROV;
  static const string PACKETPROV;
  static const string PROVHASHTABLE;
  static const string RECVAUXPKT;
  static const string RECVPACKET;
  static const string RECVPACKETDECOMP;
  static const string RECVPACKETTEMP;
  static const string RULEEXEC;

  static TypeId GetTypeId (void);

  PktfwdSdnProvCompStaticCheck ();

  virtual ~PktfwdSdnProvCompStaticCheck ();

protected:

  virtual void DoDispose (void);

  virtual void StartApplication (void);

  virtual void StopApplication (void);

  virtual void InitDatabase (void);

  virtual void DemuxRecv (Ptr<Tuple> tuple);

  virtual void Rs10_eca (Ptr<Tuple> packetProv);

  virtual void Rs11_eca (Ptr<Tuple> eMatchingPacketTemp);

  virtual void Rs12_eca (Ptr<Tuple> eMatchingPacketTemp);

  virtual void Rs13_eca (Ptr<Tuple> eMatchingPacketCount);

  virtual void Rs14_eca (Ptr<Tuple> eMatchingPacket);

  virtual void Rs1_eca (Ptr<Tuple> packetNonProv);

  virtual void Rs20_eca (Ptr<Tuple> matchingPacket);

  virtual void Rs2_eca (Ptr<Tuple> matchingPacketNonProv);

  virtual void Rs30_eca (Ptr<Tuple> matchingPacket);

  virtual void Rs31_eca (Ptr<Tuple> ePacketTemp);

  virtual void Rs32_eca (Ptr<Tuple> ePacketTemp);

  virtual void Rs33_eca (Ptr<Tuple> ePaketCount);

  virtual void Rs34_eca (Ptr<Tuple> ePacket);

  virtual void Rs3_eca (Ptr<Tuple> matchingPacketNonProv);

  virtual void Rs40_eca (Ptr<Tuple> matchingPacket);

  virtual void Rs4_eca (Ptr<Tuple> matchingPacketNonProv);

  virtual void Rh100Eca0Ins (Ptr<Tuple> initPacket);

  virtual void Rh101_eca (Ptr<Tuple> initPacketHash);

  virtual void Rh102_eca (Ptr<Tuple> initPacketCount);

  virtual void Rh103_eca (Ptr<Tuple> initPacketCount);

  virtual void Rh104_eca (Ptr<Tuple> initPacketCount);

  virtual void Rh20_eca (Ptr<Tuple> packetProv);

  virtual void Rh21_eca (Ptr<Tuple> eRecvPacketTemp);

  virtual void Rh22_eca (Ptr<Tuple> eRecvPacketTemp);

  virtual void Rs23_eca (Ptr<Tuple> eRecvPacketCount);

  virtual void Rh24_eca (Ptr<Tuple> eRecvPacket);

  virtual void Rh25_eca (Ptr<Tuple> recvPacketTemp);

  virtual void Rh26_eca (Ptr<Tuple> recvPacketDecomp);

  virtual void Rh27_eca (Ptr<Tuple> recvPacketDecomp);

  virtual void Rh2_eca (Ptr<Tuple> packetNonProv);

  virtual void Rh28Eca0Ins (Ptr<Tuple> recvPacket);

  virtual void Rh28Eca0Del (Ptr<Tuple> recvPacket);

};

} // namespace pktfwdsdnprovcompstaticcheck
} // namespace rapidnet
} // namespace ns3

#endif // PKTFWDSDNPROVCOMPSTATICCHECK_H
