//
// unicode.hpp
// foundation-cpp
//
// Created by Kristian Trenskow on 2021/07/09.
// See license in LICENSE.
//

#ifndef foundation_unicode_hpp
#define foundation_unicode_hpp

namespace foundation::types {

	class Unicode {

	public:

		static uint32_t lowerToUpper(
			const uint32_t lower);

		static uint32_t upperToLower(
			const uint32_t upper);

		static bool isWhitespace(
			const uint32_t codePoint);

	};

}

#endif /* foundation_unicode_hpp */
