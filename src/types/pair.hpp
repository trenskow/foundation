
//
// pair.hpp
// foundation
//
// Created by Kristian Trenskow on 2021/01/24.
// See license in LICENSE.
//

#ifndef foundation_pair_hpp
#define foundation_pair_hpp

namespace foundation::types {

	template<typename First, typename Second>
	class Pair: public Type {

		static_assert(std::is_base_of<Object, First>::value);
		static_assert(std::is_base_of<Object, Second>::value);

	public:

		static Type::Kind typeKind() {
			return Type::Kind::pair;
		}

		Pair(First& first, Second& second) : Type(), _first(first), _second(second) { }
		Pair(const Pair<First, Second>& other) : Type(), _first(other._first), _second(other._second) { }
		Pair(const std::pair<First&, Second&> pair) : Type(), _first(pair.first), _second(pair.second) { }

		First& first() const {
			return *_first;
		}

		void setFirst(const First& first) {
			_first = first;
		}

		Second& second() const {
			return *_second;
		}

		void setSecond(const Second& second) {
			_second = second;
		}

		virtual uint64_t hash() const override {

			auto builder = Builder();

			if constexpr (std::is_base_of<Hashable, First>::value) {
				builder.add(*_first);
			}

			if constexpr (std::is_base_of<Hashable, Second>::value) {
				builder.add(*_second);
			}

			return builder;

		}

		virtual Kind kind() const override {
			return Kind::pair;
		}

		virtual Strong<Type> clone() const override {

			Strong<First> first = nullptr;
			Strong<Second> second = nullptr;

			if constexpr (std::is_base_of<Cloneable, First>::value) {
				first = _first->clone()
					.template as<First>();
			} else {
				first = _first;
			}

			if constexpr (std::is_base_of<Cloneable, Second>::value) {
				second = _second->clone()
					.template as<Second>();
			} else {
				second = _second;
			}

			return Strong<Pair<First, Second>>(first, second)
				.template as<Type>();

		}

		Pair<First, Second>& operator=(const Pair<First, Second>& other) {
			Type::operator=(other);
			_first = other._first;
			_second = other._second;
			return *this;
		}

	private:
		Strong<First> _first;
		Strong<Second> _second;

	};

}

#endif /* foundation_pair_hpp */
