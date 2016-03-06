#ifndef FIREWALLWEAK_HELPER_H
#define FIREWALLWEAK_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "firewall-weak.h"

namespace ns3 {
namespace rapidnet {
namespace firewallweak {

class FirewallWeak;

class FirewallWeakHelper: public RapidNetApplicationHelper
{
public:
  FirewallWeakHelper ()
  {
    m_factory.SetTypeId (FirewallWeak::GetTypeId ());
  }
  virtual ~FirewallWeakHelper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<FirewallWeak> ();
  }
};

} // namespace firewallweak
} // namespace rapidnet
} // namespace ns3

#endif // FIREWALLWEAK_HELPER_H

