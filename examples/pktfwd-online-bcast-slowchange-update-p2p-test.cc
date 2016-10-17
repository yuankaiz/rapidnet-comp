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
#include "ns3/pktfwd-online-bcast-slowchange-update-module.h"
#include "ns3/pktfwd-norm-online-query-init-module.h"
#include "ns3/rapidnet-module.h"
#include "ns3/values-module.h"
#include "ns3/helper-module.h"

#define ModuleName PktfwdOnlineBcastSlowchangeUpdate
#define ModuleNameStr "PktfwdOnlineBcastSlowchangeUpdate"
#define ModuleHelper PktfwdOnlineBcastSlowchangeUpdateHelper
#define ModuleNameSpace pktfwdonlinebcastslowchangeupdate
#define QueryModuleName PktfwdNormOnlineQueryInit
#define QueryModuleNameStr "PktfwdNormOnlineQueryInit"
#define QueryModuleHelper PktfwdNormOnlineQueryInitHelper
#define QueryModuleNameSpace pktfwdnormonlinequeryinit


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

/* Program ID*/
#define programid(host, dstadd, progid)         \
  tuple(ModuleName::PROGRAMID,\
  attr("programID_attr1", Ipv4Value, host),  \
  attr("programID_attr2", Ipv4Value, dstadd), \
  attr("programID_attr3", StrValue, progid))

#define insert_programid(host, dstadd, progid)                            \
  app(host) -> Insert(programid(addr(host), addr(dstadd), progid));

/* Update the slow-changing table */
#define updateRoute(node, dst, next)		\
  tuple(ModuleName::UPDATEROUTE,\
	attr("updateRoute_attr1", Ipv4Value, node),\
	attr("updateRoute_attr2", Ipv4Value, dst),         \
	attr("updateRoute_attr3", Ipv4Value, next))

#define insert_updateroute(node, dst, next)				\
  app(node) -> Insert(updateRoute(addr(node), addr(dst), addr(next)));

/* Tuple to query*/
#define queryTuple(ret, name, loc, src, dst, data) \
  tuple(QueryModuleName::TUPLE,\
  attr("tuple_attr1", Ipv4Value, ret),  \
  attr("tuple_attr2", StrValue, name),  \
  attr("tuple_attr3", Ipv4Value, loc), \
  attr("tuple_attr4", Ipv4Value, src), \
  attr("tuple_attr5", Ipv4Value, dst),\
  attr("tuple_attr6", StrValue, data))


#define insert_queryTuple(name, loc, src, dst, data) \
  qnode->Insert(queryTuple(qnode->GetAddress(), name, addr(loc), addr(src), addr(dst), data));


#define NET_MASK "255.255.255.0"
#define BASE_ADDR "10.0."
#define ADDR_SUFFIX ".0"
#define SIMULATION_LENGTH 10


using namespace std;
using namespace ns3;
using namespace ns3::rapidnet;
using namespace ns3::rapidnet::ModuleNameSpace;
using namespace ns3::rapidnet::QueryModuleNameSpace;

ApplicationContainer apps;
ApplicationContainer queryApps;


void ShowAppAddr(int appNodeNum)
{
  std::ofstream ofile("addr_mapping.txt");
  ofile << "\n" << "Mapping: (Node ID -> Application address) " << "\n";
  // IP addresses for the nodes in apps
  int nodeID;
  for (int nodeID = 1;nodeID <= appNodeNum;nodeID++)
    {
      ofile << "Node ID: " << nodeID;
      ofile << " -> ";
      ofile << "App Ipv4ddress: " << app(nodeID)->GetAddress ();
      ofile << "\n";
    }

  // IP address for the nodes in queryApps
  ofile << "Node ID (query node): " << nodeID;
  ofile << " -> ";
  ofile << "App Ipv4ddress: " << \
    queryApps.Get(0)->GetObject<RapidNetApplicationBase>()->GetAddress ();
  ofile << "\n";  
  ofile << "\n";
  ofile.close ();
}


void Print ()
{
  PrintRelation (apps, ModuleName::LINK);
  // PrintRelation (apps, ModuleName::RECVPACKET);
  PrintRelation (apps, ModuleName::PROVHASHTABLE);
  // PrintRelation (apps, ModuleName::FLOWENTRY);
  PrintRelation (apps, ModuleName::RULEEXEC);
  PrintRelation (apps, ModuleName::PROVLINK);
  PrintRelation (apps, ModuleName::PROVREF);
  // PrintRelation (apps, ModuleName::PROVSTR);
  // PrintRelation (apps, ModuleName::PROVRESULT);
  //  PrintRelation (apps, ModuleName::RULEINFO);

  //  PrintRelation (queryApps, QueryModuleName::RECORDS);
}

/*
 * Topology: 
 * Nodes: 1 -- 2 -- 3 
 *             |    |
 *             4 -- 5
 */

void BuildTopology()
{
  
  /* Connect nodes*/
  insert_link(1, 2);
  insert_link(2, 1);
  insert_link(2, 3); 
  insert_link(3, 2); 
  insert_link(2, 4); 
  insert_link(4, 2); 
  insert_link(3, 5); 
  insert_link(5, 3); 
  insert_link(4, 5); 
  insert_link(5, 4); 
}

void SetupFlowTable()
{
  /* Set up the initial flow entries */
  insert_flowentry(1, 5, 2);
  insert_flowentry(2, 5, 3);
  insert_flowentry(3, 5, 5);
  insert_flowentry(4, 5, 5);
}

void UpdateSlowChangingTable()
{
  /* Insert a new flow entry into node 2*/
  insert_updateroute(2, 5, 4);
}

void SetProgramID()
{
  insert_programid(1,5,"1");
}

void FirstPacketInsertion()
{
  insert_packet(1, 1, 5, "data");
}

void SecondPacketInsertion()
{
  insert_packet(1, 1, 5, "hello");
}

void QueryInsertion()
{
  Ptr<RapidNetApplicationBase> qnode = queryApps.Get(0)->GetObject<RapidNetApplicationBase>();
  insert_queryTuple("recvPacket", 5, 1, 5, "data");
  insert_queryTuple("recvPacket", 5, 1, 5, "hello");
}

void InitApps(int nodeNum)
{
  NodeContainer mainNodes;
  NodeContainer queryNode;

  mainNodes.Create(nodeNum);
  queryNode.Create(1);

  int totalNum = nodeNum + 1;

  NodeContainer ptpNodes;
  ptpNodes.Add(mainNodes);
  ptpNodes.Add(queryNode);

  PointToPointHelper ptpHelper;

  std::vector<NetDeviceContainer> devices;
  //Create a full mesh for all the nodes
  //The real connection is determined by the link table
  for (int i = 0;i < totalNum;i++)
  {
    for (int j = i+1;j < totalNum;j++)
    {
      NodeContainer src = ptpNodes.Get(i);
      NodeContainer dst = ptpNodes.Get(j);
      NodeContainer pair = NodeContainer(src, dst);
      NetDeviceContainer ptpDevice = ptpHelper.Install (pair);
      devices.push_back(ptpDevice);
    }
  }

  InternetStackHelper stack;
  stack.Install (ptpNodes);

  Ipv4AddressHelper address;
  std::vector<NetDeviceContainer>::iterator itr;
  int networkID = 0;
  for (itr = devices.begin(); itr != devices.end(); itr++)
  {
    std::ostringstream baseAddr;
    baseAddr << BASE_ADDR << networkID << ADDR_SUFFIX;
    std::cout << std::endl << baseAddr.str() << std::endl;
    address.SetBase (baseAddr.str().c_str(), NET_MASK);
    address.Assign (*itr);    
    networkID++;
  }
  
  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  apps = Create<ModuleHelper>()->Install(mainNodes);
  queryApps = Create<QueryModuleHelper>()->Install(queryNode);

  ShowAppAddr(nodeNum);

  SetMaxJitter (apps, 0.001);
  SetMaxJitter (queryApps, 0.001);
}

int
main (int argc, char *argv[])
{
  LogComponentEnable(ModuleNameStr, LOG_LEVEL_INFO);
  LogComponentEnable(QueryModuleNameStr, LOG_LEVEL_INFO);
  LogComponentEnable("RapidNetApplicationBase", LOG_LEVEL_INFO);

  int nodeNum = 5;
  InitApps(nodeNum);

  apps.Start (Seconds (0.0));
  apps.Stop (Seconds (500.0));
  queryApps.Start (Seconds(0.0));
  queryApps.Stop (Seconds (500.0));

  schedule (0.0001, BuildTopology);
  schedule (2.0001, SetProgramID);
  schedule (3.0000, SetupFlowTable);
  schedule (4.0000, FirstPacketInsertion);
  schedule (6.0000, UpdateSlowChangingTable);
  schedule (10.0000, FirstPacketInsertion);
  schedule (10.0000, SecondPacketInsertion);
  schedule (300.0000, QueryInsertion);
  schedule (499.0000, Print);

  Simulator::Run ();
  Simulator::Destroy ();

  return 0;
}

