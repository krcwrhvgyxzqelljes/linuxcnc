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

#include <string>
#include <iostream>

using namespace std;
using namespace rang;

#ifdef __clang__
[[clang::no_destroy]]
#endif
static Utils::Console C(&std::cout,4);

int
main() {

  C.message  ( "message\n" );
  C.semaphore( 0, "semphore 0\n" );
  C.semaphore( 1, "semphore 1\n" );
  C.semaphore( 2, "semphore 2\n" );
  C.colors   ( 0, "colors 0\n" );
  C.colors   ( 1, "colors 1\n" );
  C.colors   ( 2, "colors 2\n" );
  C.colors   ( 3, "colors 3\n" );
  C.colors   ( 4, "colors 4\n" );
  C.colors   ( 5, "colors 5\n" );
  C.colors   ( 6, "colors 6\n" );
  C.warning  ( "warning\n" );
  C.error    ( "error\n" );
  C.fatal    ( "fatal\n" );

  C.black   ( "black\n"   );
  C.red     ( "red\n"     );
  C.green   ( "green\n"   );
  C.yellow  ( "yellow\n"  );
  C.blue    ( "blue\n"    );
  C.magenta ( "magenta\n" );
  C.cyan    ( "cyan\n"    );
  C.gray    ( "gray\n"    );

  C.black_reversed   ( "black_reversed\n"   );
  C.red_reversed     ( "red_reversed\n"     );
  C.green_reversed   ( "green_reversed\n"   );
  C.yellow_reversed  ( "yellow_reversed\n"  );
  C.blue_reversed    ( "blue_reversed\n"    );
  C.magenta_reversed ( "magenta_reversed\n" );
  C.cyan_reversed    ( "cyan_reversed\n"    );
  C.gray_reversed    ( "gray_reversed\n"    );

  for ( int i = 0; i <= 100; ++i ) {
    cout << Utils::progress_bar( i/100.0, 70 ) << '\r' << std::flush;
    Utils::sleep_for_milliseconds(50);
  }

  std::cout << "\n\n";

  for ( int i = 0; i <= 100; ++i ) {
    Utils::progress_bar( std::cout, i/100.0, 70, "working" );
    Utils::sleep_for_milliseconds(50);
  }

  std::cout << "\n\n";

  for ( int i = 0; i <= 100; ++i ) {
    Utils::progress_bar2( std::cout, i/100.0, 70, "working" );
    Utils::sleep_for_milliseconds(50);
  }

  cout << "\n\nAll Done Folks!\n";

  return 0;
}
