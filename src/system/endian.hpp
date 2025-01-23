//
// endian.hpp
// shared-foundation-cpp
//
// Created by Kristian Trenskow on 2020/04/01.
// See license in LICENSE.
//

#ifndef shared_foundation_endian_hpp
#define shared_foundation_endian_hpp

#include <stdlib.h>
#include <stdint.h>

namespace games::zerobit::shared::foundation::system {

	class Endian {

	private:

		template<typename T>
		static const T swap(T val) {
			T ret;
			uint8_t* valP = (uint8_t *)&val;
			uint8_t* retP = (uint8_t *)&ret;
			for (size_t idx = 0 ; idx < sizeof(T) ; idx++) {
				retP[idx] = valP[sizeof(T) - idx - 1];
			}
			return ret;
		}

	public:

		enum class Variant {
			little,
			big
		};

		static Variant systemVariant();

		template<typename T>
		static const T convert(T val, Variant from, Variant to) {
			if (from == to) return val;
			return swap(val);
		}

		template<typename T>
		static const T toSystemVariant(T val, Variant from) {
			return convert(val, from, systemVariant());
		}

		template<typename T>
		static const T fromSystemVariant(T val, Variant to) {
			return convert(val, systemVariant(), to);
		}

	};

}

#endif /* shared_foundation_endian_hpp */
