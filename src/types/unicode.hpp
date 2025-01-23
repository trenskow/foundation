//
// unicode.hpp
// shared-foundation-cpp
//
// Created by Kristian Trenskow on 2021/07/09.
// See license in LICENSE.
//

#ifndef shared_foundation_unicode_hpp
#define shared_foundation_unicode_hpp

namespace games::zerobit::shared::foundation::types {

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

#endif /* shared_foundation_unicode_hpp */
