/* A RapidNet application. Generated by RapidNet compiler. */

#include "pktfwd-norm-prov-comp-online.h"
#include <cstdlib>
#include "ns3/nstime.h"
#include "ns3/simulator.h"
#include "ns3/type-ids.h"
#include "ns3/rapidnet-types.h"
#include "ns3/rapidnet-utils.h"
#include "ns3/assignor.h"
#include "ns3/selector.h"
#include "ns3/rapidnet-functions.h"

using namespace std;
using namespace ns3;
using namespace ns3::rapidnet;
using namespace ns3::rapidnet::pktfwdnormprovcomponline;

const string PktfwdNormProvCompOnline::DEVICE = "device";
const string PktfwdNormProvCompOnline::EPACKETCOUNT = "epacketCount";
const string PktfwdNormProvCompOnline::EPACKETTEMP = "epacketTemp";
const string PktfwdNormProvCompOnline::EQUIHASHTABLE = "equiHashTable";
const string PktfwdNormProvCompOnline::ERECVPACKET = "erecvPacket";
const string PktfwdNormProvCompOnline::ERECVPACKETTEMP = "erecvPacketTemp";
const string PktfwdNormProvCompOnline::FLOWENTRY = "flowEntry";
const string PktfwdNormProvCompOnline::INITPACKET = "initPacket";
const string PktfwdNormProvCompOnline::INITPACKETCOUNT = "initPacketCount";
const string PktfwdNormProvCompOnline::INITPACKETHASH = "initPacketHash";
const string PktfwdNormProvCompOnline::LINK = "link";
const string PktfwdNormProvCompOnline::LINKHR = "linkhr";
const string PktfwdNormProvCompOnline::PACKET = "packet";
const string PktfwdNormProvCompOnline::PACKETPROV = "packetProv";
const string PktfwdNormProvCompOnline::PROVHASHTABLE = "provHashTable";
const string PktfwdNormProvCompOnline::RECVAUXPKT = "recvAuxPkt";
const string PktfwdNormProvCompOnline::RECVPACKET = "recvPacket";
const string PktfwdNormProvCompOnline::RECVPACKETDECOMP = "recvPacketDecomp";
const string PktfwdNormProvCompOnline::RECVPACKETTEMP = "recvPacketTemp";
const string PktfwdNormProvCompOnline::RULEEXEC = "ruleExec";

NS_LOG_COMPONENT_DEFINE ("PktfwdNormProvCompOnline");
NS_OBJECT_ENSURE_REGISTERED (PktfwdNormProvCompOnline);

TypeId
PktfwdNormProvCompOnline::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::rapidnet::pktfwdnormprovcomponline::PktfwdNormProvCompOnline")
    .SetParent<RapidNetApplicationBase> ()
    .AddConstructor<PktfwdNormProvCompOnline> ()
    ;
  return tid;
}

PktfwdNormProvCompOnline::PktfwdNormProvCompOnline()
{
  NS_LOG_FUNCTION_NOARGS ();
}

PktfwdNormProvCompOnline::~PktfwdNormProvCompOnline()
{
  NS_LOG_FUNCTION_NOARGS ();
}

void
PktfwdNormProvCompOnline::DoDispose (void)
{
  NS_LOG_FUNCTION_NOARGS ();

  RapidNetApplicationBase::DoDispose ();
}

void
PktfwdNormProvCompOnline::StartApplication (void)
{
  NS_LOG_FUNCTION_NOARGS ();

  RapidNetApplicationBase::StartApplication ();
  RAPIDNET_LOG_INFO("PktfwdNormProvCompOnline Application Started");
}

void
PktfwdNormProvCompOnline::StopApplication ()
{
  NS_LOG_FUNCTION_NOARGS ();

  RapidNetApplicationBase::StopApplication ();
  RAPIDNET_LOG_INFO("PktfwdNormProvCompOnline Application Stopped");
}

void
PktfwdNormProvCompOnline::InitDatabase ()
{
  //RapidNetApplicationBase::InitDatabase ();

  AddRelationWithKeys (DEVICE, attrdeflist (
    attrdef ("device_attr2", INT32)));

  AddRelationWithKeys (EQUIHASHTABLE, attrdeflist (
    attrdef ("equiHashTable_attr2", IPV4)));

  AddRelationWithKeys (FLOWENTRY, attrdeflist (
    attrdef ("flowEntry_attr2", IPV4)));

  AddRelationWithKeys (INITPACKET, attrdeflist (
    attrdef ("initPacket_attr2", IPV4),
    attrdef ("initPacket_attr3", IPV4),
    attrdef ("initPacket_attr4", STR)));

  AddRelationWithKeys (LINK, attrdeflist (
    attrdef ("link_attr2", IPV4)));

  AddRelationWithKeys (LINKHR, attrdeflist (
    attrdef ("linkhr_attr2", IPV4)));

  AddRelationWithKeys (PROVHASHTABLE, attrdeflist (
    attrdef ("provHashTable_attr3", LIST)));

  AddRelationWithKeys (RECVPACKET, attrdeflist (
    attrdef ("recvPacket_attr2", IPV4),
    attrdef ("recvPacket_attr3", IPV4),
    attrdef ("recvPacket_attr4", STR)));

  AddRelationWithKeys (RULEEXEC, attrdeflist (
    attrdef ("ruleExec_attr4", LIST)));

}

void
PktfwdNormProvCompOnline::DemuxRecv (Ptr<Tuple> tuple)
{
  RapidNetApplicationBase::DemuxRecv (tuple);

  if (IsRecvEvent (tuple, PACKETPROV))
    {
      Prov_rs1_1_eca (tuple);
    }
  if (IsRecvEvent (tuple, EPACKETTEMP))
    {
      Prov_rs1_2_eca (tuple);
    }
  if (IsRecvEvent (tuple, EPACKETCOUNT))
    {
      Prov_rs1_3_eca (tuple);
    }
  if (IsRecvEvent (tuple, EPACKETTEMP))
    {
      Prov_rs1_4_eca (tuple);
    }
  if (IsRecvEvent (tuple, PACKET))
    {
      Rs1_eca (tuple);
    }
  if (IsInsertEvent (tuple, INITPACKET))
    {
      Rh100Eca0Ins (tuple);
    }
  if (IsRecvEvent (tuple, INITPACKETHASH))
    {
      Rh101_eca (tuple);
    }
  if (IsRecvEvent (tuple, INITPACKETCOUNT))
    {
      Rh102_eca (tuple);
    }
  if (IsRecvEvent (tuple, INITPACKETCOUNT))
    {
      Rh103_eca (tuple);
    }
  if (IsRecvEvent (tuple, INITPACKETCOUNT))
    {
      Prov_rh1_1_eca (tuple);
    }
  if (IsRecvEvent (tuple, PACKETPROV))
    {
      Prov_rh2_1_eca (tuple);
    }
  if (IsRecvEvent (tuple, ERECVPACKETTEMP))
    {
      Prov_rh2_2_eca (tuple);
    }
  if (IsRecvEvent (tuple, ERECVPACKETTEMP))
    {
      Prov_rh2_4_eca (tuple);
    }
  if (IsRecvEvent (tuple, ERECVPACKET))
    {
      Prov_rh2_5_eca (tuple);
    }
  if (IsRecvEvent (tuple, RECVPACKETTEMP))
    {
      Prov_rh2_6_eca (tuple);
    }
  if (IsRecvEvent (tuple, RECVPACKETDECOMP))
    {
      Prov_rh2_7_eca (tuple);
    }
  if (IsRecvEvent (tuple, RECVPACKETDECOMP))
    {
      Prov_rh2_8_eca (tuple);
    }
  if (IsRecvEvent (tuple, PACKET))
    {
      Rh2_eca (tuple);
    }
  if (IsInsertEvent (tuple, RECVPACKET))
    {
      Rh3Eca0Ins (tuple);
    }
}

void
PktfwdNormProvCompOnline::Prov_rs1_1_eca (Ptr<Tuple> packetProv)
{
  RAPIDNET_LOG_INFO ("Prov_rs1_1_eca triggered");

  Ptr<RelationBase> result;

  result = GetRelation (DEVICE)->Join (
    packetProv,
    strlist ("device_attr1"),
    strlist ("packetProv_attr1"));

  result = GetRelation (FLOWENTRY)->Join (
    result,
    strlist ("flowEntry_attr2", "flowEntry_attr1"),
    strlist ("packetProv_attr3", "packetProv_attr1"));

  result = GetRelation (LINK)->Join (
    result,
    strlist ("link_attr2", "link_attr1"),
    strlist ("flowEntry_attr3", "packetProv_attr1"));

  result->Assign (Assignor::New ("PID1",
    FSha1::New (
      Operation::New (RN_PLUS,
        Operation::New (RN_PLUS,
          ValueExpr::New (StrValue::New ("device")),
          VarExpr::New ("packetProv_attr1")),
        VarExpr::New ("device_attr2")))));

  result->Assign (Assignor::New ("List",
    FAppend::New (
      VarExpr::New ("PID1"))));

  result->Assign (Assignor::New ("PID3",
    FSha1::New (
      Operation::New (RN_PLUS,
        Operation::New (RN_PLUS,
          Operation::New (RN_PLUS,
            ValueExpr::New (StrValue::New ("flowEntry")),
            VarExpr::New ("packetProv_attr1")),
          VarExpr::New ("packetProv_attr3")),
        VarExpr::New ("flowEntry_attr3")))));

  result->Assign (Assignor::New ("List3",
    FAppend::New (
      VarExpr::New ("PID3"))));

  result->Assign (Assignor::New ("List",
    FConcat::New (
      VarExpr::New ("List"),
      VarExpr::New ("List3"))));

  result->Assign (Assignor::New ("PID4",
    FSha1::New (
      Operation::New (RN_PLUS,
        Operation::New (RN_PLUS,
          ValueExpr::New (StrValue::New ("link")),
          VarExpr::New ("packetProv_attr1")),
        VarExpr::New ("flowEntry_attr3")))));

  result->Assign (Assignor::New ("List4",
    FAppend::New (
      VarExpr::New ("PID4"))));

  result->Assign (Assignor::New ("List",
    FConcat::New (
      VarExpr::New ("List"),
      VarExpr::New ("List4"))));

  result->Assign (Assignor::New ("RLOC",
    VarExpr::New ("packetProv_attr1")));

  result->Assign (Assignor::New ("R",
    ValueExpr::New (StrValue::New ("rs1"))));

  result->Assign (Assignor::New ("RID",
    FSha1::New (
      Operation::New (RN_PLUS,
        Operation::New (RN_PLUS,
          VarExpr::New ("R"),
          VarExpr::New ("RLOC")),
        VarExpr::New ("List")))));

  result = result->Select (Selector::New (
    Operation::New (RN_EQ,
      VarExpr::New ("device_attr2"),
      ValueExpr::New (Int32Value::New (0)))));

  result = result->Project (
    EPACKETTEMP,
    strlist ("RLOC",
      "flowEntry_attr3",
      "packetProv_attr2",
      "packetProv_attr3",
      "packetProv_attr4",
      "RID",
      "R",
      "List",
      "packetProv_attr5",
      "RLOC"),
    strlist ("epacketTemp_attr1",
      "epacketTemp_attr2",
      "epacketTemp_attr3",
      "epacketTemp_attr4",
      "epacketTemp_attr5",
      "epacketTemp_attr6",
      "epacketTemp_attr7",
      "epacketTemp_attr8",
      "epacketTemp_attr9",
      RN_DEST));

  Send (result);
}

void
PktfwdNormProvCompOnline::Prov_rs1_2_eca (Ptr<Tuple> epacketTemp)
{
  RAPIDNET_LOG_INFO ("Prov_rs1_2_eca triggered");

  Ptr<RelationBase> result;

  result = GetRelation (RULEEXEC)->Join (
    epacketTemp,
    strlist ("ruleExec_attr4", "ruleExec_attr3", "ruleExec_attr2", "ruleExec_attr1"),
    strlist ("epacketTemp_attr8", "epacketTemp_attr7", "epacketTemp_attr6", "epacketTemp_attr1"));

  result = AggWrapCount::New ()->Compute (result, epacketTemp);

  result = result->Project (
    EPACKETCOUNT,
    strlist ("epacketTemp_attr1",
      "epacketTemp_attr6",
      "epacketTemp_attr7",
      "epacketTemp_attr8",
      "count"),
    strlist ("epacketCount_attr1",
      "epacketCount_attr2",
      "epacketCount_attr3",
      "epacketCount_attr4",
      "epacketCount_attr5"));

  SendLocal (result);
}

void
PktfwdNormProvCompOnline::Prov_rs1_3_eca (Ptr<Tuple> epacketCount)
{
  RAPIDNET_LOG_INFO ("Prov_rs1_3_eca triggered");

  Ptr<Tuple> result = epacketCount;

  result = result->Select (Selector::New (
    Operation::New (RN_EQ,
      VarExpr::New ("epacketCount_attr5"),
      ValueExpr::New (Int32Value::New (0)))));

  result = result->Project (
    RULEEXEC,
    strlist ("epacketCount_attr1",
      "epacketCount_attr2",
      "epacketCount_attr3",
      "epacketCount_attr4"),
    strlist ("ruleExec_attr1",
      "ruleExec_attr2",
      "ruleExec_attr3",
      "ruleExec_attr4"));

  Insert (result);
}

void
PktfwdNormProvCompOnline::Prov_rs1_4_eca (Ptr<Tuple> epacketTemp)
{
  RAPIDNET_LOG_INFO ("Prov_rs1_4_eca triggered");

  Ptr<Tuple> result = epacketTemp;

  result->Assign (Assignor::New ("Hash",
    FAppend::New (
      VarExpr::New ("epacketTemp_attr6"))));

  result->Assign (Assignor::New ("NewTag",
    FConcat::New (
      VarExpr::New ("epacketTemp_attr9"),
      VarExpr::New ("Hash"))));

  result = result->Project (
    PACKETPROV,
    strlist ("epacketTemp_attr2",
      "epacketTemp_attr3",
      "epacketTemp_attr4",
      "epacketTemp_attr5",
      "NewTag",
      "epacketTemp_attr2"),
    strlist ("packetProv_attr1",
      "packetProv_attr2",
      "packetProv_attr3",
      "packetProv_attr4",
      "packetProv_attr5",
      RN_DEST));

  Send (result);
}

void
PktfwdNormProvCompOnline::Rs1_eca (Ptr<Tuple> packet)
{
  RAPIDNET_LOG_INFO ("Rs1_eca triggered");

  Ptr<RelationBase> result;

  result = GetRelation (DEVICE)->Join (
    packet,
    strlist ("device_attr1"),
    strlist ("packet_attr1"));

  result = GetRelation (FLOWENTRY)->Join (
    result,
    strlist ("flowEntry_attr2", "flowEntry_attr1"),
    strlist ("packet_attr3", "packet_attr1"));

  result = GetRelation (LINK)->Join (
    result,
    strlist ("link_attr2", "link_attr1"),
    strlist ("flowEntry_attr3", "packet_attr1"));

  result = result->Select (Selector::New (
    Operation::New (RN_EQ,
      VarExpr::New ("device_attr2"),
      ValueExpr::New (Int32Value::New (0)))));

  result = result->Project (
    PACKET,
    strlist ("flowEntry_attr3",
      "packet_attr2",
      "packet_attr3",
      "packet_attr4",
      "packet_attr5",
      "flowEntry_attr3"),
    strlist ("packet_attr1",
      "packet_attr2",
      "packet_attr3",
      "packet_attr4",
      "packet_attr5",
      RN_DEST));

  Send (result);
}

void
PktfwdNormProvCompOnline::Rh100Eca0Ins (Ptr<Tuple> initPacket)
{
  RAPIDNET_LOG_INFO ("Rh100Eca0Ins triggered");

  Ptr<Tuple> result = initPacket;

  result->Assign (Assignor::New ("PIDequi",
    FSha1::New (
      Operation::New (RN_PLUS,
        Operation::New (RN_PLUS,
          ValueExpr::New (StrValue::New ("")),
          VarExpr::New ("initPacket_attr1")),
        VarExpr::New ("initPacket_attr3")))));

  result = result->Project (
    INITPACKETHASH,
    strlist ("initPacket_attr1",
      "initPacket_attr2",
      "initPacket_attr3",
      "initPacket_attr4",
      "PIDequi"),
    strlist ("initPacketHash_attr1",
      "initPacketHash_attr2",
      "initPacketHash_attr3",
      "initPacketHash_attr4",
      "initPacketHash_attr5"));

  SendLocal (result);
}

void
PktfwdNormProvCompOnline::Rh101_eca (Ptr<Tuple> initPacketHash)
{
  RAPIDNET_LOG_INFO ("Rh101_eca triggered");

  Ptr<RelationBase> result;

  result = GetRelation (EQUIHASHTABLE)->Join (
    initPacketHash,
    strlist ("equiHashTable_attr2", "equiHashTable_attr1", "equiHashTable_attr3"),
    strlist ("initPacketHash_attr3", "initPacketHash_attr1", "initPacketHash_attr5"));

  result = AggWrapCount::New ()->Compute (result, initPacketHash);

  result = result->Project (
    INITPACKETCOUNT,
    strlist ("initPacketHash_attr1",
      "initPacketHash_attr2",
      "initPacketHash_attr3",
      "initPacketHash_attr4",
      "initPacketHash_attr5",
      "count"),
    strlist ("initPacketCount_attr1",
      "initPacketCount_attr2",
      "initPacketCount_attr3",
      "initPacketCount_attr4",
      "initPacketCount_attr5",
      "initPacketCount_attr6"));

  SendLocal (result);
}

void
PktfwdNormProvCompOnline::Rh102_eca (Ptr<Tuple> initPacketCount)
{
  RAPIDNET_LOG_INFO ("Rh102_eca triggered");

  Ptr<Tuple> result = initPacketCount;

  result = result->Select (Selector::New (
    Operation::New (RN_EQ,
      VarExpr::New ("initPacketCount_attr6"),
      ValueExpr::New (Int32Value::New (0)))));

  result = result->Project (
    EQUIHASHTABLE,
    strlist ("initPacketCount_attr1",
      "initPacketCount_attr3",
      "initPacketCount_attr5"),
    strlist ("equiHashTable_attr1",
      "equiHashTable_attr2",
      "equiHashTable_attr3"));

  Insert (result);
}

void
PktfwdNormProvCompOnline::Rh103_eca (Ptr<Tuple> initPacketCount)
{
  RAPIDNET_LOG_INFO ("Rh103_eca triggered");

  Ptr<RelationBase> result;

  result = GetRelation (DEVICE)->Join (
    initPacketCount,
    strlist ("device_attr1"),
    strlist ("initPacketCount_attr1"));

  result = GetRelation (LINKHR)->Join (
    result,
    strlist ("linkhr_attr1"),
    strlist ("initPacketCount_attr1"));

  result->Assign (Assignor::New ("PIDev",
    FSha1::New (
      Operation::New (RN_PLUS,
        Operation::New (RN_PLUS,
          Operation::New (RN_PLUS,
            Operation::New (RN_PLUS,
              ValueExpr::New (StrValue::New ("initPacket")),
              VarExpr::New ("initPacketCount_attr1")),
            VarExpr::New ("initPacketCount_attr2")),
          VarExpr::New ("initPacketCount_attr3")),
        VarExpr::New ("initPacketCount_attr4")))));

  result->Assign (Assignor::New ("PIDequiHash",
    FAppend::New (
      VarExpr::New ("initPacketCount_attr5"))));

  result->Assign (Assignor::New ("PIDevHash",
    FAppend::New (
      VarExpr::New ("PIDev"))));

  result->Assign (Assignor::New ("PIDHash",
    FConcat::New (
      VarExpr::New ("PIDequiHash"),
      VarExpr::New ("PIDevHash"))));

  result = result->Select (Selector::New (
    Operation::New (RN_EQ,
      VarExpr::New ("device_attr2"),
      ValueExpr::New (Int32Value::New (1)))));

  result = result->Select (Selector::New (
    Operation::New (RN_NEQ,
      VarExpr::New ("initPacketCount_attr6"),
      ValueExpr::New (Int32Value::New (0)))));

  result = result->Project (
    PACKET,
    strlist ("linkhr_attr2",
      "initPacketCount_attr2",
      "initPacketCount_attr3",
      "initPacketCount_attr4",
      "PIDHash",
      "linkhr_attr2"),
    strlist ("packet_attr1",
      "packet_attr2",
      "packet_attr3",
      "packet_attr4",
      "packet_attr5",
      RN_DEST));

  Send (result);
}

void
PktfwdNormProvCompOnline::Prov_rh1_1_eca (Ptr<Tuple> initPacketCount)
{
  RAPIDNET_LOG_INFO ("Prov_rh1_1_eca triggered");

  Ptr<RelationBase> result;

  result = GetRelation (DEVICE)->Join (
    initPacketCount,
    strlist ("device_attr1"),
    strlist ("initPacketCount_attr1"));

  result = GetRelation (LINKHR)->Join (
    result,
    strlist ("linkhr_attr1"),
    strlist ("initPacketCount_attr1"));

  result->Assign (Assignor::New ("PIDev",
    FSha1::New (
      Operation::New (RN_PLUS,
        Operation::New (RN_PLUS,
          Operation::New (RN_PLUS,
            Operation::New (RN_PLUS,
              ValueExpr::New (StrValue::New ("initPacket")),
              VarExpr::New ("initPacketCount_attr1")),
            VarExpr::New ("initPacketCount_attr2")),
          VarExpr::New ("initPacketCount_attr3")),
        VarExpr::New ("initPacketCount_attr4")))));

  result->Assign (Assignor::New ("Equilist",
    FAppend::New (
      VarExpr::New ("initPacketCount_attr5"))));

  result->Assign (Assignor::New ("Evlist",
    FAppend::New (
      VarExpr::New ("PIDev"))));

  result->Assign (Assignor::New ("Tag",
    FConcat::New (
      VarExpr::New ("Equilist"),
      VarExpr::New ("Evlist"))));

  result->Assign (Assignor::New ("PID1",
    FSha1::New (
      Operation::New (RN_PLUS,
        Operation::New (RN_PLUS,
          ValueExpr::New (StrValue::New ("device")),
          VarExpr::New ("initPacketCount_attr1")),
        VarExpr::New ("device_attr2")))));

  result->Assign (Assignor::New ("List",
    FAppend::New (
      VarExpr::New ("PID1"))));

  result->Assign (Assignor::New ("PID2",
    FSha1::New (
      Operation::New (RN_PLUS,
        Operation::New (RN_PLUS,
          ValueExpr::New (StrValue::New ("linkhr")),
          VarExpr::New ("initPacketCount_attr1")),
        VarExpr::New ("linkhr_attr2")))));

  result->Assign (Assignor::New ("List2",
    FAppend::New (
      VarExpr::New ("PID2"))));

  result->Assign (Assignor::New ("List",
    FConcat::New (
      VarExpr::New ("List"),
      VarExpr::New ("List2"))));

  result->Assign (Assignor::New ("RLOC",
    VarExpr::New ("initPacketCount_attr1")));

  result->Assign (Assignor::New ("R",
    ValueExpr::New (StrValue::New ("rh1"))));

  result->Assign (Assignor::New ("RID",
    FSha1::New (
      Operation::New (RN_PLUS,
        Operation::New (RN_PLUS,
          VarExpr::New ("R"),
          VarExpr::New ("RLOC")),
        VarExpr::New ("List")))));

  result = result->Select (Selector::New (
    Operation::New (RN_EQ,
      VarExpr::New ("initPacketCount_attr6"),
      ValueExpr::New (Int32Value::New (0)))));

  result = result->Select (Selector::New (
    Operation::New (RN_EQ,
      VarExpr::New ("device_attr2"),
      ValueExpr::New (Int32Value::New (1)))));

  result = result->Project (
    EPACKETTEMP,
    strlist ("RLOC",
      "linkhr_attr2",
      "initPacketCount_attr2",
      "initPacketCount_attr3",
      "initPacketCount_attr4",
      "RID",
      "R",
      "List",
      "Tag",
      "RLOC"),
    strlist ("epacketTemp_attr1",
      "epacketTemp_attr2",
      "epacketTemp_attr3",
      "epacketTemp_attr4",
      "epacketTemp_attr5",
      "epacketTemp_attr6",
      "epacketTemp_attr7",
      "epacketTemp_attr8",
      "epacketTemp_attr9",
      RN_DEST));

  Send (result);
}

void
PktfwdNormProvCompOnline::Prov_rh2_1_eca (Ptr<Tuple> packetProv)
{
  RAPIDNET_LOG_INFO ("Prov_rh2_1_eca triggered");

  Ptr<RelationBase> result;

  result = GetRelation (DEVICE)->Join (
    packetProv,
    strlist ("device_attr1"),
    strlist ("packetProv_attr1"));

  result->Assign (Assignor::New ("PID1",
    FSha1::New (
      Operation::New (RN_PLUS,
        Operation::New (RN_PLUS,
          ValueExpr::New (StrValue::New ("device")),
          VarExpr::New ("packetProv_attr1")),
        VarExpr::New ("device_attr2")))));

  result->Assign (Assignor::New ("List",
    FAppend::New (
      VarExpr::New ("PID1"))));

  result->Assign (Assignor::New ("PID2",
    FSha1::New (
      Operation::New (RN_PLUS,
        Operation::New (RN_PLUS,
          Operation::New (RN_PLUS,
            Operation::New (RN_PLUS,
              ValueExpr::New (StrValue::New ("packet")),
              VarExpr::New ("packetProv_attr1")),
            VarExpr::New ("packetProv_attr2")),
          VarExpr::New ("packetProv_attr3")),
        VarExpr::New ("packetProv_attr4")))));

  result->Assign (Assignor::New ("List2",
    FAppend::New (
      VarExpr::New ("PID2"))));

  result->Assign (Assignor::New ("List",
    FConcat::New (
      VarExpr::New ("List"),
      VarExpr::New ("List2"))));

  result->Assign (Assignor::New ("RLOC",
    VarExpr::New ("packetProv_attr1")));

  result->Assign (Assignor::New ("R",
    ValueExpr::New (StrValue::New ("rh2"))));

  result->Assign (Assignor::New ("RID",
    FSha1::New (
      Operation::New (RN_PLUS,
        Operation::New (RN_PLUS,
          VarExpr::New ("R"),
          VarExpr::New ("RLOC")),
        VarExpr::New ("List")))));

  result = result->Select (Selector::New (
    Operation::New (RN_EQ,
      VarExpr::New ("packetProv_attr3"),
      VarExpr::New ("packetProv_attr1"))));

  result = result->Select (Selector::New (
    Operation::New (RN_EQ,
      VarExpr::New ("device_attr2"),
      ValueExpr::New (Int32Value::New (1)))));

  result = result->Project (
    ERECVPACKETTEMP,
    strlist ("RLOC",
      "packetProv_attr1",
      "packetProv_attr2",
      "packetProv_attr3",
      "packetProv_attr4",
      "RID",
      "R",
      "List",
      "packetProv_attr5",
      "RLOC"),
    strlist ("erecvPacketTemp_attr1",
      "erecvPacketTemp_attr2",
      "erecvPacketTemp_attr3",
      "erecvPacketTemp_attr4",
      "erecvPacketTemp_attr5",
      "erecvPacketTemp_attr6",
      "erecvPacketTemp_attr7",
      "erecvPacketTemp_attr8",
      "erecvPacketTemp_attr9",
      RN_DEST));

  Send (result);
}

void
PktfwdNormProvCompOnline::Prov_rh2_2_eca (Ptr<Tuple> erecvPacketTemp)
{
  RAPIDNET_LOG_INFO ("Prov_rh2_2_eca triggered");

  Ptr<RelationBase> result;

  result = GetRelation (RULEEXEC)->Join (
    erecvPacketTemp,
    strlist ("ruleExec_attr4", "ruleExec_attr3", "ruleExec_attr2", "ruleExec_attr1"),
    strlist ("erecvPacketTemp_attr8", "erecvPacketTemp_attr7", "erecvPacketTemp_attr6", "erecvPacketTemp_attr1"));

  result = AggWrapCount::New ()->Compute (result, erecvPacketTemp);

  result = result->Project (
    EPACKETCOUNT,
    strlist ("erecvPacketTemp_attr1",
      "erecvPacketTemp_attr6",
      "erecvPacketTemp_attr7",
      "erecvPacketTemp_attr8",
      "count"),
    strlist ("epacketCount_attr1",
      "epacketCount_attr2",
      "epacketCount_attr3",
      "epacketCount_attr4",
      "epacketCount_attr5"));

  SendLocal (result);
}

void
PktfwdNormProvCompOnline::Prov_rh2_4_eca (Ptr<Tuple> erecvPacketTemp)
{
  RAPIDNET_LOG_INFO ("Prov_rh2_4_eca triggered");

  Ptr<Tuple> result = erecvPacketTemp;

  result = result->Project (
    ERECVPACKET,
    strlist ("erecvPacketTemp_attr2",
      "erecvPacketTemp_attr3",
      "erecvPacketTemp_attr4",
      "erecvPacketTemp_attr5",
      "erecvPacketTemp_attr6",
      "erecvPacketTemp_attr1",
      "erecvPacketTemp_attr9",
      "erecvPacketTemp_attr2"),
    strlist ("erecvPacket_attr1",
      "erecvPacket_attr2",
      "erecvPacket_attr3",
      "erecvPacket_attr4",
      "erecvPacket_attr5",
      "erecvPacket_attr6",
      "erecvPacket_attr7",
      RN_DEST));

  Send (result);
}

void
PktfwdNormProvCompOnline::Prov_rh2_5_eca (Ptr<Tuple> erecvPacket)
{
  RAPIDNET_LOG_INFO ("Prov_rh2_5_eca triggered");

  Ptr<Tuple> result = erecvPacket;

  result->Assign (Assignor::New ("Hash",
    FAppend::New (
      VarExpr::New ("erecvPacket_attr5"))));

  result->Assign (Assignor::New ("NewTag",
    FConcat::New (
      VarExpr::New ("erecvPacket_attr7"),
      VarExpr::New ("Hash"))));

  result = result->Project (
    RECVPACKETTEMP,
    strlist ("erecvPacket_attr1",
      "erecvPacket_attr2",
      "erecvPacket_attr3",
      "erecvPacket_attr4",
      "erecvPacket_attr7"),
    strlist ("recvPacketTemp_attr1",
      "recvPacketTemp_attr2",
      "recvPacketTemp_attr3",
      "recvPacketTemp_attr4",
      "recvPacketTemp_attr5"));

  SendLocal (result);
}

void
PktfwdNormProvCompOnline::Prov_rh2_6_eca (Ptr<Tuple> recvPacketTemp)
{
  RAPIDNET_LOG_INFO ("Prov_rh2_6_eca triggered");

  Ptr<Tuple> result = recvPacketTemp;

  result->Assign (Assignor::New ("PIDequi",
    FFirst::New (
      VarExpr::New ("recvPacketTemp_attr5"))));

  result->Assign (Assignor::New ("Remainlist",
    FRemoveFirst::New (
      VarExpr::New ("recvPacketTemp_attr5"))));

  result->Assign (Assignor::New ("PIDev",
    FFirst::New (
      VarExpr::New ("Remainlist"))));

  result->Assign (Assignor::New ("Hashlist",
    FRemoveFirst::New (
      VarExpr::New ("Remainlist"))));

  result = result->Project (
    RECVPACKETDECOMP,
    strlist ("recvPacketTemp_attr1",
      "recvPacketTemp_attr2",
      "recvPacketTemp_attr3",
      "recvPacketTemp_attr4",
      "PIDequi",
      "PIDev",
      "Hashlist"),
    strlist ("recvPacketDecomp_attr1",
      "recvPacketDecomp_attr2",
      "recvPacketDecomp_attr3",
      "recvPacketDecomp_attr4",
      "recvPacketDecomp_attr5",
      "recvPacketDecomp_attr6",
      "recvPacketDecomp_attr7"));

  SendLocal (result);
}

void
PktfwdNormProvCompOnline::Prov_rh2_7_eca (Ptr<Tuple> recvPacketDecomp)
{
  RAPIDNET_LOG_INFO ("Prov_rh2_7_eca triggered");

  Ptr<Tuple> result = recvPacketDecomp;

  result = result->Project (
    PROVHASHTABLE,
    strlist ("recvPacketDecomp_attr1",
      "recvPacketDecomp_attr5",
      "recvPacketDecomp_attr7"),
    strlist ("provHashTable_attr1",
      "provHashTable_attr2",
      "provHashTable_attr3"));

  Insert (result);
}

void
PktfwdNormProvCompOnline::Prov_rh2_8_eca (Ptr<Tuple> recvPacketDecomp)
{
  RAPIDNET_LOG_INFO ("Prov_rh2_8_eca triggered");

  Ptr<Tuple> result = recvPacketDecomp;

  result = result->Project (
    RECVPACKET,
    strlist ("recvPacketDecomp_attr1",
      "recvPacketDecomp_attr2",
      "recvPacketDecomp_attr3",
      "recvPacketDecomp_attr4",
      "recvPacketDecomp_attr5",
      "recvPacketDecomp_attr6"),
    strlist ("recvPacket_attr1",
      "recvPacket_attr2",
      "recvPacket_attr3",
      "recvPacket_attr4",
      "recvPacket_attr5",
      "recvPacket_attr6"));

  Insert (result);
}

void
PktfwdNormProvCompOnline::Rh2_eca (Ptr<Tuple> packet)
{
  RAPIDNET_LOG_INFO ("Rh2_eca triggered");

  Ptr<RelationBase> result;

  result = GetRelation (DEVICE)->Join (
    packet,
    strlist ("device_attr1"),
    strlist ("packet_attr1"));

  result->Assign (Assignor::New ("PIDequi",
    FFirst::New (
      VarExpr::New ("packet_attr5"))));

  result->Assign (Assignor::New ("PIDevHash",
    FRemoveFirst::New (
      VarExpr::New ("packet_attr5"))));

  result->Assign (Assignor::New ("PIDev",
    FFirst::New (
      VarExpr::New ("PIDevHash"))));

  result = result->Select (Selector::New (
    Operation::New (RN_EQ,
      VarExpr::New ("packet_attr3"),
      VarExpr::New ("packet_attr1"))));

  result = result->Select (Selector::New (
    Operation::New (RN_EQ,
      VarExpr::New ("device_attr2"),
      ValueExpr::New (Int32Value::New (1)))));

  result = result->Project (
    RECVPACKET,
    strlist ("packet_attr1",
      "packet_attr2",
      "packet_attr3",
      "packet_attr4",
      "PIDequi",
      "PIDev"),
    strlist ("recvPacket_attr1",
      "recvPacket_attr2",
      "recvPacket_attr3",
      "recvPacket_attr4",
      "recvPacket_attr5",
      "recvPacket_attr6"));

  Insert (result);
}

void
PktfwdNormProvCompOnline::Rh3Eca0Ins (Ptr<Tuple> recvPacket)
{
  RAPIDNET_LOG_INFO ("Rh3Eca0Ins triggered");

  Ptr<Tuple> result = recvPacket;

  result = result->Project (
    RECVAUXPKT,
    strlist ("recvPacket_attr1",
      "recvPacket_attr2",
      "recvPacket_attr4",
      "recvPacket_attr5",
      "recvPacket_attr6"),
    strlist ("recvAuxPkt_attr1",
      "recvAuxPkt_attr2",
      "recvAuxPkt_attr3",
      "recvAuxPkt_attr4",
      "recvAuxPkt_attr5"));

  SendLocal (result);
}

