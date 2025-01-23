//
// endian.cpp
// shared-foundation-cpp
//
// Created by Kristian Trenskow on 2025/01/22.
// See license in LICENSE.
//

#include "./endian.hpp"

using namespace games::zerobit::shared::foundation::system;

Endian::Variant Endian::systemVariant() {
	static const uint32_t test = 0x00000001;
	return *((uint8_t *)&test) == 0x01 ? Variant::little : Variant::big;
}
