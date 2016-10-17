#ifndef PKTFWDONLINEBCASTSLOWCHANGEUPDATE_HELPER_H
#define PKTFWDONLINEBCASTSLOWCHANGEUPDATE_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "pktfwd-online-bcast-slowchange-update.h"

namespace ns3 {
namespace rapidnet {
namespace pktfwdonlinebcastslowchangeupdate {

class PktfwdOnlineBcastSlowchangeUpdate;

class PktfwdOnlineBcastSlowchangeUpdateHelper: public RapidNetApplicationHelper
{
public:
  PktfwdOnlineBcastSlowchangeUpdateHelper ()
  {
    m_factory.SetTypeId (PktfwdOnlineBcastSlowchangeUpdate::GetTypeId ());
  }
  virtual ~PktfwdOnlineBcastSlowchangeUpdateHelper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<PktfwdOnlineBcastSlowchangeUpdate> ();
  }
};

} // namespace pktfwdonlinebcastslowchangeupdate
} // namespace rapidnet
} // namespace ns3

#endif // PKTFWDONLINEBCASTSLOWCHANGEUPDATE_HELPER_H

