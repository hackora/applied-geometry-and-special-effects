#ifndef LISSAJOUS_H
#define LISSAJOUS_H


#include <parametrics/gmpcurve.h>

  template <typename T>
  class Lissajous : public GMlib::PCurve<T,3> {
    GM_SCENEOBJECT(Lissajous)
  public:
    Lissajous();
    Lissajous( const Lissajous<T>& copy );

    virtual ~Lissajous();

    //****************************************
    //****** Virtual public functions   ******
    //****************************************

    // from PCurve
    bool                isClosed() const override;

  protected:
    // Virtual function from PCurve that has to be implemented locally
    void              eval(T t, int d, bool l) const override;
    T                   getStartP() const override;
    T                   getEndP()   const override;

  }; // END class Lissajous

// Include Lissajous class function implementations
#include "lissajous.c"


#endif // LISSAJOUS_H

