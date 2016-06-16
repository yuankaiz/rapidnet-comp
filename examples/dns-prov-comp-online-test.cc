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
#include<vector>
#include "stdlib.h"
#include "fstream"

using namespace std;
using namespace ns3;
using namespace ns3::rapidnet;
using namespace ns3::rapidnet::dnsprovcomponline;

//MACROS

ApplicationContainer apps;

int totalNumNodes;

#define url_tuple(src,url,host,requestID)		\
  tuple(DnsProvCompOnline::URL,\
	attr("url_attr1",Ipv4Value,src),\
	attr("url_attr2",StrValue,url),\
	attr("url_attr3",Ipv4Value,host),\
	attr("url_attr4",Int32Value,requestID))

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

#define insertURL(src,url,host,requestID)			\
  app(src)->Insert(url_tuple(addr(src),url,addr(host),requestID))

#define insertNameServer(src,domain,server)\
  app(src)->Insert(name_server_tuple(addr(src),domain,server))

#define insertAddressRecord(src,server,address)\
  app(src)->Insert(address_record_tuple(addr(src),server,addr(address)))

#define insertResult(host,url,address)					\
  app(host)->Insert(result_record(addr(host),url,addr(address)))




void Print()
{
  //PrintRelation(apps,Dns::ADDRESS_RECORD);
  //	cout<<endl;
  //	PrintRelation(apps,Dns::NAME_SERVER);
  //	cout<<endl;
  PrintRelation(apps,DnsProvCompOnline::RESULTS);
}

void insertURLTuple(int src, string url, int host, int requestID)
{
  insertURL(src,url,host,requestID);
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



struct probvals
{
  float prob;
  float cum_prob;
};

void get_zipf(float theta, int N, struct probvals *zdist)
{
   float sum = 0.0;
  float c = 0.0;
  float expo;
  float sumc = 0.0;

  int i=0;
  
  expo = 1- theta;
  
  for(i=1; i<=N;i++)
    sum+=1.0/(float)pow((double)i,(double)expo);

  c = 1.0/sum;

  for(i=0; i<N; i++)
    {
      zdist[i].prob = c/(float)pow((double)(i+1),(double)(expo));
      sumc += zdist[i].prob;
      zdist[i].cum_prob = sumc;
    }
}

void insertRandomURL(list<string> pathList)
{
  
  
  int rootNodeID = 2;
  int resultNodeID = 1;
  struct probvals *zdist;

  int N = pathList.size();
  zdist = (struct probvals *)malloc(N*sizeof(struct probvals));

  get_zipf(0.001,pathList.size(),zdist);

  //list<string>::iterator iter = pathList.begin();
  //int j=0;
  //for(;j<N;j++)
  //cout<<zdist[j].cum_prob<<endl;
  int numURL = 100;
  double timer = 0;
  while(numURL > 0)
    {
      double probRand = ((double) rand() / (RAND_MAX));
      
      int i=0;

      for(;i<pathList.size();i++)
	{
	  if(zdist[i].cum_prob >= probRand)
	    {
	      //cout<<i<<" "<<zdist[i].cum_prob<<" "<<probRand<<endl;
	      break;
	    }
	}

      list<string>::iterator iter = pathList.begin();

      for(;iter!=pathList.end();iter++)
	{
	  if(i == 0)
	    {
	      Simulator::Schedule(Seconds(timer),insertURLTuple,rootNodeID,*iter,resultNodeID,numURL);
	      //insertURL(rootNodeID,*iter,resultNodeID,numURL);
	      //delay(1);
	      break;
	    }
	  i--;
	}
      numURL--;
      timer+=0.001;
    }
    free(zdist);
}


void UpdateTable()
{
 
  int nextNodeID = 2;
  int rootNodeID = 2;
  string path = ".";
  list<string> pathList;

  int minDepth = 9;//atoi(argv[1]);
  int maxDepth = 10;//atoi(argv[2]);
		
  int level = rand()%(maxDepth-minDepth)+minDepth;
  //cout<<"HEIGHT "<<level<<endl;
  int minFanout = 2;//atoi(argv[3]);
  int maxFanout = 3;//atoi(argv[4]);
  generateTree(&nextNodeID,path,&pathList,level,minFanout,maxFanout);
  totalNumNodes = nextNodeID;
  list<string>::iterator iter = pathList.begin();
  int resultNodeID = 1;
  int counter = 0;
  double timer = 0;
  for(;counter<50;counter++)
    {
      int numRequests = 0;
      for(;numRequests < 10;numRequests++,timer+=0.01)
	{
	   Simulator::Schedule(Seconds(timer),insertURLTuple,rootNodeID,*iter,resultNodeID,numRequests);
	   //timer+=1;
	}
      iter++;
      }
  //for(;iter!=pathList.end();iter++)
  //insertURL(rootNodeID,*iter,resultNodeID);
  //insertRandomURL(pathList);
  
}



void SerializeProv(string storePath)
{
  cout<<"NUM NODES "<<totalNumNodes<<endl;
  cout<<"STORE PATH "<<storePath<<endl;
  vector<string> relNames;
  relNames.push_back("ruleExec");
  //relNames.push_back("prov");
  relNames.push_back("provHashTable");
  relNames.push_back("equiHashTable");
  relNames.push_back("resultsAux");

  for (int i = 2; i < totalNumNodes+1; i++)
    {
      int node = i;
      
      app(node) -> SerializeRel(relNames, node, storePath);
      
    }
}


/*int main(int argc,char *argv[])
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
  }*/



int main(int argc,char *argv[])
{
  LogComponentEnable("DnsProvCompOnline", LOG_LEVEL_INFO);
  LogComponentEnable("RapidNetApplicationBase", LOG_LEVEL_INFO);
  

  string storePath = "/localdrive1/harshal/prov_test_online/";
  
  CommandLine cmd;
  cmd.AddValue("storePth","The path to the directory for provenance storage",storePath);

  cmd.Parse(argc,argv);


  NodeContainer csmaNodes;
  csmaNodes.Create (1024);

  CsmaHelper csma;
  csma.SetDeviceAttribute ("EncapsulationMode", StringValue ("Dix"));
  csma.SetDeviceAttribute ("FrameSize", UintegerValue (64000));

  NetDeviceContainer csmaDevices;
  csmaDevices = csma.Install (csmaNodes);

  InternetStackHelper stack;
  stack.Install (csmaNodes);

  Ipv4AddressHelper address;

  address.SetBase ("192.0.0.0", "255.0.0.0");
  address.Assign (csmaDevices);

  Ptr<RapidNetApplicationHelper> appHelper = Create<DnsProvCompOnlineHelper> ();
  apps = appHelper->Install (csmaNodes);


  //apps = InitRapidNetApps (800, Create<DnsProvHelper> ());
  apps.Start (Seconds (0.0));
  apps.Stop (Seconds (500.0));
  
  schedule (0.0, UpdateTable);
  Simulator::Schedule (Seconds(499.0000), SerializeProv, storePath);
  schedule (500.0, Print);
  Simulator::Run ();
  //cout<<"Total Number Nodes "<<totalNumNodes<<endl;
  Simulator::Destroy ();
  return 0;
}
