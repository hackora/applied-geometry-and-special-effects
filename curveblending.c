  template <typename T>
  inline
  CurveBlending<T>::CurveBlending( PCurve <T, 3>* curve1, PCurve <T, 3>* curve2, T  x) {

    _d = 1;
    _k = 2;
    _x = x;

    _curve1 = curve1;
    _curve2 = curve2;
  }


  template <typename T>
  inline
  CurveBlending<T>::CurveBlending( const CurveBlending<T>& copy ) : PCurve<T,3>( copy ) {

      _d = copy._d;
      _k = copy._k;
      _x = copy._x;
      _curve1 = copy._curve1;
      _curve2 = copy._curve2;

  }


  template <typename T>
  CurveBlending<T>::~CurveBlending() {}

  template <typename T>
  T CurveBlending<T>::getEndP() const {

    return _curve1->getParEnd() + (1- _x)* _curve2->getParDelta();
  }

  template <typename T>
  T CurveBlending<T>::getStartP() const {

    return _curve1->getParStart();
  }

  template <typename T>
  inline
  bool CurveBlending<T>::isClosed() const {

    return false;
  }

  template <typename T>
  inline
  void CurveBlending<T>::eval( T t, int /*d_not_used*/, bool /*l*/) const {

      this->_p.setDim( _d + 1 );

      T end1 = _curve1->getParEnd();
      T delta1 = _curve1->getParDelta();
      T start2 = _curve2->getParStart();
      T delta2 = _curve2->getParDelta();

      if(t< end1 - delta1*_x)
          this->_p = _curve1->evaluateParent(t,0);

      else if(t< end1){
          auto t_hat = (t-(end1 - delta1*_x))/ (_x*delta1);
          auto var1 = _curve1->evaluateParent(t,0);
          auto var2 = _curve2->evaluateParent(start2 + t_hat * _x * delta2,0);
          this->_p = var1 + getB(t_hat) * (var2 - var1 );
      }

      else if(t<= end1 + (1- _x)*delta2  )
          this->_p = _curve2->evaluateParent((start2+_x*delta2+t-end1),0);

  }

  template <typename T>
  inline
  T CurveBlending<T>::getB(T t) const {

      //BFunction B(t) = 3*t^2 - 2*t^3
      //BlendBok page 149

      T B =  3*t*t - 2 * t*t*t;
      return (B);
  }
