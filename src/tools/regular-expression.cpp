//
// regular-expression.cpp
// foundation-cpp
//
// Created by Kristian Trenskow on 2025/01/22.
// See license in LICENSE.
//

#include "../exceptions/exception.hpp"

#include "regular-expression.hpp"

using namespace foundation::tools;
using namespace foundation::exceptions::tools;

RegularExpression::RegularExpression(
	const String& pattern
) noexcept(false)
  : _pattern(pattern) {

	int flags = REG_EXTENDED;

	if (pattern.length() < 2 || *pattern.substring(0, 1) != String("/")) {
		throw InvalidPatternException();
	}

	size_t lastSlash = pattern.lastIndexOf("/");

	if (lastSlash == NotFound || lastSlash == 0) {
		throw InvalidPatternException();
	}

	String options = pattern.substring(lastSlash + 1);
	String regexPattern = pattern.substring(1, lastSlash - 1);

	if (options.contains("i")) {
		flags |= REG_ICASE;
	}

	int ret = regexPattern
		.mapCString<int>([&](const char* pattern) {
			return regcomp(&this->_regex, pattern, flags);
		});

	if (ret) {
		throw InvalidPatternException();
	}

}

RegularExpression::~RegularExpression() {
	regfree(&this->_regex);
}

String RegularExpression::pattern() const noexcept {
	return this->_pattern;
}

bool RegularExpression::test(
	const String& string
) const {
	return string.mapCString<bool>([&](const char* string) {
		return regexec(&this->_regex, string, 0, nullptr, 0) == 0;
	});
}
