/****************************************************************************\
  Copyright (c) Enrico Bertolazzi 2019
  All Rights Reserved.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the file license.txt for more details.
\****************************************************************************/

#include <iostream>

namespace Utils {

  class TestClass {
    double m_a;
    double m_b;
    double m_c;
  public:
    explicit TestClass() : m_a(0), m_b(0), m_c(0) {}
    ~TestClass() = default;

    void set_a( double a );
    void set_b( double b );
    void set_c( double c );

    double a() const { return m_a; }
    double b() const { return m_b; }
    double c() const { return m_c; }

    void info( std::ostream & ) const;
  };
}
