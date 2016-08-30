// This source file is part of the 'tau' open source project.
// Copyright (c) 2016, Yuriy Vosel.
// Licensed under Boost Software License.
// See LICENSE.txt for the licence information.

#pragma once

#if defined(TAU_HAS_BOOST_ACCESS)
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
namespace tau {
namespace layout_generation {

	template <typename T>
		struct my_shared_ptr {
			typedef boost::shared_ptr<T> MyInternalType;
			static MyInternalType createInstance(const T & object)
			{
				return boost::make_shared<T>(object);
			}
		};
}
} // namespace tau

#elif defined(TAU_CPP_03_COMPATIBILITY)

#include <cstddef>  //for size_t

namespace tau {
namespace layout_generation {

	struct PtrWithCount
	{
		void * payload;
		size_t count;
		
		PtrWithCount(void * toStore):count(0), payload(toStore){};
		
		void countUp() {
			++count;
		}
		
		void countDown() {
			--count;
		}
	};

	//NOTE: this base class is here for the sole purpose of restricting access to m_countedPtr.
	//  if it is declared in the my_shared_ptrHolder, we have to make it public, so that
	//  different instantiations of template have access to each other's field.
	class my_shared_ptrHolder_base {
	protected:
		PtrWithCount * m_countedPtr;
		
		my_shared_ptrHolder_base(PtrWithCount * prtToTrack) : m_countedPtr(prtToTrack) {};
		void reassignTrackedPtr(my_shared_ptrHolder_base const & other) {
			m_countedPtr = other.m_countedPtr;
		}
	};
	template <typename U>
		class my_shared_ptrHolder : public my_shared_ptrHolder_base
		{
			void countDown(PtrWithCount * ptr)
			{
				if (ptr != 0) {
					ptr->countDown();
					if (ptr->count == 0) {
						delete (static_cast<U*>(ptr->payload));
						delete ptr;
					}
				}
			}
		public:
			my_shared_ptrHolder() : my_shared_ptrHolder_base(0) {}
			
			my_shared_ptrHolder(U * pointerToStore) : my_shared_ptrHolder_base(new PtrWithCount(pointerToStore))
			{				
				m_countedPtr->countUp();
			};
			
			my_shared_ptrHolder(const my_shared_ptrHolder<U> & other) : my_shared_ptrHolder_base(other)
			{				
				m_countedPtr->countUp();
			};

			my_shared_ptrHolder<U> & operator =(const my_shared_ptrHolder<U> & other)
			{
				//NOTE: we don't need to check for self-assignment here because the implementation works fine in this case
				PtrWithCount * old = m_countedPtr;
				reassignTrackedPtr(other);
				m_countedPtr->countUp();
				countDown(old);
				return *this;
			};
			
			template <typename V>
			my_shared_ptrHolder(const my_shared_ptrHolder<V> & other) : my_shared_ptrHolder_base(other)
			{
				V * ptrV = 0;
				U * ptrU = static_cast<U*>(ptrV); //TODO: find a better way to check that pointers can be converted from one to another
				m_countedPtr->countUp();
			};

			template <typename V>
			my_shared_ptrHolder<U> & operator =(const my_shared_ptrHolder<V> & other)
			{
				V * ptrV = 0;
				U * ptrU = static_cast<U*>(ptrV); //TODO: find a better way to check that pointers can be converted from one to another
				//NOTE: we don't need to check for self-assignment here because the implementation works fine in this case
				PtrWithCount * old = m_countedPtr;
				reassignTrackedPtr(other);
				m_countedPtr->countUp();
				countDown(old);
				return *this;
			};
		
			U * get() const
			{
				return static_cast<U*>(m_countedPtr->payload);
			}

			~my_shared_ptrHolder() {
				countDown(m_countedPtr);
			};
		};

	template <typename T>
		struct my_shared_ptr {
			typedef my_shared_ptrHolder<T> MyInternalType;
			static MyInternalType createInstance(const T & object)
			{
				return my_shared_ptrHolder<T>(new T(object));
			}
			static MyInternalType wrapPtr(T * ptr)
			{
				return my_shared_ptrHolder<T>(ptr);
			}
		};
}
} // namespace tau
#else
//default way - use of c++11 shared pointer 
#include <memory>
namespace tau {
namespace layout_generation {
	template <typename T>
		struct my_shared_ptr {
			typedef std::shared_ptr<T> MyInternalType;
			static MyInternalType createInstance(const T & object)
			{
				return std::make_shared<T>(object);
			}
		};
}
} // namespace tau
#endif
