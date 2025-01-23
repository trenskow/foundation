//
// math.hpp
// shared-foundation-cpp
//
// Created by Kristian Trenskow on 2020/04/05.
// See license in LICENSE.
//

#ifndef shared_foundation_math_hpp
#define shared_foundation_math_hpp

#include <type_traits>

#include <string.h>

namespace games::zerobit::shared::foundation::tools {

	namespace math {

		template<typename T>
		inline const T max(T a, T b) {
			return (a > b ? a : b);
		}

		template<typename T>
		inline const T min(T a, T b) {
			return (a < b ? a : b);
		}

		template<typename T>
		inline constexpr T limit() {

			static_assert(std::is_integral<T>::value);

			if constexpr (std::is_signed<T>::value) {
				return (1ULL << (sizeof(T) * 8 - 1)) - 1;
			} else {
				return ~static_cast<T>(0);
			}

		}

	}

}

#endif /* shared_foundation_math_hpp */
