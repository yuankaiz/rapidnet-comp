 /* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "ns3/core-module.h"
#include "ns3/simulator-module.h"
#include "ns3/node-module.h"
#include "ns3/pktfwd-norm-prov-ruleauxtable-module.h"
#include "ns3/rapidnet-module.h"
#include "ns3/values-module.h"
#include "ns3/helper-module.h"


#define ModuleName PktfwdNormProvRuleauxtable
#define ModuleNameStr "PktfwdNormProvRuleauxtable"
#define ModuleHelper PktfwdNormProvRuleauxtableHelper
#define ModuleNameSpace pktfwdnormprovruleauxtable


/* Rule recording flag*/
#define recordrule(node)\
  tuple(ModuleName::RECORDRULE,\
  attr("recordRule_attr1", Ipv4Value, node))

#define insert_recordrule(node)                                            \
  app(node) -> Insert(recordrule(addr(node)));

/* Links connecting network devices*/
#define link(sw, nei)\
  tuple(ModuleName::LINK,\
  attr("link_attr1", Ipv4Value, sw),  \
  attr("link_attr2", Ipv4Value, nei))

#define insert_link(sw, nei)                                            \
  app(sw) -> Insert(link(addr(sw), addr(nei)));

/* Input packets */
#define initpacket(host, srcadd, dstadd, data)         \
  tuple(ModuleName::INITPACKET,\
  attr("initPacket_attr1", Ipv4Value, host),  \
  attr("initPacket_attr2", Ipv4Value, srcadd), \
  attr("initPacket_attr3", Ipv4Value, dstadd), \
  attr("initPacket_attr4", StrValue, data))

#define insert_packet(host, srcadd, dstadd, data)                            \
  app(host) -> Insert(initpacket(addr(host), addr(srcadd), addr(dstadd), data));

/* flow entry */
#define flowentry(sw, dst, next)		\
  tuple(ModuleName::FLOWENTRY,\
	attr("flowEntry_attr1", Ipv4Value, sw),\
	attr("flowEntry_attr2", Ipv4Value, dst),         \
	attr("flowEntry_attr3", Ipv4Value, next))

#define insert_flowentry(sw, dst, next)				\
  app(sw) -> Insert(flowentry(addr(sw), addr(dst), addr(next)));


using namespace std;
using namespace ns3;
using namespace ns3::rapidnet;
using namespace ns3::rapidnet::ModuleNameSpace;

ApplicationContainer apps;

void Print ()
{
  PrintRelation (apps, ModuleName::RECVPACKET);
  PrintRelation (apps, ModuleName::PROVSTR);
  PrintRelation (apps, ModuleName::RULEINFO);
}

/*
 * Topology: 
 * Nodes: 1 -- 2 -- 3
 */
void BuildTopology()
{
  
  /* Connect switches to hosts and each other*/
  insert_link(1, 2);
  //  insert_link(2, 1);
  insert_link(2, 3);    
  //  insert_link(3, 2);
}

void SetupFlowTable()
{
  /* Set up the flow entry for switch 5*/
  insert_flowentry(1, 3, 2);
  insert_flowentry(2, 3, 3);
}

void InitRuleRecording()
{
  insert_recordrule(1);
  insert_recordrule(2);
  insert_recordrule(3);
}

void PacketInsertion()
{
  insert_packet(1, 1, 3, "data");
}


void InitApps(int nodeNum)
{
  NodeContainer mainNodes;

  mainNodes.Create(nodeNum);

  NodeContainer csmaNodes;
  csmaNodes.Add(mainNodes);

  CsmaHelper csma;

  NetDeviceContainer csmaDevices;
  csmaDevices = csma.Install(csmaNodes);

  InternetStackHelper stack;
  stack.Install(csmaNodes);

  Ipv4AddressHelper address;
  Ipv4Address base = "10.1.1.0";

  address.SetBase(base, "255.255.255.0");
  address.Assign(csmaDevices);

  apps.Add(Create<ModuleHelper>()->Install(mainNodes));

  SetMaxJitter (apps, 0.001);
}

int
main (int argc, char *argv[])
{
  LogComponentEnable(ModuleNameStr, LOG_LEVEL_INFO);
  LogComponentEnable("RapidNetApplicationBase", LOG_LEVEL_INFO);

  int nodeNum = 3;
  InitApps(nodeNum);

  apps.Start (Seconds (0.0));
  apps.Stop (Seconds (500.0));

  schedule (0.0001, BuildTopology);
  schedule (1.0000, InitRuleRecording);
  schedule (3.0000, SetupFlowTable);
  schedule (4.0000, PacketInsertion);

  schedule (499.0000, Print);

  Simulator::Run ();
  Simulator::Destroy ();

  return 0;
}

