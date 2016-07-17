#pragma once
#include "stdafx.h"
#include "Object.h"
#include "Physics.h"
#include "GameState.h"


///<summary>
///Loads specific car data that ModuleG2D doesn't handle.
///</summary>
class Module_SOW_Car : public ModuleG2D
{
public:
    int load(const XMLReader& xmldata) override;
private:

};

class SOW_Wheel
{
public:
	SOW_Wheel()
	{
		m_steer = 0.;
		m_wheel_radius = 0.;
		m_current_rps = 0.;
	}
	
	void setMass(const PE::Mass& m) {
		m_mass = m;
	}
	
	PE::Mass getMass() const {
		return m_mass;
	}
	
	void setSteering(float s) {
		m_steer = s;
	}
	
	float getSteering() const {
		return m_steer;
	}
	
	void setCurrentRPS(float rps){
		this->m_current_rps = rps;
	}
	
	void setWheelRadius(float r){
		this->m_wheel_radius = r;
	}
	
	float getCurrentRPS() const {
		return this->m_current_rps;
	}
	
	float getWheelRadius() const {
		return this->m_wheel_radius;
	}
	
	PE::Force getResultingForce(float baserot,
								float static_friction_coefficient,
								float kinetic_friction_coefficient,
								float rolling_friction_coefficient,
								const PE::Velocity& basevel,
								float dt
   							)
	{
		PE::Force f;
		
		//Set the global rotation
		float rot = baserot + m_steer;
		
		//Get the vector of the current "wheel-velocity"
		float wheel_cf = 2*PI*m_wheel_radius;
		PE::Velocity wheelvel(cos(rot) * wheel_cf * m_current_rps, sin(rot) * wheel_cf * m_current_rps);
		
		//Get the velocity difference between the vehicle speed and the tire speed
		auto veldiff = wheelvel - basevel;
		
		PE::Acceleration a(veldiff.getVector().x.get_d() / dt, veldiff.getVector().y.get_d() / dt);
		if(a.getVector().absLength().get_d() <= EARTH_GRAVITY * static_friction_coefficient)
		{
			//We are in stiction
			
			//Synchronize speeds (not sure if this is actually correct...) #TODO: Check
			m_current_rps = basevel.abs() / wheel_cf;
			
			//Return the force 
			f.setVector(a.getVector() * static_friction_coefficient * m_mass.getScalar().get_d());
			return f;
		}
		else
		{
			//We are gliding
			
			f.setVector(a.getVector() * kinetic_friction_coefficient * m_mass.getScalar().get_d());
			return f;
		}
		
		
		return f;
	}
	
private:	
	float m_steer;
	
	PE::Mass m_mass;
	
	float m_current_rps;
	float m_wheel_radius;
};


class SOW_Car : public GObjectBase, public PE::PhysicObject
{
	/*||-----||
	 *    |
	 *    |
	 *    |    
	 *||-----||
	 */
public:
    SOW_Car();
    virtual ~SOW_Car();
   
	
    ///<summary>
    ///Gets the motor torque and sets the member variable accordingly
    ///</summary>
    virtual float getMotorTorque() {
        return (this->m_mtr_trq = (m_mtr_pwr) / (2.0f*float(PI)*m_mtr_rpm));
    }
    
    virtual int load(const XMLReader& data) override
    {
		
		return 0;
	}

    virtual int update(float dt)
	{
		
		m_wheel_front_left.setSteering(m_steering);
		m_wheel_front_right.setSteering(m_steering);
		
		m_wheel_front_left.setCurrentRPS(m_mtr_rpm * m_gear_ratios[m_current_gear] * m_differential_ratio);
		
		const float c_f_s = 0.8;
		const float c_f_k = 0.6;
		const float c_f_r = 0.1;
		auto f1 = m_wheel_front_left.getResultingForce(getRotation(), c_f_s, c_f_k, c_f_r, getVelocity(), dt);
		auto f2 = m_wheel_front_right.getResultingForce(getRotation(), c_f_s, c_f_k, c_f_r, getVelocity(), dt);
		
		auto f3 = m_wheel_rear_left.getResultingForce(getRotation(), c_f_s, c_f_k, c_f_r, getVelocity(), dt);
		auto f4 = m_wheel_rear_right.getResultingForce(getRotation(), c_f_s, c_f_k, c_f_r, getVelocity(), dt);
		
		PE::Force total = f1 + f2 + f3 + f4;
		this->addForceToCycle(total);
		this->finishPhysicsCycle(dt);
		
		this->setPosition(x().get_d() + getVelocity().getVector().x.get_d() * dt,
						  y().get_d() + getVelocity().getVector().y.get_d() * dt);
		
		return 0;
	}
    
    virtual mpf_class x() const override
    {
		return this->getPosition().x;
	}
    
    
    virtual mpf_class y() const override
    {
		return this->getPosition().y;
	}
	
	void accel()
	{
		m_ped_gas_state = 50.f;
	}
	
	void stopAccel()
	{
		m_ped_gas_state = 0.f;
	}
	
protected:
	
	//Wheels
	SOW_Wheel m_wheel_front_left;
	SOW_Wheel m_wheel_front_right;
	SOW_Wheel m_wheel_rear_left;
	SOW_Wheel m_wheel_rear_right;
	
	float m_steering;
	
    float m_max_fuel;//The fuel capacity
    float m_fuel_left; //The remaining fuel (m_fuel_left <= m_max_fuel)
    
    float m_mtr_rpm; //The motor rpm
    float m_mtr_pwr; //The motor power (in W)
    float m_mtr_trq; //The motor torque (in Nm)
    float m_mtr_displacement; //"Hubraum"
	
    float m_ped_gas_state;		//The gas pedal state (max is 100 meaning fully pressed)
    float m_ped_brake_state;	//The brake pedal state (max is 100 meaning fully pressed)
    float m_ped_clutch_state;	//The clutch pedal state (max is 100 meaning fully pressed)

    std::vector<float> m_gear_ratios;	//The vector of gear ratios
    char m_current_gear;				//The current gear index
    float m_differential_ratio;			//The differential ratio
    float m_transmission_efficiency;	//The transmission efficiency

};

