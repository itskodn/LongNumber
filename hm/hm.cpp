#include "hm.hpp"
#include <iostream>
#include <cmath>
#include <algorithm>

LongNumber::LongNumber()
{
    precision = 0;
    sign = false;
    array = std::vector<char>();
    point_position = 0;
}
LongNumber::LongNumber(int value, int precision) : precision(precision), sign(value < 0)
{
    array.clear();
    value = std::abs(value);
    point_position = 32;
    for (int i = 0; i < 32; i++)
    {
        array.push_back((value & 1 << (32 - 1 - i)) != 0);
    }
    make_norm();
}
LongNumber::LongNumber(const LongNumber &other)
{
    precision = other.precision;
    sign = other.sign;
    array = other.array;
    point_position = other.point_position;
}
LongNumber &LongNumber::operator=(const LongNumber &other)
{
    if (&other != this)
    {
        precision = other.precision;
        sign = other.sign;
        array = other.array;
        point_position = other.point_position;
    }
    return *this;
}
LongNumber::LongNumber(long double number, int _precision) : precision(_precision)
{
    sign = (number < 0);
    point_position = 1;
    array.push_back(0);

    if (number == 0)
    {
        return;
    }

    number = std::abs(number);
    int exponent;
    long double mantissa = std::frexp(number, &exponent);
    while (mantissa != 0 && array.size() < precision)
    {
        mantissa *= 2;
        int bit = static_cast<int>(mantissa);
        array.push_back(bit);
        mantissa -= bit;
    }

    if (exponent != 0)
    {
        *this = (exponent < 0) ? this->operator>>(-exponent) : this->operator<<(exponent);
    }
    make_norm();
}

LongNumber::LongNumber(double number, int _precision) : precision(_precision)
{
    sign = (number < 0);
    point_position = 1;
    array.push_back(0);

    if (number == 0)
    {
        return;
    }

    number = std::abs(number);
    int exponent;
    double mantissa = std::frexp(number, &exponent);
    while (mantissa != 0 && array.size() < precision)
    {
        mantissa *= 2;
        int bit = static_cast<int>(mantissa);
        array.push_back(bit);
        mantissa -= bit;
    }

    if (exponent != 0)
    {
        *this = (exponent < 0) ? this->operator>>(-exponent) : this->operator<<(exponent);
    }
    make_norm();
}

void LongNumber::setPrecision(int new_precision)
{
    precision = new_precision;
    make_norm();
}
LongNumber operator""_longnum(long double number)
{
    return LongNumber(number, 1000);
}

LongNumber::~LongNumber() {}

bool LongNumber::operator==(const LongNumber &other) const
{
    return sign == other.sign && precision == other.precision && array == other.array && point_position == other.point_position;
}

bool LongNumber::operator!=(const LongNumber &other) const
{
    return !(*this == other);
}

LongNumber LongNumber::operator>>(int shift) const
{
    LongNumber result(*this);
    result.array.insert(result.array.begin(), shift, false);
    result.make_norm();
    return result;
}
LongNumber LongNumber::operator<<(int shift) const
{
    LongNumber result(*this);
    result.point_position += shift;
    result.array.insert(result.array.end(), shift, false);
    result.make_norm();
    return result;
}

bool LongNumber::operator<(const LongNumber &other) const
{
    if (sign != other.sign)
    {
        return sign;
    }
    if (point_position != other.point_position)
    {
        return (point_position < other.point_position);
    }
    unsigned int size = std::max(array.size(), other.array.size()); 
    for (int i = 0; i < size; i++)
    {
        bool arr1_bit = (i < array.size() ? array[i] : 0);
        bool arr2_bit = (i < other.array.size() ? other.array[i] : 0);
        if (arr1_bit != arr2_bit)
        {
            return arr1_bit < arr2_bit;
        }
    }
    return false;
}

bool LongNumber::operator>=(const LongNumber &other) const
{
    return !(*this < other);
}

bool LongNumber::operator<=(const LongNumber &other) const
{
    return (*this < other) || (*this == other);
}

bool LongNumber::operator>(const LongNumber &other) const
{
    return !(*this <= other);
}

LongNumber LongNumber::abs() const
{
    LongNumber rez = *this;
    rez.sign = false;
    return rez;
}

LongNumber LongNumber::plus_bit(const LongNumber &number1, const LongNumber &number2) const
{
    LongNumber first(number1), second(number2);
    align(first, second);
    std::vector<char> a = first.array, b = second.array;
    unsigned int size = std::max(a.size(), b.size());
    std::vector<char> rez(size + 1, 0);
    bool carry = 0;
    for (int i = size - 1; i >= 0; i--)
    {
        bool arr1_bit = a[i];
        bool arr2_bit = b[i];
        bool sum = arr1_bit ^ arr2_bit ^ carry;
        carry = (arr1_bit + arr2_bit + carry) >> 1;
        rez[i + 1] = sum;
    }
    rez[0] = carry;
    LongNumber ans(first);
    ans.point_position = first.point_position + 1;
    ans.array = rez;
    ans.make_norm();
    return ans;
}

LongNumber LongNumber::minus_bit(const LongNumber &number1, const LongNumber &number2) const
{
    LongNumber first(number1), second(number2);
    align(first, second);
    std::vector<char> a = first.array, b = second.array;
    unsigned int size = std::max(a.size(), b.size());
    std::vector<char> rez(size, 0);
    bool borrow = false;

    for (int i = (int)a.size() - 1; i >= 0; --i)
    {
        bool bit1 = a[i];
        bool bit2 = (i < b.size()) ? b[i] : 0;

        rez[i] = (bit1 ^ bit2 ^ borrow);

        if ((bit1 == false && bit2 == true) || (bit1 == false && borrow == true) || (bit2 == true && borrow == true))
            borrow = true;
        else
            borrow = false;
    }

    LongNumber ans(number1);
    ans.array = rez;
    ans.make_norm();

    return ans;
}

LongNumber LongNumber::operator+(const LongNumber &other) const
{
    if (!sign && !other.sign)
    {
        return plus_bit(*this, other);
    }
    if (sign && other.sign)
    {
        LongNumber ans = plus_bit(*this, other);
        ans.sign = !ans.sign;
        return ans;
    }

    if (!sign && other.sign)
    {
        if (*this > negative(other))
        {
            return minus_bit(*this, other);
        }
        else
        {
            return negative(minus_bit(other, *this));
        }
    }
    else
    {
        if (negative(*this) < other)
        {
            return minus_bit(other, *this);
        }
        else
        {
            return minus_bit(*this, other);
        }
    }
}

LongNumber LongNumber::negative(const LongNumber &number) const
{
    LongNumber ans(number);
    ans.sign = !ans.sign;
    return ans;
}
LongNumber LongNumber::operator-(const LongNumber &other) const
{
    LongNumber a = *this;
    LongNumber b = other;
    align(a, b);

    if (!sign && !other.sign)
    {
        if (*this > other)
        {
            return minus_bit(*this, other);
        }
        else
        {
            return negative(minus_bit(other, *this));
        }
    }

    if (sign && sign == other.sign)
    {
        return *this + negative(other);
    }

    if (!sign && other.sign)
    {
        return plus_bit(*this, other);
    }

    if (sign && !other.sign)
    {
        return negative(plus_bit(*this, other));
    }

    return *this;
}

std::vector<char> getArray(const LongNumber &ln)
{
    return ln.array;
}

LongNumber LongNumber::operator*(const LongNumber &other) const
{
    int mx_prec = std::max(precision, other.precision);

    LongNumber res(0, mx_prec);
    LongNumber first(*this), second(other);

    align(first, second);
    
    first.sign = second.sign = false;
    first = first << (second.point_position - 1);

    for (int i = 0; i < (int)second.array.size(); ++i, first = first >> 1)
    {
        if (second.array[i] == false)
            continue;
        
        res = res + first;
    }

    res.sign = (sign != other.sign);
    res.make_norm();

    return res;

}

void LongNumber::align(LongNumber &a, LongNumber &b) const
{
    int magn = std::max(a.point_position, b.point_position);
    int prec = std::max(a.precision, b.precision);

    a.array.insert(a.array.begin(), magn - a.point_position, false);
    a.point_position += magn - a.point_position;
    a.array.insert(a.array.end(), prec - (a.array.size() - a.point_position), false);

    b.array.insert(b.array.begin(), magn - b.point_position, false);
    b.point_position += magn - b.point_position;
    b.array.insert(b.array.end(), prec - (b.array.size() - b.point_position), false);
}

void LongNumber::make_norm()
{
    if (point_position > array.size())
        point_position = array.size();
    
    while (array.size() > 1 && point_position < array.size() && (array.back() == false || precision + point_position < (int)array.size()))
    {
        array.pop_back();
    }

    while (array.size() > 1 && point_position > 1 && array.front() == false)
    {
        array.erase(array.begin());
        point_position--;
    }
}

LongNumber LongNumber::operator/(const LongNumber &div) const
{

    LongNumber q(0, std::max(precision, div.precision));
    LongNumber d = *this;
    LongNumber normDiv = div;

    d = d << (normDiv.array.size() - normDiv.point_position);
    normDiv = normDiv << (normDiv.array.size() - normDiv.point_position);

    while (d.array.size() - d.point_position <= d.precision)
        d.array.push_back(false);

    q.sign = (d.sign != normDiv.sign);
    d.sign = false;
    normDiv.sign = false;

    LongNumber r(0, precision);

    q.array.clear();
    q.point_position = -1;

    for (int i = 0; i < (int)d.array.size(); ++i)
    {
        r = r << 1;
        r.array[(int)r.array.size() - 1] = d.array[i];
        if (r >= normDiv)
        {
            r = r - normDiv;
            q.array.push_back(true);
        }
        else
        {
            q.array.push_back(false);
        }
        if (i >= d.point_position && q.point_position == -1)
        {
            q.point_position = q.array.size() - 1;
        }
    }

    q.make_norm();

    return q;
}

std::string LongNumber::multiplyByTwo(const std::string &s) const
{
    std::string result = "";
    int carry = 0;
    for (int i = (int)s.size() - 1; i >= 0; --i)
    {
        int digit = s[i] - '0';
        int value = digit * 2 + carry;
        carry = value / 10;
        result.push_back(char(value % 10 + '0'));
    }

    result.push_back(char('0' + carry));

    std::reverse(result.begin(), result.end());
    return result;
}

std::string LongNumber::divideByTwo(const std::string &s) const
{
    std::string result = "";
    int carry = 0;
    for (char c : s)
    {
        int digit = c - '0';
        int value = carry * 10 + digit;
        carry = value % 2;
        result.push_back(char('0' + value / 2));
    }
    while (carry != 0)
    {
        carry *= 10;
        result.push_back(char('0' + carry / 2));
        carry %= 2;
    }
    return result;
}

std::string LongNumber::addTwoStrings(const std::string &num1, const std::string &num2, int type) const
{
    int len = std::max(num1.size(), num2.size());
    std::string a(num1), b(num2);
    if (type == 0)
    {
        a.insert(a.begin(), len - num1.size(), '0');
        b.insert(b.begin(), len - num2.size(), '0');
    }
    else
    {
        a.insert(a.end(), len - num1.size(), '0');
        b.insert(b.end(), len - num2.size(), '0');
    }
    std::string result = "";
    int carry = 0;
    for (int i = len - 1; i >= 0; --i)
    {
        int digit1 = (i < a.size() ? a[i] : '0') - '0';
        int digit2 = (i < b.size() ? b[i] : '0') - '0';
        int value = digit1 + digit2 + carry;
        carry = value / 10;
        value %= 10;
        result.push_back(char('0' + value));
    }
    if (type == 0)
        result.push_back(char('0' + carry));
    std::reverse(result.begin(), result.end());
    return result;
}

std::string LongNumber::getValue() const
{
    std::string int_part = "", frac_part = "";
    std::string t = "1";

    for (int i = point_position - 1; i >= 0; --i)
    {
        if (array[i])
            int_part = addTwoStrings(int_part, t, 0);

        if (i > 0)
            t = multiplyByTwo(t);

        while (t[0] == '0')
            t.erase(t.begin());

        while (int_part[0] == '0')
            int_part.erase(int_part.begin());
    }

    t = "5";
    for (int i = point_position; i < (int)array.size(); ++i)
    {
        if (array[i])
            frac_part = addTwoStrings(frac_part, t, 1);

        if (i < array.size() - 1)
            t = divideByTwo(t);

        while (t.back() == '0')
            t.pop_back();

        while (frac_part.back() == '0')
            frac_part.pop_back();
    }

    while (int_part[0] == '0')
        int_part.erase(int_part.begin());

    while (frac_part.back() == '0')
        frac_part.pop_back();

    if (int_part.empty())
        int_part = "0";
    if (frac_part.empty())
        frac_part = "0";
    if(sign == true)
    int_part = "-" + int_part;

    return int_part + "." + frac_part;
}

std::ostream &operator<<(std::ostream &out, const LongNumber &number)
{
    out << number.getValue();
    return out;
}
