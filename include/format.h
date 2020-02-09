#ifndef FORMAT_H
#define FORMAT_H

#include <string>

namespace Format {
std::string ElapsedTime(long times);  // TODO: See src/format.cpp
std::string TimeFormatter(int time);  // since we have to keep formatting of minutes and seconds
};                                    // namespace Format

#endif