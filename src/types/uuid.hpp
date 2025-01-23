//
// uuid.hpp
// shared-foundation-cpp
//
// Created by Kristian Trenskow on 2022/08/28.
// See license in LICENSE.
//

#ifndef shared_foundation_uuid_hpp
#define shared_foundation_uuid_hpp

#include "./type.hpp"

namespace games::zerobit::shared::foundation::types {

	class UUID : public Type {

	public:

		static Type::Kind typeKind();

		UUID();

		UUID(
			const UUID& other);

		UUID(
			const String& string);

		virtual ~UUID();

		String string();

		virtual Kind kind() const override;

		virtual uint64_t hash() const override;

		virtual bool operator==(
			const Type& other
		) const override;

		bool operator!=(
			const UUID& other
		) const;

		UUID& operator=(
			const UUID& other);

	private:

		static String _base64Characters();

		String _encode(
			uint64_t value
		) const;

		uint64_t _decode(
			const String& string
		) const noexcept(false);

		uint64_t _lower;
		uint64_t _upper;

	};

}

#endif /* shared_foundation_uuid_hpp */
