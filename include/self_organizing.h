/*
	(C) Copyright Thierry Seegers 2010-2011. Distributed under the following license:

	Boost Software License - Version 1.0 - August 17th, 2003

	Permission is hereby granted, free of charge, to any person or organization
	obtaining a copy of the software and accompanying documentation covered by
	this license (the "Software") to use, reproduce, display, distribute,
	execute, and transmit the Software, and to prepare derivative works of the
	Software, and to permit third-parties to whom the Software is furnished to
	do so, all subject to the following:

	The copyright notices in the Software and this entire statement, including
	the above license grant, this restriction and the following disclaimer,
	must be included in all copies of the Software, in whole or in part, and
	all derivative works of the Software, unless such copies or derivative
	works are solely in the form of machine-executable object code generated by
	a source language processor.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
	SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
	FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
	ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
	DEALINGS IN THE SOFTWARE.
*/

#if !defined(SELFORGANIZING_H)
	 #define SELFORGANIZING_H

#include <algorithm>
#include <iterator>
#include <list>
#include <type_traits>
#include <utility>
#include <vector>

//!\brief All structures to instaniate self-organizing lists and vectors with various find policies.
namespace self_organizing
{

//!\brief The different reorganization strategies to use when accessing elements.
namespace find_policy
{
	//! When an element is searched, its count is incremented by one and moved ahead of all elements with a smaller count.
	struct count
	{
		//! Finds an element according the \ref self_organizing::find_policy::count policy.
		template<typename Impl, typename F>
		static typename Impl::iterator find_if(Impl& impl, const F& f)
		{
			typename Impl::iterator i = impl.begin(), h = i;

			while(i != impl.end())
			{
				if(h->first != i->first)
				{
					h = i;
				}

				if(f(i->second))
				{
					break;
				}

				++i;
			}

			if(i != impl.end())
			{
				++i->first;

				if(h != i)
				{
					typename Impl::iterator::value_type t(std::move(*i));
					impl.erase(i);
					impl.emplace(h, std::move(t));
					i = h;
				}
			}

			return i;
		}
	};

	//! When an element is searched, it is moved to the beginning of the container.
	struct move_to_front
	{
		//! Finds an element according the \ref self_organizing::find_policy::move_to_front policy.
		template<typename Impl, typename F>
		static typename Impl::iterator find_if(Impl& impl, const F& f)
		{
			typename Impl::iterator i = std::find_if(impl.begin(), impl.end(), f);
			
			if(i != impl.end())
			{
				typename Impl::iterator::value_type t(std::move(*i));
				impl.erase(i);
				impl.emplace(impl.begin(), std::move(t));
				i = impl.begin();
			}

			return i;
		}
	};

	//! When an element is searched, it is swapped with the element immediately in front of it.
	struct transpose
	{
		//! Finds an element according the \ref self_organizing::find_policy::transpose policy.
		template<typename Impl, typename F>
		static typename Impl::iterator find_if(Impl& impl, const F& f)
		{
			typename Impl::iterator i = std::find_if(impl.begin(), impl.end(), f);

			if(i != impl.end() && i != impl.begin())
			{
				typename Impl::iterator h = std::prev(i);
				std::iter_swap(i, h);
				i = h;
			}

			return i;
		}
	};
}

//! Not to be used directly.
namespace detail
{

//!\brief Base class for self_organizing::list and self_organizing::vector.
//!
//!\tparam Container Either std::list or std::vector.
//!\tparam T The value type.
//!\tparam FindPolicy A policy type from self_organizing::find_policy.
template<template<typename, typename> class Container, typename T, typename FindPolicy>
class container
{
protected:
	typedef Container<T, std::allocator<T>> impl_type;				//!< Convenience typedef.

	impl_type c_;													//!< Internal representation of the data.

public:
	typedef T value_type;											//!< Convenience typedef.
	typedef typename impl_type::size_type size_type;				//!< Convenience typedef.
	typedef typename impl_type::reference reference;				//!< Convenience typedef.
	typedef typename impl_type::const_reference const_reference;	//!< Convenience typedef.
	typedef typename impl_type::pointer pointer;					//!< Convenience typedef.
	typedef typename impl_type::const_pointer const_pointer;		//!< Convenience typedef.
	typedef typename impl_type::iterator iterator;					//!< Convenience typedef.
	typedef typename impl_type::const_iterator const_iterator;		//!< Convenience typedef.

	//! Default constructor.
	container()
	{}
	
	//! Range constructor.
	template<typename InputIt>
	container(InputIt first, InputIt last)
	{
		std::copy(first, last, std::back_inserter(c_));
	}
	
	//!\brief Returns a \ref self_organizing::detail::container<Container, T, FindPolicy>::reference "reference" to the first element.
	reference front()
	{
		return *begin();
	}

	//!\brief Returns a \ref self_organizing::detail::container<Container, T, FindPolicy>::const_reference "const_reference" to the first element.
	const_reference front() const
	{
		return *cbegin();
	}

	//!\brief Returns a \ref self_organizing::detail::container<Container, T, FindPolicy>::reference "reference" to the last element.
	reference back()
	{
		return *--end();
	}

	//!\brief Returns a \ref self_organizing::detail::container<Container, T, FindPolicy>::const_reference "const_reference" to the last element.
	const_reference back() const
	{
		return *--end();
	}

	//!\brief Returns an \ref self_organizing::detail::container<Container, T, FindPolicy>::iterator "iterator" to the first element.
	iterator begin()
	{
		return c_.begin();
	}

	//!\brief Returns an \ref self_organizing::detail::container<Container, T, FindPolicy>::const_iterator "const_iterator" to the first element.
	const_iterator begin() const
	{
		return c_.begin();
	}

	//!\brief Returns an \ref self_organizing::detail::container<Container, T, FindPolicy>::const_iterator "const_iterator" to the first element.
	const_iterator cbegin() const
	{
		return c_.cbegin();
	}

	//!\brief Returns an \ref self_organizing::detail::container<Container, T, FindPolicy>::iterator "iterator" to one past the last element.
	iterator end()
	{
		return c_.end();
	}

	//!\brief Returns an \ref self_organizing::detail::container<Container, T, FindPolicy>::const_iterator "const_iterator" to one past the last element.
	const_iterator end() const
	{
		return c_.end();
	}

	//!\brief Returns an \ref self_organizing::detail::container<Container, T, FindPolicy>::const_iterator "const_iterator" to one past the last element.
	const_iterator cend() const
	{
		return c_.cend();
	}

	//!\brief Returns \c true if the container has no element.
	bool empty() const
	{
		return c_.empty();
	}

	//!\brief Returns the number of elements in the container.
	size_type size() const
	{
		return c_.size();
	}

	//!\brief Returns the maximum size of the container.
	size_type max_size() const
	{
		return c_.max_size();
	}

	//!\brief Removes all elements from the container.
	void clear()
	{
		return c_.clear();
	}

	//!\brief Removes the element at position \c i.
	iterator erase(iterator i)
	{
		return c_.erase(i);
	}

	//!\brief Removes the element at position \c i.
	iterator erase(const_iterator i)
	{
		return c_.erase(i);
	}

	//!\brief Removes the elements in the range <tt>[first; last)</tt>.
	iterator erase(iterator first, iterator last)
	{
		return c_.erase(first, last);
	}

	//!\brief Removes the elements in the range <tt>[first; last)</tt>.
	iterator erase(const_iterator first, const_iterator last)
	{
		return c_.erase(first, last);
	}
	 
	//!\brief Adds an element at the end of the container.
	//!\param value The element to add.
	void push_back(const value_type& value)
	{
		return c_.push_back(value);
	}

	//!\brief Finds an element.
	//!\param value The element to find.
	//!\return An iterator to the element, if found. Otherwise \ref self_organizing::detail::container<Container, T, FindPolicy>::end "end".
	iterator find(const value_type& value)
	{
		return find_if([&value](const value_type& v){ return v == value; });
	}

	//!\brief Finds an element satisfying a specific criteria.
	//!\param f The predicate to apply to elements. Must have the signature <tt>bool predicate(const value_type&);</tt>
	//!\return An iterator to the element, if found. Otherwise \ref self_organizing::detail::container<Container, T, FindPolicy>::end "end".
	template<typename F>
	iterator find_if(F f)
	{
		return FindPolicy::find_if(c_, f);
	}
};

//!\cond
template<template<typename, typename> class Container, typename T>
class container<Container, T, find_policy::count>
{
	typedef Container<std::pair<size_t, T>, std::allocator<std::pair<size_t, T>>> impl_type;

	impl_type c_;

public:
	typedef T value_type;
	typedef typename impl_type::size_type size_type;
	typedef T& reference;
	typedef T const& const_reference;
	typedef T* pointer;
	typedef T const* const_pointer;

	template<typename U>
	class const_iterator_
	{
		typedef typename container<Container, U, find_policy::count>::impl_type::const_iterator impl_type;
		impl_type i;

	public:
		typedef const_iterator_<U> self_type;
		typedef std::bidirectional_iterator_tag iterator_category;

		typedef U value_type;
		typedef typename impl_type::difference_type difference_type;
		typedef U const* pointer;
		typedef U const& reference;

		const_iterator_() {}

		const_iterator_(const self_type& o) : i(o.i) {}

		const_iterator_(impl_type i) : i(i) {}

		const_iterator_(typename container<Container, U, find_policy::count>::impl_type::iterator i) : i(i) {}

		operator impl_type()
		{
			return i;
		}

		reference operator*()
		{
			return i->second;
		}

		self_type& operator++()
		{
			++i;
			return *this;
		}

		self_type operator++(int)
		{
			return i++;
		}

		self_type& operator--()
		{	
			--i;
			return *this;
		}

		self_type operator--(int)
		{	
			return i--;
		}

		bool operator==(const self_type& j)
		{
			return i == j.i;
		}

		bool operator!=(const self_type& j)
		{
			return !(*this == j);
		}
	};

	template<typename U>
	class iterator_
	{
		typedef typename container<Container, U, find_policy::count>::impl_type::iterator impl_type;
		impl_type i;

	public:
		typedef iterator_<U> self_type;
		typedef std::bidirectional_iterator_tag iterator_category;

		typedef U value_type;
		typedef typename impl_type::difference_type difference_type;
		typedef U* pointer;
		typedef U& reference;

		iterator_() {}

		iterator_(const self_type& o) : i(o.i) {}

		iterator_(const impl_type& i) : i(i) {}
		
		operator impl_type()
		{
			return i;
		}

		reference operator*()
		{
			return i->second;
		}
		
		pointer operator->()
		{
			return &i->second;
		}

		self_type& operator++()
		{	
			++i;
			return *this;
		}

		self_type operator++(int)
		{	
			return i++;
		}

		self_type& operator--()
		{	
			--i;
			return *this;
		}

		self_type operator--(int)
		{	
			return i--;
		}

		bool operator==(const self_type& j)
		{
			return i == j.i;
		}

		bool operator!=(const self_type& j)
		{
			return !(*this == j);
		}
	};

	typedef const_iterator_<T> const_iterator;
	typedef iterator_<T> iterator;

	container()
	{}

	template<typename InputIt>
	container(InputIt first, InputIt last)
	{
		for(; first != last; ++first)
		{
			c_.push_back(std::make_pair(0, *first));
		}
	}
	
	reference front()
	{
		return *begin();
	}

	const_reference front() const
	{
		return *cbegin();
	}

	reference back()
	{
		return *--end();
	}

	const_reference back() const
	{
		return *--cend();
	}

	iterator begin()
	{
		return c_.begin();
	}

	const_iterator begin() const
	{
		return c_.begin();
	}

	const_iterator cbegin() const
	{
		return c_.begin();
	}

	iterator end()
	{
		return c_.end();
	}

	const_iterator end() const
	{
		return c_.end();
	}

	const_iterator cend() const
	{
		return c_.end();
	}

	bool empty() const
	{
		return c_.empty();
	}

	size_type size() const
	{
		return c_.size();
	}

	size_type max_size() const
	{
		return c_.max_size();
	}

	void clear()
	{
		c_.clear();
	}

	iterator erase(iterator i)
	{
		return c_.erase((typename impl_type::iterator)i);
	}

	iterator erase(const_iterator i)
	{
		return c_.erase((typename impl_type::const_iterator)i);
	}
	 
	iterator erase(iterator first, iterator last)
	{
		return c_.erase((typename impl_type::iterator)first, (typename impl_type::iterator)last);
	}

	iterator erase(const_iterator first, const_iterator last)
	{
		return c_.erase((typename impl_type::const_iterator)first, (typename impl_type::const_iterator)last);
	}

	void push_back(const value_type& value)
	{
		return c_.push_back(std::make_pair(0, value));
	}

	iterator find(const value_type& value)
	{
		return find_if([&value](const value_type& v){ return v == value; });
	}

	template<typename F>
	iterator find_if(F f)
	{
		return find_policy::count::find_if(c_, f);
	}
};
//!\endcond

}

//!\brief A self-organizing container that uses a std::list to hold its data.
//!\tparam T The value type.
//!\tparam FindPolicy The policy to use when finding elements.
template<typename T, typename FindPolicy>
class list : public detail::container<std::list, T, FindPolicy>
{
public:
	//! Default constructor.
	list() : detail::container<std::list, T, FindPolicy>()
	{}

	//! Range constructor.
	template<typename InputIt>
	list(InputIt first, InputIt last) : detail::container<std::list, T, FindPolicy>(first, last)
	{}

	//!\brief Insert an element at the beginning of the list.
	//!
	//! Cannot be used in conjunction with \ref self_organizing::find_policy::count.
	typename detail::container<std::list, T, FindPolicy>::iterator push_front(const T& value)
	{
		detail::container<std::list, T, FindPolicy>::c_.push_front(value);
	}
};

//!\cond
template<typename T>
class list<T, find_policy::count> : public detail::container<std::list, T, find_policy::count>
{
public:
	list() : detail::container<std::list, T, find_policy::count>()
	{}

	template<typename InputIt>
	list(InputIt first, InputIt last) : detail::container<std::list, T, find_policy::count>(first, last)
	{}
};
//!\endcond

//!\brief A self-organizing container that uses a std::vector to hold its data.
//!\tparam T The value type.
//!\tparam FindPolicy The policy to use when finding elements.
template<typename T, typename FindPolicy>
class vector : public detail::container<std::vector, T, FindPolicy>
{
public:
	//! Default constructor.
	vector() : detail::container<std::vector, T, FindPolicy>()
	{}

	//! Range constructor.
	template<typename InputIt>
	vector(InputIt first, InputIt last) : detail::container<std::vector, T, FindPolicy>(first, last)
	{}

	//!\brief Insert an element at the beginning of the list.
	//!
	//! Cannot be used in conjunction with \ref self_organizing::find_policy::count.
	typename detail::container<std::vector, T, FindPolicy>::iterator push_front(const T& value)
	{
		detail::container<std::vector, T, FindPolicy>::c_.insert(detail::container<std::vector, T, FindPolicy>::c_.begin(), value);
	}
};

//!\cond
template<typename T>
class vector<T, find_policy::count> : public detail::container<std::vector, T, find_policy::count>
{
public:
	vector() : detail::container<std::vector, T, find_policy::count>()
	{}

	template<typename InputIt>
	vector(InputIt first, InputIt last) : detail::container<std::vector, T, find_policy::count>(first, last)
	{}
};
//!\endcond
}

#endif

/*!

\file self_organizing.h
\brief The only file you need.
\author Thierry Seegers

\mainpage self_organizing

 - \ref introduction
 - \ref considerations
 - \ref principles
 - \ref sample
 - \ref license

\section introduction Introduction

\subsection why Why this project?

I started this project the same way I started a few others already.
I cracked open my "Data Structures and Algorithm Analysis" book from college and browsed its table of contents for an interesting concept to implement in C++11.
This project is about exploring what the new C++ standard offers and putting it in practice in smallish, self-contained libraries.
I'm also using this project to try out GitHub's offering.
This project is \e not the most perfomant and efficient self-orginizing list you've ever seen.
In fact, after running some crude performance benchmark, I've concluded its advantage window is quite narrow.
More on that down below.

\subsection whatis What is a self-organizing list?

A self-organizing list is a container that attempts to optimize future searches based on past searches.
Because a search in a list is a linear operation, the closer to the head an element is, the faster it is to find it.
Popular elements should therefore move closer to the head of the list over time to optimize future searches.
Less popular elements will drift to the tail of the list.
You might recognize in this description an implementation of the "80/20" rule for lists.

Different self-organizing strategies exist, the following three are offered in this library.
\li \b Count. Elements are kept ordered by the frequency of past searches.
 An element that has been searched for \c N times is moved ahead of all elements that have been searched for less than \c N times.
\li \b Transpose. When an element is searched for, it is swapped with the element in front of it.
\li \b Move-to-front. When an element is searched for, it is moved to the front of the list.

\section considerations Technical considerations

Because of the motivation for this project to learn C++11 and put it in practice, you will obviously need a C++11 compliant compiler.
I'm using Visual Studio 2012 to develop but I additionally test with g++ 4.6 and XCode 4.5.
I'm not using advanced or arcane C++11 features, so earlier version of these toolchains might also work.

\section principles Design principles and considerations

\subsection policies Policy-based design

The public API of this library consists of two container classes and three policy classes to be used as template parameters for the container classes.

The two container classes are \ref self_organizing::list and \ref self_organizing::vector.
They distinguish themselves by which container from the \c std namespace they use to hold data.
Why offer \c std::vector as a possible data holder for something called "self-organizing list"?
Because I can.
But also because it helps to compare performance of such an animal with a that of a self-organizing list implemented with an actual list.

The policy classes each implement a self-organizing strategy.
All three classes are defined in \ref self_organizing::find_policy.

\subsection search Searching for an element

Searching for an element in a \c std::vector or a \c std::list is performed with the generic \c std::find from the <tt>&lt;algorithm&gt;</tt> header.
On the ther hand, \ref self_organizing::list and \ref self_organizing::vector provide \ref self_organizing::detail::container::find "find" member function.
It is important to note that, by definition, a container's elements are re-ordered when a \c find operation is performed.
Therefore, the \c find member is not \c const.

\subsection iterators Iterator invalidation

Iterator invalidation rules for a self-organizing container follow the same rules as for containers from the \c std namespace with one exception.
\ref self_organizing::vector::iterator is invalidated after performing a search.
This follows from the facts that a self-organizing container is re-ordered after a search is performed and that when elements from a \c std::vector are re-ordered, it's iterators are invalidated.
This exception does not apply to \ref self_organizing::list::iterator.

\subsection performance Performance

So, are they worth it?

We already know that when searching for elements in a container in a purely random fashion, a self-organizing list will offer no performance improvement, only overhead.
There would indeed be no useful information from past searches to use and optimize future searches.
Self-organizing lists must only be used when searches are not random.

I tested the performance of the following scenario.
I randomly shuffled integers from 0 to 999999 and copied them to nine containers:
 - One \c std::list, one \c std::vector and one \c std::set.
 - Three self_organizing::list, each with a different \ref self_organizing::find_policy "find_policy".
 - Three self_organizing::vector, each with a different \ref self_organizing::find_policy "find_policy".

 I then generated 100000 random integers multiple times from the same range using a <a href=http://en.wikipedia.org/wiki/Normal_distribution>normal distribution</a>.
Each time, the mean was kept at 50000 and the variance was changed. Here are the results.

\image html performance.png

First obvious conclusion: \c std::set crushes the competition.
It barely registers.
But, then again, this scenario is essentially perfect for it.
All data is known ahead of time and I'm only measuring the time taken to search, not the time taken to set up to container.
\c std::set reorganized the data in order during construction and and its \a log(N) search performance did the rest.

At the other side of the spectrum, \c self_organizing::vector<find_policy::count> and \c self_organizing::vector<find_policy::move_to_front> can't keep up at all.
The maintenance required is utterly misadapted to a \c vector since both policies constanly rearrange elements using insertion, an operation that \c vector is not made for.

What's left in between?
In third place we have \c std::vector.
The nature of this test favors packed and static data, so that's no surprise.
But ahead of it by a nose is \c self_organizing::vector<find_policy::transpose>.
The transposition strategy of swapping element incurs little overhead to a \c vector and afforded it the second place.

Where are our lists?
\c std::list and \c self_organizing::list<find_policy::transpose> performed in lock-step, mimicking their \c vector cousins.
Only, three times slower.
I'm surprised the transposition strategy didn't help that much.
Remains, \c self_organizing::list<find_policy::count> and \c self_organizing::list<find_policy::move_to_front>.
Now those are interesting because they show a pattern of improving performance as the search variance is reduced.
\c self_organizing::list<find_policy::move_to_front> even managed to come ahead of the \c vectors in the last test!


\section sample Sample code

\include examples/example.cpp

\section license License

\verbatim
Boost Software License - Version 1.0 - August 17th, 2003

Permission is hereby granted, free of charge, to any person or organization
obtaining a copy of the software and accompanying documentation covered by
this license (the "Software") to use, reproduce, display, distribute,
execute, and transmit the Software, and to prepare derivative works of the
Software, and to permit third-parties to whom the Software is furnished to
do so, all subject to the following:

The copyright notices in the Software and this entire statement, including
the above license grant, this restriction and the following disclaimer,
must be included in all copies of the Software, in whole or in part, and
all derivative works of the Software, unless such copies or derivative
works are solely in the form of machine-executable object code generated by
a source language processor.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
\endverbatim

*/

