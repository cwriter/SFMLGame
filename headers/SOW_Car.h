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

class SOW_Car : public GObjectBase
{
public:
	SOW_Car();
	virtual ~SOW_Car();
	///<summary>
	///Returns the force that the wheel could apply to the road of there was no friction issue (too much force will cause spins)
	///</summary>
	virtual PE::Force getDriveForce() {
		//Add third dimension for the wheel (will just be 0)
		auto more_dims = m_rear_wheel_orientation.addRow(0.0); 
		//Get the force vector from the cross product from wheel direction and the point where the wheel hits the ground, then remove the third dimension
		auto less_dims = more_dims.crossProduct(PE::Vector<3, double>({ 0.,0.,-m_wheel_radius })).removeRow();
		//Return the calulated force
		return PE::Force(less_dims *
				getMotorTorque() *
				m_gear_ratios[m_current_gear] *
				m_differential_ratio *
				m_transmission_efficiency
			);
	}
	///<summary>
	///Gets the motor torque and sets the member variable accordingly
	///</summary>
	virtual float getMotorTorque(){
		return (this->m_mtr_trq = (m_mtr_pwr) / (2.0f*float(PI)*m_mtr_rpm));
	}

protected:
	float m_max_fuel;//The fuel capacity
	float m_fuel_left; //The remaining fuel (m_fuel_left <= m_max_fuel)

	float m_mtr_rpm; //The motor rpm
	float m_mtr_pwr; //The motor power (in W)
	float m_mtr_trq; //The motor torque (in Nm)
	float m_mtr_displacement; //"Hubraum"

	float m_ped_gas_state;		//The gas pedal state (max is 100 meaning fully pressed)
	float m_ped_brake_state;	//The brake pedal state (max is 100 meaning fully pressed)
	float m_ped_clutch_state;	//The clutch pedal state (max is 100 meaning fully pressed)

	PE::Vector2df m_front_wheel_orientation; //The direction the wheels are pointing
	PE::Vector2df m_rear_wheel_orientation; //equals car orientation
	float m_wheel_radius;

	PE::Mass m_mass; //The mass of the car
	
	std::vector<float> m_gear_ratios;	//The vector of gear ratios
	char m_current_gear;				//The current gear index
	float m_differential_ratio;			//The differential ratio	
	float m_transmission_efficiency;	//The transmission efficiency
};

