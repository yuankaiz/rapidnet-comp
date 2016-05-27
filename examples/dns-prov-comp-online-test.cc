#include "ns3/core-module.h"
#include "ns3/simulator-module.h"
#include "ns3/node-module.h"
#include "ns3/dns-prov-comp-online-module.h"
#include "ns3/rapidnet-module.h"
#include "ns3/values-module.h"

#include<iostream>
#include<list>
#include<time.h>
#include<sstream>

using namespace std;
using namespace ns3;
using namespace ns3::rapidnet;
using namespace ns3::rapidnet::dnsprovcomponline;

//MACROS

#define url_tuple(src,url,host)		\
  tuple(DnsProvCompOnline::URL,\
	attr("url_attr1",Ipv4Value,src),\
	attr("url_attr2",StrValue,url),\
	attr("url_attr3",Ipv4Value,host))

#define name_server_tuple(src,domain,server)\
  tuple(DnsProvCompOnline::NAME_SERVER,\
	attr("name_server_attr1",Ipv4Value,src),\
	attr("name_server_attr2",StrValue,domain),\
	attr("name_server_attr3",StrValue,server))

#define address_record_tuple(src,server,address)\
  tuple(DnsProvCompOnline::ADDRESS_RECORD,\
	attr("address_record_attr1",Ipv4Value,src),\
	attr("address_record_attr2",StrValue,server),\
	attr("address_record_attr3",Ipv4Value,address))

#define result_tuple(host,url,address)		\
  tuple(DnsProvCompOnline::RESULT,\
	attr("result_attr1",Ipv4Value,src),\
	attr("result_attr2",StrValue,url),\
	attr("result_attr3",Ipv4Value,host))

#define insertURL(src,url,host)				\
  app(src)->Insert(url_tuple(addr(src),url,addr(host)))

#define insertNameServer(src,domain,server)\
  app(src)->Insert(name_server_tuple(addr(src),domain,server))

#define insertAddressRecord(src,server,address)\
  app(src)->Insert(address_record_tuple(addr(src),server,addr(address)))

#define insertResult(host,url,address)					\
  app(host)->Insert(result_record(addr(host),url,addr(address)))


ApplicationContainer apps;

void Print()
{
  //PrintRelation(apps,Dns::ADDRESS_RECORD);
  //	cout<<endl;
  //	PrintRelation(apps,Dns::NAME_SERVER);
  //	cout<<endl;
  PrintRelation(apps,DnsProvCompOnline::RESULTS);
}


void generateTree(int *nextNodeID, string currentPath, list<string> *pathList, int level,\
			int minFanout, int maxFanout)
{
	if(level > 0)
	{
		int fanout = rand()%(maxFanout-minFanout)+minFanout;
		int i=0;
		int rootID = *nextNodeID;

		for(i=0;i<fanout;i++)
		{
			(*nextNodeID)++;
			if(level-1 > 0)
			{
			  //cout<<"Adding at "<<rootID<<" "<<"."<<*nextNodeID<<currentPath<<endl;
				stringstream newPath;
				newPath<<"."<<*nextNodeID<<currentPath;	
				insertNameServer(rootID,newPath.str(),newPath.str()+"server");
				insertAddressRecord(rootID,newPath.str()+"server",*nextNodeID);
				generateTree(nextNodeID,newPath.str(),pathList,level-1,minFanout,maxFanout);
			}
			else
			{ 
			  //cout<<"Adding at "<<rootID<<" "<<*nextNodeID<<currentPath<<endl;
				stringstream newPath;
				newPath<<*nextNodeID<<currentPath;
				insertNameServer(rootID,newPath.str(),newPath.str()+"server");
				insertAddressRecord(rootID,newPath.str()+"server",*nextNodeID);
				generateTree(nextNodeID,newPath.str(),pathList,level-1,minFanout,maxFanout);
			}	
		}
	}
	else
	{
		(*pathList).push_back(currentPath);
	}
}

void UpdateTable()
{
  //Final results will be at 1
  //list< Ptr<Value> > path;
  /*insertURL(2,"mailto.cis.upenn.edu",1);

  //list< Ptr<Value> > path2;
  insertURL(2,"www.csail.mit.edu",1);

  insertURL(2,"www.ibm.net",1);

  //Create root node
  insertNameServer(2,".edu","edu.server");
  insertAddressRecord(2,"edu.server",3);

  insertNameServer(2,".com","com.server");
  insertAddressRecord(2,"com.server",4);

  insertNameServer(2,".net","net.server");
  insertAddressRecord(2,"net.server",5);

  //edu server
  insertNameServer(3,".upenn.edu","upenn.edu.server");
  insertAddressRecord(3,"upenn.edu.server",6);
  
  insertNameServer(3,".mit.edu","mit.edu.server");
  insertAddressRecord(3,"mit.edu.server",7);

  //Com server
  insertNameServer(4,".google.com","google.com.server");
  insertAddressRecord(4,"google.com.server",8);

  insertNameServer(4,".yahoo.com","yahoo.com.server");
  insertAddressRecord(4,"yahoo.com.server",9);

  //net server
  insertNameServer(5,".ibm.net","ibm.net.server");
  insertAddressRecord(5,"ibm.net.server",10);

  //upenn server
  insertNameServer(6,".cis.upenn.edu","cis.upenn.server");
  insertAddressRecord(6,"cis.upenn.server",11);

  insertNameServer(6,".sas.upenn.edu","sas.upenn.edu.server");
  insertAddressRecord(6,"sas.upenn.edu.server",12);

  //mit server
  insertNameServer(7,".csail.mit.edu","csail.mit.edu.server");
  insertAddressRecord(7,"csail.mit.edu.server",13);

  //google server
  insertNameServer(8,"www.google.com","www.google.com.server");
  insertAddressRecord(8,"www.google.com.server",14);

  //yahoo server
  insertNameServer(9,"www.yahoo.com","www.yahoo.com.server");
  insertAddressRecord(9,"www.yahoo.com.server",15);

  //ibm server
  insertNameServer(10,"www.ibm.net","www.ibm.net.server");
  insertAddressRecord(10,"www.ibm.net.server",16);

  //cis.upenn.edu server
  insertNameServer(11,"www.cis.upenn.edu","www.cis.upenn.edu.server");
  insertAddressRecord(11,"www.cis.upenn.edu.server",17);

  insertNameServer(11,"mailto.cis.upenn.edu","mailto.cis.upenn.edu.server");
  insertAddressRecord(11,"mailto.cis.upenn.edu.server",18);

  //sas.upenn.edu server
  insertNameServer(12,"www.sas.upenn.edu","www.sas.upenn.edu.server");
  insertAddressRecord(12,"www.sas.upenn.edu.server",19);

  //csail.mit.edu server
  insertNameServer(13,"www.csail.mit.edu","www.csail.mit.edu.server");
  insertAddressRecord(13,"www.csail.mit.edu.server",20);*/
  
  int nextNodeID = 2;
  int rootNodeID = 2;
  string path = ".";
  list<string> pathList;

  int minDepth = 3;//atoi(argv[1]);
  int maxDepth = 4;//atoi(argv[2]);
		
  int level = rand()%(maxDepth-minDepth)+minDepth;
  //cout<<"HEIGHT "<<level<<endl;
  int minFanout = 3;//atoi(argv[3]);
  int maxFanout = 4;//atoi(argv[4]);
  generateTree(&nextNodeID,path,&pathList,level,minFanout,maxFanout);
  list<string>::iterator iter = pathList.begin();
  int resultNodeID = 1;
  for(;iter!=pathList.end();iter++)
    insertURL(rootNodeID,*iter,resultNodeID);
  
}


int main(int argc,char *argv[])
{
  LogComponentEnable("DnsProvCompOnline", LOG_LEVEL_INFO);
  LogComponentEnable("RapidNetApplicationBase", LOG_LEVEL_INFO);

  apps = InitRapidNetApps (50, Create<DnsProvCompOnlineHelper> ());
  apps.Start (Seconds (0.0));
  apps.Stop (Seconds (30.0));

  schedule (0.0, UpdateTable);
  schedule (30.0, Print);
  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}
