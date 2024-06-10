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

#include <fstream>
#include <string>
#include <vector>
#include <map>

using std::cout;
using std::string;
using std::vector;
using std::map;

int
main() {
  try {

    fmt::print( "get_host_name            = {}\n", Utils::get_host_name() );
    fmt::print( "get_user_name            = {}\n", Utils::get_user_name() );
    fmt::print( "get_home_directory       = {}\n", Utils::get_home_directory() );
    fmt::print( "get_executable_path_name = {}\n", Utils::get_executable_path_name() );

    map<string,string> mac_addr;
    Utils::get_MAC_address( mac_addr );
    for ( auto & s: mac_addr )
      fmt::print( "get_MAC_address {} -> {}\n", s.first, s.second );

    vector<string> addr;
    Utils::get_IP_address( addr );
    for ( auto & s: addr )
      fmt::print( "get_IP_address: {}\n", s );

    // WRITE
    char const *files[] = {
      "/c/Users",
      "C:/Users",
      "/c/Windows",
      "C:/Windows/win.ini",
      "/c/Windows/win.ini",
      "./src_tests",
      "./src_tests/test_System.cc",
      "C:/tools/msys64/usr/bin",
      "C:/tools/msys64/usr/bin/dir.exe",
      nullptr
    };

    for ( char const ** pp = files; *pp != nullptr; ++pp ) {
      fmt::print(
        "dir/file: {:5}/{:5}: \"{}\"\n",
        Utils::check_if_dir_exists(*pp),
        Utils::check_if_file_exists(*pp),
        *pp
      );
    }

  } catch ( std::exception const & exc ) {
    cout << "Error: " << exc.what() << '\n';
  } catch ( ... ) {
    cout << "Unknown error\n";
  }
  cout << "All done folks\n\n";
  return 0;
}
