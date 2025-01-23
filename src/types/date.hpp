//
// date.hpp
// foundation
//
// Created by Kristian Trenskow on 2020/04/03.
// See license in LICENSE.
//

#ifndef foundation_date_hpp
#define foundation_date_hpp

#include <time.h>
#include <sys/time.h>

#include "./type.hpp"
#include "./comparable.hpp"
#include "./duration.hpp"

using namespace foundation::exceptions::types;

namespace foundation::types {

	class Date : public Type, public Comparable<Date> {

	public:

		static Type::Kind typeKind();

		enum class TimeZone {
			utc,
			local
		};

		enum class Month: uint8_t {
			january = 1,
			february,
			march,
			april,
			may,
			june,
			july,
			august,
			september,
			october,
			november,
			december
		};

		enum class Weekday: uint8_t {
			sunday = 0,
			monday,
			tuesday,
			wednesday,
			thursday,
			friday,
			saturday
		};

		static constexpr Date::Weekday _epochWeekday = Date::Weekday::thursday;

	private:

		Duration _time;
		TimeZone _timeZone;

		static uint16_t daysInMonth(
			Month month,
			bool isLeapYear = false);

		static int64_t _daysFromEpoch(
			int64_t y,
			unsigned m,
			unsigned d);

		static void _components(
			int64_t time,
			int64_t *year,
			uint8_t *month,
			uint8_t *day);

		void _set(
			const int64_t year,
			const uint8_t month,
			const uint8_t day,
			const uint8_t hours,
			const uint8_t minutes,
			const uint8_t seconds,
			uint64_t microseconds);

		static const Duration _localOffset();

	public:

		static const uint8_t daysInWeek = 7;

		static const Date& epoch();

		static bool isLeapYear(
			int64_t year);

		Date(
			const Date& other);

		Date();

		Date(
			const Duration& time,
			TimeZone timeZone = TimeZone::utc),

		Date(
			const double time,
			TimeZone timeZone = TimeZone::utc);

		Date(
			const int64_t year,
			const uint8_t month,
			const uint8_t day,
			const uint8_t hours = 0,
			const uint8_t minutes = 0,
			const uint8_t seconds = 0,
			uint64_t microseconds = 0);

		Date(
			const String& iso8601);

		virtual ~Date();

		int64_t year() const;
		Month month() const;
		int16_t day() const;
		Weekday weekday() const;

		const Duration durationSinceMidnight() const;
		bool isLeapYear();

		uint8_t hours() const;
		uint8_t minutes() const;
		uint8_t seconds() const;
		uint32_t microseconds() const;

		const Duration durationSinceEpoch() const;

		Duration since(
			const Date& other
		) const;

		Date to(TimeZone timeZone) const;

		String toISO8601() const;

		virtual Kind kind() const override;

		virtual uint64_t hash() const override;

		virtual bool operator==(
			const Type& other
		) const override;

		Date operator+(
			const Duration& duration
		) const;

		Date operator-(
			const Duration& duration
		) const;

		const Duration operator-(
			const Date& other
		) const;

		void operator+=(
			const Duration& duration);

		void operator-=(
			const Duration& duration);

		virtual bool operator>(
			const Date& other
		) const override;

		Date& operator=(
			const Date& other);

		operator double() const;

	};

}

#endif /* foundation_date_hpp */
