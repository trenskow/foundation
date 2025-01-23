//
// environment.cpp
// shared-foundation-cpp
//
// Created by Kristian Trenskow on 2025/01/22.
// See license in LICENSE.
//

#include "environment.hpp"

using namespace games::zerobit::shared::foundation::system;

String Environment::getVariable(const String& name) {
	return String(name.mapCString<const char*>([](const char* name) {
		return getenv(name);
	}));
}