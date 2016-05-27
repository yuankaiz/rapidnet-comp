/* A RapidNet application. Generated by RapidNet compiler. */

#ifndef PKTFWDSDN_H
#define PKTFWDSDN_H

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
namespace pktfwdsdn {

class PktfwdSdn : public RapidNetApplicationBase
{
public:
  static const string DEVICE;
  static const string FLOWENTRY;
  static const string INITPACKET;
  static const string LINK;
  static const string LINKHR;
  static const string MATCHINGPACKET;
  static const string MAXPRIORITY;
  static const string PACKET;
  static const string RECVPACKET;

  static TypeId GetTypeId (void);

  PktfwdSdn ();

  virtual ~PktfwdSdn ();

protected:

  virtual void DoDispose (void);

  virtual void StartApplication (void);

  virtual void StopApplication (void);

  virtual void InitDatabase (void);

  virtual void DemuxRecv (Ptr<Tuple> tuple);

  virtual void Rs1_eca (Ptr<Tuple> packet);

  virtual void Rs2_eca (Ptr<Tuple> matchingPacket);

  virtual void Rs3_eca (Ptr<Tuple> matchingPacket);

  virtual void Rs4_eca (Ptr<Tuple> matchingPacket);

  virtual void Rh1Eca0Ins (Ptr<Tuple> device);

  virtual void Rh1Eca1Ins (Ptr<Tuple> initPacket);

  virtual void Rh1Eca2Ins (Ptr<Tuple> linkhr);

  virtual void Rh2_eca (Ptr<Tuple> packet);

};

} // namespace pktfwdsdn
} // namespace rapidnet
} // namespace ns3

#endif // PKTFWDSDN_H
