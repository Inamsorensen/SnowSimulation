#include <QtGui/QGuiApplication>
#include <iostream>
#include "OpenGL_Scene.h"

#include "Wind.h"


//int main(int argc, char **argv)
//{
//  QGuiApplication app(argc, argv);
//  // create an OpenGL format specifier
//  QSurfaceFormat format;
//  // set the number of samples for multisampling
//  // will need to enable glEnable(GL_MULTISAMPLE); once we have a context
//  format.setSamples(4);
//  #if defined( DARWIN)
//    // at present mac osx Mountain Lion only supports GL3.2
//    // the new mavericks will have GL 4.x so can change
//    format.setMajorVersion(3);
//    format.setMinorVersion(2);
//  #else
//    // with luck we have the latest GL version so set to this
//    format.setMajorVersion(4);
//    format.setMinorVersion(3);
//  #endif
//  // now we are going to set to CoreProfile OpenGL so we can't use and old Immediate mode GL
//  format.setProfile(QSurfaceFormat::CoreProfile);
//  // now set the depth buffer to 24 bits
//  format.setDepthBufferSize(24);
//  // now we are going to create our scene window
//  OpenGL_Scene window;
//  // and set the OpenGL format
//  window.setFormat(format);
//  // we can now query the version to see if it worked
//  std::cout<<"Profile is "<<format.majorVersion()<<" "<<format.minorVersion()<<"\n";
//  // set the window size
//  window.resize(1024, 720);
//  // and finally show
//  window.show();

//  return app.exec();

//  return EXIT_SUCCESS;
//}

int main()
{
//  WindGrid testGrid=WindGrid(ngl::Vec3(0,0,0),2,2,2,2);
 WindGrid* testGrid=WindGrid::createWindGrid(ngl::Vec3(0,0,0),2,2,2,2);
  testGrid=WindGrid::getWindGrid();
  ngl::Vec3 particlePos=ngl::Vec3(-0.8,0.8,0.8);
  WindCell* cellFound=testGrid->findCellFromPosition(particlePos);

//  testGrid.printCellDetails();

  if (cellFound!=nullptr)
  {
    std::cout<<"i "<<cellFound->m_indexX<<" j "<<cellFound->m_indexY<<" k "<<cellFound->m_indexZ<<"\n";
    std::cout<<"xmin "<<cellFound->m_xmin<<" ymin "<<cellFound->m_ymin<<" zmin "<<cellFound->m_zmin<<"\n";
  }
  else
  {
    std::cout<<"not found\n";
  }

  return EXIT_SUCCESS;
}
