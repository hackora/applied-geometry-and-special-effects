#ifndef ERBSPLINESURF_H
#define ERBSPLINESURF_H

#include "parametrics/gmpsurf.h"
#include "simplesubsurf.h"

using namespace GMlib;

  template <typename T>
  class ERBSplineSurf : public PSurf<T,3> {
    GM_SCENEOBJECT(ERBSplineSurf)
  public:
    ERBSplineSurf(PSurf <T, 3>* s, int  n1, int n2);
    ERBSplineSurf( const ERBSplineSurf<T>& surface );
    virtual ~ERBSplineSurf();

    void                      generateKnotVectors(int n, T start, T end, bool u);
    void                      generateLocalSurfacesVector(PSurf <T, 3>* s, int n1, int n2);

    //****************************************
    //****** Virtual public functions   ******
    //****************************************

    // from PSurf
    bool                      isClosedU() const override;
    bool                      isClosedV() const override;

  protected:
    // Virtual function from PSurf that has to be implemented locally
    void                   eval(T u, T v, int d1, int d2, bool lu, bool lv) const override;
    T                         getStartPU() const override;
    T                         getEndPU()   const override;
    T                         getStartPV() const override;
    T                         getEndPV()   const override;
    void                   localSimulate(double dt) override;


    // Help function
    int                      getIndex(T t, bool u) const;
    T                         getW(int i ,int d,T t, bool u) const;
    T                         getDerW( int i ,int d, bool u) const;    //derivative of W
    T                         getB(T t ) const;    //the B function
    T                         getDerB(T t ) const;    //derivative of the B function

    // Protected data for the curve
    DMatrix< PSimpleSubSurf <T>*>       _c; // local subSurfaces
    int                                                                 _d; // polynomial degree
    int                                                                 _k; //order
    DVector<T>                                              _tU; // knot vector U
    DVector<T>                                              _tV; // knot vector V
    bool                                                            _closedU = false;
    bool                                                            _closedV = false;

  }; // END class ERBSplineSurf


// Include ERBSplineSurf class function implementations
#include "erbsplinesurf.c"


#endif // ERBSPLINESURF_H


