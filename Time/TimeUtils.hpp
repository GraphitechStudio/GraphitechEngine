/*
** EPITECH PROJECT, 2025
** G_CPP_500_LYN_5_2_rtype_7
** File description:
** TimeUtils.hpp
*/

#ifndef TIMEUTILS_HPP
#define TIMEUTILS_HPP

#include <Graphitech.hpp>

namespace Graphitech {

    /**
     * @brief Static utility class for time-related operations.
     *
     * @details TimeUtils provides static methods for retrieving
     * formatted time and date strings. This class cannot be
     * instantiated and all methods are static.
     *
     * @ingroup Time
     */
    class TimeUtils final
    {
        public:
            /**
             * @brief Retrieves the current time and date as a formatted string.
             *
             * @details Returns a string containing the current system time
             * and date in a human-readable format. The exact format may
             * vary depending on system locale settings.
             *
             * @return std::string A formatted string representing the current time and date.
             */
            static std::string GetCurrentTimeAndData();
    };
}


#endif //TIMEUTILS_HPP