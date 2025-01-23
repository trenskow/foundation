//
// hashable.hpp
// fart
//
// Created by Kristian Trenskow on 2018/09/09.
// See license in LICENSE.
//

#ifndef hashable_hpp
#define hashable_hpp

#include "../exceptions/exception.hpp"
#include "../tools/math.hpp"

using namespace foundation::exceptions;
using namespace foundation::tools;

namespace foundation::types {

	class Hashable {

		public:

			class Builder {

			public:

				Builder();

				Builder& add(
					uint64_t value);

				Builder& add(
					double value);

				Builder& add(
					float value);

				Builder& add(
					const Hashable& hashable);

				operator uint64_t();

			private:

				uint64_t _hash;

			};

			Hashable();

			Hashable(
				const Hashable&);

			Hashable(
				Hashable&&);

			virtual uint64_t hash() const;

			Hashable& operator=(
				const Hashable&);

			Hashable& operator=(
				Hashable&&);

		protected:

			virtual Builder hashBuilder() const;

	};

}

#endif /* hashable_hpp */
