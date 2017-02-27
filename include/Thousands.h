#ifndef THOUSANDS_H
#define THOUSANDS_H

// Stolen from:
//
//  http://stackoverflow.com/questions/4728155/how-do-you-set-the-cout-locale-to-insert-commas-as-thousands-separators
//
// to allow me to automagically insert ',' (or '.' depending on the locale) into big numbers.
// I'll be using this on the report and debug files.
//
// Note.
// I'm not convinced that this requires the parameter or the do_thousands_sep()
// function as without it, I still get commas - which I think are from the locale.
// I might be wrong. It happens. But I'm leaving them out for now in the hope that
// "foreign" locales will get their chosen separator too.

#include <string>
#include <iomanip>

using std::string;
using std::numpunct;
using std::locale;



/** @brief A class that is 'imbued' onto the output streams to allow locale
 *         dependant thousands separators to be applied to all numbers
 *         output to that stream.
 */template<typename T> class ThousandsSeparator : public numpunct<T> {
public:
    ThousandsSeparator(T Separator) : m_Separator(Separator) {}

protected:
//    T do_thousands_sep() const  {
//        return m_Separator;
//    }

    virtual string do_grouping() const
    {
        return "\03";  // Ie, every three digits.
    }

private:
    T m_Separator;
};

#endif // THOUSANDS_H
