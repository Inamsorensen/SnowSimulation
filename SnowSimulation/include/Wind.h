#ifndef WIND
#define WIND

#include <vector>

#include <ngl/Vec3.h>

struct WindCell
{
  WindCell(int _indexX, int _indexY, int _indexZ, float _xmin, float _xmax, float _ymin, float _ymax, float _zmin, float _zmax);

  int m_indexX;
  int m_indexY;
  int m_indexZ;

  float m_xmin;
  float m_xmax;
  float m_ymin;
  float m_ymax;
  float m_zmin;
  float m_zmax;

  ngl::Vec3 velocity;
  ngl::Vec3 pressure;
};

class WindGrid
{
public:
  static WindGrid* createWindGrid(ngl::Vec3 _centre, float _width, float _depth, float _height, int _divisions, ngl::Vec3 _velocityIn, ngl::Vec3 _velocityOut, ngl::Vec3 _pressureIn, ngl::Vec3 _pressureOut);
  static WindGrid* getWindGrid();

  WindCell* findCellFromPosition(ngl::Vec3 _position);
  void printCellDetails();



private:

  WindGrid(ngl::Vec3 _centre, float _width, float _depth, float _height, int _divisions, ngl::Vec3 _velocityIn, ngl::Vec3 _velocityOut, ngl::Vec3 _pressureIn, ngl::Vec3 _pressureOut);

  void setInitialVelocityField(ngl::Vec3 _velocityIn, ngl::Vec3 _velocityOut);
  void setInitialPressureField(ngl::Vec3 _pressureIn, ngl::Vec3 _pressureOut);

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
};


#endif // WIND

