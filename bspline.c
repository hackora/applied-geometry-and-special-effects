#include <gmSceneModule>

  template <typename T>
  inline
  BSpline<T>::BSpline(const DVector< Vector<T, 3> >& c) {

    _c = c;
    _d = 2;
    _k = _d+1;

    generateKnotVector(_c.getDim());
    /*for(unsigned int i=0; i<_c.getDim();i++){

        auto selector = new GMlib::Selector<T,3>(_c[i],i,this,0.05);
        this->insert(selector);
    }*/
  }

  template <typename T>
  inline
  BSpline<T>::BSpline(const DVector< Vector<T, 3> >& p, int n ) {

      _d = 2;
      _k = _d+1;

    generateKnotVector(n);
    generateControlPointsVector(p,n);
   /* for(unsigned int i=0; i<n;i++){

        auto selector = new GMlib::Selector<T,3>(_c[i],i,this,0.05);
        this->insert(selector);
    }*/
  }


  template <typename T>
  inline
  BSpline<T>::BSpline( const BSpline<T>& copy ) : PCurve<T,3>( copy ) {

      _c = copy._c;
      _d = copy._d;
      _k = copy._k;
      _t = copy._t;

  }


  template <typename T>
  BSpline<T>::~BSpline() {}

  template <typename T>
  inline
  void BSpline<T>::generateKnotVector(int n) {

    _t.setDim( n +_k );
    int step_knots = _t.getDim() - ( _k * 2 );

    T knot_value = T(0);
    int i = 0;

    // Set the start knots
    for( ; i < _k; i++ )
      _t[i] = knot_value;

    // Set the "step"-knots
    for( int j = 0; j < step_knots; j++ )
      _t[i++] = ++knot_value;

    // Set the end knots
    knot_value++;
    for( ; i < _t.getDim(); i++ )
      _t[i] = knot_value;
  }

  template <typename T>
  inline
  void BSpline<T>::generateControlPointsVector(const DVector< Vector<T, 3> >& p, int n){

      int  m = p.getDim();

      DMatrix<T> A(m,n);
      for(unsigned int i=0;i<m;i++){
          for(unsigned int j=0;j<n;j++){
              A[i][j] = T(0);
          }
      }

      _c.setDim(n);

      int index =0;
      T end = getEndP();
      T start= getStartP();
      T dt = (end-start) / T(m-1);
      for(unsigned int i=0;i<m;i++){

          T t = _t(start) + T(i) * dt;
          index = getIndex(t);

          DMatrix<T> der1(1,2);
          DMatrix<T> der2(2,3);

          der1[0][0] = T(1 - getW(index,1,t));
          der1[0][1] = T(getW(index,1,t));

          der2[0][0] = T(1 - getW(index-1,2,t));
          der2[0][1] = T(getW(index-1,2,t));
          der2[0][2] = T(0);
          der2[1][0] = T(0);
          der2[1][1] = T(1 -getW(index,2,t));
          der2[1][2] = T(getW(index,2,t));

          T b0 = der1[0][0] * der2[0][0] + der1[0][1] * der2[1][0];
          T b1 = der1[0][0] * der2[0][1] + der1[0][1] * der2[1][1];
          T b2 = der1[0][0] * der2[0][2] + der1[0][1] * der2[1][2];

          A[i][index-2] =  b0;
          A[i][index-1] =  b1;
          A[i][index]     =  b2;
      }

      //Least square method Ax = b (x is _c and b is p)

      DMatrix<T> AT = A;
      AT.transpose();
      DMatrix<T> B = AT * A;
      DVector< Vector<T, 3>> X = AT * p;
      B.invert();
      _c =  B * X;
  }

  template <typename T>
  T BSpline<T>::getEndP() const {

    return _t(_c.getDim());
  }

  template <typename T>
  T BSpline<T>::getStartP() const {

    return _t(_d);
  }

  template <typename T>
  int BSpline<T>::getIndex(T t) const {
      int i = _d;
      int n = _c.getDim();
      for(;i<n;i++){
          if(_t(i)<=t && t< _t(i+1))
              break;
      }
      if(i>=n)
          i = n-1;

      return(i);

  }

  template <typename T>
  inline
  bool BSpline<T>::isClosed() const {

    return false;
  }

  template <typename T>
  inline
  void BSpline<T>::eval( T t, int /*d_not_used*/, bool /*l*/) const {

      this->_p.setDim( _d + 1 );

      int i = getIndex(t);

      DMatrix<T> der1(1,2);
      DMatrix<T> der2(2,3);

      der1[0][0] = T(1 - getW(i,1,t));
      der1[0][1] = T(getW(i,1,t));

      der2[0][0] = T(1 - getW(i-1,2,t));
      der2[0][1] = T(getW(i-1,2,t));
      der2[0][2] = T(0);
      der2[1][0] = T(0);
      der2[1][1] = T(1 -getW(i,2,t));
      der2[1][2] = T(getW(i,2,t));

      T b0 = der1[0][0] * der2[0][0] + der1[0][1] * der2[1][0];
      T b1 = der1[0][0] * der2[0][1] + der1[0][1] * der2[1][1];
      T b2 = der1[0][0] * der2[0][2] + der1[0][1] * der2[1][2];

      this->_p[0] = _c[i-2] * b0 + _c[i-1] * b1 + _c[i] * b2;

  }

  template <typename T>
  inline
  T BSpline<T>::getW(  int i ,int d,T t) const {

      T W = (t - _t(i))/ (_t(i+d)- _t(i));
      return (W);
  }
