#ifndef PKTFWDNORMDISTHLISTRMITMAUXTABLES_HELPER_H
#define PKTFWDNORMDISTHLISTRMITMAUXTABLES_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "pktfwd-norm-dist-hlist-rmitm-auxtables.h"

namespace ns3 {
namespace rapidnet {
namespace pktfwdnormdisthlistrmitmauxtables {

class PktfwdNormDistHlistRmitmAuxtables;

class PktfwdNormDistHlistRmitmAuxtablesHelper: public RapidNetApplicationHelper
{
public:
  PktfwdNormDistHlistRmitmAuxtablesHelper ()
  {
    m_factory.SetTypeId (PktfwdNormDistHlistRmitmAuxtables::GetTypeId ());
  }
  virtual ~PktfwdNormDistHlistRmitmAuxtablesHelper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<PktfwdNormDistHlistRmitmAuxtables> ();
  }
};

} // namespace pktfwdnormdisthlistrmitmauxtables
} // namespace rapidnet
} // namespace ns3

#endif // PKTFWDNORMDISTHLISTRMITMAUXTABLES_HELPER_H

