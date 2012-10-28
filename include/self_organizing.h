#if !defined(SELFORGANIZING_H)
	 #define SELFORGANIZING_H

#include <algorithm>
#include <iterator>
#include <list>
#include <type_traits>
#include <utility>
#include <vector>

namespace self_organizing
{

namespace find_policy
{
	struct count
	{
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

	struct move_to_front
	{
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

	struct transpose
	{
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

//!\cond
namespace detail
{

template<template<typename, typename> class Container, typename T, typename FindPolicy>
class container
{
protected:
	typedef Container<T, std::allocator<T>> impl_type;

	impl_type c_;

public:
	typedef T value_type;
	typedef typename impl_type::size_type size_type;
	typedef typename impl_type::reference reference;
	typedef typename impl_type::const_reference const_reference;
	typedef typename impl_type::pointer pointer;
	typedef typename impl_type::const_pointer const_pointer;
	typedef typename impl_type::iterator iterator;
	typedef typename impl_type::const_iterator const_iterator;

	container()
	{}
	
	template<typename InputIt>
	container(InputIt first, InputIt last)
	{
		std::copy(first, last, std::back_inserter(c_));
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
		return *--end();
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
		return c_.cbegin();
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
		return c_.cend();
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
		return c_.clear();
	}

	iterator erase(iterator i)
	{
		return c_.erase(i);
	}

	iterator erase(const_iterator i)
	{
		return c_.erase(i);
	}

		iterator erase(iterator first, iterator last)
	{
		return c_.erase(first, last);
	}

	iterator erase(const_iterator first, const_iterator last)
	{
		return c_.erase(first, last);
	}
	 
	void push_back(const value_type& value)
	{
		return c_.push_back(value);
	}

	iterator find(const value_type& value)
	{
		return find_if([&value](const value_type& v){ return v == value; });
	}

	template<typename F>
	iterator find_if(F f)
	{
		return FindPolicy::find_if(c_, f);
	}
};

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
		return c_.erase(i);
	}

	iterator erase(const_iterator i)
	{
		return c_.erase(i);
	}
	 
	iterator erase(iterator first, iterator last)
	{
		return c_.erase(first, last);
	}

	iterator erase(const_iterator first, const_iterator last)
	{
		return c_.erase(first, last);
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

}
//!\endcond

template<typename T, typename FindPolicy>
class list : public detail::container<std::list, T, FindPolicy>
{
public:
	list() : detail::container<std::list, T, FindPolicy>()
	{}

	template<typename InputIt>
	list(InputIt first, InputIt last) : detail::container<std::list, T, FindPolicy>(first, last)
	{}

	typename detail::container<std::list, T, FindPolicy>::iterator push_front(const T& value)
	{
		detail::container<std::list, T, FindPolicy>::c_.push_front(value);
	}
};

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

template<typename T, typename FindPolicy>
class vector : public detail::container<std::vector, T, FindPolicy>
{
public:
	vector() : detail::container<std::vector, T, FindPolicy>()
	{}

	template<typename InputIt>
	vector(InputIt first, InputIt last) : detail::container<std::vector, T, FindPolicy>(first, last)
	{}

	typename detail::container<std::vector, T, FindPolicy>::iterator push_front(const T& value)
	{
		detail::container<std::vector, T, FindPolicy>::c_.insert(detail::container<std::vector, T, FindPolicy>::c_.begin(), value);
	}
};

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

}

#endif