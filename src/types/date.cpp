//
// date.cpp
// shared-foundation-cpp
//
// Created by Kristian Trenskow on 2025/01/22.
// See license in LICENSE.
//

#include "date.hpp"

using namespace games::zerobit::shared::foundation::types;

Type::Kind Date::typeKind() {
	return Type::Kind::date;
}

uint16_t Date::daysInMonth(
	Month month,
	bool isLeapYear
) {

	switch (month) {
		case Month::february: return isLeapYear ? 29 : 28;
		case Month::april: return 30;
		case Month::june: return 30;
		case Month::september: return 30;
		case Month::november: return 30;
		default: break;
	}

	return 31;

}

int64_t Date::_daysFromEpoch(
	int64_t y,
	unsigned m,
	unsigned d) {

	y -= m <= 2;
	const int64_t era = (y >= 0 ? y : y-399) / 400;
	const unsigned yoe = static_cast<unsigned>(y - era * 400);      // [0, 399]
	const unsigned doy = (153*(m + (m > 2 ? -3 : 9)) + 2)/5 + d-1;  // [0, 365]
	const unsigned doe = yoe * 365 + yoe/4 - yoe/100 + doy;         // [0, 146096]
	return era * 146097 + static_cast<int64_t>(doe) - 719468;

}

void Date::_components(
	int64_t time,
	int64_t *year,
	uint8_t *month,
	uint8_t *day
) {

	time += 719468;
	const int64_t era = (time >= 0 ? time : time - 146096) / 146097;
	const unsigned doe = static_cast<unsigned>(time - era * 146097);          // [0, 146096]
	const unsigned yoe = (doe - doe/1460 + doe/36524 - doe/146096) / 365;  // [0, 399]
	const int64_t y = static_cast<int64_t>(yoe) + era * 400;
	const unsigned doy = doe - (365*yoe + yoe/4 - yoe/100);                // [0, 365]
	const unsigned mp = (5*doy + 2)/153;                                   // [0, 11]
	const unsigned d = doy - (153*mp+2)/5 + 1;                             // [1, 31]
	const unsigned m = mp + (mp < 10 ? 3 : -9);                            // [1, 12]
	if (year != nullptr) *year = y + (m <= 2);
	if (month != nullptr) *month = m;
	if (day != nullptr) *day = d;

}

void Date::_set(
	const int64_t year,
	const uint8_t month,
	const uint8_t day,
	const uint8_t hours,
	const uint8_t minutes,
	const uint8_t seconds,
	uint64_t microseconds) {

	_time = Duration::fromDays(_daysFromEpoch(year, month, day));
	_time += Duration::fromHours(hours);
	_time += Duration::fromMinutes(minutes);
	_time += Duration::fromSeconds(seconds);
	_time += Duration::fromMicroseconds(microseconds);

}

const Duration Date::_localOffset() {

	time_t rawTime;

	::time(&rawTime);

	return localtime(&rawTime)->tm_gmtoff;

}

const Date& Date::epoch() {
	static const Date epoch = Duration::zero();
	return epoch;
}

bool Date::isLeapYear(
	int64_t year
) {
	return year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);
}

Date::Date(
	const Date& other
) : Type(),
	_time(other._time),
	_timeZone(other._timeZone) { }

Date::Date(
) : _timeZone(TimeZone::utc) {

	struct timeval tv;

	gettimeofday(&tv, nullptr);

	time_t time = ::time(nullptr);

	_time = Duration::fromSeconds((double)time + ((double)tv.tv_usec / 1000000));

}

Date::Date(
	const Duration& time,
	TimeZone timeZone
) : _time(time),
	_timeZone(timeZone) { }

Date::Date(
	const double time,
	TimeZone timeZone
) : Date(Duration(time), timeZone) { }

Date::Date(
	const int64_t year,
	const uint8_t month,
	const uint8_t day,
	const uint8_t hours,
	const uint8_t minutes,
	const uint8_t seconds,
	uint64_t microseconds
) : Date() {
	this->_set(year, month, day, hours, minutes, seconds, microseconds);
}

Date::Date(
	const String& iso8601
)  : Date() {

	int64_t year = 0;
	uint8_t month = 0;
	uint8_t day = 0;
	uint8_t hours = 0;
	uint8_t minutes = 0;
	uint8_t seconds = 0;
	uint64_t microseconds = 0;
	Duration timeZoneOffset;

	auto parts = iso8601.split("T");

	if (parts->count() > 0) {

		auto datePart = parts->itemAtIndex(0);

		year = datePart->substring(0, 4)->doubleValue();

		if (datePart->length() == 8) {
			month = datePart->substring(4, 2)->doubleValue();
			day = datePart->substring(6, 2)->doubleValue();
		}
		else if (datePart->length() == 10) {
			month = datePart->substring(5, 2)->doubleValue();
			day = datePart->substring(8, 2)->doubleValue();
		}
		else throw ISO8601Exception();

		if (parts->count() > 1) {

			auto separators = Array<String>({ "+", "-", "Z"});

			auto timeParts = parts->itemAtIndex(1)->split(separators, IncludeSeparator::prefix);

			auto timeComponentsPart = timeParts->itemAtIndex(0);

			auto timeComponentsSubParts = timeComponentsPart->split(".");

			if (timeComponentsSubParts->count() > 0) {

				auto hmsComponentParts = timeComponentsSubParts->itemAtIndex(0);

				hours = hmsComponentParts->substring(0, 2)->doubleValue();

				if (hmsComponentParts->length() == 5) {
					minutes = hmsComponentParts->substring(2, 2)->doubleValue();
					seconds = hmsComponentParts->substring(4, 2)->doubleValue();
				}
				else if (hmsComponentParts->length() == 8) {
					minutes = hmsComponentParts->substring(3, 2)->doubleValue();
					seconds = hmsComponentParts->substring(6, 2)->doubleValue();
				}

				if (timeComponentsSubParts->count() > 1) {
					microseconds = timeComponentsSubParts->itemAtIndex(1)->doubleValue();
				}

			} else throw ISO8601Exception();

			if (timeParts->count() > 1 && timeParts->itemAtIndex(1)->length() > 0) {
				timeZoneOffset = Duration::parse(timeParts->itemAtIndex(1));
			} else throw ISO8601Exception();

		}

	}
	else ISO8601Exception();

	this->_set(year, month, day, hours, minutes, seconds, microseconds);

	_time -= timeZoneOffset;

}

Date::~Date() { }

int64_t Date::year() const {
	int64_t year;
	_components(this->_time.days(), &year, nullptr, nullptr);
	return year;
}

Date::Month Date::month() const {
	uint8_t month;
	_components(this->_time.days(), nullptr, &month, nullptr);
	return Month(month);
}

int16_t Date::day() const {
	uint8_t day;
	_components(this->_time.days(), nullptr, nullptr, &day);
	return day;
}

Date::Weekday Date::weekday() const {
	return static_cast<Weekday>(((int64_t)this->_time.days() + static_cast<int64_t>(_epochWeekday)) % daysInWeek);
}

const Duration Date::durationSinceMidnight() const {
	auto seconds = this->_time.seconds();
	double daysSeconds = floor(this->_time.days()) * Duration::day();
	return Duration(seconds - daysSeconds);
}

bool Date::isLeapYear() {
	return Date::isLeapYear(this->year());
}

uint8_t Date::hours() const {
	return this->durationSinceMidnight() / Duration::hour();
}

uint8_t Date::minutes() const {
	return (this->durationSinceMidnight().seconds() - (this->hours() * Duration::hour())) / Duration::minute();
}

uint8_t Date::seconds() const {
	return (this->durationSinceMidnight().seconds()) - (this->hours() * Duration::hour()) - (this->minutes() * Duration::minute());
}

uint32_t Date::microseconds() const{
	return (this->_time.seconds() - floor(this->_time.seconds())) * 1000000;
}

const Duration Date::durationSinceEpoch() const{
	return this->_time;
}

Duration Date::since(
	const Date& other
) const{
	return this->durationSinceEpoch() - other.durationSinceEpoch();
}

Date Date::to(
	TimeZone timeZone
) const{
	if (this->_timeZone == timeZone) return Date(*this);
	if (timeZone == TimeZone::local) return Date(this->_time + _localOffset(), TimeZone::local);
	return Date(this->_time - _localOffset(), TimeZone::utc);
}

String Date::toISO8601() const{

	String ret;

	ret.append(String::format("%02lld-%02d-%02dT%02d:%02d:%02d",
								this->year(),
								this->month(),
								this->day(),
								this->hours(),
								this->minutes(),
								this->seconds()));

	auto microseconds = this->microseconds();

	if (microseconds != 0) {
		ret.append(String::format(".%llu", microseconds));
	}

	switch (this->_timeZone) {
		case TimeZone::utc:
			ret.append("Z");
			break;
		case TimeZone::local: {
			ret.append(_localOffset().toString(Duration::ToStringOptions::prefixPositive));
			break;
		}
	}

	return ret;

}

Type::Kind Date::kind() const{
	return Kind::date;
}

uint64_t Date::hash() const {
	double seconds = this->_time.seconds();
	uint64_t hash;
	memcpy(&hash, &seconds, sizeof(uint64_t));
	return hash;
}

bool Date::operator==(
	const Type& other
) const {
	if (other.kind() != Kind::date) return false;
	return this->to(TimeZone::utc)._time == ((const Date&)other).to(TimeZone::utc)._time;
}

Date Date::operator+(
	const Duration& duration
) const {
	return Date(this->seconds() + duration.seconds());
}

Date Date::operator-(
	const Duration& duration
) const{
	return Date(this->seconds() - duration.seconds());
}

const Duration Date::operator-(
	const Date& other
) const {
	return this->seconds() - other.seconds();
}

void Date::operator+=(
	const Duration& duration
) {
	this->_time += duration;
}

void Date::operator-=(
	const Duration& duration
) {
	this->_time -= duration;
}

bool Date::operator>(
	const Date& other
) const	{
	return this->to(TimeZone::utc).durationSinceEpoch().seconds() > other.to(TimeZone::utc).durationSinceEpoch().seconds();
}

Date& Date::operator=(
	const Date& other
) {
	Type::operator=(other);
	this->_time = other._time;
	this->_timeZone = other._timeZone;
	return *this;
}

Date::operator double() const {
	return this->_time.seconds();
}
