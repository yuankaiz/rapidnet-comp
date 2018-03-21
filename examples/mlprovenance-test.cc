#include "ns3/core-module.h"
#include "ns3/simulator-module.h"
#include "ns3/node-module.h"
#include "ns3/mlprovenance-module.h"
#include "ns3/rapidnet-module.h"
#include "ns3/values-module.h"
#include <fstream>
#include <list>
#include <string>

#define clf(node,id) \
  ns3::rapidnet::tuple (Mlprovenance::CLF, \
    attr ("clf_attr1", Ipv4Value, node),\
    attr ("clf_attr2", Int32Value, id))

#define image(node,id, img) \
  ns3::rapidnet::tuple (Mlprovenance::IMAGE, \
    attr ("image_attr1", Ipv4Value, node),\
    attr ("image_attr2", Int32Value, id), \
    attr ("image_attr3", StrValue, img))

#define associationrule(node, item, rule, probability)\
  ns3::rapidnet::tuple (Mlprovenance::ASSOCIATIONRULE, \
    attr ("associationrule_attr1", Ipv4Value, node),\
    attr ("associationrule_attr2", StrValue, item),\
    attr ("associationrule_attr3", ListValue, rule),\
    attr ("associationrule_attr4", RealValue, probability))

#define insertclf(node, id) \
  app(node)->Insert (clf (addr(node), id))

#define insertimage(node,id, img) \
  app(node)->Insert (image (addr(node),id, img));

#define insertrule(node,item,rule,probability) \
  app(node)->Insert (associationrule(addr(node),item,rule,probability));
 
using namespace std;
using namespace ns3;
using namespace ns3::rapidnet;
using namespace ns3::rapidnet::mlprovenance;

ApplicationContainer apps;

void Print ()
{
  PrintRelation (apps, Mlprovenance::IMAGE);
  PrintRelation (apps, Mlprovenance::PREDICTION);
  PrintRelation (apps, Mlprovenance::IDENTIFIEDPATTERN);
  PrintRelation (apps, Mlprovenance::RESULT);
}

// Create a chain topology
void
UpdateTables1 ()
{
  insertclf(1, 1);
}

void
UpdateTables2 ()
{
  ifstream myfile;
  myfile.open("examples/association_rule_for_mlprovenance/association_rules.csv");
  string store;
  while(getline(myfile,store,',')){
    list<Ptr<Value> > rule;
    string itemlhs;
    int num_items = stoi(store);
    for(int x=0;x<num_items;x++){
      getline(myfile,store,',');
      rule.push_back(listattrdef(store,StrValue));
      if(x==0) itemlhs=store;
    }
    getline(myfile,store);
    float prob = stof(store);
    insertrule(1,itemlhs,rule,prob);
 }
}

static string image_names[] = {
    "cup.jpg",
    "dumbbell.jpeg",
    "barbell.png",
    "espresso.jpg",
    "llama.jpg",
    "mitten.jpeg",
    "paintbrush.png",
    "pillow.jpeg",
    "sock.jpeg",
    "teapot.jpg",
    "tractor.jpg",
    "diaper.jpg"
};

static int inserted_i = 0;

void insertNextImage()
{
  insertimage(1, inserted_i + 1, "/home/iped/nfs/rapidnet/images/" + image_names[inserted_i]);
  inserted_i++;
}

void SerializeProv(string storepath)
{
  vector<string> relNames;
  relNames.push_back("ruleExec");
  relNames.push_back("prov");
  app(1)->SerializeRel(relNames, 1, storepath);
}

int
main (int argc, char *argv[])
{
  /* LogComponentEnable("Mlprovenance", LOG_LEVEL_INFO); */
  /* LogComponentEnable("RapidNetApplicationBase", LOG_LEVEL_INFO); */

  string storepath = "output/prov/";
  std::ofstream ofs;
  ofs.open("output/prov/timing", std::ofstream::out | std::ofstream::trunc);
  ofs.close();

  apps = InitRapidNetApps (1, Create<MlprovenanceHelper> ());
  apps.Start (Seconds (0.0));
  apps.Stop (Seconds (2000.0));

  /* schedule (0.0, UpdateTables); */
  schedule (1.0, UpdateTables1);
  schedule (2.0, UpdateTables2);

  for (int i=0; i < sizeof(image_names) / sizeof(*image_names); i++) {
      schedule(10.0 * i, insertNextImage);
  }

  /* schedule (1000.0, Print); */
  Simulator::Schedule (Seconds(1900.0), SerializeProv, storepath);

  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}

