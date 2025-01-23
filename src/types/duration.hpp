// duration.hpp
// foundation-cpp
//
// Created by Kristian Trenskow on 2020/04/04.
// See license in LICENSE.
//

#ifndef foundation_duration_hpp
#define foundation_duration_hpp

#include <math.h>

#include "./string.hpp"
#include "./comparable.hpp"

namespace foundation::types {

	class Duration : public Comparable<Duration> {

	private:

		double _seconds;

	public:

		static const Duration& zero();
		static const Duration& microsecond();
		static const Duration& millisecond();
		static const Duration& second();
		static const Duration& minute();
		static const Duration& hour();
		static const Duration& day();
		static const Duration& year(bool isLeapYear = false);

		static Duration parse(const String& string);

		Duration();
		Duration(double duration);
		~Duration();

		static Duration fromMicroseconds(double microseconds);
		static Duration fromMilliseconds(double milliseconds);
		static Duration fromSeconds(double seconds);
		static Duration fromMinutes(double minutes);
		static Duration fromHours(double hours);
		static Duration fromDays(double days);

		double microseconds() const;
		double milliseconds() const;
		double seconds() const;
		double minutes() const;
		double hours() const;
		double days() const;

		enum class ToStringOptions: uint8_t {
			prefixPositive = 1 << 0
		};

		String toString(ToStringOptions options) const;

		operator double() const;

		bool operator==(const Duration& other) const;
		virtual bool operator>(const Duration& other) const override;
		Duration operator+(const Duration& other) const;
		Duration operator-(const Duration& other) const;
		Duration operator*(const double other) const;
		Duration operator/(const double other) const;
		void operator+=(const Duration& other);
		void operator-=(const Duration& other);
		void operator*=(const double other);
		void operator/=(const double other);

	};

}

#endif /* foundation_duration_hpp */
