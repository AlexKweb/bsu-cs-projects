#include "date.h"

#include <QDate>

#include <algorithm>
#include <cctype>
#include <cstring>
#include <iomanip>
#include <sstream>
#include <vector>

DateFormat Date::s_ioFormat = DateFormat::European;

namespace {

std::string trim(std::string s)
{
    auto notspace = [](unsigned char c) { return !std::isspace(c); };
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), notspace));
    s.erase(std::find_if(s.rbegin(), s.rend(), notspace).base(), s.end());
    return s;
}

std::string toLowerAscii(std::string s)
{
    for (char &c : s)
        c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    return s;
}

std::string replaceAll(std::string s, const std::string &from, const std::string &to)
{
    size_t pos = 0;
    while ((pos = s.find(from, pos)) != std::string::npos) {
        s.replace(pos, from.size(), to);
        pos += to.size();
    }
    return s;
}

bool isLeapAstro(int yAstro)
{
    if (yAstro % 4 != 0)
        return false;
    if (yAstro % 100 != 0)
        return true;
    return (yAstro % 400) == 0;
}

int daysInMonthAstro(int yAstro, int m)
{
    static const int md[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (m == 2 && isLeapAstro(yAstro))
        return 29;
    return md[m];
}

const char *kMonthGen[] = {
    nullptr,
    "января", "февраля", "марта", "апреля", "мая", "июня",
    "июля", "августа", "сентября", "октября", "ноября", "декабря",
};

const char *kDayNeuter[] = {
    nullptr,
    "первое", "второе", "третье", "четвёртое", "пятое", "шестое", "седьмое", "восьмое", "девятое", "десятое",
    "одиннадцатое", "двенадцатое", "тринадцатое", "четырнадцатое", "пятнадцатое", "шестнадцатое", "семнадцатое",
    "восемнадцатое", "девятнадцатое", "двадцатое", "двадцать первое", "двадцать второе", "двадцать третье",
    "двадцать четвёртое", "двадцать пятое", "двадцать шестое", "двадцать седьмое", "двадцать восьмое",
    "двадцать девятое", "тридцатое", "тридцать первое",
};

int dayFromRussianWord(const std::string &low)
{
    for (int d = 1; d <= 31; ++d) {
        if (low == toLowerAscii(std::string(kDayNeuter[d])))
            return d;
    }
    return 0;
}

std::string capitalizeFirst(std::string s)
{
    if (s.empty())
        return s;
    s[0] = static_cast<char>(std::toupper(static_cast<unsigned char>(s[0])));
    return s;
}

std::string join(const std::vector<std::string> &parts, const std::string &sep)
{
    std::string r;
    for (size_t i = 0; i < parts.size(); ++i) {
        if (i)
            r += sep;
        r += parts[i];
    }
    return r;
}

// Genitive forms for 1..99 used in year phrases (after thousands), masculine-ish as in "двадцать пятого"
std::string tensUnitsGenitive(int n)
{
    static const char *u[] = {
        "", "первого", "второго", "третьего", "четвёртого", "пятого", "шестого", "седьмого", "восьмого", "девятого",
    };
    static const char *teens[] = {
        "десятого", "одиннадцатого", "двенадцатого", "тринадцатого", "четырнадцатого", "пятнадцатого",
        "шестнадцатого", "семнадцатого", "восемнадцатого", "девятнадцатого",
    };
    static const char *t[] = {"", "", "двадцать", "тридцать", "сорок", "пятьдесят", "шестьдесят", "семьдесят",
                              "восемьдесят", "девяносто",
    };
    if (n <= 0 || n >= 100)
        return {};
    if (n < 10)
        return u[n];
    if (n < 20)
        return teens[n - 10];
    static const char *tGen[] = {"", "", "двадцатого", "тридцатого", "сорокового", "пятидесятого", "шестидесятого",
                                 "семидесятого", "восьмидесятого", "девяностого",
    };
    int t10 = n / 10;
    int u10 = n % 10;
    if (u10 == 0)
        return std::string(tGen[t10]);
    return std::string(t[t10]) + " " + u[u10];
}

std::string yearPhraseAd(int userYear)
{
    if (userYear <= 0)
        return {};
    int thousands = userYear / 1000;
    int rem = userYear % 1000;
    std::vector<std::string> chunks;

    if (thousands == 1)
        chunks.push_back("одна тысяча");
    else if (thousands == 2)
        chunks.push_back("две тысячи");
    else if (thousands == 3)
        chunks.push_back("три тысячи");
    else if (thousands == 4)
        chunks.push_back("четыре тысячи");
    else if (thousands >= 5)
        chunks.push_back(std::to_string(thousands) + " тысяч"); // rare fallback

    int hundreds = rem / 100;
    int rest = rem % 100;
    static const char *h[] = {"", "сто", "двести", "триста", "четыреста", "пятьсот", "шестьсот", "семьсот",
                              "восемьсот", "девятьсот",
    };
    if (hundreds > 0)
        chunks.push_back(h[hundreds]);

    std::string tail = tensUnitsGenitive(rest);
    if (!tail.empty())
        chunks.push_back(tail);

    if (chunks.empty())
        return tensUnitsGenitive(userYear);

    return join(chunks, " ");
}

std::string yearPhraseBcUser(int bcYearAbs)
{
    // user year -77 => bcYearAbs 77 -> "семьдесят седьмого"
    return tensUnitsGenitive(bcYearAbs);
}

std::string canonicalYearRussian(int userYear)
{
    if (userYear > 0)
        return yearPhraseAd(userYear);
    int bc = -userYear;
    return yearPhraseBcUser(bc);
}

bool splitRussianDate(const std::string &input, int &dayOut, int &monthOut, int &yearUserOut, bool &bcOut)
{
    std::string s = toLowerAscii(trim(replaceAll(input, "ё", "е")));
    bcOut = false;
    if (s.size() >= 15 && s.substr(s.size() - 15) == "до нашей эры") {
        bcOut = true;
        s = trim(s.substr(0, s.size() - 15));
    }
    const std::string tail = " года";
    if (s.size() >= tail.size() && s.substr(s.size() - tail.size()) == tail)
        s = trim(s.substr(0, s.size() - tail.size()));

    int month = 0;
    size_t mpos = std::string::npos;
    for (int m = 1; m <= 12; ++m) {
        size_t p = s.find(kMonthGen[m]);
        if (p != std::string::npos && (mpos == std::string::npos || p < mpos)) {
            mpos = p;
            month = m;
        }
    }
    if (month == 0 || mpos == std::string::npos)
        return false;

    std::string dayPart = trim(s.substr(0, mpos));
    std::string yearPart = trim(s.substr(mpos + std::strlen(kMonthGen[month])));

    int d = dayFromRussianWord(dayPart);
    if (d == 0)
        return false;

    std::string ycanon = toLowerAscii(trim(yearPart));
    int yUser = 0;
    bool found = false;
    if (!bcOut) {
        for (int y = 1; y <= 12000 && !found; ++y) {
            if (toLowerAscii(canonicalYearRussian(y)) == ycanon) {
                yUser = y;
                found = true;
            }
        }
    } else {
        for (int bc = 1; bc <= 4000 && !found; ++bc) {
            int uy = -bc;
            if (toLowerAscii(canonicalYearRussian(uy)) == ycanon) {
                yUser = uy;
                found = true;
            }
        }
    }
    if (!found)
        return false;

    dayOut = d;
    monthOut = month;
    yearUserOut = yUser;
    return true;
}

std::string formatEuropean(int yUser, int m, int d)
{
    std::ostringstream os;
    os << std::setfill('0') << std::setw(2) << d << '.' << std::setw(2) << m << '.';
    if (yUser < 0)
        os << '-' << std::setw(4) << std::setfill('0') << (-yUser);
    else
        os << std::setw(4) << std::setfill('0') << yUser;
    return os.str();
}

std::string formatAmerican(int yUser, int m, int d)
{
    std::ostringstream os;
    if (yUser < 0)
        os << '-' << std::setw(4) << std::setfill('0') << (-yUser);
    else
        os << std::setw(4) << std::setfill('0') << yUser;
    os << '.' << std::setw(2) << std::setfill('0') << m << '.' << std::setw(2) << d;
    return os.str();
}

bool parseDottedTriple(const std::string &s, int &a, int &b, int &c, DateFormat fmt)
{
    std::string t = trim(s);
    if (t.empty())
        return false;

    bool negYear = false;
    if (fmt == DateFormat::American && !t.empty() && t[0] == '-') {
        negYear = true;
        t = t.substr(1);
    } else if (fmt == DateFormat::European) {
        // DD.MM.-YYYY handled by scanning last dot segment for leading minus
    }

    std::vector<std::string> parts;
    {
        std::stringstream ss(t);
        std::string item;
        while (std::getline(ss, item, '.')) {
            if (!item.empty() || parts.empty())
                parts.push_back(item);
        }
    }
    if (parts.size() != 3)
        return false;

    auto parseInt = [](const std::string &x, int &out) -> bool {
        if (x.empty())
            return false;
        size_t i = 0;
        if (x[0] == '-') {
            if (x.size() == 1)
                return false;
            ++i;
        }
        for (; i < x.size(); ++i) {
            if (!std::isdigit(static_cast<unsigned char>(x[i])))
                return false;
        }
        try {
            out = std::stoi(x);
        } catch (...) {
            return false;
        }
        return true;
    };

    int v0, v1, v2;
    if (!parseInt(parts[0], v0) || !parseInt(parts[1], v1) || !parseInt(parts[2], v2))
        return false;

    if (fmt == DateFormat::European) {
        int d = v0;
        int m = v1;
        std::string ystr = parts[2];
        int y = 0;
        if (!ystr.empty() && ystr[0] == '-') {
            if (!parseInt(ystr.substr(1), y))
                return false;
            y = -y;
        } else {
            if (!parseInt(ystr, y))
                return false;
        }
        a = d;
        b = m;
        c = y;
    } else {
        // American YYYY.MM.DD — minus already stripped for year
        int y = v0;
        if (negYear)
            y = -y;
        a = y;
        b = v1;
        c = v2;
    }
    return true;
}

} // namespace

int Date::userYearToAstro(int yUser)
{
    if (yUser < 0)
        return -(std::abs(yUser) - 1);
    return yUser;
}

int Date::astroYearToUser(int yAstro)
{
    if (yAstro > 0)
        return yAstro;
    return -(std::abs(yAstro) + 1);
}

bool Date::isValidGregorian(int yUser, int m, int d)
{
    if (m < 1 || m > 12 || d < 1)
        return false;
    if (yUser == 0)
        return false;
    int ya = userYearToAstro(yUser);
    return d <= daysInMonthAstro(ya, m);
}

bool Date::gregorianToJdn(int yUser, int m, int d, int &outJdn)
{
    if (!isValidGregorian(yUser, m, d))
        return false;
    int ya = userYearToAstro(yUser);
    QDate q(ya, m, d);
    if (!q.isValid())
        return false;
    outJdn = q.toJulianDay();
    return true;
}

bool Date::jdnToGregorian(int jdn, int &yUser, int &m, int &d)
{
    QDate q = QDate::fromJulianDay(jdn);
    if (!q.isValid())
        return false;
    yUser = astroYearToUser(q.year());
    m = q.month();
    d = q.day();
    return true;
}

Date::Date() : m_jdn(0) {}

Date::Date(int julianDay) : m_jdn(julianDay) {}

Date Date::today()
{
    QDate q = QDate::currentDate();
    return Date(q.toJulianDay());
}

int Date::year() const
{
    int y, m, d;
    if (!jdnToGregorian(m_jdn, y, m, d))
        return 0;
    return y;
}

int Date::month() const
{
    int y, m, d;
    if (!jdnToGregorian(m_jdn, y, m, d))
        return 0;
    return m;
}

int Date::day() const
{
    int y, m, d;
    if (!jdnToGregorian(m_jdn, y, m, d))
        return 0;
    return d;
}

int Date::daysLivedSince(const Date &birth) const
{
    return m_jdn - birth.m_jdn;
}

Date Date::operator+(int days) const
{
    return Date(m_jdn + days);
}

Date Date::operator-(int days) const
{
    return Date(m_jdn - days);
}

int Date::operator-(const Date &other) const
{
    return m_jdn - other.m_jdn;
}

std::string Date::toString() const
{
    if (!isValid())
        return {};
    int y, m, d;
    if (!jdnToGregorian(m_jdn, y, m, d))
        return {};

    switch (s_ioFormat) {
    case DateFormat::European:
        return formatEuropean(y, m, d);
    case DateFormat::American:
        return formatAmerican(y, m, d);
    case DateFormat::RussianWords: {
        std::string body = std::string(kDayNeuter[d]) + " " + kMonthGen[m] + " " + canonicalYearRussian(y) + " года";
        if (y < 0)
            body += " до нашей эры";
        return capitalizeFirst(body);
    }
    }
    return {};
}

bool Date::fromString(const std::string &s)
{
    m_jdn = 0;
    switch (s_ioFormat) {
    case DateFormat::European: {
        int dd, mm, yy;
        if (!parseDottedTriple(s, dd, mm, yy, DateFormat::European))
            return false;
        int j = 0;
        if (!gregorianToJdn(yy, mm, dd, j))
            return false;
        m_jdn = j;
        return true;
    }
    case DateFormat::American: {
        int yy, mm, dd;
        std::string t = trim(s);
        bool neg = false;
        if (!t.empty() && t[0] == '-') {
            neg = true;
            t = trim(t.substr(1));
        }
        std::vector<std::string> parts;
        {
            std::stringstream ss(t);
            std::string item;
            while (std::getline(ss, item, '.')) {
                if (!item.empty() || parts.empty())
                    parts.push_back(item);
            }
        }
        if (parts.size() != 3)
            return false;
        auto parseInt = [](const std::string &x, int &out) -> bool {
            if (x.empty())
                return false;
            for (char ch : x) {
                if (!std::isdigit(static_cast<unsigned char>(ch)))
                    return false;
            }
            try {
                out = std::stoi(x);
            } catch (...) {
                return false;
            }
            return true;
        };
        if (!parseInt(parts[0], yy) || !parseInt(parts[1], mm) || !parseInt(parts[2], dd))
            return false;
        if (neg)
            yy = -yy;
        int j = 0;
        if (!gregorianToJdn(yy, mm, dd, j))
            return false;
        m_jdn = j;
        return true;
    }
    case DateFormat::RussianWords: {
        int d, m, y;
        bool bc;
        if (!splitRussianDate(s, d, m, y, bc))
            return false;
        int j = 0;
        if (!gregorianToJdn(y, m, d, j))
            return false;
        m_jdn = j;
        return true;
    }
    }
    return false;
}

std::ostream &operator<<(std::ostream &os, const Date &d)
{
    os << d.toString();
    return os;
}

std::istream &operator>>(std::istream &is, Date &d)
{
    std::string token;
    if (Date::ioFormat() == DateFormat::RussianWords) {
        std::string line;
        std::getline(is >> std::ws, line);
        d.fromString(line);
    } else {
        is >> token;
        d.fromString(token);
    }
    return is;
}
