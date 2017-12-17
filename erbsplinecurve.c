  template <typename T>
  inline
  ERBSplineCurve<T>::ERBSplineCurve(PCurve<T, 3> *p, int n ) {

    _d = 1;
    _k = 2;

    _closed = p->isClosed();

    if(_closed){
        n++;
    }

    _c.setDim(n);

    T start= p->getParStart();
    T end = p->getParEnd();
    generateKnotVector(n,start,end);
    generateLocalCurvesVector(p,n);
  }


  template <typename T>
  inline
  ERBSplineCurve<T>::ERBSplineCurve( const ERBSplineCurve<T>& copy ) : PCurve<T,3>( copy ) {

      _c = copy._c;
      _d = copy._d;
      _k = copy._k;
      _t = copy._t;

  }


  template <typename T>
  ERBSplineCurve<T>::~ERBSplineCurve() {}

  template <typename T>
  inline
  void ERBSplineCurve<T>::generateKnotVector(int n, T start, T end) {

    _t.setDim( n +_k );
    T dt = (end-start) / T(n-1);

    if(!_closed){

        _t[0] = start;
        _t[1] = start;

        for(int i=2;i<n;i++){
            _t[i] = start + T(i-1) * dt;
        }

        _t[n] = end;
        _t[n+1] = end;
    }

    else{

        _t[1] = start;

        for(int i=2;i<n;i++){
            _t[i] = start + T(i-1) * dt;
        }
        _t[n] = end;
        _t[n+1] = _t[n]+(_t[2] - _t[1]);
        _t[0] = start - (_t[n]-_t[n-1]);

    }

//    std::cout<<_t<<std::endl;
  }

  template <typename T>
  inline
  void ERBSplineCurve<T>::generateLocalCurvesVector(PCurve <T, 3>* p, int n){

      for(unsigned int index=0;index<n;index++){

          if(_closed && index == n-1){
              _c[n-1] = _c[0];
          }
          else{
              _c[index] = new PSubCurve<T> (p,_t(index),_t(index+2),_t(index+1));
              this->insert(_c[index]);
              _c[index]->toggleDefaultVisualizer();
              _c[index]->replot(100);
              _c[index]->setCollapsed(true);
              _c[index]->setVisible(true);
          }

      }
  }

  template <typename T>
  T ERBSplineCurve<T>::getEndP() const {

    return _t(_c.getDim());
  }

  template <typename T>
  T ERBSplineCurve<T>::getStartP() const {

    return _t(_d);
  }

  template <typename T>
  int ERBSplineCurve<T>::getIndex(T t) const {
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
  bool ERBSplineCurve<T>::isClosed() const {

    return _closed;
  }

  template <typename T>
  inline
  void ERBSplineCurve<T>::eval( T t, int /*d_not_used*/, bool /*l*/) const {

      this->_p.setDim( _d + 1 );

      int i = getIndex(t);

      T b0 = T(1 - getB(getW(i,1,t)));
      T b1 = T(getB(getW(i,1,t)));

      this->_p[0] = _c[i-1]->evaluateParent(t,0)(0) * b0 + _c[i]->evaluateParent(t,0)(0) * b1;

  }

  template <typename T>
  inline
  T ERBSplineCurve<T>::getW(  int i ,int d,T t) const {

      T W = (t - _t(i))/ (_t(i+d)- _t(i));
      return (W);
  }

  template <typename T>
  inline
  T ERBSplineCurve<T>::getB(T t) const {

      //Rational BFunction B(t) = t^2 / (2*t^2-2*t+1)
      //B'(t) = 2t(-t+1) / (2t^2-2t+1)^2
      //BlendBok page 156

      T B = t*t / (2*t*t-2*t+1);
      return (B);
  }

  template <typename T>
  inline
  T ERBSplineCurve<T>::getDerB(T t) const {

      //Rational BFunction B(t) = t^2 / (2*t^2-2*t+1)
      //B'(t) = 2t(-t+1) / (2t^2-2t+1)^2
      //BlendBok page 156

      T B = 2*t * (1-t) / (2*t*t-2*t+1)*(2*t*t-2*t+1);
      return (B);
  }

  template <typename T>
  inline
  void ERBSplineCurve<T>::localSimulate(double dt){

      for(unsigned int i=0;i<_c.getDim()-1;i++){
          _c[i]->rotate(4*sin(dt),Vector<float,3>(0.0,0.0,1.0));
      }
  }
