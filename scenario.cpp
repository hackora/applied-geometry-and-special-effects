#include "scenario.h"

#include "testtorus.h"
#include "lissajous.h"
#include "bspline.h"
#include "erbsplinecurve.h"
#include "curveblending.h"
#include "erbsplinesurf.h"

//// hidmanager
//#include "hidmanager/defaulthidmanager.h"

// gmlib
#include <gmOpenglModule>
#include <gmSceneModule>
#include <gmParametricsModule>

// qt
#include <QQuickItem>



void Scenario::initializeScenario() {

    // Insert a light
    GMlib::Point<GLfloat,3> init_light_pos( 2.0, 4.0, 10 );
    GMlib::PointLight *light = new GMlib::PointLight(  GMlib::GMcolor::white(), GMlib::GMcolor::white(),
                                                       GMlib::GMcolor::white(), init_light_pos );
    light->setAttenuation(0.8, 0.002, 0.0008);
    scene()->insertLight( light, false );

    // Insert Sun
    scene()->insertSun();

    // Default camera parameters
    int init_viewport_size = 600;
    GMlib::Point<float,3> init_cam_pos(  0.0f, 0.0f, 0.0f );
    GMlib::Vector<float,3> init_cam_dir( 0.0f, 1.0f, 0.0f );
    GMlib::Vector<float,3> init_cam_up(  0.0f, 0.0f, 1.0f );

    // Projection cam
    auto proj_rcpair = createRCPair("Projection");
    proj_rcpair.camera->set(init_cam_pos,init_cam_dir,init_cam_up);
    proj_rcpair.camera->setCuttingPlanes( 1.0f, 8000.0f );
    proj_rcpair.camera->rotateGlobal( GMlib::Angle(-45), GMlib::Vector<float,3>( 1.0f, 0.0f, 0.0f ) );
    proj_rcpair.camera->translateGlobal( GMlib::Vector<float,3>( 0.0f, -20.0f, 20.0f ) );
    scene()->insertCamera( proj_rcpair.camera.get() );
    proj_rcpair.renderer->reshape( GMlib::Vector<int,2>(init_viewport_size, init_viewport_size) );

    // Front cam
    auto front_rcpair = createRCPair("Front");
    front_rcpair.camera->set( init_cam_pos + GMlib::Vector<float,3>( 0.0f, -50.0f, 0.0f ), init_cam_dir, init_cam_up );
    front_rcpair.camera->setCuttingPlanes( 1.0f, 8000.0f );
    scene()->insertCamera( front_rcpair.camera.get() );
    front_rcpair.renderer->reshape( GMlib::Vector<int,2>(init_viewport_size, init_viewport_size) );

    // Side cam
    auto side_rcpair = createRCPair("Side");
    side_rcpair.camera->set( init_cam_pos + GMlib::Vector<float,3>( -50.0f, 0.0f, 0.0f ), GMlib::Vector<float,3>( 1.0f, 0.0f, 0.0f ), init_cam_up );
    side_rcpair.camera->setCuttingPlanes( 1.0f, 8000.0f );
    scene()->insertCamera( side_rcpair.camera.get() );
    side_rcpair.renderer->reshape( GMlib::Vector<int,2>(init_viewport_size, init_viewport_size) );

    // Top cam
    auto top_rcpair = createRCPair("Top");
    top_rcpair.camera->set( init_cam_pos + GMlib::Vector<float,3>( 0.0f, 0.0f, 50.0f ), -init_cam_up, init_cam_dir );
    top_rcpair.camera->setCuttingPlanes( 1.0f, 8000.0f );
    scene()->insertCamera( top_rcpair.camera.get() );
    top_rcpair.renderer->reshape( GMlib::Vector<int,2>(init_viewport_size, init_viewport_size) );


    // Surface visualizers
    auto surface_visualizer = new GMlib::PSurfDefaultVisualizer<float,3>();
    //surface_visualizer->setDerivatives(1,0);
    // surface_visualizer->setColor(GMlib::GMcolor::red());

    // Surface
    //  auto surface = new GMlib::PTorus<float>(1,1,1);
    //  surface->toggleDefaultVisualizer();
    //  surface->insertVisualizer(surface_visualizer);
    //  surface->replot(100,100,1,1);
    //  scene()->insert(surface);

    //  auto sphere = new GMlib::PSphere<float>(0.9999);
    //  sphere->toggleDefaultVisualizer();
    //  sphere->insertVisualizer(surface_visualizer);
    //  sphere->replot(200,200,1,1);
    //  scene()->insert(sphere);



    //Lissajous Curve

    auto lissajous_curve = new Lissajous<float>;
    lissajous_curve->toggleDefaultVisualizer();
    lissajous_curve->replot(500,0);
    scene()->insert(lissajous_curve);
    lissajous_curve->setVisible(false);

      DVector< Vector<float, 3> > p;
      int m = 100;
      p.setDim(m);


      for(int i=0;i<m;i++){
          p[i]= lissajous_curve->getPosition(lissajous_curve->getParStart()+i* lissajous_curve->getParDelta()/(m-1));
      }

    //  //Bsplines

    //  //first constructor

//      auto bspline1 = new BSpline<float>(p);
//      bspline1->toggleDefaultVisualizer();
//      bspline1->translateGlobal(GMlib::Vector<float,3>(12.5,0.0,0.0));
//      bspline1->setColor(GMlib::GMcolor::blueViolet());
//      bspline1->replot(300,0);
//      scene()->insert(bspline1);

//      //second constructor

//      auto bspline2 = new BSpline<float>(p,50);
//      bspline2->toggleDefaultVisualizer();
//      bspline2->translateGlobal(GMlib::Vector<float,3>(-12.5,0.0,0.0));
//      bspline2->setColor(GMlib::GMcolor::paleVioletRed());
//      bspline2->replot(300,0);
//      scene()->insert(bspline2);


    //Blending 2 curves

    //  auto line1 = new GMlib::PLine<float>(GMlib::Point<float,3>(0.0,0.0,0.0), GMlib::Point<float,3>(6.0,0.0,0.0) );
    //  line1->toggleDefaultVisualizer();
    //  line1->replot(100,0);
    //  line1->setColor(GMlib::GMcolor::blue());
    //  line1->setVisible(true);
    //  scene()->insert(line1);

    //  auto line2 = new GMlib::PLine<float>(GMlib::Point<float,3>(-5.0,-1.0,0.0),GMlib::Point<float,3>(1.0,-1.0,0.0));
    //  line2->toggleDefaultVisualizer();
    //  line2->replot(100,0);
    //  line2->setColor(GMlib::GMcolor::yellowGreen());
    //  line2->setVisible(true);
    //  scene()->insert(line2);

    //  auto blending = new CurveBlending<float>(line2,line1,0.3);
    //  blending->toggleDefaultVisualizer();
    //  blending->replot(100,0);
    //  scene()->insert(blending);


    //GERBS  Curve

      erbs = new ERBSplineCurve<float>(lissajous_curve, 30);
      erbs->toggleDefaultVisualizer();
      erbs->setColor(GMlib::GMcolor::black());
      erbs->replot(300,0);
      erbs->setVisible(true);
      scene()->insert(erbs);




    // GERBS Surface

    //Plane

    //  auto plane = new GMlib::PPlane<float>(GMlib::Point<float,3>(0.0f, 0.0f, 0.0f), GMlib::Vector<float,3>(10.0f, 0.0f, 0.0f),GMlib::Vector<float,3>(0.0f, 0.0f,-10.0f) );
    //  plane->insertVisualizer(surface_visualizer);
    //  plane->toggleDefaultVisualizer();

    //    plane->replot(100,100,1,1);
    //    plane->setMaterial(GMlib::GMmaterial::emerald());
    //    plane->setVisible(false);
    //    scene()->insert(plane);

    //Torus

    //    auto torus = new GMlib::PTorus<float>();
    //    torus->toggleDefaultVisualizer();
    //    torus->insertVisualizer(surface_visualizer);
    //    torus->replot(100,100,1,1);
    //    torus->setVisible(false);
    //    scene()->insert(torus);

    //    //Sphere

    //    auto sphere = new GMlib::PSphere<float>(2);
    //    sphere->toggleDefaultVisualizer();
    //    sphere->insertVisualizer(surface_visualizer);
    //    sphere->replot(10,10,1,1);
    //    sphere->setVisible(false);
    //    scene()->insert(sphere);

    //    //Cone

    //    auto cone = new GMlib::PCone<float>(2,4);
    //    cone->toggleDefaultVisualizer();
    //    cone->insertVisualizer(surface_visualizer);
    //    cone->replot(10,10,1,1);
    //    cone->setVisible(false);
    //    scene()->insert(cone);

    //  erbsSurf = new ERBSplineSurf<float>(cylinder, 4,4);
    //  erbsSurf->insertVisualizer(surface_visualizer);
    //  erbsSurf->toggleDefaultVisualizer();
    //  erbsSurf->translate((GMlib::Vector<float,3>(5.0,0.0,0.0)));
    //  erbsSurf->setMaterial(GMlib::GMmaterial::ruby());
    //  erbsSurf->replot(10,10,1,1);
    //  erbsSurf->setVisible(true);
    //  scene()->insert(erbsSurf);


    //  erbsSurft = new ERBSplineSurf<float>(torus, 5 , 7);
    //  erbsSurft->toggleDefaultVisualizer();
    //  erbsSurft->translate((GMlib::Vector<float,3>(-7.0,0.0,0.0)));
    //  erbsSurft->setMaterial(GMlib::GMmaterial::ruby());
    //  erbsSurft->replot(10,10,1,1);
    //  erbsSurft->setVisible(true);
    //  scene()->insert(erbsSurft);

    //  erbsSurfs = new ERBSplineSurf<float>(sphere, 4,4);
    //  erbsSurfs->toggleDefaultVisualizer();
    //  erbsSurfs->setMaterial(GMlib::GMmaterial::ruby());
    //  erbsSurfs->replot(10,10,1,1);
    //  erbsSurfs->setVisible(true);
    //  scene()->insert(erbsSurfs);

    //Cylinder

//    auto cylinder = new GMlib::PCylinder<float>(2,2,5);
//    cylinder->toggleDefaultVisualizer();
//    cylinder->insertVisualizer(surface_visualizer);
//    cylinder->replot(10,10,1,1);
//    cylinder->setVisible(false);
//    scene()->insert(cylinder);

//    erbsSurfc = new ERBSplineSurf<float>(cylinder, 4,4);
//    erbsSurfc->toggleDefaultVisualizer();
//    erbsSurfc->setMaterial(GMlib::GMmaterial::ruby());
//    erbsSurfc->replot(10,10,1,1);
//    erbsSurfc->setVisible(true);
//    scene()->insert(erbsSurfc);


}

void Scenario::cleanupScenario() {

}

void Scenario::glUpdate(){

    if(erbs)
        erbs->replot(100,0);
    if(erbsSurf)
        erbsSurf->replot(50,50,1,1);
    if(erbsSurft)
        erbsSurft->replot(50,50,1,1);
    if(erbsSurfs)
        erbsSurfs->replot(50,50,1,1);
    if(erbsSurfc)
        erbsSurfc->replot(50,50,1,1);
}


