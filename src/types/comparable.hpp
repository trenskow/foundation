//
// comparable.hpp
// shared-foundation-cpp
//
// Created by Kristian Trenskow on 2023/01/21.
// See license in LICENSE.
//

#ifndef shared_foundation_comparable_hpp
#define shared_foundation_comparable_hpp

#include "./comparison.hpp"

namespace games::zerobit::shared::foundation::types {

	template<typename T>
	class Comparable {

		public:

			virtual bool operator>(const T& other) const = 0;

			inline bool operator<(const T& other) const {
				return !(other >= (T&)*this);
			}

			inline bool operator>=(const T& other) const {
				return !other.operator<((T&)*this);
			}

			inline bool operator<=(const T& other) const {
				return !this->operator>(other);
			}

			Comparison compareTo(const T& other) const {
				if (this->operator>(other)) return Comparison::biggerThan;
				if (other.operator>((T&)*this)) return Comparison::lessThan;
				return Comparison::equals;
			}

	};

}

#endif /* shared_foundation_comparable_hpp */
