#ifndef __numcpp_constants_h
#define __numcpp_constants_h

#include <algorithm>
#include <numeric>
#include <cmath>
namespace NUMCPP {

	template<typename T>
	struct CONSTANTS {

	public:

		static T zero;

        static T one;

        static T two;
        
        static T half;

        static T radix; 

        static T safe_min; 

        static T safe_max;

        static T huge;

        static T tiny;

        static T tsml, tbig, ssml, sbig;

    };


    template <>
    double CONSTANTS<double>::zero = 0;

    template <>
    double CONSTANTS<double>::one=1;

    template <>
    double CONSTANTS<double>::two = 2;

    template <>
    inline double CONSTANTS<double>::half = 0.5;

    template <typename T>
    T CONSTANTS<T>::radix=std::numeric_limits<T>::radix;
    
    template <typename T>
    T CONSTANTS<T>::safe_min= std::pow<T>(radix, std::min(std::numeric_limits<T>::min_exponent - 1,
            1 - std::numeric_limits<T>::max_exponent));

    template <typename T>
    T CONSTANTS<T>::safe_max=std::pow<T>(radix, std::max(std::numeric_limits<T>::max_exponent - 1,
            1 - std::numeric_limits<T>::min_exponent));
    
    template <typename T>
    T CONSTANTS<T>::tiny=std::numeric_limits<T>::min();

    template <typename T>
    T CONSTANTS<T>::huge= std::numeric_limits<T>::max();

    template <>
    double CONSTANTS<double>::tsml = std::pow<double>(radix, std::ceil(
        std::numeric_limits<double>::min_exponent - 1) * 0.5);
    template <>
    double CONSTANTS<double>::tbig = std::pow<double>(radix, std::floor(
        std::numeric_limits<double>::max_exponent - std::numeric_limits < double>::digits + 1) * 0.5);
    template <>
    double CONSTANTS<double>::ssml = std::pow<double>(radix, -std::floor(
        std::numeric_limits<double>::min_exponent - std::numeric_limits < double>::digits) * 0.5);
    template <>
    double CONSTANTS<double>::sbig = std::pow<double>(radix, -std::ceil(
        std::numeric_limits<double>::max_exponent + std::numeric_limits < double>::digits - 1) * 0.5);


}

#endif
