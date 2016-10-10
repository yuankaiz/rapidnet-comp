#ifndef PKTFWDNORMONLINEQUERYINIT_HELPER_H
#define PKTFWDNORMONLINEQUERYINIT_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "pktfwd-norm-online-query-init.h"

namespace ns3 {
namespace rapidnet {
namespace pktfwdnormonlinequeryinit {

class PktfwdNormOnlineQueryInit;

class PktfwdNormOnlineQueryInitHelper: public RapidNetApplicationHelper
{
public:
  PktfwdNormOnlineQueryInitHelper ()
  {
    m_factory.SetTypeId (PktfwdNormOnlineQueryInit::GetTypeId ());
  }
  virtual ~PktfwdNormOnlineQueryInitHelper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<PktfwdNormOnlineQueryInit> ();
  }
};

} // namespace pktfwdnormonlinequeryinit
} // namespace rapidnet
} // namespace ns3

#endif // PKTFWDNORMONLINEQUERYINIT_HELPER_H

