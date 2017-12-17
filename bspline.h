#ifndef BSPLINE_H
#define BSPLINE_H


#include "parametrics/gmpcurve.h"

using namespace GMlib;


  template <typename T>
  class BSpline : public PCurve<T,3> {
    GM_SCENEOBJECT(BSpline)
  public:
    BSpline( const DVector< Vector<T, 3> >& c );
    BSpline( const DVector< Vector<T, 3> >& p, int n );
    BSpline( const BSpline<T>& curve );
    virtual ~BSpline();

    void                      generateKnotVector(int n);
    void                      generateControlPointsVector(const DVector< Vector<T, 3> >& p, int n);

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
    int                      getIndex(T t) const;
    T                         getW(int i, int d, T t ) const;

    // Protected data for the curve
    DVector< Vector<T,3> >    _c; // control points (controlpolygon)
    int                                             _d; // polynomial degree
    int                                             _k; //order
    DVector<T>                           _t; // knot vector

  }; // END class BSpline


// Include BSpline class function implementations
#include "bspline.c"


#endif // BSPLINE_H


