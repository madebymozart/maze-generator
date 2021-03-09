/// Copyright (c) 2017 Mozart Louis
/// This code is licensed under MIT license (see LICENSE for details)

#include "generator.hxx"

Generator::Generator(int dimensions)
    : gen_(rd_()),
      matrix_(dimensions),
      dimensions_(dimensions) {

  /// Number can't be even so this will automatically be incremented
  if (matrix_ % 2 == 0)matrix_++;

  /// Cut the cut the matrix in half to allocate the right amount of memory for
  /// the stack
  matrix_ = (matrix_ - 1) / 2;
  initializeStack();

  /// Initialize the neighboring matrix
  initializeNMatrix();
}

Generator::~Generator() {
  delete[] stack_;
  delete[] n_matrix_;
}

int **Generator::initializeMatrix() {
  /// Creating empty maze
  auto empty_maze = new int *[dimensions_];
  for (auto i = 0; i < dimensions_; i++)
    empty_maze[i] = new int[dimensions_];

  return empty_maze;
}

void Generator::initializeNMatrix() {
  n_matrix_ = new int *[4];
  for (auto i = 0; i < 4; i++) n_matrix_[i] = new int[3];

  /// Set initial for matrix
  /// Unit vectors for left, up, right and down
  n_matrix_[0][0] = -1;
  n_matrix_[0][1] = 0;
  n_matrix_[1][0] = 0;
  n_matrix_[1][1] = 1;
  n_matrix_[2][0] = 1;
  n_matrix_[2][1] = 0;
  n_matrix_[3][0] = 0;
  n_matrix_[3][1] = -1;
}

void Generator::initializeStack() {
  const int s = matrix_ * matrix_ * 100;
  stack_ = new int *[s];
  for (auto i = 0; i < s; i++) stack_[i] = new int[2];

  /// Add 2 to the matrix to assure we don't run our of bounds
  matrix_ += 2;
}

void Generator::push(int row, int col) {
  /// Stack is a stack of tuples which represent x, y coordinates which are on
  if (stack_index_ >= 0) {
    stack_[stack_index_][0] = row;
    stack_[stack_index_][1] = col;
  }
  stack_index_++;
}

void Generator::pop(int *row, int *col) {
  stack_index_--;
  /// See comment for push
  if (stack_index_ >= 0) {
    *row = stack_[stack_index_][0];
    *col = stack_[stack_index_][1];
  }
}

int **Generator::generateMaze(int start_row, int start_col, const int gid_default) {
  /// Initialize empty maze and visited matrix
  auto generated_maze = initializeMatrix();
  generated_maze[start_row][start_col] = gid_default;
  auto visited = initializeMatrix();
  visited[start_row][start_col] = 1;
  push(start_row, start_col);

  /// Creating Temp copy of the initial values
  int t_row = start_row;
  int t_col = start_col;

  while (stack_index_ != 0) {
    int row, col, n_index = 0, k = 0, counter = 0;

    /// Pop references to the next values and use those to find our next steps
    pop(&row, &col);
    start_row = row;
    start_col = col;

    /// Update the cell to visited
    visited[row][col] = 1;

    while (k == 0 && counter < 4) {
      /// Create our random distribution list for a random number between 1 & 99999;
      std::uniform_int_distribution<> dis(1, 99999);
      n_index = dis(gen_) % (6342 - dis(gen_) % 2243);
      n_index %= 4;

      /// Update row and col to the nearest_matrix with the random index
      row = start_row + n_matrix_[n_index][0];
      col = start_col + n_matrix_[n_index][1];
      n_matrix_[n_index][2] = 1;

      /// Where the magic happens
      if (!(row < 0 || col < 0 || (row == 0 || col == 0 || row == matrix_ - 1
          || col == matrix_ - 1))) {
        if (visited[row][col] == 0) {
          if (start_row == row && start_col != col) {
            if (start_col > col) {
              generated_maze[t_row][t_col - 1] = gid_default;
              generated_maze[t_row][t_col - 2] = gid_default;
              t_col -= 2;
            } else {
              generated_maze[t_row][t_col + 1] = gid_default;
              generated_maze[t_row][t_col + 2] = gid_default;
              t_col += 2;
            }
          } else if (start_col == col && start_row != row) {
            if (start_row > row) {
              generated_maze[t_row - 1][t_col] = gid_default;
              generated_maze[t_row - 2][t_col] = gid_default;
              t_row -= 2;
            } else {
              generated_maze[t_row + 1][t_col] = gid_default;
              generated_maze[t_row + 2][t_col] = gid_default;
              t_row += 2;
            }
          }

          push(start_row, start_col);
          push(row, col);
          visited[row][col] = 1;
          k = 1;

        } else {
          counter = n_matrix_[0][2] + n_matrix_[1][2]
              + n_matrix_[2][2] + n_matrix_[3][2];
        }
      }
    }

    if (counter == 4) {
      pop(&row, &col);
      if (start_row == row && start_col != col) {
        if (start_col > col)t_col -= 2;
        else t_col += 2;
      } else if (start_col == col && start_row != row) {
        if (start_row > row) t_row -= 2;
        else t_row += 2;
      }
      push(row, col);
    }

    n_matrix_[0][2] = 0;
    n_matrix_[1][2] = 0;
    n_matrix_[2][2] = 0;
    n_matrix_[3][2] = 0;
  }

  /// Update the last index to gid_default
  generated_maze[dimensions_ - 2][dimensions_ - 2] = gid_default;

  /// Clean up
  delete[] visited;
  delete[] stack_;

  /// :) Returning perfect generated maze
  return generated_maze;
}
