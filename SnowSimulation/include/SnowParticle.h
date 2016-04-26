#ifndef SNOWPARTICLE
#define SNOWPARTICLE

#include <math.h>
#include <cmath>

#include <ngl/Vec3.h>

class Emitter;

class snowParticle
{
public:
  snowParticle(ngl::Vec3 _position, Emitter* _emitter, float _liftRadius, float _liftOmega);
  void update(float _timeStep);
  void draw() const;

private:
  ngl::Vec3 m_position;
  ngl::Vec3 m_velocity;
  ngl::Vec3 m_acceleration;

  float m_liftRadius;
  float m_liftOmega;

  float m_lifeTime;

  ngl::Vec3 m_gravity=ngl::Vec3(0,-9.81,0);

  const Emitter* m_emitter;
};

#endif // SNOWPARTICLE

