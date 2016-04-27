#ifndef WIND
#define WIND

#include <vector>

#include <ngl/Vec3.h>

struct WindCell
{
  WindCell(int _indexX, int _indexY, int _indexZ, float _xmin, float _xmax, float _ymin, float _ymax, float _zmin, float _zmax, ngl::Vec3 _velocity, ngl::Vec3 _pressure);

  int m_indexX;
  int m_indexY;
  int m_indexZ;

  float m_xmin;
  float m_xmax;
  float m_ymin;
  float m_ymax;
  float m_zmin;
  float m_zmax;

  WindCell* m_parent;
  std::vector <WindCell*> m_children;
  WindCell* m_sibling;

  ngl::Vec3 m_velocity;
  ngl::Vec3 m_pressure;
};

struct TreeCell
{
  TreeCell* m_parent;
  std::vector <TreeCell*> m_children;
  TreeCell* m_leftSibling;
  TreeCell* m_rightSibling;

  int indexX;
  int indexY;
  int indexZ;

  int m_windGridIndex;

  float m_xmin;
  float m_xmax;
  float m_ymin;
  float m_ymax;
  float m_zmin;
  float m_zmax;

  int containerLayer;


};

class WindGrid
{
public:
  static WindGrid* createWindGrid(ngl::Vec3 _centre, float _width, float _depth, float _height, int _divisions, ngl::Vec3 _velocityIn=ngl::Vec3(1.0, 1.0, 1.0), ngl::Vec3 _velocityOut=ngl::Vec3(0.0,0.0,0.0), ngl::Vec3 _pressureIn=ngl::Vec3(0.0,0.0,0.0), ngl::Vec3 _pressureOut=ngl::Vec3(0.0,0.0,0.0));
  static WindGrid* getWindGrid();

  WindCell* findCellFromPosition(ngl::Vec3 _position);
  void printCellDetails();
  void buildTree(TreeCell* current, int currentLayer, int totalLayers);



private:

  WindGrid(ngl::Vec3 _centre, float _width, float _depth, float _height, int _divisions, ngl::Vec3 _velocityIn=ngl::Vec3(1.0, 1.0, 1.0), ngl::Vec3 _velocityOut=ngl::Vec3(0.0,0.0,0.0), ngl::Vec3 _pressureIn=ngl::Vec3(0.0,0.0,0.0), ngl::Vec3 _pressureOut=ngl::Vec3(0.0,0.0,0.0));

  ngl::Vec3 setInitialVelocityField(ngl::Vec3 _velocityIn, ngl::Vec3 _velocityOut, int _i, int _j, int _k);
  ngl::Vec3 setInitialPressureField(ngl::Vec3 _pressureIn, ngl::Vec3 _pressureOut, int _i, int _j, int _k);

  static WindGrid* m_instance;

  ngl::Vec3 m_centre;
  float m_width;
  float m_depth;
  float m_height;

  int m_divisions;

  int m_sizeCellsX;
  int m_sizeCellsY;
  int m_sizeCellsZ;

  std::vector <WindCell> m_gridCells;

  int countBottom;
};


#endif // WIND

