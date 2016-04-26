#include "SnowParticle.h"
#include "Emitter.h"

#include <ngl/ShaderLib.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/Transformation.h>
#include <ngl/Random.h>



snowParticle::snowParticle(ngl::Vec3 _position, Emitter *_emitter, float _liftRadius, float _liftOmega)
{
  m_position=_position;

  m_emitter=_emitter;

  m_liftRadius=_liftRadius;
  m_liftOmega=_liftOmega;

  m_lifeTime=0;

}

void snowParticle::update(float _timeStep)
{
  m_lifeTime+=_timeStep;

  //Calculate Lift velocity
  ngl::Vec3 circVelocity;
  circVelocity.m_x=(-1)*m_liftOmega*m_liftRadius*sin(m_liftOmega*m_lifeTime);
  circVelocity.m_y=m_liftOmega*m_liftRadius*cos(m_liftOmega*m_lifeTime);
  circVelocity.m_z=0;

  //Update acceleration
  ngl::Vec3 m_acceleration=m_gravity;

  //Update velocity
  m_velocity+=m_gravity*_timeStep;

  //Update position
  m_position+=(0.5*pow(_timeStep,2))*m_acceleration;
  m_position+=circVelocity*_timeStep;
}

void snowParticle::draw() const
{
  ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();
  ngl::Transformation transformation;
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  //Pass in shader name as parameter
  shader->use(m_emitter->getShaderName());
  transformation.setPosition(m_position);

  ngl::Mat4 MV;
  ngl::Mat4 MVP;
  ngl::Mat3 normalMatrix;
  ngl::Mat4 M;
  M=transformation.getMatrix();
  MV=M*m_emitter->getCamera()->getViewMatrix();
  MVP=MV*m_emitter->getCamera()->getProjectionMatrix();
  normalMatrix=MV;
  normalMatrix.inverse();

  shader->setShaderParamFromMat4("MV",MV);
  shader->setShaderParamFromMat4("MVP",MVP);
  shader->setShaderParamFromMat3("normalMatrix",normalMatrix);
  shader->setShaderParamFromMat4("M",M);

  prim->draw("sphere");
}
