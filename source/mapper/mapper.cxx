/// Copyright (c) 2017 Mozart Louis
/// This code is licensed under MIT license (see LICENSE.txt for details)

#include "mapper.hxx"

#include <fstream>
#include <thread>
#include <list>

Mapper::Mapper(const char* config) {
  /// open the json file
  std::ifstream file(config);

  /// As long as the file isn't bad and open, parse the json
  try {
    if (!file.bad() && file.is_open()) file >> j_;
    else throw std::exception();
  } catch (const std::exception&) {
    std::cout << std::endl << "JSON Config \"" << config << "\" either does not "
        "exist, is malformed or corrupt :(" << std::endl;
  }

  /// Close file
  file.close();
}

Mapper::~Mapper() = default;

std::vector<int> Mapper::execute(std::string output) {
  /// Output
  std::system(("mkdir " + output).c_str());

  /// List of thread instances
  std::vector<int> threads;

  /// info needed from the json config file
  const std::string name = j_.at(TMX_NAME);

  const int amount = j_.at(TMX_AMOUNT);
  const int gid_default = j_.at(TMX_GID_DEFAULT);
  const int dimensions_increment = j_.at(TMX_DIMENSIONS_INCREMENT);
  int dimensions = j_.at(TMX_DIMENSIONS);
  // int dimensions_repeat = j_.at(TMX_DIMENSIONS_REPEAT);

  /// running a loop to generate x amount of mazes
  for (auto i = 1; i <= amount; i++) {
    threads.push_back(amount);
    std::function<void()> func = [=, &threads /* &dimensions_repeat */]() {
      /// Create a generator instance
      const auto f_dims = dimensions + ((i - 1) * dimensions_increment);

      auto generator = new Generator(f_dims);
      auto maze = generator->generateMaze(1, 1, gid_default);

      /// Map the file if the user has defined the gids
      if (j_.find(TMX_GID_MAPPER) != j_.end()) map(maze, f_dims);

      /// Save the tmx file
      save(maze, name, f_dims, i, output);

      /// Clean up
      delete[] maze;

      /// Pop the amount id out of the vector
      threads.pop_back();
    };

    std::thread t1([=]() { if (func != nullptr) func(); });
    t1.detach();
  }

  return threads;
}

void Mapper::map(matrix maze, const int dimensions) {
  for (int ii = 0; ii < dimensions; ii++) {
    for (int jj = 0; jj < dimensions; jj++) {
      /// If the cell in the matrix is empty continue to the next cell
      if (maze[ii][jj] == 0) continue;

      /// Creating an unordered map for the lookup we will do to verify if the
      /// pattern should be applied
      std::unordered_set<std::string> x3lookup;

      /// Create the list of names for each temp direction
      std::list<std::string> x3m = {TOP_LEFT, TOP_CENTER, TOP_RIGHT,
                                    CENTER_LEFT, CENTER, CENTER_RIGHT,
                                    BOTTOM_LEFT, BOTTOM_CENTER, BOTTOM_RIGHT};
      auto x3m_iter = x3m.begin();

      for (auto i = -1; i <= 1; i++)
        for (auto j = -1; j <= 1; j++) {
          if (maze[ii + i][jj + j] != 0)
            x3lookup.emplace(*x3m_iter);

          /// Move iterator to the next sport
          ++x3m_iter;
        }

      /// Compare
      if (isCross(x3lookup))
        maze[ii][jj] = getGIDFromJson(TMX_GID_CROSS);
      else if (isHorizontalDown(x3lookup))
        maze[ii][jj] = getGIDFromJson(TMX_GID_HORIZONTAL_DOWN);
      else if (isHorizontalUp(x3lookup))
        maze[ii][jj] = getGIDFromJson(TMX_GID_HORIZONTAL_UP);
      else if (isVerticalLeft(x3lookup))
        maze[ii][jj] = getGIDFromJson(TMX_GID_VERTICAL_LEFT);
      else if (isVerticalRight(x3lookup))
        maze[ii][jj] = getGIDFromJson(TMX_GID_VERTICAL_RIGHT);
      else if (isBottomLeft(x3lookup))
        maze[ii][jj] = getGIDFromJson(TMX_GID_BOTTOM_LEFT);
      else if (isBottomRight(x3lookup))
        maze[ii][jj] = getGIDFromJson(TMX_GID_BOTTOM_RIGHT);
      else if (isUpLeft(x3lookup))
        maze[ii][jj] = getGIDFromJson(TMX_GID_UP_LEFT);
      else if (isUpRight(x3lookup))
        maze[ii][jj] = getGIDFromJson(TMX_GID_UP_RIGHT);
      else if (isHorizontal(x3lookup))
        maze[ii][jj] = getGIDFromJson(TMX_GID_HORIZONTAL);
      else if (isVertical(x3lookup))
        maze[ii][jj] = getGIDFromJson(TMX_GID_VERTICAL);
      else if (isEndDown(x3lookup))
        maze[ii][jj] = getGIDFromJson(TMX_GID_END_DOWN);
      else if (isEndLeft(x3lookup))
        maze[ii][jj] = getGIDFromJson(TMX_GID_END_LEFT);
      else if (isEndRight(x3lookup))
        maze[ii][jj] = getGIDFromJson(TMX_GID_END_RIGHT);
      else if (isEndUp(x3lookup))
        maze[ii][jj] = getGIDFromJson(TMX_GID_END_UP);
    }
  }
}

void Mapper::save(matrix& maze, const std::string& name, const int& dimensions,
                  const int& amount, const std::string& dir) {
  std::ofstream output;
  std::stringstream ss;
  std::string data;

  /// Loop through the generated maze to create the string data in CSV format
  for (int ii = 0; ii < dimensions; ii++)
    for (int jj = 0; jj < dimensions; jj++)
      ss << std::to_string(maze[ii][jj]) << ",";

  /// Stream into our data string. We need to pop the last char in the string
  /// because it will always be a leftover comma.
  ss >> data;
  data.pop_back();

  /// creating our final tmx map data and saving to the system
  const std::string tmx = generateTMXHeader(dimensions) + data + generateTMXTail();

  /// Create the files
  output.open(dir + "/" + name + "_" + std::to_string(amount) + ".tmx");
  if (output.is_open() && !output.bad()) output << tmx;
  output.close();
}

std::string Mapper::generateTMXHeader(const int& dimensions) {
  const std::string layer = j_[TMX_LAYER];
  const std::string tile_set = j_[TMX_TILE_SET];
  const std::string tile_set_name = j_[TMX_TILE_SET_NAME];
  const std::string dimensions_str = std::to_string(dimensions);
  const std::string tile_width_str = std::to_string(int(j_[TMX_TILE_WIDTH]));
  const std::string tile_height_str = std::to_string(int(j_[TMX_TILE_HEIGHT]));

  return "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
             "<map version=\"1.0\" tiledversion=\"1.0.2\" orientation=\"orthogonal\" "
             "renderorder=\"right-down\" width=\"" + dimensions_str
         + "\" height=\""
         + dimensions_str + "\" "
             "tilewidth=\"" + tile_width_str
         + "\" tileheight=\"" + tile_height_str + "\" nextobjectid=\"1\">\n"
             " <tileset firstgid=\"1\" name=\"" + tile_set_name + "\" "
             " tilewidth=\"" + tile_width_str
         + "\" tileheight=\"" + tile_height_str + "\" tilecount=\"35\" "
             "columns=\"5\">\n"
             "  <image source=\"" + tile_set + "\" width=\"540\" "
             "height=\"756\"/>\n"
             " </tileset>\n"
             " <layer name=\"" + layer + "\" width=\"" + dimensions_str
         +
         "\" height=\""
         + dimensions_str + "\">\n"
             "  <data encoding=\"csv\">";
}

std::string Mapper::generateTMXTail() { return "</data>\n</layer>\n</map>"; }
