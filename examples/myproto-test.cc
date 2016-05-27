#include "ns3/core-module.h"
#include "ns3/simulator-module.h"
#include "ns3/node-module.h"
#include "ns3/myproto-module.h"
#include "ns3/rapidnet-module.h"
#include "ns3/values-module.h"

#define link(src, next, cost) \
  tuple (Myproto::LINK, \
    attr ("link_attr1", Ipv4Value, src), \
    attr ("link_attr2", Ipv4Value, next), \
    attr ("link_attr3", Int32Value, cost))

#define insertlink(from, to, cost) \
  app(from)->Insert (link (addr (from), addr (to), cost)); \
  app(to)->Insert (link (addr (to), addr (from), cost));

#define deletelink(from, to, cost) \
  app(from)->Delete (link (addr (from), addr (to), cost)); \
  app(to)->Delete (link (addr (to), addr (from), cost));

using namespace std;
using namespace ns3;
using namespace ns3::rapidnet;
using namespace ns3::rapidnet::myproto;

ApplicationContainer apps;

void Print ()
{
  PrintRelation (apps, Myproto::PATH);
  PrintRelation (apps, Myproto::BESTPATH);
}

// Create a chain topology
void
UpdateLinks1 ()
{
  insertlink (1, 2, 7);
  insertlink (2, 3, 4)
  insertlink (3, 4, 2);
  insertlink (4, 5, 6);
}

int
main (int argc, char *argv[])
{
  LogComponentEnable("Myproto", LOG_LEVEL_INFO);
  LogComponentEnable("RapidNetApplicationBase", LOG_LEVEL_INFO);

  apps = InitRapidNetApps (5, Create<MyprotoHelper> ());
  apps.Start (Seconds (0.0));
  apps.Stop (Seconds (10.0));

  schedule (0.0, UpdateLinks1);
  schedule (10.0, Print);

  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}
