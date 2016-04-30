#pragma once
#include "stdafx.h"

//SFMLGameEngine defaults
namespace SFG
{
	///<summary>
	///Manages pointers by counting references
	///</summary>
	template<typename T>
	class Pointer
	{
	public:
		///<summary>
		///Default constructor; sets everything to nullptr
		///</summary>
		Pointer()
		{
			this->m_ref_count = nullptr;
			this->m_ptr = nullptr;

#ifdef _DEBUG
			SFG::Util::getStackTrace(this->m_constructed_in);
#endif
		}

		///<summary>
		///Creates a new SFG::Pointer from a raw pointer
		///</summary>
		Pointer(T* from) {
			this->m_ptr = from;
			this->m_ref_count = new size_t(1);
			if (m_ref_count == nullptr) {
				//Big issue
				printf("[Critical] Failed to allocate memory in SFG::Pointer constructor in %s:%d\n", __FILE__, __LINE__);
			}
#ifdef _DEBUG
			SFG::Util::getStackTrace(this->m_constructed_in);
#endif
		}

		///<summary>
		///Creates a new SFG::Pointer from a raw reference pointer
		///</summary>
		Pointer(T& from) {
			//Pointer(&from);
			this->m_ptr = &from;
			this->m_ref_count = new size_t(1);
			if (m_ref_count == nullptr) {
				//Big issue
				printf("[Critical] Failed to allocate memory in SFG::Pointer constructor in %s:%d\n", __FILE__, __LINE__);
			}
#ifdef _DEBUG
			SFG::Util::getStackTrace(this->m_constructed_in);
#endif
		}

		///<summary>
		///Shares <c>other</c>'s data with <c>this</c>.
		///</summary>
		Pointer(const Pointer<T>& other) {
			//Check if pointers already set not necessary here: It's a constructor
			this->m_ptr = other.getElement(); //Get other's data ptr
			m_ref_count = other.getCountPtr(); //Get other's count ptr

			//Increase the refcount
			inc_refcount();
#ifdef _DEBUG
			std::string tmp;
			SFG::Util::getStackTrace(tmp);
			this->m_constructed_in = tmp; 
#endif
		}
		///<summary>
		///Shares <c>other</c>'s data with <c>this</c> when other is of different type.
		///</summary>
		template<typename C>
		Pointer(const Pointer<C>& other) {
			//Check if pointers already set not necessary here: It's a constructor
			
			//The following code will become necessary when ambiguous function call problems arise!!! #TODO: Check
			/*this->m_ptr = dynamic_cast<T*>(other.getElement()); //Get other's data ptr
			assert(m_ptr != nullptr);	//This could happen...*/
			
			this->m_ptr =(T*)other.getElement(); //Get other's data ptr
			
			m_ref_count = other.getCountPtr(); //Get other's count ptr

			//Increase the refcount
			inc_refcount();
#ifdef _DEBUG
			SFG::Util::getStackTrace(this->m_constructed_in);
#endif
		}

		///<summary>
		///Resets the value held by the pointer with "to"
		///</summary>
		///<param name="to">The new object to hold</param>
		bool reset(T* to)
		{
			//Release the old references
			release();
			//Set a new ref counter
			m_ref_count = new size_t(1);
			if (m_ref_count == nullptr)
			{
				printf("[Critical] Failed to allocate memory for reference counter in %s:%d\n", __FILE__, __LINE__);
				return false;
			}
			//Set a new pointer
			m_ptr = to;

			assert(m_ptr == to);
			assert(m_ref_count != nullptr);
			assert(*m_ref_count >= 1);

#ifdef _DEBUG
			SFG::Util::getStackTrace(this->m_constructed_in);
#endif

			return true;
		}

		///<summary>
		///Resets the value held by the pointer with "to"
		///</summary>
		///<param name="to">The Pointer to share with</param>
		bool reset(const SFG::Pointer<T>& to)
		{
			//Release the old references
			release();
			//Get the other refcount
			m_ref_count = to.getCountPtr();
			if (m_ref_count == nullptr)
			{
				printf("[Critical] Failed to allocate memory for reference counter in %s:%d\n", __FILE__, __LINE__);
				return false;
			}
			//increase the ref count
			inc_refcount();
			//Set a new pointer
			m_ptr = to.getElement();

			assert(m_ptr == to.getElement());
			assert(m_ref_count != nullptr);
			assert(*m_ref_count >= 1);
			//m_ref_count == nullptr implies this->m_ptr == nullptr
			assert(!(this->m_ref_count == nullptr) || this->m_ptr == nullptr);
#ifdef _DEBUG
			SFG::Util::getStackTrace(this->m_constructed_in);
#endif
			return true;
		}
	private:
		///<summary>
		///Converts Pointer<c>T</c> to T*
		///</summary>
		operator T*() const  {
			return getElement();
		}

		///<summary>
		///Converts Pointer<c>T</c> to T&
		///</summary>
		operator T&() const
		{
			return *this->getElement();
		}

	public:

		template<typename C>
		SFG::Pointer<C> cast() const
		{
			//return (C*)this->m_ptr;
			return SFG::Pointer<C>(*this);
		}

		T* const getElement() const {
			return this->m_ptr;
		}

		///<summary>
		///Grants direct access to T*
		///</summary>
		T* const operator->() const {
			assert(m_ptr != nullptr);
			return this->m_ptr;
		}

		~Pointer()
		{
#ifdef _DEBUG
			if (m_ref_count == nullptr)
			{
				//Nothing was done at all, print warning and exit

				printf("[Warning] Unused Pointer destroyed\n");
				//DebugBreak();
				return;
			}
#endif
			release();
		}

		size_t* getCountPtr() const {
			return this->m_ref_count;
		}

		///<summary>
		///Returns whether the pointer held by this is dereferencable.
		///</summary>
		bool isValid() const {
			return (m_ptr != nullptr);
		}

		///<summary>
		///Deep checks whether the pointer held by this is equal to the pointer held by ptr
		///</summary>
		bool operator==(const SFG::Pointer<T>& ptr)
		{
			return (this->m_ptr == ptr.getElement());
		}

		///<summary>
		///Deep checks whether the pointer held by this is equal to the pointer ptr
		///</summary>
		bool operator==(T* ptr)
		{
			return (this->m_ptr == ptr);
		}

	private:
		SFG::Pointer<T>& operator=(T* obj)
		{
			return *this;
		}
	public:


		SFG::Pointer<T>& operator=(const SFG::Pointer<T>& obj)
		{
			/*
			//First, release
			release();

			//Set the reference counter
			this->m_ref_count = obj.getCountPtr();
			if (this->m_ref_count != nullptr)
			{
				//Increase refcount
				inc_refcount();
			}
			else
			{
				assert(obj.getElement() == nullptr);
			}
			//Set the pointer
			this->m_ptr = obj.getElement();

			//m_ref_count == nullptr implies this->m_ptr == nullptr
			assert(!(this->m_ref_count == nullptr) || this->m_ptr == nullptr);

			return *this;*/
			reset(obj);
			return *this;
		}

		///<summary>
		///Decreases the refcount manually
		///</summary>
		void dec_refcount(size_t val = 1)
		{
			assert(m_ref_count != nullptr);
			assert(*m_ref_count != 0);
			(*m_ref_count)-=val;
		}

		///<summary>
		///Increases the refcount manually
		///</summary>
		void inc_refcount(size_t val = 1)
		{
			assert(m_ref_count != nullptr);
			(*m_ref_count)+=val;
		}

		///<summary>
		///Releases the references
		///</summary>
		void release()
		{
			if (m_ref_count != nullptr)
			{
				//Decrease the counter
				dec_refcount();
				if (*m_ref_count == 0)
				{
					//Delete the object too
					if (m_ptr != nullptr) delete m_ptr;
					//Delete the ref counter
					delete m_ref_count;
				}
			}
			else
			{
				//If the ref count is nullptr, then the pointer has to be nullptr too
				assert(m_ptr == nullptr);
			}

			//Reset
			m_ptr = nullptr;
			m_ref_count = nullptr;
		}

	private:
		T* m_ptr;
		size_t* m_ref_count;
#ifdef _DEBUG
		std::string m_constructed_in;
#endif
	};

};
