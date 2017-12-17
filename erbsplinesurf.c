  template <typename T>
  inline
  ERBSplineSurf<T>::ERBSplineSurf(PSurf <T, 3>* s, int  n1, int n2){

    _d = 1;
    _k = 2;

    _closedU = s->isClosedU();
    _closedV = s->isClosedV();

    if(_closedU){
        n1++;
    }

    if(_closedV){
        n2++;
    }
    _s.setDim(n1,n2);

    T startU= s->getParStartU();
    T endU = s->getParEndU();
    T startV= s->getParStartV();
    T endV = s->getParEndV();

    generateKnotVectors(n1, startU, endU, true); //fill in _tU
    generateKnotVectors(n2, startV, endV, false); //fill in _tV
    generateLocalSurfacesVector(s, n1, n2);

    //static transformation
    _s[3][3]->translate(Vector<float,3>(0.0,0.0,5.0));
    _s[0][1]->translate(Vector<float,3>(0.0,0.0,-5.0));

    _s[3][3]->rotate(90,Vector<float,3>(1.0,0.0,0.0));
    _s[0][1]->rotate(90,Vector<float,3>(1.0,0.0,0.0));

  }


  template <typename T>
  inline
  ERBSplineSurf<T>::ERBSplineSurf( const ERBSplineSurf<T>& copy ) : PSurf<T,3>( copy ) {

      _s = copy._s;
      _d = copy._d;
      _k = copy._k;
      _tU = copy._tU;
      _tV = copy._tV;
      _closedU = copy._closedU;
      _closedV = copy._closedV;

  }


  template <typename T>
  ERBSplineSurf<T>::~ERBSplineSurf() {}

  template <typename T>
  inline
  void ERBSplineSurf<T>::generateKnotVectors(int n, T start, T end, bool u) {

      T dt = (end-start) / T(n-1);

    if(u){
        if(_closedU){
            _tU.setDim( n +_k );
            _tU[1] = start;

            for(int i=2;i<n;i++){
                _tU[i] = start + T(i-1) * dt;
            }
            _tU[n] = end;
            _tU[n+1] = _tU[n]+(_tU[2] - _tU[1]);
            _tU[0] = start - (_tU[n]-_tU[n-1]);

        }
        else{
            _tU.setDim( n +_k );
            _tU[0] = start;
            _tU[1] = start;

            for(int i=2;i<n;i++){
                _tU[i] = start + T(i-1) * dt;
            }

            _tU[n] = end;
            _tU[n+1] = end;
        }
    }
    else{

        if(_closedV){
            _tV.setDim( n +_k );
            _tV[1] = start;

            for(int i=2;i<n;i++){
                _tV[i] = start + T(i-1) * dt;
            }
            _tV[n] = end;
            _tV[n+1] = _tV[n]+(_tV[2] - _tV[1]);
            _tV[0] = start - (_tV[n]-_tV[n-1]);
        }
        else{
            _tV.setDim( n +_k );
            _tV[0] = start;
            _tV[1] = start;

            for(int i=2;i<n;i++){
                _tV[i] = start + T(i-1) * dt;
            }

            _tV[n] = end;
            _tV[n+1] = end;
        }
    }



  }

  template <typename T>
  inline
  void ERBSplineSurf<T>::generateLocalSurfacesVector(PSurf <T, 3>* s, int n1, int n2){

      int number1 =n1, number2 = n2;
      if(_closedU)
          number1-=1;
      if(_closedV)
          number2-=1;

      for(int indexU=0;indexU<number1;indexU++){

          for(int indexV=0;indexV<number2;indexV++){
              _s[indexU][indexV] = new PSimpleSubSurf<T> (s,_tU(indexU),_tU(indexU+2),_tU(indexU+1),_tV(indexV),_tV(indexV+2),_tV(indexV+1));
              this->insert(_s[indexU][indexV]);
              _s[indexU][indexV]->toggleDefaultVisualizer();
              _s[indexU][indexV]->replot(5,5,1,1);
              _s[indexU][indexV]->setCollapsed(true);
          }
      }


      if(_closedU ){
          for(int indexV=0;indexV<n2;indexV++){

              _s[n1-1][indexV] = _s[0][indexV];

          }
      }

      if(_closedV ){
          for(int indexU=0;indexU<n1;indexU++){

              _s[indexU][n2-1] = _s[indexU][0];
          }
      }
  }

  template <typename T>
  T ERBSplineSurf<T>::getEndPU() const {

    return _tU[_s.getDim1()];
  }

  template <typename T>
  T ERBSplineSurf<T>::getStartPU() const {

    return _tU(_d);
  }

  template <typename T>
  T ERBSplineSurf<T>::getEndPV() const {

    return _tV[_s.getDim2()];;
  }

  template <typename T>
  T ERBSplineSurf<T>::getStartPV() const {

    return _tV(_d);
  }

  template <typename T>
  int ERBSplineSurf<T>::getIndex(T t, bool u) const {
      int i = _d;
      if(u){
          int n = _s.getDim1();
          for(;i<n;i++){
              if(_tU(i)<=t && t< _tU(i+1))
                  break;
          }
          if(i>=n)
              i = n-1;
      }
      else{
          int n = _s.getDim2();
          for(;i<n;i++){
              if(_tV(i)<=t && t< _tV(i+1))
                  break;
          }
          if(i>=n)
              i = n-1;
      }

      return(i);

  }

  template <typename T>
  inline
  bool ERBSplineSurf<T>::isClosedU() const {

    return _closedU;
  }

  template <typename T>
  inline
  bool ERBSplineSurf<T>::isClosedV() const {

    return _closedV;
  }

  template <typename T>
  inline
  void ERBSplineSurf<T>::eval( T u, T v, int d1, int d2, bool lu, bool lv) const{

      this->_p.setDim(2,2);
      DMatrix<T> Bu(2,2);
      DMatrix<T> Bv(2,2);
      DMatrix<DMatrix<GMlib::Vector<T,3>>> C(2,2);

      int i1 = getIndex(u,true);
      int i2 = getIndex(v,false);


      C[0][0] =  _s(i1 -1)(i2-1)->evaluateParent(u,v,d1,d2);
      C[0][1] =  _s(i1 -1)(i2)->evaluateParent(u,v,d1,d2);
      C[1][0] =  _s(i1)(i2 -1)->evaluateParent(u,v,d1,d2);
      C[1][1] =  _s(i1)(i2)->evaluateParent(u,v,d1,d2);

      Bu[0][0] = T(1 - getB(getW(i1,1,u,true)));
      Bu[0][1] = T(getB(getW(i1,1,u,true)));
      Bu[1][0] = T(- getDerW(i1,1,true)*getDerB(getW(i1,1,u,true)));
      Bu[1][1] = T(getDerW(i1,1,true)*getDerB(getW(i1,1,u,true)));

      Bv[0][0] = T(1 - getB(getW(i2,1,v,false)));
      Bv[0][1] = T(- getDerW(i2,1,false)*getDerB(getW(i2,1,v,false)));
      Bv[1][0] = T(getB(getW(i2,1,v,false)));
      Bv[1][1] = T(getDerW(i2,1,false)*getDerB(getW(i2,1,v,false)));

      //DMatrix<DMatrix<GMlib::Vector<T,3>>> result = Bu * (C^Bv); //  casting problem (supposed to be DMatrix of Vector)

      auto h1 = Bu[0][0]*C(0)(0)(0)(0) + Bu[0][1] * C(1)(0)(0)(0);
      auto h2 = Bu[1][0]*C(0)(0)(0)(0) + Bu[1][1] * C(1)(0)(0)(0);
      auto h3 = Bu[0][0]*C(0)(1)(0)(0) + Bu[0][1] * C(1)(1)(0)(0);
      auto h4 = Bu[1][0]*C(0)(1)(0)(0) + Bu[1][1] * C(1)(1)(0)(0);
      auto h5 = Bu[0][0]*C(0)(0)(1)(0) + Bu[0][1] * C(1)(0)(1)(0);
      auto h6 = Bu[0][0]*C(1)(0)(1)(0) + Bu[0][1] * C(1)(1)(1)(0);
      auto h7 = Bu[0][0]*C(0)(0)(0)(1) + Bu[0][1] * C(1)(0)(0)(1);
      auto h8 = Bu[0][0]*C(1)(0)(0)(1) + Bu[0][1] * C(1)(1)(0)(1);

      this->_p[0][0] = h1 * Bv[0][0] + h3 *  Bv[1][0];                                                                                    //S
      this->_p[1][0] = (h2+h5) * Bv[0][0] + (h4+h6) *  Bv[1][0]  ;                                                            //Su
      this->_p[0][1] = h1 * Bv[0][1] + h3*  Bv[1][1]  + h7 * Bv[0][0] + h8 * Bv[1][0] ;                      //Sv

  }

  template <typename T>
  inline
  T ERBSplineSurf<T>::getW(  int i ,int d,T t, bool u) const {

      T W = T(0);
      if(u)
          W = (t - _tU(i))/ (_tU(i+d)- _tU(i));
      else
          W = (t - _tV(i))/ (_tV(i+d)- _tV(i));

      return (W);
  }

  template <typename T>
  inline
  T ERBSplineSurf<T>::getDerW(  int i ,int d, bool u) const {

      T W = T(0);
      if(u)
          W = 1/ (_tU(i+d)- _tU(i));
      else
          W = 1/ (_tV(i+d)- _tV(i));
      return (W);
  }

  template <typename T>
  inline
  T ERBSplineSurf<T>::getB(T t) const {

      //Rational BFunction B(t) = t^2 / (2*t^2-2*t+1)
      //B'(t) = 2t(-t+1) / (2t^2-2t+1)^2
      //BlendBok page 156

      T B = t*t / (2*t*t-2*t+1);

      return (B);
  }

  template <typename T>
  inline
  T ERBSplineSurf<T>::getDerB(T t) const {

      //Rational BFunction B(t) = t^2 / (2*t^2-2*t+1)
      //B'(t) = 2t(-t+1) / (2t^2-2t+1)^2
      //BlendBok page 156

      T B = 2*t * (1-t) / (2*t*t-2*t+1)*(2*t*t-2*t+1);

      return (B);
  }

  template <typename T>
  inline
  void ERBSplineSurf<T>::localSimulate(double dt){

//      for(int i=0;i<_s.getDim1();i++){
//          for(int j=0;j<_s.getDim2();j++){

//          _s[i][j]->rotate(dt,Vector<float,3>(0.0,0.0,1.0));
//          }
//      }
  }
