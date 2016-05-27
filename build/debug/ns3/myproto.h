/* A RapidNet application. Generated by RapidNet compiler. */

#ifndef MYPROTO_H
#define MYPROTO_H

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
namespace myproto {

class Myproto : public RapidNetApplicationBase
{
public:
  static const string BESTPATH;
  static const string LINK;
  static const string PATH;
  static const string PATHDELETE;
  static const string R2LOCAL1R2LINKZSEND;
  static const string R2LOCAL2PATHSEND;
  static const string R2LINKZ;
  static const string R2LINKZDELETE;

  static TypeId GetTypeId (void);

  Myproto ();

  virtual ~Myproto ();

protected:

  virtual void DoDispose (void);

  virtual void StartApplication (void);

  virtual void StopApplication (void);

  virtual void InitDatabase (void);

  virtual void DemuxRecv (Ptr<Tuple> tuple);

  virtual void R1Eca0Ins (Ptr<Tuple> link);

  virtual void R1Eca0Del (Ptr<Tuple> link);

  virtual void R2Local1Eca0RemoteIns (Ptr<Tuple> r2Local1r2linkZsend);

  virtual void R2Local1Eca0RemoteDel (Ptr<Tuple> r2linkZDelete);

  virtual void R2Local1Eca0Ins (Ptr<Tuple> link);

  virtual void R2Local1Eca0Del (Ptr<Tuple> link);

  virtual void R2Local2Eca0RemoteIns (Ptr<Tuple> r2Local2pathsend);

  virtual void R2Local2Eca0RemoteDel (Ptr<Tuple> pathDelete);

  virtual void R2Local2Eca0Ins (Ptr<Tuple> r2linkZ);

  virtual void R2Local2Eca0Del (Ptr<Tuple> r2linkZ);

  virtual void R2Local2Eca1Ins (Ptr<Tuple> bestPath);

  virtual void R2Local2Eca1Del (Ptr<Tuple> bestPath);

  virtual void R3eca (Ptr<Tuple> path);

  virtual void R3eca2 (Ptr<Tuple> path);

  Ptr<Aggregator> m_aggr_bestpathMinC;
};

} // namespace myproto
} // namespace rapidnet
} // namespace ns3

#endif // MYPROTO_H
