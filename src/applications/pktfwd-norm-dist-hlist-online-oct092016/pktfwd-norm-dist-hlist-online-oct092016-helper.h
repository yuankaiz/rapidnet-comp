#ifndef PKTFWDNORMDISTHLISTONLINEOCT092016_HELPER_H
#define PKTFWDNORMDISTHLISTONLINEOCT092016_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "pktfwd-norm-dist-hlist-online-oct092016.h"

namespace ns3 {
namespace rapidnet {
namespace pktfwdnormdisthlistonlineoct092016 {

class PktfwdNormDistHlistOnlineOct092016;

class PktfwdNormDistHlistOnlineOct092016Helper: public RapidNetApplicationHelper
{
public:
  PktfwdNormDistHlistOnlineOct092016Helper ()
  {
    m_factory.SetTypeId (PktfwdNormDistHlistOnlineOct092016::GetTypeId ());
  }
  virtual ~PktfwdNormDistHlistOnlineOct092016Helper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<PktfwdNormDistHlistOnlineOct092016> ();
  }
};

} // namespace pktfwdnormdisthlistonlineoct092016
} // namespace rapidnet
} // namespace ns3

#endif // PKTFWDNORMDISTHLISTONLINEOCT092016_HELPER_H

