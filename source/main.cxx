/// Copyright (c) 2017 Mozart Louis
/// This code is licensed under MIT license (see LICENSE.txt for details)

#include "cmd/cmd.hxx"
#include "mapper/mapper.hxx"

void configure(cli::Parser& parser) {
  parser.set_required<std::string>("f", "file", "JSON Config File");
  parser.set_required<std::string>("o", "output", "Output Directory");
}

int main(int argc, char** argv) {
  std::cout << std::endl
            << "###### TMX Maze Generator - A perfect maze generation tool for Tiled"
            << std::endl;
  std::cout << "###### Copyright (c) 2017 Mozart Louis" << std::endl;

  /// Create command line parser to handle all the command things
  cli::Parser parser(argc, argv);
  configure(parser);

  /// Check to see if there are any errors with the user input
  parser.run_and_exit_if_error();


  /// Initialize mapper and execute
  const std::string file = parser.get<std::string>("f");
  std::cout << "###### Reading \"" << file << "\"..." << std::endl;
  auto mapper = new Mapper(file.c_str());

  /// Loop until all threads are finished
  std::cout << "###### Generating..." << std::endl;
  auto threads = mapper->execute(parser.get<std::string>("o"));
  while (!threads.empty()) {}

  /// Clean up program
  delete (mapper);

  std::cout << "###### Done!" << std::endl;

  /// Exit
  return 0;
}


