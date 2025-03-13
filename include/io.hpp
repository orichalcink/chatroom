#ifndef NETWORKING_IO_HPP
#define NETWORKING_IO_HPP

#include <iostream>
#include <limits>

#if defined(_WIN32) || defined(__WIN64)

inline constexpr const char* colors[] = {""};
inline constexpr size_t color_count   = 1u;
inline constexpr const char* res      = "";

#else

inline constexpr size_t color_count   = 16u;
inline constexpr const char* res      = "\033[0m";
inline constexpr const char* colors[] = {
   "\033[38;2;255;128;255m", "\033[38;2;255;128;0m",   "\033[38;2;255;255;0m",
   "\033[38;2;128;255;0m",   "\033[38;2;0;255;0m",     "\033[38;2;0;255;255m",
   "\033[38;2;0;0;255m",     "\033[38;2;128;0;255m",   "\033[38;2;139;69;19m",
   "\033[38;2;84;57;44m",    "\033[38;2;0;0;0m",       "\033[38;2;64;64;64m",
   "\033[38;2;128;128;128m", "\033[38;2;192;192;192m", "\033[38;2;255;255;255m"
};

#endif

inline void clear_line(int lines = 1)
{
   for (int i = 0; i <= lines; ++i)
   {
      std::cout << "\r\033[K";
      if (i < lines)
         std::cout << "\033[F";
   }
   std::cout << std::flush;
}

inline int get_integer_input(const std::string& prompt)
{
   int number;

   while (true)
   {
      std::cout << prompt;
      std::cin >> number;

      if (!std::cin.fail())
      {
         std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
         return number;
      }

      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      clear_line();
      std::cout << "Invalid input. Please enter a valid integer.\n";
   }
} 

inline std::string get_valid_username(const std::string& prompt)
{
   std::cout << prompt;

   std::string username;
   std::getline(std::cin >> std::ws, username);

   while (username.size() > 24 || username.size() < 3)
   {
      clear_line();
      std::cout << "Invalid username length, try again: ";
      std::getline(std::cin >> std::ws, username);
   }
   return username;
}

#endif // NETWORKING_IO_HPP
