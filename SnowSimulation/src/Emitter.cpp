#include "Emitter.h"

#include <cstdlib>

#include "ngl/Random.h"

Emitter::Emitter(ngl::Vec3 _position, int _width, int _depth, int _height, int _divisions, int _noParticles, float _maxLiftOmega, float _maxLiftRadius)
{
  m_position=_position;
  m_width=_width;
  m_depth=_depth;
  m_height=_height;
  m_divisions=_divisions;

  m_boundBox.m_centre=_position;
  m_boundBox.m_xmin=_position.m_x-(_width/2.0);
  m_boundBox.m_xmax=_position.m_x+(_width/2.0);
  m_boundBox.m_ymin=_position.m_y-(_height/2.0);
  m_boundBox.m_ymax=_position.m_y+(_height/2.0);
  m_boundBox.m_zmin=_position.m_z-(_depth/2.0);
  m_boundBox.m_zmax=_position.m_z+(_depth/2.0);

  m_noParticles=_noParticles;

  m_maxLiftRadius=_maxLiftRadius;
  m_maxLiftOmega=_maxLiftOmega;

  float liftOmega;
  float liftRadius;

  for (int i=0; i<m_noParticles; i++)
  {
    ngl::Random* rand = ngl::Random::instance();
    rand->setSeed(i);
    liftOmega=rand->randomPositiveNumber(m_maxLiftOmega);
    liftRadius=rand->randomPositiveNumber(m_maxLiftRadius);

    ngl::Vec3 particlePosition=m_position;
//    particlePosition.m_x*=rand->randomPositiveNumber(1);
//    particlePosition.m_z*=rand->randomPositiveNumber(1);
    particlePosition.m_x+=rand->randomNumber(1);
    particlePosition.m_z+=rand->randomNumber(1);
    particlePosition.m_y+=(m_height/2.01);

    snowParticle* newParticle=new snowParticle(particlePosition, this, liftRadius, liftOmega);
    m_particles.push_back(newParticle);
  }

  WindGrid* wind=WindGrid::createWindGrid(m_position, m_width, m_depth, m_height, m_divisions);
}

Emitter::~Emitter()
{
  int noCurrParticles=m_particles.size();

  if (noCurrParticles!=0)
  {
    for (int i=0; i<noCurrParticles; i++)
    {
      delete m_particles[i];
      std::cout<<"deleted snow particle\n";
    }
  }
  m_particles.clear();
}

void Emitter::update(float _timeStep)
{
  for (int i=0; i<m_noParticles; i++)
  {
    ngl::Vec3 particlePosition=m_particles[i]->getPosition();
    std::cout<<"posX "<<particlePosition.m_x<<" posY "<<particlePosition.m_y<<" posZ "<<particlePosition.m_z<<"\n";

    if(particlePosition.m_x>m_boundBox.m_xmin && particlePosition.m_x<m_boundBox.m_xmax)
    {
      if (particlePosition.m_y>m_boundBox.m_ymin && particlePosition.m_y<m_boundBox.m_ymax)
      {
        if (particlePosition.m_z>m_boundBox.m_zmin && particlePosition.m_z<m_boundBox.m_zmax)
        {
          m_particles[i]->update(_timeStep);
          ngl::Vec3 newParticlePosition=m_particles[i]->getPosition();
          std::cout<<"posX "<<newParticlePosition.m_x<<" posY "<<newParticlePosition.m_y<<" posZ "<<newParticlePosition.m_z<<"\n";
        }
        else
        {
          m_particles[i]->reset();
        }
      }
      else
      {
        m_particles[i]->reset();
      }
    }
    else
    {
      m_particles[i]->reset();
    }

//    m_particles[i]->update(_timeStep);
  }
}

void Emitter::drawParticles()
{
  for (int i=0; i<m_noParticles; i++)
  {
    m_particles[i]->draw();
  }
}
