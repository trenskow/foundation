//
// regular-expression.hpp
// shared-foundation-cpp
//
// Created by Kristian Trenskow on 2024/09/08.
// See license in LICENSE.
//

#ifndef shared_foundation_regular_expression_hpp
#define shared_foundation_regular_expression_hpp

#include <regex.h>

#include "../types/string.hpp"

using namespace games::zerobit::shared::foundation::types;

namespace games::zerobit::shared::foundation::tools {

	class RegularExpression : public Object {

		public:

			RegularExpression(
				const String& pattern
			) noexcept(false);

			~RegularExpression();

			String pattern() const noexcept;

			bool test(
				const String& string
			) const;

		private:

			String _pattern;
			regex_t _regex;

	};

}

#endif /* shared_foundation_regular_expression_hpp */
