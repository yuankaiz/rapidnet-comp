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
#include "ns3/pktfwd-norm-module.h"
#include "ns3/rapidnet-module.h"
#include "ns3/values-module.h"
#include "ns3/helper-module.h"
#include "ns3/gnuplot.h"

/* Device identification*/
#define device(host, dvtype)\
  tuple(PktfwdNorm::DEVICE,\
  attr("device_attr1", Ipv4Value, host),  \
  attr("device_attr2", Int32Value, dvtype))

#define insert_device(host, dvtype)                                            \
  app(host) -> Insert(device(addr(host), dvtype));

/* Links connecting hosts to switches*/
#define linkhr(host, sw)\
  tuple(PktfwdNorm::LINKHR,\
  attr("linkhr_attr1", Ipv4Value, host),  \
  attr("linkhr_attr2", Ipv4Value, sw))

#define insert_linkhr(host, sw)                                            \
  app(host) -> Insert(linkhr(addr(host), addr(sw)));

/* Links connecting switches to other devices*/
#define link(sw, nei)\
  tuple(PktfwdNorm::LINK,\
  attr("link_attr1", Ipv4Value, sw),  \
  attr("link_attr2", Ipv4Value, nei))

#define insert_link(sw, nei)                                            \
  app(sw) -> Insert(link(addr(sw), addr(nei)));

/* Input packets */
#define initpacket(host, srcadd, dstadd, data)         \
  tuple(PktfwdNorm::INITPACKET,\
  attr("initPacket_attr1", Ipv4Value, host),  \
  attr("initPacket_attr2", Ipv4Value, srcadd), \
  attr("initPacket_attr3", Ipv4Value, dstadd), \
  attr("initPacket_attr4", StrValue, data))

#define insert_packet(host, srcadd, dstadd, data)                            \
  app(host) -> Insert(initpacket(addr(host), addr(srcadd), addr(dstadd), data));

/* flow entry */
#define flowentry(sw, dst, next)		\
  tuple(PktfwdNorm::FLOWENTRY,\
	attr("flowEntry_attr1", Ipv4Value, sw),\
	attr("flowEntry_attr2", Ipv4Value, dst),         \
	attr("flowEntry_attr3", Ipv4Value, next))

#define insert_flowentry(sw, dst, next)				\
  app(sw) -> Insert(flowentry(addr(sw), addr(dst), addr(next)))

#define SWITCH 0
#define HOST 1
#define NET_MASK "255.255.255.0"
#define BASE_ADDR "10.0."
#define ADDR_SUFFIX ".0"
#define SIMULATION_LENGTH 10

const string plotFileName = "bandwidth_100n.plt";
const string plotTitle = "bandwidth usage";
const string dataTitle = "bandwidth_data";

using namespace std;
using namespace ns3;
using namespace ns3::rapidnet;
using namespace ns3::rapidnet::pktfwdnorm;

ApplicationContainer apps;

int m_bytesTotal = 0;

void Print ()
{
  PrintRelation (apps, PktfwdNorm::RECVPACKET);
}

void SetupDevices()
{
  insert_device(1, HOST);
  insert_device(2, HOST);
  insert_device(3, HOST);
  insert_device(4, HOST);
  insert_device(5, SWITCH);
  insert_device(6, SWITCH);      
}

/*
 * Topology: 
 * Hosts: 1, 2, 3, 4
 * Switches: 5, 6
 */
void BuildTopology()
{
  
  /* Connect hosts to switches */
  insert_linkhr(1, 5);
  insert_linkhr(4, 6);
  insert_linkhr(2, 5);
  insert_linkhr(3, 6);

  /* Connect switches to hosts and each other*/
  insert_link(5, 1);
  insert_link(5, 2);
  insert_link(5, 6);    
  insert_link(6, 3);
  insert_link(6, 4);
  insert_link(6, 5);
}

void SetupFlowTable()
{
  /* Set up the flow entry for switch 5*/
  insert_flowentry(5, 4, 6);
  insert_flowentry(5, 3, 6);
  insert_flowentry(5, 1, 6);

  /* Set up the flow entry for switch 6*/
  insert_flowentry(6, 3, 3);
  insert_flowentry(6, 4, 4);
  insert_flowentry(6, 1, 5);
}

void ShowAppAddr(ApplicationContainer& apps, int totalNum)
{
  std::ofstream ofile("addr_mapping.txt");
  ofile << "\n" << "Mapping: (Node ID -> Application address) " << "\n";
  for (int i = 0;i < totalNum;i++)
    {
      int nodeID = i + 1;
      ofile << "Node ID: " << nodeID;
      ofile << " -> ";
      ofile << "App Ipv4ddress: " << app(nodeID)->GetAddress ();
      ofile << "\n";
    }
  ofile << "\n";
  ofile.close ();
}

void PacketInsertion()
{
  insert_packet(1, 1, 4, "data");
}

void MeasureBandwidth(std::string context, Ptr< const Packet > packet)
{
  m_bytesTotal += packet->GetSize ();
}

void Throughput(Gnuplot2dDataset& dataset)
{
  if (Simulator::Now () > Seconds (SIMULATION_LENGTH))
    {
      return;
    }
  
  double seconds = Simulator::Now().GetSeconds ();
  dataset.Add (seconds, m_bytesTotal/2);
  std::cout << std::endl << "Throughput: " << m_bytesTotal/2 << std::endl;
  m_bytesTotal = 0;
  Simulator::Schedule (Seconds (2), Throughput, dataset);
}

int
main (int argc, char *argv[])
{
  LogComponentEnable("PktfwdNorm", LOG_LEVEL_INFO);
  LogComponentEnable("RapidNetApplicationBase", LOG_LEVEL_INFO);
  //  LogComponentEnable("Ipv4ListRouting", LOG_LEVEL_INFO);

  int nodeNum = 6;
  //  apps = InitRapidNetApps (nodeNum, Create<PktfwdNormHelper> ());

  /* Point-to-point device model*/
  Ptr<RapidNetApplicationHelper> appHelper;
  appHelper = Create<PktfwdNormHelper> ();

  NodeContainer ptpNodes;
  ptpNodes.Create (nodeNum);

  PointToPointHelper ptpHelper;
  // ptpHelper.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  // ptpHelper.SetChannelAttribute ("Delay", StringValue ("2ms"));
  std::vector<NetDeviceContainer> devices;
  //Create a full mesh for all the nodes
  //The real connection is determined by the link table
  for (int i = 0;i < nodeNum;i++)
  {
    for (int j = i+1;j < nodeNum;j++)
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

  apps = appHelper->Install (ptpNodes);
  
  ShowAppAddr(apps, nodeNum);

  apps.Start (Seconds (0.0));
  apps.Stop (Seconds (SIMULATION_LENGTH));

  schedule (0.0001, BuildTopology);
  schedule (0.0010, SetupDevices);
  schedule (3.0000, SetupFlowTable);
  schedule (4.0000, PacketInsertion);
  schedule (SIMULATION_LENGTH, Print);

  string config_path = "/NodeList/*/DeviceList/*/$ns3::PointToPointNetDevice/PhyRxEnd";
  Config::Connect (config_path, MakeCallback (&MeasureBandwidth));

  // string config_path2 = "/NodeList/*/DeviceList/*/$ns3::PointToPointNetDevice/PhyTxEnd";
  // Config::Connect (config_path2, MakeCallback (&MeasureSize));

  //Draw the graph
  Gnuplot plot (plotFileName);
  plot.SetTitle (plotTitle);
  plot.SetLegend ("Time (s)", "Kbps");

  //Create dataset
  Gnuplot2dDataset dataset;
  dataset.SetTitle (dataTitle);
  dataset.SetStyle (Gnuplot2dDataset::LINES_POINTS);

  Simulator::Schedule (Seconds (2), Throughput, dataset);  

  Simulator::Run ();
  Simulator::Destroy ();

  //Plot the graph
  plot.AddDataset (dataset);
  std:ofstream plotFile (plotFileName.c_str());
  plot.GenerateOutput (plotFile);
  plotFile.close ();

  return 0;
}

