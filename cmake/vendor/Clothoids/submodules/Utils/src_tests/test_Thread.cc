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

using std::cout;

class Counter {
  Utils::BinarySearch<int> bs;
public:
  Counter() {
    bool ok ;
    int * pdata = bs.search( std::this_thread::get_id(), ok );
    *pdata = 0;
  }

  void
  inc() {
    bool ok;
    int * pdata = bs.search( std::this_thread::get_id(), ok );
    if ( !ok ) std::cerr << "Counter::inc failed thread\n";
    ++(*pdata);
  }

  void
  print() {
    bool ok;
    int * pdata = bs.search( std::this_thread::get_id(), ok );
    if ( !ok ) std::cerr << "Counter::inc failed thread\n";
    fmt::print( "thread {}, counter = {}\n", std::this_thread::get_id(), *pdata );
  }
};

static
void
do_test() {
  Counter c;
  for ( int i = 0; i < 10000000; ++i ) {
    //Utils::sleep_for_milliseconds(1);
    c.inc();
  }
  c.print();
}

static
void
do_passa( int ii ) {
  cout << "passa ii=" << ii << '\n';
}

int
main() {
  std::vector<std::thread> threads_tab;
  for ( int i = 0; i < 100; ++i ) {
    //Utils::sleep_for_milliseconds(1);
    threads_tab.emplace_back(do_test);
  }
  for ( auto & t : threads_tab ) t.join();
  cout << "Test WorkerLoop\n\n";

  Utils::WorkerLoop wl;
  for ( int i = 0; i < 100; ++i ) {
    std::function<void()> exe = [i]() -> void { cout << "passing i=" << i << '\n'; };
    wl.exec( exe );
    wl.run( do_passa, i );
  }
  cout << "WorkerLoop done\n\n";
  wl.exec();
  wl.exec();
  wl.exec();
  wl.exec();
  wl.wait();
  cout << "All done folks!\n\n";
  return 0;
}
