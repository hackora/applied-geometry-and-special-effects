#ifndef SCENARIO_H
#define SCENARIO_H


#include "application/gmlibwrapper.h"

// qt
#include <QObject>

//GMlib

namespace GMlib {
    template <typename T, int n>
    class PCurve;

    template <typename T, int n>
    class PSurf;
}



class Scenario : public GMlibWrapper {
  Q_OBJECT
public:
  using GMlibWrapper::GMlibWrapper;

  void    initializeScenario() override;
  void    cleanupScenario() override;

public slots:

  void glUpdate();

private:

  GMlib::PCurve<float,3>*       erbs{nullptr};
  GMlib::PSurf<float,3>*          erbsSurf{nullptr};
  GMlib::PSurf<float,3>*          erbsSurft{nullptr};
  GMlib::PSurf<float,3>*          erbsSurfs{nullptr};
  GMlib::PSurf<float,3>*          erbsSurfc{nullptr};


};

#endif // SCENARIO_H
