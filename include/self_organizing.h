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
		template<typename Impl, typename T>
		static typename Impl::iterator find(Impl& impl, const T& value)
		{
			typename Impl::iterator i = impl.begin(), h = i;

			while(i != impl.end())
			{
				if(h->first != i->first)
				{
					h = i;
				}

				if(i->second == value)
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
					Impl::iterator::value_type t(std::move(*i));
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
		template<typename Impl, typename T>
		static typename Impl::iterator find(Impl& impl, const T& value)
		{
			typename Impl::iterator i = std::find(impl.begin(), impl.end(), value);
			
			if(i != impl.end())
			{
				Impl::iterator::value_type t(std::move(*i));
				impl.erase(i);
				impl.emplace(impl.begin(), std::move(t));
				i = impl.begin();
			}

			return i;
		}
	};

	struct transpose
	{
		template<typename Impl, typename T>
		static typename Impl::iterator find(Impl& impl, const T& value)
		{
			typename Impl::iterator i = std::find(impl.begin(), impl.end(), value);

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

namespace insertion_policy
{
	struct count
	{
		template<typename Impl, typename T>
		static typename Impl::iterator insert(Impl& impl, const T& value)
		{
			return impl.insert(impl.end(), std::make_pair(0, value));
		}

		template<typename Impl, typename InputIt>
		static void insert(Impl& impl, InputIt first, InputIt last)
		{
			for(; first != last; ++first)
			{
				insert(impl, *first);
			}
		}
	};

	struct insert_back
	{
		template<typename Impl, typename T>
		static typename Impl::iterator insert(Impl& impl, const T& value)
		{
			return impl.insert(impl.end(), value);
		}

		template<typename Impl, typename InputIt>
		static void insert(Impl& impl, InputIt first, InputIt last)
		{
			for(; first != last; ++first)
			{
				insert(impl, *first);
			}
		}
	};

	struct insert_front
	{
		template<typename Impl, typename T>
		static typename Impl::iterator insert(Impl& impl, const T& value)
		{
			return impl.insert(impl.begin(), value);
		}

		template<typename Impl, typename InputIt>
		static void insert(Impl& impl, InputIt first, InputIt last)
		{
			for(; first != last; ++first)
			{
				insert(impl, *first);
			}
		}
	};
}

namespace detail
{

template<template<typename, typename> class Container, typename T, typename FindPolicy, typename InsertionPolicy>
class container
{
	typedef typename Container<T, std::allocator<T>> impl_type;

	impl_type c_;

public:
	static_assert(std::is_same<FindPolicy, find_policy::count>::value ^ !std::is_same<InsertionPolicy, insertion_policy::count>::value, "Incompatible policy types selected.");

	typedef T value_type;
	typedef typename impl_type::size_type size_type;
	typedef typename impl_type::reference reference;
	typedef typename impl_type::const_reference const_reference;
	typedef typename impl_type::pointer pointer;
	typedef typename impl_type::const_pointer const_pointer;
	typedef typename impl_type::iterator iterator;
	typedef typename impl_type::const_iterator const_iterator;

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

	iterator insert(const value_type& value)
	{
		return InsertionPolicy::insert(detail::list<T>::c_, value);
	}

	template<class InputIt>
	void insert(InputIt first, InputIt last)
	{
		return InsertionPolicy::insert(c_, first, last);
	}

	iterator erase(iterator i)
	{
		return c_.erase(i);
	}

	iterator erase(const_iterator i)
	{
		return c_.erase(i);
	}

	iterator find(const value_type& value)
	{
		return FindPolicy::find(c_, value);
	}
};

template<template<typename, typename> class Container, typename T>
class container<Container, T, find_policy::count, insertion_policy::count>
{
	typedef typename Container<std::pair<size_t, T>, std::allocator<std::pair<size_t, T>>> impl_type;

	impl_type c_;

public:
	typedef T value_type;
	typedef typename impl_type::size_type size_type;
	typedef typename T& reference;
	typedef typename T const& const_reference;
	typedef typename T* pointer;
	typedef typename T const* const_pointer;

	template<typename U>
	class const_iterator
	{
		typedef typename container<Container, U, find_policy::count, insertion_policy::count>::impl_type::const_iterator impl_type;
		typename impl_type i;

	public:
		typedef const_iterator<U> self_type;
		typedef std::bidirectional_iterator_tag iterator_category;

		typedef U value_type;
		typedef typename impl_type::difference_type difference_type;
		typedef U const* pointer;
		typedef U const& reference;

		const_iterator() {}

		const_iterator(typename const self_type& o) : i(o.i) {}

		const_iterator(typename impl_type i) : i(i) {}

		const_iterator(typename container<Container, U, find_policy::count, insertion_policy::count>::impl_type::iterator i) : i(i) {}

		operator typename impl_type()
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
	class iterator
	{
		typedef typename container<Container, U, find_policy::count, insertion_policy::count>::impl_type::iterator impl_type;
		typename impl_type i;

	public:
		typedef typename iterator<U> self_type;
		typedef std::bidirectional_iterator_tag iterator_category;

		typedef U value_type;
		typedef typename impl_type::difference_type difference_type;
		typedef U* pointer;
		typedef U& reference;

		iterator() {}

		iterator(const self_type& o) : i(o.i) {}

		iterator(const impl_type& i) : i(i) {}
		
		operator typename impl_type()
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

	iterator<T> begin()
	{
		return c_.begin();
	}

	const_iterator<T> begin() const
	{
		return c_.begin();
	}

	iterator<T> end()
	{
		return c_.end();
	}

	const_iterator<T> end() const
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

	iterator<T> insert(const value_type& value)
	{
		return insertion_policy::count::insert(c_, value);
	}

	template<class InputIt>
	void insert(InputIt first, InputIt last)
	{
		return insertion_policy::count::insert(c_, first, last);
	}

	iterator<T> erase(iterator<T> i)
	{
		return c_.erase(i);
	}

	iterator<T> erase(const_iterator<T> i)
	{
		return c_.erase(i);
	}

	iterator<T> find(const value_type& value)
	{
		return find_policy::count::find(c_, value);
	}
};

}

template<typename T, typename FindPolicy, typename InsertionPolicy>
class list : public detail::container<std::list, T, FindPolicy, InsertionPolicy> {};

template<typename T>
class list<T, find_policy::count, insertion_policy::count> : public detail::container<std::list, T, find_policy::count, insertion_policy::count> {};

template<typename T, typename FindPolicy, typename InsertionPolicy>
class vector : public detail::container<std::vector, T, FindPolicy, InsertionPolicy> {};

template<typename T>
class vector<T, find_policy::count, insertion_policy::count> : public detail::container<std::vector, T, find_policy::count, insertion_policy::count> {};

}

#endif