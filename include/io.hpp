#ifndef NETWORKING_IO_HPP
#define NETWORKING_IO_HPP

#include <iostream>
#include <limits>

using namespace std::string_literals;

inline constexpr const char* pink    = "\033[38;2;255;128;255m";
inline constexpr const char* orange  = "\033[38;2;255;128;0m";
inline constexpr const char* yellow  = "\033[38;2;255;255;0m";
inline constexpr const char* l_green = "\033[38;2;128;255;0m";
inline constexpr const char* green   = "\033[38;2;0;255;0m";
inline constexpr const char* cyan    = "\033[38;2;0;255;255m";
inline constexpr const char* blue    = "\033[38;2;0;0;255m";
inline constexpr const char* purple  = "\033[38;2;128;0;255m";
inline constexpr const char* brown   = "\033[38;2;139;69;19m";
inline constexpr const char* d_brown = "\033[38;2;84;57;44m";
inline constexpr const char* black   = "\033[38;2;0;0;0m";
inline constexpr const char* d_gray  = "\033[38;2;64;64;64m";
inline constexpr const char* gray    = "\033[38;2;128;128;128m";
inline constexpr const char* l_gray  = "\033[38;2;192;192;192m";
inline constexpr const char* white   = "\033[38;2;255;255;255m";

// No colors on windows :(
#if defined(_WIN32) || defined(_WIN64)

inline constexpr const char* red      = "";
inline constexpr const char* res      = "";
inline constexpr size_t color_count   = 1u;
inline constexpr const char* colors[] = {red};

#else

inline constexpr const char* red      = "\033[38;2;255;0;0m";
inline constexpr const char* res      = "\033[0m";

inline constexpr size_t color_count = 16u;
inline constexpr const char* colors[] = {pink, red, orange, yellow, l_green,
   green, cyan, blue, purple, brown, d_brown, black, d_gray, gray, l_gray, white};

#endif

/// @brief Clear lines from terminal.
/// @param lines Amount of lines to clear.
inline void clearln(int lines = 1)
{
   for (int i = 0; i <= lines; ++i)
   {
      std::cout << "\r\033[K";
      if (i < lines)
         std::cout << "\033[F";
   }
   std::cout << std::flush;
}

/// @brief Get number input.
/// @param prompt Prompt to display.
/// @return Number.
inline int getInteger(const std::string& prompt)
{
   int number;

   while (true)
   {
      std::cout << prompt;
      std::cin >> number;

      if (std::cin.fail())
      {
         std::cin.clear();
         std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
         clearln();
         std::cout << red << "Invalid input. Please enter a valid integer.\n";
      }
      else
      {
         std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
         return number;
      } 
   }
} 

#endif // NETWORKING_IO_HPP
