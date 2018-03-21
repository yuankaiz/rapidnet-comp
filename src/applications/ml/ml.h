/* A RapidNet application. Generated by RapidNet compiler. */

#ifndef ML_H
#define ML_H

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
namespace ml {

class Ml : public RapidNetApplicationBase
{
public:
  static const string ASSOCIATIONRULE;
  static const string CITERATE;
  static const string CLIST;
  static const string CLF;
  static const string IDENTIFIEDPATTERN;
  static const string IMAGE;
  static const string PCLF;
  static const string PITERATE;
  static const string PLIST;
  static const string PREDICTION;
  static const string RESULT;

  static TypeId GetTypeId (void);

  Ml ();

  virtual ~Ml ();

protected:

  virtual void DoDispose (void);

  virtual void StartApplication (void);

  virtual void StopApplication (void);

  virtual void InitDatabase (void);

  virtual void DemuxRecv (Ptr<Tuple> tuple);

  virtual void R1Eca0Ins (Ptr<Tuple> clf);

  virtual void R1Eca0Del (Ptr<Tuple> clf);

  virtual void R2Eca0Ins (Ptr<Tuple> image);

  virtual void R2Eca1Ins (Ptr<Tuple> pClf);

  virtual void R3_eca (Ptr<Tuple> pList);

  virtual void R4_eca (Ptr<Tuple> pIterate);

  virtual void R5_eca (Ptr<Tuple> pIterate);

  virtual void R6Eca0Ins (Ptr<Tuple> associationrule);

  virtual void R6Eca0Del (Ptr<Tuple> associationrule);

  virtual void R6Eca1Ins (Ptr<Tuple> prediction);

  virtual void R6Eca1Del (Ptr<Tuple> prediction);

  virtual void R7Eca0Ins (Ptr<Tuple> identifiedPattern);

  virtual void R8_eca (Ptr<Tuple> cList);

  virtual void R9_eca (Ptr<Tuple> cIterate);

  virtual void R10_eca (Ptr<Tuple> cIterate);

};

} // namespace ml
} // namespace rapidnet
} // namespace ns3

#endif // ML_H