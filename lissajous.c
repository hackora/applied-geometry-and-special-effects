#include "core/gmangle"

template <typename T>
  inline
  Lissajous<T>::Lissajous() {

  }


  template <typename T>
  inline
  Lissajous<T>::Lissajous( const Lissajous<T>& copy ) : GMlib::PCurve<T,3>( copy ) {}


  template <typename T>
  Lissajous<T>::~Lissajous() {}



  //***************************************************
  // Overrided (public) virtual functons from PCurve **
  //***************************************************

  template <typename T>
  bool Lissajous<T>::isClosed() const {
    return true;
  }



  //******************************************************
  // Overrided (protected) virtual functons from PCurve **
  //******************************************************

  template <typename T>
  void Lissajous<T>::eval( T t, int d, bool /*l*/ ) const {

    this->_p.setDim( d + 1 );

    const T a = T(1);
    const T b = T(2);
    const T A = T(6);
    const T B = T(5);

    this->_p[0][0] =  T(A*cos(a*t));
    this->_p[0][1] =  T(B*sin(b*t));
    this->_p[0][2] =  T(0);

//    this->_p[0][0] =  T(sin(t)*cos(20*t));
//    this->_p[0][1] =  T(sin(t)*sin(20*t));
//    this->_p[0][2] = T(cos(t));
  }


  template <typename T>
  T Lissajous<T>::getStartP() const {
    return T(0);
  }


  template <typename T>
  T Lissajous<T>::getEndP() const {
    return T( M_2PI );
  }
