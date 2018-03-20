#ifndef MLPROVENANCE_HELPER_H
#define MLPROVENANCE_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "mlprovenance.h"

namespace ns3 {
namespace rapidnet {
namespace mlprovenance {

class Mlprovenance;

class MlprovenanceHelper: public RapidNetApplicationHelper
{
public:
  MlprovenanceHelper ()
  {
    m_factory.SetTypeId (Mlprovenance::GetTypeId ());
  }
  virtual ~MlprovenanceHelper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<Mlprovenance> ();
  }
};

} // namespace mlprovenance
} // namespace rapidnet
} // namespace ns3

#endif // MLPROVENANCE_HELPER_H

