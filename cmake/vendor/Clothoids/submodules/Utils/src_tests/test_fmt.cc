/*--------------------------------------------------------------------------*\
 |                                                                          |
 |  Copyright (C) 2017                                                      |
 |                                                                          |
 |         , __                 , __                                        |
 |        /|/  \               /|/  \                                       |
 |         | __/ _   ,_         | __/ _   ,_                                |
 |         |   \|/  /  |  |   | |   \|/  /  |  |   |                        |
 |         |(__/|__/   |_/ \_/|/|(__/|__/   |_/ \_/|/                       |
 |                           /|                   /|                        |
 |                           \|                   \|                        |
 |                                                                          |
 |      Enrico Bertolazzi                                                   |
 |      Dipartimento di Ingegneria Industriale                              |
 |      Universita` degli Studi di Trento                                   |
 |      email: enrico.bertolazzi@unitn.it                                   |
 |                                                                          |
\*--------------------------------------------------------------------------*/

#include "Utils.hh"
#include <iostream>
#include <chrono>
#include <string>

using namespace std;
using integer   = int;
using real_type = double;

int
main() {
  fmt::print("Don't {}\n\n", "panic");
  fmt::print("I'd rather be {1} than {0}.\n\n", "right", "happy");
  fmt::print("Hello, {name}! The answer is {number}. Goodbye, {name}.\n\n",
             fmt::arg("name", "World"), fmt::arg("number", 42));
  using namespace fmt::literals;
  fmt::print("Hello, {name}! The answer is {number}. Goodbye, {name}.\n\n",
             "name"_a="World", "number"_a=42);

  std::string message = fmt::format("The answer is {}.", 42);
  std::cout << message << "\n\n";

  //  run time error
  // std::string s = fmt::format(FMT_STRING("{:d}"), "foo");
  std::time_t t = std::time(nullptr);

  // Prints "The date is 2020-11-07." (with the current date):
  fmt::print("The date is {:%Y-%m-%d}.", fmt::localtime(t));

  #if 0
  // C++14
  using namespace std::literals::chrono_literals;

  // Prints "Default format: 42s 100ms":
  fmt::print("Default format: {} {}\n", 42s, 100ms);

  // Prints "strftime-like format: 03:15:30":
  fmt::print("strftime-like format: {:%H:%M:%S}\n", 3h + 15min + 30s);
  #endif

  fmt::print(fmt::emphasis::bold | fg(fmt::color::red),
             "Elapsed time: {0:.2f} seconds\n\n", 1.23);

  fmt::print("Elapsed time: {0:.2f} seconds\n\n",
             fmt::styled(1.23, fmt::fg(fmt::color::green) |
                               fmt::bg(fmt::color::blue)));

  fmt::printf("Elapsed time: %.2f seconds\n\n", 1.23);

  fmt::print("\n\nAll done!\n");
  return 0;
}
