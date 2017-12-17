#ifndef ERBSplineCurve_H
#define ERBSplineCurve_H


#include "parametrics/gmpcurve.h"
#include "parametrics/curves/gmpsubcurve.h"

using namespace GMlib;


  template <typename T>
  class ERBSplineCurve : public PCurve<T,3> {
    GM_SCENEOBJECT(ERBSplineCurve)
  public:
    ERBSplineCurve(PCurve <T, 3>* p, int  n);
    ERBSplineCurve( const ERBSplineCurve<T>& curve );
    virtual ~ERBSplineCurve();

    void                      generateKnotVector(int n, T start, T end);
    void                      generateLocalCurvesVector(PCurve <T, 3>* p, int n);

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
    void localSimulate(double dt) override;

    // Help function
    int                      getIndex(T t) const;
    T                         getW(int i, int d, T t) const;
    T                         getB(T t ) const;    //the B function
    T                         getDerB(T t ) const;    //derivative of the B function

    // Protected data for the curve
    DVector< PSubCurve <T>* >       _c; // control curves (controlpolygon)
    int                                                         _d; // polynomial degree
    int                                                         _k; //order
    DVector<T>                                       _t; // knot vector
    bool                                                     _closed =false;

  }; // END class ERBSplineCurve


// Include ERBSplineCurve class function implementations
#include "erbsplinecurve.c"


#endif // ERBSplineCurve_H


