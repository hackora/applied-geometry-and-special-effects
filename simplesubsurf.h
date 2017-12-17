/**********************************************************************************
**
** Copyright (C) 1994 2017 UiT - The Arctic University of Norway
** Contact: GMlib Online Portal
**
** This file is not a part of the Geometric Modeling Library, GMlib.
**
** GMlib is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** GMlib is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with GMlib.  If not, see <http://www.gnu.org/licenses/>.
**
**********************************************************************************/


#ifndef GM_PARAMETRICS_SURFACES_PSIMPLESUBSURF_H
#define GM_PARAMETRICS_SURFACES_PSIMPLESUBSURF_H


#include <gmCoreModule>
#include <gmParametricsModule>



  template <typename T>
  class PSimpleSubSurf : public GMlib::PSurf<T,3> {
    GM_SCENEOBJECT(PSimpleSubSurf)
  public:
  // Constructors and destructor
    PSimpleSubSurf( GMlib::PSurf<T,3>* s, T su, T eu, T sv, T ev);
    PSimpleSubSurf( GMlib::PSurf<T,3>* s, T su, T eu, T u, T sv, T ev, T v);
    PSimpleSubSurf( const PSimpleSubSurf<T>& copy );

    virtual ~PSimpleSubSurf() {}

  protected:
    // Virtual functions from PSurf, which have to be implemented locally
    void          eval(T u, T v, int d1, int d2, bool lu = true, bool lv = true ) const override;
    T             getStartPU() const override;
    T             getEndPU()   const override;
    T             getStartPV() const override;
    T             getEndPV()   const override;


    // Protected data for the surface
    GMlib::PSurf<T,3>*     _s;  // The original surface
    T                      _su; // Start parameter value in u-direction
    T                      _sv; // Start parameter value in v-direction
    T                      _eu; // End parameter value in u-direction
    T                      _ev; // End parameter value in v-direction
    T                      _u;  // Center parameter value in u-direction
    T                      _v;  // Center parameter value in v-direction
    GMlib::Vector<float,3> _trans;// Translation so center is origin in local coordinate system

  private:

    // Private help functions
    void    set(GMlib::PSurf<T,3>* s, T su, T eu, T u, T sv, T ev, T v);

  }; // END class PSimpleSubSurf



  //***********************************************************
  // Include PSimpleSubSurf class function implementations ****
  //       sometimes located in a simplesubsurf.c file     ****
  //***********************************************************

  //*****************************************
  // Constructors and destructor           **
  //*****************************************

    template <typename T>
    inline
    PSimpleSubSurf<T>::PSimpleSubSurf( GMlib::PSurf<T,3>* s, T su, T eu, T sv, T ev )
    {
      set(s, su, eu, (su+eu)/2, sv, ev, (sv+ev)/2);
      // Set local coordinate system, origin in center point
      GMlib::DMatrix<GMlib::Vector<T,3> > tr = _s->evaluateParent( _u, _v, 0, 0 );
      _trans = tr[0][0];
      this->translateParent(_trans);
    }


    template <typename T>
    inline
    PSimpleSubSurf<T>::PSimpleSubSurf( PSurf<T,3>* s, T su, T eu, T u, T sv, T ev, T v )
    {
      set(s, su, eu, u, sv, ev, v);
      // Set local coordinate system, origin in center point
      GMlib::DMatrix<GMlib::Vector<T,3> > tr = _s->evaluateParent( _u, _v, 0, 0 );
      _trans = tr[0][0];
      this->translateParent( _trans );
    }


    template <typename T>
    inline
    PSimpleSubSurf<T>::PSimpleSubSurf( const PSimpleSubSurf<T>& copy ) : GMlib::PSurf<T,3>( copy )
    {
      set(copy._s, copy._su, copy._eu, copy._u, copy._sv, copy._ev, copy._v);
      // Set local coordinate system, origin in center point
      _trans = copy._trans;
    }




    //*****************************************************
    // Overrided (protected) virtual functons from PSurf **
    //*****************************************************

    template <typename T>
    void PSimpleSubSurf<T>::eval( T u, T v, int d1, int d2, bool /*lu*/, bool /*lv*/) const {

      this->_p.setDim(3,3);
      this->_p = _s->evaluateParent( u, v, d1, d2 );
      this->_p[0][0] -=  _trans;
    }


    template <typename T>
    T PSimpleSubSurf<T>::getStartPU() const {
      return _su;
    }


    template <typename T>
    T PSimpleSubSurf<T>::getEndPU() const {
      return _eu;
    }


    template <typename T>
    T PSimpleSubSurf<T>::getStartPV() const {
      return _sv;
    }


    template <typename T>
    T PSimpleSubSurf<T>::getEndPV() const {
      return _ev;
    }



    //***************************
    // Private help functions  **
    //***************************

    template <typename T>
    inline
    void PSimpleSubSurf<T>::set(GMlib::PSurf<T,3>* s, T su, T eu, T u, T sv, T ev, T v) {
      _s  = s;
      _su = su;
      _sv = sv;
      _eu = eu;
      _ev = ev;
      _u  = u;
      _v  = v;
    }


#endif // GM_PARAMETRICS_SURFACES_PSIMPLESUBSURF_H
