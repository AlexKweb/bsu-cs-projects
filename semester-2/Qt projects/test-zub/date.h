#ifndef DATE_H
#define DATE_H

#include <iosfwd>
#include <string>

enum class DateFormat {
    European,   // DD.MM.YYYY
    RussianWords,
    American    // YYYY.MM.DD
};

class Date {
public:
    Date();
    explicit Date(int julianDay);

    static Date today();

    bool isValid() const { return m_jdn != 0; }

    int julianDay() const { return m_jdn; }

    void setJulianDay(int jdn) { m_jdn = jdn; }

    int year() const;
    int month() const;
    int day() const;

    /** Days from birth to *this (positive if this is after birth). */
    int daysLivedSince(const Date &birth) const;

    Date operator+(int days) const;
    Date operator-(int days) const;
    int operator-(const Date &other) const;

    std::string toString() const;
    bool fromString(const std::string &s);

    static void setIoFormat(DateFormat f) { s_ioFormat = f; }
    static DateFormat ioFormat() { return s_ioFormat; }

private:
    int m_jdn;

    static DateFormat s_ioFormat;

    static bool gregorianToJdn(int yUser, int m, int d, int &outJdn);
    static bool jdnToGregorian(int jdn, int &yUser, int &m, int &d);
    static bool isValidGregorian(int yUser, int m, int d);

    static int userYearToAstro(int yUser);
    static int astroYearToUser(int yAstro);
};

std::ostream &operator<<(std::ostream &os, const Date &d);
std::istream &operator>>(std::istream &is, Date &d);

#endif
