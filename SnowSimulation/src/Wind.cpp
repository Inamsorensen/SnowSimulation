#include "Wind.h"

#include <cmath>
#include <iostream>
#include <stdexcept>

WindCell::WindCell(int _indexX, int _indexY, int _indexZ, float _xmin, float _xmax, float _ymin, float _ymax, float _zmin, float _zmax, ngl::Vec3 _velocity, ngl::Vec3 _pressure)
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

  m_velocity=_velocity;
  m_pressure=_pressure;

}

WindGrid* WindGrid::m_instance=nullptr;

WindGrid::WindGrid(ngl::Vec3 _centre, float _width, float _depth, float _height, int _divisions, ngl::Vec3 _velocityIn, ngl::Vec3 _velocityOut, ngl::Vec3 _pressureIn, ngl::Vec3 _pressureOut)
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

        ngl::Vec3 initialVelocity=setInitialVelocityField(_velocityIn, _velocityOut, i, j, k);
        ngl::Vec3 initialPressure=setInitialPressureField(_pressureIn, _pressureOut, i, j, k);

        m_gridCells.push_back(WindCell(i, j, k, xmin, xmax, ymin, ymax, zmin, zmax, initialVelocity, initialPressure));

      }
    }
  }

  //For optimised search, create tree

//  int currentDivision=0;
//  int totalDivisions=pow(m_divisions,3);

  countBottom=0;

  int currentLayer=0;
  int totalCellNo=pow(m_divisions,3);
  int finalLayerNo=log(totalCellNo)/log(8);
  std::cout<<"Final Layer Number: "<<finalLayerNo<<"\n";

  buildTree(nullptr, currentLayer, finalLayerNo);


}

void WindGrid::buildTree(TreeCell* current,int currentLayer, int totalLayers)
{
  //End when at bottom of tree
  if(currentLayer == totalLayers)
  {

    ++countBottom;
    std::cout<<"Count Bottom: "<<countBottom<<"\n";
    std::cout<<"Current Layer: "<<currentLayer<<"\n";

    current->m_windGridIndex=(pow(m_divisions,2)*current->indexX)+(m_divisions*current->indexY)+current->indexZ;

    std::cout<<"Bottom layer reached\n";
    std::cout<<"Wind Grid Index: "<<current->m_windGridIndex<<"\n";
    std::cout<<"i: "<<current->indexX<<" j: "<<current->indexY<<" k: "<<current->indexZ<<"\n";

    return;


  }

  //Create root
  if (current==nullptr)
  {
    std::cout<<"CurrentDivisions: "<<currentLayer<<"\n";
    //Create root
    TreeCell root;
    root.m_xmin=m_centre.m_x-(m_width/2.0);
    root.m_xmax=m_centre.m_x+(m_width/2.0);
    root.m_ymin=m_centre.m_y-(m_height/2.0);
    root.m_ymax=m_centre.m_y+(m_height/2.0);
    root.m_zmin=m_centre.m_z-(m_depth/2.0);
    root.m_zmax=m_centre.m_z+(m_depth/2.0);

    std::cout<<"Root:";
    std::cout<<"xmin "<<root.m_xmin<<" xmax "<<root.m_xmax<<" ymin "<<root.m_ymin<<" ymax "<<root.m_ymax<<" zmin "<<root.m_zmin<<" zmax "<<root.m_zmax<<"\n";

    root.containerLayer=0;

    root.indexX=0;
    root.indexY=0;
    root.indexZ=0;

    buildTree(&root,currentLayer,totalLayers);
  }
  else
  {

    std::cout<<"Current Layer: "<<currentLayer<<"\n";

    // child nodes
    float child_width=(current->m_xmax-current->m_xmin)/2.0;
    float child_height=(current->m_ymax-current->m_ymin)/2.0;
    float child_depth=(current->m_zmax-current->m_zmin)/2.0;

    int ChildCount=0;

    for(int i= 0; i<2; ++i)
    {
      for (int j=0; j<2; ++j)
      {
        for (int k=0; k<2; ++k)
        {
          TreeCell* child = new TreeCell;
          // To do - we have to find out the boundaries of each child
          child->m_xmin=current->m_xmin+(i*child_width);
          child->m_xmax=current->m_xmin+((1+i)*child_width);
          child->m_ymin=current->m_ymin+(j*child_height);
          child->m_ymax=current->m_ymin+((1+j)*child_height);
          child->m_zmin=current->m_zmin+(k*child_depth);
          child->m_zmax=current->m_zmin+((1+k)*child_depth);

          ChildCount+=1;
//          std::cout<<"Child:"<<ChildCount<<"\n";
//          std::cout<<"xmin"<<child->m_xmin<<" xmax "<<child->m_xmax<<" ymin "<<child->m_ymin<<" ymax "<<child->m_ymax<<" zmin "<<child->m_zmin<<" zmax "<<child->m_zmax<<"\n";

          child->m_parent=current;
          child->containerLayer=child->m_parent->containerLayer+1;

          child->indexX=(2*current->indexX)+i;
          child->indexY=(2*current->indexY)+j;
          child->indexZ=(2*current->indexZ)+k;

//          std::cout<<"i: "<<child->indexX<<" j: "<<child->indexY<<" k: "<<child->indexZ<<"\n";

          current->m_children.push_back(child);
        }
      }
    }

    if (current->m_children.size()>0)
    {
      TreeCell* parent=current;
      for (int iChild=0; iChild<8; ++iChild)
      {
        current=parent->m_children[iChild];
        currentLayer=parent->containerLayer+1;

        if (iChild==0)
        {
          current->m_leftSibling=nullptr;
          current->m_rightSibling=parent->m_children[iChild+1];
        }
        else if (iChild==7)
        {
          current->m_leftSibling=parent->m_children[iChild-1];
          current->m_rightSibling=nullptr;
        }
        else
        {
          current->m_leftSibling=parent->m_children[iChild-1];
          current->m_rightSibling=parent->m_children[iChild+1];
        }

        buildTree(current, currentLayer, totalLayers);
      }
    }
  }
}



WindGrid* WindGrid::createWindGrid(ngl::Vec3 _centre, float _width, float _depth, float _height, int _divisions, ngl::Vec3 _velocityIn, ngl::Vec3 _velocityOut, ngl::Vec3 _pressureIn, ngl::Vec3 _pressureOut)
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
    std::cout<<"velX "<<m_gridCells[i].m_velocity.m_x<<" velY "<<m_gridCells[i].m_velocity.m_y<<" velZ "<<m_gridCells[i].m_velocity.m_z<<"\n";
    std::cout<<"presX "<<m_gridCells[i].m_pressure.m_x<<" presY "<<m_gridCells[i].m_pressure.m_y<<" presZ "<<m_gridCells[i].m_pressure.m_z<<"\n";
  }

}


ngl::Vec3 WindGrid::setInitialPressureField(ngl::Vec3 _pressureIn, ngl::Vec3 _pressureOut, int _i, int _j, int _k)
{
  float gradientX=(_pressureOut.m_x-_pressureIn.m_x)/m_divisions;
  float gradientY=(_pressureOut.m_y-_pressureIn.m_y)/m_divisions;
  float gradientZ=(_pressureOut.m_z-_pressureIn.m_z)/m_divisions;

  ngl::Vec3 pressureCell;

  pressureCell.m_x=_pressureIn.m_x+(gradientX*_i);
  pressureCell.m_y=_pressureIn.m_y+(gradientY*_j);
  pressureCell.m_z=_pressureIn.m_z+(gradientZ*_k);

  return pressureCell;
}

ngl::Vec3 WindGrid::setInitialVelocityField(ngl::Vec3 _velocityIn, ngl::Vec3 _velocityOut, int _i, int _j, int _k)
{
  float gradientX=(_velocityOut.m_x-_velocityIn.m_x)/m_divisions;
  float gradientY=(_velocityOut.m_y-_velocityIn.m_y)/m_divisions;
  float gradientZ=(_velocityOut.m_z-_velocityIn.m_z)/m_divisions;

  ngl::Vec3 velocityCell;

  velocityCell.m_x=_velocityIn.m_x+(gradientX*_i);
  velocityCell.m_y=_velocityIn.m_y+(gradientY*_j);
  velocityCell.m_z=_velocityIn.m_z+(gradientZ*_k);

  return velocityCell;

}
