#ifndef EMITTER
#define EMITTER

#include <ngl/Vec3.h>
#include <cstdlib>
#include <iostream>
#include <vector>

#include <ngl/Camera.h>

#include "SnowParticle.h"
#include "Wind.h"

struct BoundingBox
{
  ngl::Vec3 m_centre;
  float m_xmin;
  float m_xmax;
  float m_ymin;
  float m_ymax;
  float m_zmin;
  float m_zmax;

};

class Emitter
{
public:

  Emitter(ngl::Vec3 _position, int _width, int _depth, int _height, int _divisions, int _noParticles, float _maxLiftOmega, float _maxLiftRadius);
  ~Emitter();
  void update(float _timeStep);
  void drawParticles();

  inline void setCamera(ngl::Camera* _camera){m_camera=_camera;}
  inline ngl::Camera* getCamera() const {return m_camera;}
  inline void setShaderName(const std::string &_shaderName){m_shaderName=_shaderName;}
  inline std::string getShaderName() const {return m_shaderName;}


private:
  ngl::Vec3 m_position;
  int m_width;
  int m_depth;
  int m_height;
  int m_divisions;

  int m_noParticles;

  float m_maxLiftRadius;
  float m_maxLiftOmega;

  BoundingBox m_boundBox;
  std::vector <snowParticle*> m_particles;

  ngl::Camera* m_camera;
  std::string m_shaderName;



};

#endif // EMITTER

