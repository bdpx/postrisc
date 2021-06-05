#pragma once

#include <sstream> // for std::istringstream
#include <boost/algorithm/string/predicate.hpp> // for boost::algorithm::iequals

namespace postrisc {
namespace util {

// Custom translator for bool (only supports std::string)
struct BoolTranslator {
    typedef std::string internal_type;
    typedef bool        external_type;

    // Converts a string to bool
    boost::optional<external_type> get_value(const internal_type& str)
    {
        if (!str.empty()) {
            using boost::algorithm::iequals;

            if (iequals(str, "true") ||
                iequals(str, "yes") ||
                iequals(str, "enabled") ||
                str == "1")
                return boost::optional<external_type>(true);
            else
                return boost::optional<external_type>(false);
        } else {
            return boost::optional<external_type>(boost::none);
        }
    }

    // Converts a bool to string
    boost::optional<internal_type> put_value(const external_type& b)
    {
        return boost::optional<internal_type>(b ? "true" : "false");
    }
};

template<typename T> class hexadecimal {
public:
    hexadecimal(T value_) : value(value_) {}
    operator T () const { return value; }
private:
    T value;
};

// Custom translator for hex (only supports std::string)
template<typename T> struct HexadecimalTranslator {
    typedef std::string internal_type;
    typedef hexadecimal<T>  external_type;

    // Converts a string to bool
    boost::optional<external_type> get_value(const internal_type& str)
    {
        if (!str.empty()) {
            char c;
            std::istringstream is(str);
            T result;
            if ((is >> std::boolalpha >> std::hex >> result) && !(is >> c)) {
                return boost::optional<external_type>(result);
            } else {
                return boost::optional<external_type>(boost::none);
            }
        } else {
            return boost::optional<external_type>(boost::none);
        }
    }

    // Converts a bool to string
    boost::optional<internal_type> put_value(const external_type& b)
    {
        std::ostringstream os;
        os << "0x" << fmt::hex(b);
        return boost::optional<internal_type>(os.str());
    }
};

} // namespace util
} // namespace postrisc

/*  Specialize translator_between so that it uses our custom translator for
    bool value types. Specialization must be in boost::property_tree
    namespace. */
namespace boost {
namespace property_tree {

template<typename Ch, typename Traits, typename Alloc>
struct translator_between<std::basic_string< Ch, Traits, Alloc >, bool>
{
    typedef postrisc::util::BoolTranslator type;
};

template<typename Ch, typename Traits, typename Alloc, typename T>
struct translator_between<std::basic_string< Ch, Traits, Alloc >, postrisc::util::hexadecimal<T>>
{
    typedef postrisc::util::HexadecimalTranslator<T> type;
};

} // namespace property_tree
} // namespace boost
