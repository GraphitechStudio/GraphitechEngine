/*
** EPITECH PROJECT, 2025
** G_CPP_500_LYN_5_2_rtype_7
** File description:
** Debug.cpp
*/
#include <Debugger/Debug.hpp>
#include <Time/TimeUtils.hpp>

void Graphitech::Debug::Enable()
{
    if (!_enabled) {
        _enableStartTime = TimeUtils::GetCurrentTimeAndData();

        for (auto &c: _enableStartTime)
            if (c == ':')
                c = '_';
        _enabled = true;
    }
}

void Graphitech::Debug::Disable() noexcept { _enabled = false; }

void Graphitech::Debug::Info(const std::string &msg, const std::string &mod)
{
    if (_enabled) {
        std::string t = "[INFO-" + TimeUtils::GetCurrentTimeAndData() + "-" + mod + "] " + msg;
        WriteText(std::move(t));
    }
}

void Graphitech::Debug::Warn(const std::string &msg, const std::string &mod)
{
    if (_enabled) {
        std::string t = "[WARN-" + TimeUtils::GetCurrentTimeAndData() + "-" + mod + "] " + msg;
        WriteText(std::move(t));
    }
}

void Graphitech::Debug::Crit(const std::string &msg, const std::string &mod)
{
    if (_enabled) {
        std::string t = "[CRIT-" + TimeUtils::GetCurrentTimeAndData() + "-" + mod + "] " + msg;
        WriteText(std::move(t));
    }
}

void Graphitech::Debug::Todo(const std::string &msg, const std::string &mod)
{
    if (_enabled) {
        std::string t = "[TODO-" + TimeUtils::GetCurrentTimeAndData() + "-" + mod + "] " + msg;
        WriteText(std::move(t));
    }
}

void Graphitech::Debug::Log(const std::string &msg)
{
    if (_enabled) {
        std::string text = msg;
        WriteText(std::move(text));
    }
}

bool Graphitech::Debug::IsEnabled() noexcept { return _enabled; }

void Graphitech::Debug::WriteText(const std::string &text)
{
    if (!_disableWriteToConsole)
        std::cout << text << std::endl;
    if (!_disableWriteToFile) {
        // Ensure Logs directory exists and build a safe file path
        std::filesystem::path logDir("Logs");
        if (!std::filesystem::is_directory(logDir))
            std::filesystem::create_directory(logDir);

        std::string fileName = "Log_" + _enableStartTime + ".log";
        // Replace spaces with underscores to avoid awkward filenames
        for (auto &c: fileName)
            if (c == ' ')
                c = '_';

        std::filesystem::path filePath = logDir / fileName;

        std::ofstream f(filePath.string(), std::ios::app);
        if (f.is_open()) {
            f << text << std::endl;
        }
    }
}
