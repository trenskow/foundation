//
// comparison.hpp
// shared-foundation-cpp
//
// Created by Kristian Trenskow on 2023/01/21.
// See license in LICENSE.
//

#ifndef shared_foundation_comparison_hpp
#define shared_foundation_comparison_hpp

namespace games::zerobit::shared::foundation::types {

	enum class Comparison : int8_t {
		lessThan = -1,
		equals = 0,
		biggerThan = 1
	};

};

#endif /* shared_foundation_comparison_hpp */
