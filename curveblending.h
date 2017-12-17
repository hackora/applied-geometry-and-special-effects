#ifndef CURVEBLENDING_H
#define CURVEBLENDING_H


#include "parametrics/gmpcurve.h"
#include "parametrics/curves/gmpsubcurve.h"

using namespace GMlib;


  template <typename T>
  class CurveBlending : public PCurve<T,3> {
    GM_SCENEOBJECT(CurveBlending)
  public:
    CurveBlending(PCurve <T, 3>* curve1, PCurve <T, 3>* curve2, T  x);
    CurveBlending( const CurveBlending<T>& curve );
    virtual ~CurveBlending();

    //****************************************
    //****** Virtual public functions   ******
    //****************************************

    // from PCurve
    bool                      isClosed() const override;

  protected:
    // Virtual function from PCurve that has to be implemented locally
    void                   eval(T t, int /*d_not_used*/, bool /*l*/) const override;
    T                         getStartP() const override;
    T                         getEndP()   const override;

    // Help function
    T                         getB(T t ) const;    //the B function

    // Protected data for the curve
    PCurve <T,3>*                                  _curve1;
    PCurve<T,3>*                                   _curve2;
    int                                                         _d; //degree
    int                                                         _k; //order
    T                                                            _x;
  }; // END class CurveBlending


// Include CurveBlending class function implementations
#include "curveblending.c"


#endif // CURVEBLENDING_H
