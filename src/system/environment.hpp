//
// environment.hpp
// shared-foundation-cpp
//
// Created by Kristian Trenskow on 2021/01/16.
// See license in LICENSE.
//

#ifndef shared_foundation_environment_hpp
#define shared_foundation_environment_hpp

#include <stdlib.h>

#include "../types/string.hpp"

using namespace games::zerobit::shared::foundation::types;

namespace games::zerobit::shared::foundation::system {

	class Environment {

	public:
		static String getVariable(
			const String& name);

	};

}

#endif /* shared_foundation_environment_hpp */
