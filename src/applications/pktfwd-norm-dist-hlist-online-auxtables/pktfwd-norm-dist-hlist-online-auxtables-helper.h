#ifndef PKTFWDNORMDISTHLISTONLINEAUXTABLES_HELPER_H
#define PKTFWDNORMDISTHLISTONLINEAUXTABLES_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "pktfwd-norm-dist-hlist-online-auxtables.h"

namespace ns3 {
namespace rapidnet {
namespace pktfwdnormdisthlistonlineauxtables {

class PktfwdNormDistHlistOnlineAuxtables;

class PktfwdNormDistHlistOnlineAuxtablesHelper: public RapidNetApplicationHelper
{
public:
  PktfwdNormDistHlistOnlineAuxtablesHelper ()
  {
    m_factory.SetTypeId (PktfwdNormDistHlistOnlineAuxtables::GetTypeId ());
  }
  virtual ~PktfwdNormDistHlistOnlineAuxtablesHelper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<PktfwdNormDistHlistOnlineAuxtables> ();
  }
};

} // namespace pktfwdnormdisthlistonlineauxtables
} // namespace rapidnet
} // namespace ns3

#endif // PKTFWDNORMDISTHLISTONLINEAUXTABLES_HELPER_H

