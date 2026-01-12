/*
** EPITECH PROJECT, 2025
** G_CPP_500_LYN_5_2_rtype_7
** File description:
** TimeUtils.cpp
*/
#include <Time/TimeUtils.hpp>
std::string Graphitech::TimeUtils::GetCurrentTimeAndData()
{
    time_t t0 = time(nullptr);
    char str[26];
    ctime_r(&t0, str);
    std::string res = std::string(str);
    res.pop_back();
    return res;
}
