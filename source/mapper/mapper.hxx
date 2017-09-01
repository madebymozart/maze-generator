/**
 * Copyright (c) 2017 Mozart Louis
 * This code is licensed under MIT license (see LICENSE.txt for details)
 */

#ifndef __MAPPER_HXX__
#define __MAPPER_HXX__

/**
 * JSON Related Keys
 */
#define TMX_NAME "tmx_name"
#define TMX_LAYER "tmx_layer"
#define TMX_DIMENSIONS "tmx_dimensions"
#define TMX_DIMENSIONS_INCREMENT "tmx_dimensions_increment"
#define TMX_DIMENSIONS_REPEAT "tmx_dimensions_repeat"
#define TMX_AMOUNT "tmx_amount"
#define TMX_TILE_SET "tmx_tile_set"
#define TMX_TILE_SET_NAME "tmx_tile_set_name"
#define TMX_TILE_WIDTH "tmx_tile_width"
#define TMX_TILE_HEIGHT "tmx_tile_height"
#define TMX_GID_DEFAULT "tmx_gid_default"
#define TMX_GID_MAPPER "tmx_gid_mapper"
#define TMX_GID_BOTTOM_LEFT "gid_bottom_left"
#define TMX_GID_BOTTOM_RIGHT "gid_bottom_right"
#define TMX_GID_CROSS "gid_cross"
#define TMX_GID_END_DOWN "gid_end_down"
#define TMX_GID_END_LEFT "gid_end_left"
#define TMX_GID_END_RIGHT "gid_end_right"
#define TMX_GID_END_UP "gid_end_up"
#define TMX_GID_HORIZONTAL "gid_horizontal"
#define TMX_GID_HORIZONTAL_DOWN "gid_horizontal_down"
#define TMX_GID_HORIZONTAL_UP "gid_horizontal_up"
#define TMX_GID_VERTICAL "gid_vertical"
#define TMX_GID_VERTICAL_LEFT "gid_vertical_left"
#define TMX_GID_VERTICAL_RIGHT "gid_vertical_right"
#define TMX_GID_UP_LEFT "gid_up_left"
#define TMX_GID_UP_RIGHT "gid_up_right"

/**
 * Matrix 3x3 Names
 */
#define TOP_LEFT "top_left"
#define TOP_CENTER "top_center"
#define TOP_RIGHT "top_right"
#define CENTER_LEFT "center_left"
#define CENTER "center"
#define CENTER_RIGHT "center_right"
#define BOTTOM_LEFT "bottom_left"
#define BOTTOM_CENTER "bottom_center"
#define BOTTOM_RIGHT "bottom_right"

#include <iostream>
#include <string>
#include <unordered_set>

#include "../globals.hxx"
#include "../json/json.hxx"
#include "../generator/generator.hxx"

class Mapper {
 public:
  /**
   * Constructor
   *
   * @param config JSON config file
   */
  explicit Mapper(const char* config);

  /**
   * Destructor
   */
  ~Mapper();

  /**
   * Using the json config file, this function will generate mazes and map them to
   * GIDs depending on your config.
   */
  std::vector<int> execute(std::string output);

 private:
  /**
   * This maps the current map to a GID based tile set. This is particularly
   * useful for mazes that have special designs.
   *
   * @param maze Reference to the maze
   * @param dimensions The dimensions of the map
   */
  void map(matrix maze, const int dimensions);

  /**
   * Direction Checkers. Ordered in priority
   */
  bool isCross(std::unordered_set<std::string>& lookup);
  bool isHorizontalDown(std::unordered_set<std::string>& lookup);
  bool isHorizontalUp(std::unordered_set<std::string>& lookup);
  bool isVerticalLeft(std::unordered_set<std::string>& lookup);
  bool isVerticalRight(std::unordered_set<std::string>& lookup);
  bool isBottomLeft(std::unordered_set<std::string>& lookup);
  bool isBottomRight(std::unordered_set<std::string>& lookup);
  bool isUpLeft(std::unordered_set<std::string>& lookup);
  bool isUpRight(std::unordered_set<std::string>& lookup);
  bool isHorizontal(std::unordered_set<std::string>& lookup);
  bool isVertical(std::unordered_set<std::string>& lookup);
  bool isEndDown(std::unordered_set<std::string>& lookup);
  bool isEndLeft(std::unordered_set<std::string>& lookup);
  bool isEndRight(std::unordered_set<std::string>& lookup);
  bool isEndUp(std::unordered_set<std::string>& lookup);

  /**
   * Returns correct gid from JSON
   */
  int getGIDFromJson(std::string gid);

  /**
   * Saves the generated maze to a tmx file that can be opened oin Tiled Map Editor
   *
   * @param maze       Reference to the maze
   * @param name       Name of the tmx file
   * @param dimensions The dimensions of the map
   * @param amount     The amount of mazes to produce
   * @param dir     The output directory ti save the mazes in
   */
  void save(matrix& maze, const std::string& name, const int& dimensions,
            const int& amount, const std::string& dir);

  /**
   * Generate header for tmx file
   */
  std::string generateTMXHeader(const int& dimensions);

  /**
   * Generate tail for tmx file
   */
  std::string generateTMXTail();

  /// Json parser using nlohmann
  nlohmann::json j_;
};

inline bool Mapper::isCross(std::unordered_set<std::string>& lookup) {
  return lookup.find(BOTTOM_CENTER) != lookup.end() &&
         lookup.find(CENTER_LEFT) != lookup.end() &&
         lookup.find(CENTER_RIGHT) != lookup.end() &&
         lookup.find(TOP_CENTER) != lookup.end();
}

inline bool Mapper::isHorizontalDown(std::unordered_set<std::string>& lookup) {
  return lookup.find(BOTTOM_CENTER) != lookup.end() &&
         lookup.find(CENTER_LEFT) != lookup.end() &&
         lookup.find(CENTER_RIGHT) != lookup.end();
}

inline bool Mapper::isHorizontalUp(std::unordered_set<std::string>& lookup) {
  return lookup.find(CENTER_LEFT) != lookup.end() &&
         lookup.find(CENTER_RIGHT) != lookup.end() &&
         lookup.find(TOP_CENTER) != lookup.end();
}

inline bool Mapper::isVerticalLeft(std::unordered_set<std::string>& lookup) {
  return lookup.find(BOTTOM_CENTER) != lookup.end() &&
         lookup.find(CENTER_LEFT) != lookup.end() &&
         lookup.find(TOP_CENTER) != lookup.end();
}

inline bool Mapper::isVerticalRight(std::unordered_set<std::string>& lookup) {
  return lookup.find(BOTTOM_CENTER) != lookup.end() &&
         lookup.find(CENTER_RIGHT) != lookup.end() &&
         lookup.find(TOP_CENTER) != lookup.end();
}

inline bool Mapper::isBottomLeft(std::unordered_set<std::string>& lookup) {
  return lookup.find(BOTTOM_CENTER) != lookup.end() &&
         lookup.find(CENTER_LEFT) != lookup.end();
}

inline bool Mapper::isBottomRight(std::unordered_set<std::string>& lookup) {
  return lookup.find(BOTTOM_CENTER) != lookup.end() &&
         lookup.find(CENTER_RIGHT) != lookup.end();
}

inline bool Mapper::isUpLeft(std::unordered_set<std::string>& lookup) {
  return lookup.find(CENTER_LEFT) != lookup.end() &&
         lookup.find(TOP_CENTER) != lookup.end();
}

inline bool Mapper::isUpRight(std::unordered_set<std::string>& lookup) {
  return lookup.find(CENTER_RIGHT) != lookup.end() &&
         lookup.find(TOP_CENTER) != lookup.end();
}

inline bool Mapper::isHorizontal(std::unordered_set<std::string>& lookup) {
  return lookup.find(CENTER_RIGHT) != lookup.end() &&
         lookup.find(CENTER_LEFT) != lookup.end();
}

inline bool Mapper::isVertical(std::unordered_set<std::string>& lookup) {
  return lookup.find(BOTTOM_CENTER) != lookup.end() &&
         lookup.find(TOP_CENTER) != lookup.end();
}

inline bool Mapper::isEndDown(std::unordered_set<std::string>& lookup) {
  return lookup.find(TOP_CENTER) != lookup.end();
}

inline bool Mapper::isEndLeft(std::unordered_set<std::string>& lookup) {
  return lookup.find(CENTER_LEFT) != lookup.end();
}

inline bool Mapper::isEndRight(std::unordered_set<std::string>& lookup) {
  return lookup.find(CENTER_RIGHT) != lookup.end();
}

inline bool Mapper::isEndUp(std::unordered_set<std::string>& lookup) {
  return lookup.find(BOTTOM_CENTER) != lookup.end();
}

inline int Mapper::getGIDFromJson(std::string gid) {
  const int id = j_[TMX_GID_MAPPER][gid];
  return id;
}

#endif /// __MAPPER_HXX__