// duration.cpp
// foundation
//
// Created by Kristian Trenskow on 2025/01/22.
// See license in LICENSE.
//

#include "duration.hpp"

using namespace foundation::types;

const Duration& Duration::zero() {
	static const Duration zero = 0;
	return zero;
}

const Duration& Duration::microsecond() {
	static const Duration microsecond = millisecond().seconds() / 1000.0;
	return microsecond;
}

const Duration& Duration::millisecond() {
	static const Duration millisecond = 1.0 / 1000.0;
	return millisecond;
}

const Duration& Duration::second() {
	static const Duration second = 1;
	return second;
}

const Duration& Duration::minute() {
	static const Duration minute = 60 * second().seconds();
	return minute;
}

const Duration& Duration::hour() {
	static const Duration hour = 60 * minute().seconds();
	return hour;
}

const Duration& Duration::day() {
	static const Duration day = 24 * hour().seconds();
	return day;
}

const Duration& Duration::year(bool isLeapYear) {
	static const Duration year = 365 * day().seconds();
	static const Duration leapYear = 366 * day().seconds();
	return isLeapYear ? leapYear : year;
}

Duration Duration::parse(const String& string) {
	if (string == String("Z")) return Duration::zero();

	String parse = string;
	if (parse.length() == 0) throw DurationParserException();

	double multiplier = 1;
	auto prefix = parse.substring(0, 1);
	if (*prefix == String("+") || *prefix == String("-")) {
		multiplier = (*prefix == String("-") ? -1 : 1);
		parse = parse.substring(1);
	}

	uint64_t hours = 0;
	uint64_t minutes = 0;

	if (parse.length() == 5 && *parse.substring(2, 1) == String(":")) {
		hours = parse.substring(0, 2)->doubleValue();
		minutes = parse.substring(3, 2)->doubleValue();
	} else if (parse.length() == 4) {
		hours = parse.substring(0, 2)->doubleValue();
		minutes = parse.substring(2, 4)->doubleValue();
	} else if (parse.length() == 2) {
		hours = parse.doubleValue();
	} else {
		throw DurationParserException();
	}

	return Duration::fromHours(hours) + Duration::fromMinutes(minutes) * multiplier;
}

Duration::Duration() : _seconds(0) {}
Duration::Duration(double duration) : _seconds(duration) {}
Duration::~Duration() {}

Duration Duration::fromMicroseconds(double microseconds) {
	return Duration::fromMilliseconds(microseconds / 1000);
}

Duration Duration::fromMilliseconds(double milliseconds) {
	return Duration::fromSeconds(milliseconds / 1000);
}

Duration Duration::fromSeconds(double seconds) {
	return Duration(seconds);
}

Duration Duration::fromMinutes(double minutes) {
	return Duration::fromSeconds(minutes * 60);
}

Duration Duration::fromHours(double hours) {
	return Duration::fromMinutes(hours * 60);
}

Duration Duration::fromDays(double days) {
	return Duration::fromHours(days * 24);
}

double Duration::microseconds() const {
	return this->milliseconds() * 1000;
}

double Duration::milliseconds() const {
	return this->seconds() * 1000;
}

double Duration::seconds() const {
	return this->_seconds;
}

double Duration::minutes() const {
	return this->seconds() / minute().seconds();
}

double Duration::hours() const {
	return this->seconds() / hour().seconds();
}

double Duration::days() const {
	return this->seconds() / day().seconds();
}

String Duration::toString(ToStringOptions options) const {
	double offset = _seconds;
	double absOffset = fabs(offset);
	uint64_t hours = Duration(absOffset).hours();
	uint64_t minutes = Duration(absOffset - Duration::fromHours(hours)).minutes();
	String prefix = offset < 0 ? "-" : (static_cast<uint8_t>(options) & static_cast<uint8_t>(ToStringOptions::prefixPositive) ? "+" : "");
	return prefix.mapCString<String>([&hours, &minutes](const char* prefix) {
		return String(String::format("%s%02llu:%02llu", prefix, hours, minutes));
	});
}

Duration::operator double() const {
	return this->seconds();
}

bool Duration::operator==(const Duration& other) const {
	return _seconds == other._seconds;
}

bool Duration::operator>(const Duration& other) const {
	return _seconds > other._seconds;
}

Duration Duration::operator+(const Duration& other) const {
	return this->seconds() + other.seconds();
}

Duration Duration::operator-(const Duration& other) const {
	return this->seconds() - other.seconds();
}

Duration Duration::operator*(const double other) const {
	return this->seconds() * other;
}

Duration Duration::operator/(const double other) const {
	return this->seconds() / other;
}

void Duration::operator+=(const Duration& other) {
	_seconds += other;
}

void Duration::operator-=(const Duration& other) {
	_seconds -= other;
}

void Duration::operator*=(const double other) {
	_seconds *= other;
}

void Duration::operator/=(const double other) {
	_seconds /= other;
}
