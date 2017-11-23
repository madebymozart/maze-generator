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
  explicit Mapper(const char *config);

  /**
   * Destructor
   */
  ~Mapper();

  /**
   * Using the json config file, this function will generate mazes and map them to GIDs depending on your config.
   */
  std::vector<int>& execute(std::string output);

 private:
  /**
   * Saves the generated maze to a tmx file that can be opened oin Tiled Map Editor
   *
   * @param maze       Reference to the maze
   * @param name       Name of the tmx file
   * @param dimensions The dimensions of the map
   * @param amount     The amount of mazes to produce
   * @param dir        The output directory ti save the mazes in
   */
  void save(matrix &maze, const std::string &name, const int &dimensions,
            const int &amount, const std::string &dir);

  /**
   * Generate header for tmx file
   */
  std::string generateTMXHeader(const int &dimensions);

  /**
   * Generate tail for tmx file
   */
  std::string generateTMXTail();

  /// List of thread instances
  std::vector<int> threads_;

  /// Json parser using nlohmann
  nlohmann::json j_;
};

#endif /// __MAPPER_HXX__