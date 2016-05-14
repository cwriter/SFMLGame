#pragma once
#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <assert.h>
#include "Message.h"
#include "Pointer.h"
#include "Object.h"


#ifndef GRAVITY_CONSTANT
//The gravity constant (in m^3 * kg^-1 * s^-2)
#define GRAVITY_CONSTANT 6.67408e-11
#endif

#ifndef LIGHTSPEED_CONSTANT
#define LIGHTSPEED_CONSTANT 299792458.
#endif

namespace PE
{
	class Force;
	class Time;
	class Acceleration;
	class Velocity;
	class Mass;
	class Junction;


	template <typename T>
	class Matrix
	{
	public:
		Matrix(size_t width, size_t height) : m_width(width), m_height(height)
		{
			m_data.resize(width*height, T(0));
		}

		Matrix<T> operator*(const Matrix<T>& other)
		{
			assert(m_width == other.m_height);
			Matrix<T> ret(m_width, other.m_height);

			for (size_t row_i = 0; row_i < m_height; row_i++)
			{
				for (size_t col_i = 0; col_i < other.m_width; col_i++)
				{
					//Sum up
					T sum(0);
					for (size_t c = 0; c < m_width; c++)
					{
						sum += T(this->get(c, row_i) * other.get(col_i, c));
					}

					//Do the calculations
					ret.set(col_i, row_i, sum);
				}
			}

			return ret;
		}

		///<summary>
		///Compares if two matrices are equal.
		///</summary>
		bool operator==(const Matrix<T>& other) const
		{
			if (other.m_width != m_width || other.m_height != m_height) return false;
			//Compare bytes
			if (memcmp(m_data.data(), other.m_data.data(), m_height*m_width*sizeof(T)) == 0)
				return true; // return true if no differences have been found
			else
				return false;// otherwise just return false
		}

		///<summary>
		///Sets the entry at (x,y) to value
		///</summary>
		void set(size_t x, size_t y, T value)
		{
			assert(x < m_width);
			assert(y < m_height);

			m_data[y*m_width + x] = value;
		}

		///<summary>
		///Returns the entry at (x,y).
		///</summary>
		T get(size_t x, size_t y) const
		{
			assert(x < m_width);
			assert(y < m_height);

			return m_data[y*m_width + x];
		}

		///<summary>
		///Very time-consuming operation (compared to getRow())
		///</summary>
		std::vector<T> getColumn(size_t index)
		{
			assert(index < m_width);
			std::vector<T> ret(m_height);
			for (size_t i = 0; i < m_height; i++)
			{
				ret[i] = m_data[index + m_width*i];
			}

			return ret;
		}
		///<summary>
		///Gets a row
		///</summary>
		std::vector<T> getRow(size_t index)
		{
			assert(index < m_height);
			std::vector<T> ret(m_data.begin() + index*m_width, m_data.begin() + index*m_width + m_width);

			return ret;
		}

		size_t m_width;
		size_t m_height;
		std::vector<T> m_data; //Formatted as lines pinned together (line1line2...)
	};

	template <size_t dim, typename T>
	class Vector
	{
	public:
		Vector()
		{
			for (size_t i = 0; i < dim; i++)
			{
				this->m_vec[i] = T(0);
			}
		}
		Vector(std::initializer_list<T> list) {
			size_t i = 0;
			for (const T* it = begin(list); it != end(list); ++it) {
				this->m_vec[i] = *it;
				i++;
			}
		}
		~Vector()
		{

		}
		T absLength() const
		{
			return sqrt(pow(x, T(2)) + pow(y, T(2)));
		}

		Vector<dim + 1, T> addRow(T val = T(0)) {
			Vector<dim + 1, T> ret;
			for (size_t i = 0; i < dim; i++)
				ret.m_vec[i] = this->m_vec[i];

			ret.m_vec[dim] = val;
			return ret;
		}

		Vector<dim - 1, T> removeRow() {
			Vector<dim - 1, T> ret;
			for (size_t i = 0; i < dim - 1; i++)
				ret.m_vec[i] = this->m_vec[i];
			return ret;
		}

		Vector<dim, T> operator+(const Vector<dim, T>& v) const
		{
			Vector<dim, T> ret;
			for (size_t i = 0; i < dim; i++)
			{
				ret.m_vec[i] = this->m_vec[i] + v.m_vec[i];
			}
			return ret;
		}

		Vector<dim, T> operator-(const Vector<dim, T>& v) const
		{
			Vector<dim, T> ret;
			for (size_t i = 0; i < dim; i++)
			{
				ret.m_vec[i] = this->m_vec[i] - v.m_vec[i];
			}
			return ret;
		}

		Vector<dim, T> operator*(const double s) const
		{
			Vector<dim, T> ret;
			for (size_t i = 0; i < dim; i++)
			{
				ret.m_vec[i] = this->m_vec[i] * s;
			}
			return ret;
		}
		Vector<dim, T> operator/(const double s) const
		{
			Vector<dim, T> ret;
			for (size_t i = 0; i < dim; i++)
			{
				ret.m_vec[i] = this->m_vec[i] / s;
			}
			return ret;
		}

		Vector<dim, T> operator=(const Vector<dim, T>& v)
		{
			for (size_t i = 0; i < dim; i++)
			{
				this->m_vec[i] = v.m_vec[i];
			}
			return *this;
		}

		T dotProduct(const Vector<dim, T>& v) const
		{
			T res = T(0);
			for (size_t i = 0; i < dim; i++)
			{
				res += this->m_vec[i] * v->m_vec[i];
			}
			return res;
		}


		Vector<3, T> crossProduct(Vector<3, T> v) const
		{
			Vector<3, T> ret;
			ret.x = this->y*v.z - this->z*v.y;
			ret.y = this->z*v.x - this->x*v.z;
			ret.z = this->x*v.y - this->y*v.x;

			return ret;
		}

		T& x = m_vec[0];
		T& y = m_vec[1];
		T& z = m_vec[2];

		T m_vec[dim];



	};

	typedef Vector<2, double> Vector2df;

	class Mass
	{
	public:
		Mass(double s) {
			this->m_scalar = s;
		}
		Mass() {
			m_scalar = 0;
		}

		inline double getScalar() const { return this->m_scalar; }
		
		inline Mass operator+(const Mass& m) const
		{
			return Mass(m.getScalar());
		}
		
		inline Mass operator+=(const Mass& m)
		{
			this->m_scalar = m.getScalar();
			return *this;
		}
	private:
		double m_scalar;
	};

	class Time
	{
	public:
		Time() {
			val = 0.;
		}
		Time(double time) {
			val = time;
		}
		inline double getValue() const { return val; }
	private:
		double val;
	};
	class Acceleration
	{
	public:
		Acceleration()
		{

		}
		Acceleration(const Force& f, const Mass& m);

		Acceleration operator+(const Acceleration& a)
		{
			Acceleration ret;
			ret.m_vec = this->m_vec + a.getVector();
			return ret;
		}

		inline Vector2df getVector() const { return this->m_vec; }
	private:

		Vector2df m_vec;
	};


	class Velocity
	{
	public:
		Velocity()
		{

		}

		Velocity(double x, double y)
		{
			this->m_vec.x = x;
			this->m_vec.y = y;
		}
		Velocity(const Acceleration& a, const Time& dt)
		{
			this->m_vec.x = a.getVector().x*dt.getValue();
			this->m_vec.y = a.getVector().y*dt.getValue();
		}

		double abs() const {
			return m_vec.absLength();
		}

		inline Vector2df getVector() const { return this->m_vec; }
		inline void setVector(Vector2df& vector) { this->m_vec = vector; }

		Velocity operator+(const Velocity& v)
		{
			Velocity ret;
			ret.m_vec = this->m_vec + v.getVector();
			return ret;
		}
	private:
		Vector2df m_vec;
	};


	class Force
	{
	public:
		Force(const Vector2df& v){
			m_vec = v;
		}
		Force() {

		}

		inline Vector2df getVector() const { return this->m_vec; }

		void setVector(const Vector2df& v) { this->m_vec = v; }

		Force operator+(const Force& f) const
		{
			return Force(this->m_vec + f.getVector());
		}

		Acceleration operator/(const Mass& m) const
		{
			return Acceleration(*this, m);
		}

		Force operator*(double f) const {
			return Force(PE::Vector2df({ m_vec.x*f, m_vec.y*f }));
		}
	private:
		Vector2df m_vec;
	};

	class PhysicObject
		: public virtual MsgObject
	{
	public:
		enum POflag
		{
			None = 0,//Normal
			Unaffected = 1,//Don't calculate
			Sleep = 2
		};

		enum DestructionType
		{
			Disappear = 0,	//Normal, 0 overhead, clean, instant
			Particles		//Expensive
		};

		enum PhysicObjectMessages
		{
			Loaded = Loadable::LoadableMessages::Loaded,
			Unloaded = Loadable::LoadableMessages::Unloaded,
			Preloaded = Loadable::LoadableMessages::Preloaded,
			Destroyed = Loadable::LoadableMessages::Destroyed,
		};


		PhysicObject()
		{
			m_flags = POflag::None;
		}
		virtual ~PhysicObject()
		{
			std::string o;
			SFG::Util::getStackTrace(o);
			//printf("PhysicObject destroyed:\n%s", o.c_str());
		}

		inline bool isActive() const {
			return !(m_flags & POflag::Unaffected);
		}

		void activate(int flag, bool b)
		{
			if (b)
				m_flags |= flag;
			else
				m_flags &= ~flag;
		}

		Mass getMass() const {
			return m_mass;
		}

		Mass getRelativeMass() const {
			return Mass(m_mass.getScalar() / sqrt(1 - (std::pow(getVelocity().abs(), 2.0) / std::pow(LIGHTSPEED_CONSTANT, 2.0))));
		}

		void setMass(const Mass& m) {
			this->m_mass = m;
		}

		Velocity getVelocity() const {
			return this->m_vel;
		}

		void setVelocity(const Velocity& v) {
			this->m_vel = v;
		}

		Acceleration getAcceleration() const {
			return this->m_acc;
		}

		void setAcceleration(const Acceleration& a) {
			this->m_acc = a;
		}

		int getDestructionType() {
			return this->m_destruction_type;
		}

		void setDestructionType(int t) {
			this->m_destruction_type = t;
		}

		void addForceToCycle(const Force& f)
		{
			m_force = m_force + f;
		}

		virtual double x() const = 0;
		virtual double y() const = 0;

		///<summary>
		///Applies forces to the relative mass a = F/(y*m)
		///</summary>
		///<param name="time">Time in ms</param>
		void finishPhysicsCycle(double time);



	private:
		Mass m_mass;
		Velocity m_vel;
		Acceleration m_acc;
		Force m_force;

		int m_destruction_type;

		int m_flags;

	};

	//A junction
	class Junction
	{
	public:
		enum JunctionType
		{
			Hook = 0 //dF = dx * D (D = dF / dx)
		};
	private:

		int m_type;
		int m_status;

		int m_D;
		int m_Unexpandedlength;
		bool m_twoWay;	//Whether the Junction can damp both ways or not


		Vector2df m_sustainableDrift;
		Vector2df m_sustainableAngle;
		Vector2df m_counterDriftForce;
		Vector2df m_counterAngleForce;

	};


	///<summary>
	///The engine itself. It will handle all objects that require physics handling
	///</summary>
	class PhysicsEngine
		: public MsgObject
	{
	public:
		PhysicsEngine();
		virtual ~PhysicsEngine();

		///<summary>
		///Sets mutual forces (Kepler's law of gravity). Complexity is O(n!)
		///</summary>
		///<param name="threshhold">Set to an appropriate mass to ignore tiny forces.
		///This function will ignore forces where the maximum mass of m1 and m2
		///does not exceed the threshhold to compensate for forces too small to have reasonable impact.
		///Default is 1.0*10^9kg or 1000 tons.
		///</param>
		void applyMutualForces(Mass threshhold = Mass(1.0e9));

		///<summary>
		///Add an object to the engine.
		///</summary>
		void addObject(const SFG::Pointer<PhysicObject>& ptr);
		
		///<summary>
		///Removes an object from the engine.
		///</summary>
		void removeObject(const SFG::Pointer<PhysicObject>& ptr);
		
		

		int notify(int msg) override
		{
			return 0;
		}

		///<summary>
		///Removes all links to every object previously used in this class.
		///</summary>
		void removeAllElements();
	protected:

		///<summary>
		///Handler for object messages
		///</summary>
		int object_message_handler(int msg, const SFG::Pointer<MsgPackage>& pkg);
	private:
		std::map<PhysicObject*, SFG::Pointer<PhysicObject>> m_objects;	//The objects managed by this physics engine
	};

};
