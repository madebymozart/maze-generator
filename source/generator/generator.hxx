/**
 * Copyright (c) 2017 Mozart Louis
 * This code is licensed under MIT license (see LICENSE.txt for details)
 */

#ifndef __GENERATOR_HXX__
#define __GENERATOR_HXX__

#include <iostream>
#include <random>

#include "../globals.hxx"

using matrix = int**;

class IGenerator {
  /**
   * Interface method for generating a maze
   */
   virtual matrix generateMaze(int /* s_row */, int /* s_col */, const int gid_default)=0;
}

class Generator : IGenerator{
 public:
  /**
   * Constructor
   */
  explicit Generator(int /* dimensions */);

  /**
   * Destructor
   */
  ~Generator();

  /**
   * Generates the maze using.
   */
  int** generateMaze(int /* s_row */, int /* s_col */, const int gid_default) override;
 private:
  /**
   * Initializes and empty matrix and just initializes it all with 0
   */
  int** initializeMatrix();

  /**
   * Initializes the neighbor matrix
   */
  void initializeNMatrix();

  /**
   * Initialize Stack
   */
  void initializeStack();

  /**
   * Push of the stack
   */
  void push(int /* row */, int /* col */);

  /**
   * Pops of the stack
   */
  void pop(int* /* row */, int* /* col */);

  /// Used to obtain a seed for the random number engine
  std::random_device rd_;

  ///Standard mersenne_twister_engine seeded with rd_()
  std::mt19937 gen_;

  /// The stack
  matrix stack_;

  /// The neighboring matrix
  matrix n_matrix_;

  /// the current stack index
  int stack_index_ = 0;

  /// The maze dimensions
  int matrix_, dimensions_ = 0;
};

#endif /// __GENERATOR_HXX__
