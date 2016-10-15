#ifndef PKTFWDONLINEBCASTSLOWCHANGEUPDATEQUERY_HELPER_H
#define PKTFWDONLINEBCASTSLOWCHANGEUPDATEQUERY_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "pktfwd-online-bcast-slowchange-update-query.h"

namespace ns3 {
namespace rapidnet {
namespace pktfwdonlinebcastslowchangeupdatequery {

class PktfwdOnlineBcastSlowchangeUpdateQuery;

class PktfwdOnlineBcastSlowchangeUpdateQueryHelper: public RapidNetApplicationHelper
{
public:
  PktfwdOnlineBcastSlowchangeUpdateQueryHelper ()
  {
    m_factory.SetTypeId (PktfwdOnlineBcastSlowchangeUpdateQuery::GetTypeId ());
  }
  virtual ~PktfwdOnlineBcastSlowchangeUpdateQueryHelper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<PktfwdOnlineBcastSlowchangeUpdateQuery> ();
  }
};

} // namespace pktfwdonlinebcastslowchangeupdatequery
} // namespace rapidnet
} // namespace ns3

#endif // PKTFWDONLINEBCASTSLOWCHANGEUPDATEQUERY_HELPER_H

