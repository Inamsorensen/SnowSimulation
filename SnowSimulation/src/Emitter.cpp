#include "Emitter.h"

#include <cstdlib>

#include "ngl/Random.h"

Emitter::Emitter(ngl::Vec3 _position, int _width, int _depth, int _noParticles, float _maxLiftOmega, float _maxLiftRadius)
{
  m_position=_position;
  m_width=_width;
  m_depth=_depth;
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
//    snowParticle* newParticle=new snowParticle(m_position, this, liftRadius, liftOmega);
    snowParticle* newParticle=new snowParticle(m_position*rand->randomPositiveNumber(1), this, liftRadius, liftOmega);
    m_particles.push_back(newParticle);
  }
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
    m_particles[i]->update(_timeStep);
  }
}

void Emitter::drawParticles()
{
  for (int i=0; i<m_noParticles; i++)
  {
    m_particles[i]->draw();
  }
}
