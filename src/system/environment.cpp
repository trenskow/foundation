//
// environment.cpp
// foundation
//
// Created by Kristian Trenskow on 2025/01/22.
// See license in LICENSE.
//

#include "environment.hpp"

using namespace foundation::system;

String Environment::getVariable(const String& name) {
	return String(name.mapCString<const char*>([](const char* name) {
		return getenv(name);
	}));
}