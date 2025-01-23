//
// regular-expression.hpp
// foundation-cpp
//
// Created by Kristian Trenskow on 2024/09/08.
// See license in LICENSE.
//

#ifndef foundation_regular_expression_hpp
#define foundation_regular_expression_hpp

#include <regex.h>

#include "../types/string.hpp"

using namespace foundation::types;

namespace foundation::tools {

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

#endif /* foundation_regular_expression_hpp */
