#include <iostream>
#include "Utils.hh"

using std::cout;

int
main() {
  Utils::Table::Style style;
  Utils::Table::Table table;
  cout << "Starting table test ...\n\n";
  try {

    std::vector<std::vector<std::string>> rows{
      {"Karl Kangaroo", "13. Sep 1988", "jumping"},
      {"Austin Ape", "24. Jul 2000", "climbing, jumping,\nsurfing"},
      {"..."},
      {"Bertha Bear", "3. Feb 1976", "sleeping", "Sherwood Forest"},
      {"Toni Tiger", "31. Jan 1935", "sleeping, hunting"},
      {"Paul Penguin", "6. Oct 1954"},
      {"Gira Giraffe", "10. Sep 1943", "", "London Zoo"},
      {"To be continued ..."}
    };

    style.padding_left(3);
    style.padding_right(2);

    table.setup( style, rows );
    table.title("A simple Test-Table");
    table.headings({"Name", "Birthday", "Tags", "Adress"});
    table.align_column(1, Utils::Table::Alignment::RIGHT);
    table.align_column(2, Utils::Table::Alignment::CENTER);
    table[2][0].col_span(4);
    table[7][0].col_span(3);
    table.align_column(5-1, Utils::Table::Alignment::RIGHT);

  } catch ( std::exception const & e ) {
    std::cerr << e.what() << std::endl;
  }
  cout << table;
  cout << "\nStopping table test ...\n";
  cout << "\nAll Done Folks!\n";
  return 0;
}
