#pragma once

#include <functional> // std::less
#include <iterator> // std::iterator_traits

namespace sort {

	// This is C++ magic which will allows our function
	// to default to using a < b if the comparator arg
	// is unspecified. It uses defines std::less<T>
	// for the iterator's value_type.
	//
	// For example: if you have a vector<float>, the 
	// iterator's value type will be float. std::less 
	// will select the < for sorting floats as the 
	// default comparator.
	template<typename RandomIter>
	using less_for_iter = std::less<typename std::iterator_traits<RandomIter>::value_type>;

	/* Efficiently swap two items - use this to implement your sorts */
	template<typename T>
	void swap(T & a, T & b) noexcept { // Using move semantics
		T temp = std::move(a);
		a = std::move(b);
		b = std::move(temp);
	
	}

	template<typename RandomIter, typename Comparator = less_for_iter<RandomIter>>
	void bubble(RandomIter begin, RandomIter end, Comparator comp = Comparator{}) {
		// Random access iterators have the same traits you defined in the Vector class
		// For instance, difference_type represents an iterator difference
		// You may delete the types you don't use to remove the compiler warnings
		using _it             = std::iterator_traits<RandomIter>;
		using difference_type = typename _it::difference_type;
		//using value_type      = typename _it::value_type;
		using reference       = typename _it::reference;
		//using pointer         = typename _it::pointer;

		// TODO
		bool switched = false; // Best case
		for (difference_type i = end - begin; i > 0; --i) {
			switched = false;
			for (long int j = 0; j < i - 1; ++j) {
				reference current = *(begin+j);
				reference next = *(begin+j+1);
				if (comp(next, current)) {
					swap(current, next);
					switched = true;
				}
				
			}
			if (!switched) {
				break;
			}
		}
	}

	template<typename RandomIter, typename Comparator = less_for_iter<RandomIter>>
	void insertion(RandomIter begin, RandomIter end, Comparator comp = Comparator{}) {
		using _it             = std::iterator_traits<RandomIter>;
		using difference_type = typename _it::difference_type;
		using value_type      = typename _it::value_type;

		value_type value;
		difference_type spot;

		for (difference_type i = 1; i < end - begin; ++i) {
			value = std::move(*(begin+i));
			spot = i;

			while (spot > 0 && comp(value, *(begin + spot - 1))) {
				*(begin+spot) = std::move(*(begin + spot - 1));
				--spot;
				
			}
			swap(*(begin + spot), value); 
				
		}

	}

	template<typename RandomIter, typename Comparator = less_for_iter<RandomIter>>
	void selection(RandomIter begin, RandomIter end, Comparator comp = Comparator{}) { 
		using _it             = std::iterator_traits<RandomIter>;
		using difference_type = typename _it::difference_type;
		
		difference_type minimum;
		for (difference_type i = 0; i < end - begin - 1; ++i) {
			minimum = i;

			for (difference_type j = i + 1; j < end - begin; ++j) {

				if (comp(*(begin+j), *(begin+minimum))) {
					minimum = j;
				}
				
			}
			if (minimum != i) {
				swap(*(begin + minimum), *(begin + i));
			}
		}
	
	}
}