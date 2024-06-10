/****************************************************************************\
  Copyright (c) Enrico Bertolazzi 2019
  All Rights Reserved.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the file license.txt for more details.
\****************************************************************************/

#include "TestClass.hh"
#include "Utils.hh"

#include <iostream>

namespace Utils {

  void
  TestClass::set_a(double a) {
    m_a = a;
  }

  void
  TestClass::set_b(double b) {
    m_b = b;
  }

  void
  TestClass::set_c(double c) {
    m_c = c;
  }

  void TestClass::info( ostream & stream ) const {
    fmt::print( stream, "a={:<10} b={:<10} c={:<10}\n", m_a, m_b, m_c );
  }
}
