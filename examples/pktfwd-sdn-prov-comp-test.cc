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
#include "ns3/pktfwd-sdn-prov-comp-module.h"
#include "ns3/rapidnet-module.h"
#include "ns3/values-module.h"
#include "ns3/helper-module.h"

/* Device identification*/
#define device(host, dvtype)\
  tuple(PktfwdSdnProvComp::DEVICE,\
  attr("device_attr1", Ipv4Value, host),  \
  attr("device_attr2", Int32Value, dvtype))

#define insert_device(host, dvtype)                                            \
  app(host) -> Insert(device(addr(host), dvtype));


/* Links connecting hosts to switches*/
#define linkhr(host, sw)\
  tuple(PktfwdSdnProvComp::LINKHR,\
  attr("linkhr_attr1", Ipv4Value, host),  \
  attr("linkhr_attr2", Ipv4Value, sw))

#define insert_linkhr(host, sw)                                            \
  app(host) -> Insert(linkhr(addr(host), addr(sw)));

/* Links connecting switches to other devices*/
#define link(sw, nei)\
  tuple(PktfwdSdnProvComp::LINK,\
  attr("link_attr1", Ipv4Value, sw),  \
  attr("link_attr2", Ipv4Value, nei))

#define insert_link(sw, nei)                                            \
  app(sw) -> Insert(link(addr(sw), addr(nei)));

/* Input packets */
#define initpacket(host, srcadd, dstadd)         \
  tuple(PktfwdSdnProvComp::INITPACKET,\
  attr("initPacket_attr1", Ipv4Value, host),  \
  attr("initPacket_attr2", Ipv4Value, srcadd), \
  attr("initPacket_attr3", Ipv4Value, dstadd))

#define insert_packet(host, srcadd, dstadd)                            \
  app(host) -> Insert(initpacket(addr(host), addr(srcadd), addr(dstadd)));

/* Max priority */
#define maxPriority(sw, priority)\
  tuple(PktfwdSdnProvComp::MAXPRIORITY,\
	attr("maxPriority_attr1", Ipv4Value, sw),\
	attr("maxPriority_attr2", Int32Value, priority))

#define insert_priority(sw, priority)\
  app(sw) -> Insert(maxPriority(addr(sw), priority));

/* flow entry */
#define flowentry(sw, dst, next, priority)		\
  tuple(PktfwdSdnProvComp::FLOWENTRY,\
	attr("flowEntry_attr1", Ipv4Value, sw),\
	attr("flowEntry_attr2", Ipv4Value, dst),         \
	attr("flowEntry_attr3", Ipv4Value, next),	\
	attr("flowEntry_attr4", Int32Value, priority))

#define insert_flowentry(sw, dst, next, priority)				\
  app(sw) -> Insert(flowentry(addr(sw), addr(dst), addr(next), priority))

#define SWITCH 0
#define HOST 1

using namespace std;
using namespace ns3;
using namespace ns3::rapidnet;
using namespace ns3::rapidnet::pktfwdsdnprovcomp;

ApplicationContainer apps;

void Print ()
{
  PrintRelation (apps, PktfwdSdnProvComp::RECVPACKET);
  PrintRelation (apps, PktfwdSdnProvComp::RULEEXEC);
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
  insert_priority(5, 2);
  insert_flowentry(5, 4, 6, 2);
  insert_flowentry(5, 3, 6, 1);
  insert_flowentry(5, 1, 6, 0);

  /* Set up the flow entry for switch 6*/
  insert_priority(6, 2);
  insert_flowentry(6, 3, 3, 2);
  insert_flowentry(6, 4, 4, 1);
  insert_flowentry(6, 1, 5, 0);
}

void PacketInsertion()
{
  insert_packet(1, 1, 4);
  insert_packet(1, 1, 3);  
}

int
main (int argc, char *argv[])
{
  LogComponentEnable("PktfwdSdnProvComp", LOG_LEVEL_INFO);
  LogComponentEnable("RapidNetApplicationBase", LOG_LEVEL_INFO);

  int nodeNum = 6;
  apps = InitRapidNetApps (nodeNum, Create<PktfwdSdnProvCompHelper> ());

  apps.Start (Seconds (0.0));
  apps.Stop (Seconds (20.0));

  schedule (0.0001, BuildTopology);
  schedule (0.0010, SetupDevices);
  schedule (3.0000, SetupFlowTable);
  schedule (4.0000, PacketInsertion);
  schedule (19.0000, Print);

  Simulator::Run ();
  Simulator::Destroy ();

  return 0;
}

