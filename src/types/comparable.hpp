//
// comparable.hpp
// foundation
//
// Created by Kristian Trenskow on 2023/01/21.
// See license in LICENSE.
//

#ifndef foundation_comparable_hpp
#define foundation_comparable_hpp

#include "./comparison.hpp"

namespace foundation::types {

	template<typename T>
	class Comparable {

		public:

			bool equals(const T& other) const {
				return !this->greaterThan(other) && !other.greaterThan((T&)*this);
			}

			virtual bool greaterThan(const T& other) const = 0;

			inline bool lessThan(const T& other) const {
				return other.greaterThanOrEqual((T&)*this);
			}

			inline bool greaterThanOrEqual(const T& other) const {
				return this->greaterThan(other) || this->equals(other);
			}

			inline bool lessThanOrEqual(const T& other) const {
				return !this->greaterThan(other);
			}

			Comparison compareTo(const T& other) const {
				if (this->operator>(other)) return Comparison::biggerThan;
				if (other.operator>((T&)*this)) return Comparison::lessThan;
				return Comparison::equals;
			}

	};

}

#endif /* foundation_comparable_hpp */
