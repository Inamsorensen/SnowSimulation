#include "Wind.h"

#include <cmath>
#include <iostream>
#include <stdexcept>

WindCell::WindCell(int _indexX, int _indexY, int _indexZ, float _xmin, float _xmax, float _ymin, float _ymax, float _zmin, float _zmax)
{
  m_indexX=_indexX;
  m_indexY=_indexY;
  m_indexZ=_indexZ;

  m_xmin=_xmin;
  m_xmax=_xmax;
  m_ymin=_ymin;
  m_ymax=_ymax;
  m_zmin=_zmin;
  m_zmax=_zmax;

}

WindGrid* WindGrid::m_instance=nullptr;

WindGrid::WindGrid(ngl::Vec3 _centre, float _width, float _depth, float _height, int _divisions, ngl::Vec3 _velocityIn=ngl::Vec3(1.0, 1.0, 1.0), ngl::Vec3 _velocityOut=ngl::Vec3(0.0,0.0,0.0), ngl::Vec3 _pressureIn=ngl::Vec3(0.0,0.0,0.0), ngl::Vec3 _pressureOut=ngl::Vec3(0.0,0.0,0.0))
{
  m_centre=_centre;
  m_width=_width;
  m_depth=_depth;
  m_height=_height;
  m_divisions=_divisions;

  m_sizeCellsX=m_width/(float)m_divisions;
  m_sizeCellsY=m_height/(float)m_divisions;
  m_sizeCellsZ=m_depth/(float)m_divisions;

  for (int i=0; i<m_divisions;i++)
  {
    for (int j=0; j<m_divisions; j++)
    {
      for (int k=0; k<m_divisions; k++)
      {

        float xmin=m_centre.m_x-(m_width/2.0)+(i*m_sizeCellsX);
        float xmax=xmin+m_sizeCellsX;
        float ymin=m_centre.m_y-(m_height/2.0)+(j*m_sizeCellsY);
        float ymax=ymin+m_sizeCellsY;
        float zmin=m_centre.m_z-(m_depth/2.0)+(k*m_sizeCellsZ);
        float zmax=zmin+m_sizeCellsZ;

        m_gridCells.push_back(WindCell(i, j, k, xmin, xmax, ymin, ymax, zmin, zmax));

      }
    }
  }
}


WindGrid* WindGrid::createWindGrid(ngl::Vec3 _centre, float _width, float _depth, float _height, int _divisions, ngl::Vec3 _velocityIn=ngl::Vec3(1.0, 1.0, 1.0), ngl::Vec3 _velocityOut=ngl::Vec3(0.0,0.0,0.0), ngl::Vec3 _pressureIn=ngl::Vec3(0.0,0.0,0.0), ngl::Vec3 _pressureOut=ngl::Vec3(0.0,0.0,0.0))
{
  if(m_instance==nullptr)
  {
    m_instance=new WindGrid(_centre, _width, _depth, _height, _divisions, _velocityIn, _velocityOut, _pressureIn, _pressureOut);
  }

  return m_instance;

}

WindGrid* WindGrid::getWindGrid()
{

  if (m_instance==nullptr)
  {
    throw std::invalid_argument("You need to create wind first!");
  }
  return m_instance;
}

WindCell* WindGrid::findCellFromPosition(ngl::Vec3 _position)
{
  WindCell* cell=nullptr;

  int noCells=m_gridCells.size();

  for (int i=0; i<noCells; i++)
  {
    if(_position.m_x>=m_gridCells[i].m_xmin && _position.m_x<m_gridCells[i].m_xmax)
    {
      if (_position.m_y>=m_gridCells[i].m_ymin && _position.m_y<m_gridCells[i].m_ymax)
      {
        if (_position.m_z>=m_gridCells[i].m_zmin && _position.m_z<m_gridCells[i].m_zmax)
        {
          cell=&m_gridCells[i];
        }
      }
    }
  }

  return cell;
}

void WindGrid::printCellDetails()
{
  int noCells=m_gridCells.size();

  for (int i=0; i<noCells; i++)
  {
    std::cout<<"i "<<m_gridCells[i].m_indexX<<" j "<<m_gridCells[i].m_indexY<<" k "<<m_gridCells[i].m_indexZ<<"\n";
    std::cout<<"xmin "<<m_gridCells[i].m_xmin<<" ymin "<<m_gridCells[i].m_ymin<<" zmin "<<m_gridCells[i].m_zmin<<"\n";
    std::cout<<"xmax "<<m_gridCells[i].m_xmax<<" ymax "<<m_gridCells[i].m_ymax<<" zmax "<<m_gridCells[i].m_zmax<<"\n";
  }

}


void WindGrid::setInitialPressureField(ngl::Vec3 _pressureIn, ngl::Vec3 _pressureOut)
{

}

void WindGrid::setInitialVelocityField(ngl::Vec3 _velocityIn, ngl::Vec3 _velocityOut)
{



}
