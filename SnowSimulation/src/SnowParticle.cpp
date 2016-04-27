#include "SnowParticle.h"
#include "Emitter.h"

#include <ngl/ShaderLib.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/Transformation.h>
#include <ngl/Random.h>



snowParticle::snowParticle(ngl::Vec3 _position, Emitter *_emitter, float _liftRadius, float _liftOmega)
{
  m_position=_position;
  m_origin=_position;

  m_emitter=_emitter;

  m_liftRadius=_liftRadius;
  m_liftOmega=_liftOmega;

  m_lifeTime=0;

}

void snowParticle::update(float _timeStep)
{


  m_lifeTime+=10*_timeStep;

  //Calculate Lift velocity
  ngl::Vec3 circVelocity;
  circVelocity.m_x=(-1)*m_liftOmega*m_liftRadius*sin(m_liftOmega*m_lifeTime);
  circVelocity.m_y=0;
  circVelocity.m_z=m_liftOmega*m_liftRadius*cos(m_liftOmega*m_lifeTime);

  //Calculate velocity due to wind
  WindGrid* wind=WindGrid::getWindGrid();
  WindCell* windCell=wind->findCellFromPosition(m_position);

  ngl::Vec3 windVelocity=windCell->m_velocity;

//  std::cout<<"windVelX "<<windVelocity.m_x<<" windVelY "<<windVelocity.m_y<<" windVelZ "<<windVelocity.m_z<<"\n";

  ngl::Vec3 fluidVelocity=windVelocity-m_velocity;

  std::cout<<"fluidVelX "<<fluidVelocity.m_x<<" fluidVelY "<<fluidVelocity.m_y<<" fluidVelZ "<<fluidVelocity.m_z<<"\n";

  float fluidVel2=fluidVelocity.dot(fluidVelocity); //NB! Should be divided by Vmax
  ngl::Vec3 fluidDir=fluidVelocity;
  fluidDir.normalize();

  ngl::Vec3 dragAcceleration=(fluidVel2*9.81)*fluidDir;

//  std::cout<<"dragX "<<dragAcceleration.m_x<<" dragY "<<dragAcceleration.m_y<<" dragZ "<<dragAcceleration.m_z<<"\n";

  //Update acceleration due to gravity
  ngl::Vec3 m_acceleration=m_gravity+dragAcceleration;
//  ngl::Vec3 m_acceleration=m_gravity;

  //Update position
  m_position+=m_velocity*_timeStep;

  //Update velocity
  m_velocity+=(m_acceleration)*_timeStep;
  std::cout<<"velX "<<m_velocity.m_x<<" velY "<<m_velocity.m_y<<" velZ "<<m_velocity.m_z<<"\n";

  //Update position
//  m_position+=(0.5*pow(_timeStep,2))*m_acceleration;
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

void snowParticle::reset()
{
  m_position=m_origin;
  m_velocity=ngl::Vec3(0.0,0.0,0.0);
  m_lifeTime=0.0;
}
