// Copyright 2021 Samir Farhat Dominguez safarhat@bu.edu
// Copyright 2021 Muhammad Fahad Farid fahadf@bu.edu
// Copyright 2021 Kevin Vogt-Lowell kjv@bu.edu
#include <iostream>
#include <string>
#include <vector>
using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;

// helper method for shifting tiles to top left corner
vector<string> tile_shifter(vector<string> original_tile) {
  int topmost_index, leftmost_index;
  bool piece_found = false;
  string row;

  // find topmost *
  for (int i = 0; i < original_tile.size(); i++) {
    row = original_tile.at(i);

    for (int j = 0; j < original_tile.size(); j++) {
      if (row.at(j) == '*') {
        piece_found = true;
        topmost_index = i;
        break;
      }
    }
    if (piece_found) {
      break;
    }
  }
  piece_found = false;
  // find leftmost *
  for (int i = 0; i < original_tile.size(); i++) {
    for (int j = 0; j < original_tile.size(); j++) {
      row = original_tile.at(j);

      if (row.at(i) == '*') {
        piece_found = true;
        leftmost_index = i;
        break;
      }
    }
    if (piece_found) {
      break;
    }
  }
  int ssize = original_tile.at(0).size();
  // shift tile up
  for (int i = topmost_index; i > 0; i--) {
    original_tile.erase(original_tile.begin());
  }
  string str = "";
  for (int j = 0; j < ssize; j++)
    str = str +  ".";
  for (int i = 0; i < (ssize - original_tile.size()); i++) {
    original_tile.push_back(str);
  }
  // shift tile left
  for (int i = 0; i < original_tile.size(); i++) {
    (original_tile.at(i)).erase(0, leftmost_index);
  }
  for (int i = 0; i < ssize; i++) {
    string strr = original_tile.at(i);
    for (int j = 0; j < (ssize - strr.length() ); j++)
      strr += ".";
    original_tile.at(i) = strr;
  }
  return original_tile;
}
typedef int TileID;

class Tile {
 public:
  vector<string> tile_specs;
  vector<string> orig_tile_specs;
  TileID tile_id;

  // constructor
  Tile(vector<string> tile_specs, TileID tile_id) {
    this->tile_specs = tile_shifter(tile_specs);
    this->orig_tile_specs = tile_shifter(tile_specs);
    this->tile_id = tile_id;
  }

  // print out tile in tilebox format
  void show() const {
    for (int i = 0; i < tile_specs.size(); i++) {
      cout << tile_specs.at(i) << endl;
    }
  }
  // NEED TO IMPLEMENT THESE FUNCTIONS
  void rotate() {
    vector<string> temp;
    for (int j = tile_specs.at(0).size() - 1; j >= 0; j--) {
      string str = "";
      for (int i = 0; i < tile_specs.size(); i++) {
        string t = tile_specs.at(i);
        str += (t.at(j));
      }
      temp.push_back(str);
    }
    this->tile_specs = tile_shifter(temp);
  }
  void fliplr() {
    vector<string> temp;
    for (int i = 0; i < tile_specs.size(); i++) {
      string str = "";
      string t = tile_specs.at(i);
      for (int j = tile_specs.at(0).size() - 1; j >= 0; j--)
        str += (t.at(j));
      temp.push_back(str);
    }
    this->tile_specs = tile_shifter(temp);
  }
  void flipud() {
    string temp;
    int tile_size = tile_specs.size();
    for (int i = tile_size - 1, j = 0; j < tile_size / 2; i--, j++) {
      temp = tile_specs.at(j);
      int flag = 0;
      for (int k = 0; k < tile_specs.at(i).size(); k++)
        if (tile_specs.at(i).at(k) == '*')
          flag = 1;
      if (flag == 0)
        i--;
      tile_specs.at(j) = tile_specs.at(i);
      tile_specs.at(i) = temp;
    }
  }
};
class Blokus {
  // collection of Tiles
 public:
  vector<Tile*> tile_collection;
  vector<string> board = {};
  TileID next_id;
  int weight;
  // constructor
  Blokus() {
    next_id = 100;
  }
  Tile* find_tile(TileID search_id) {
    // use binary search for efficiency, since list of tiles is ordered
    int min_index = 0, max_index = tile_collection.size() - 1;
    int midpoint = (min_index + max_index) / 2;
    Tile* current_tile = tile_collection.at(midpoint);

    while (current_tile->tile_id != search_id) {
      if (current_tile->tile_id < search_id) {
        min_index = midpoint + 1;
      } else {
        max_index = midpoint - 1;
      }
      midpoint = (min_index + max_index) / 2;
      current_tile = tile_collection.at(midpoint);
    }
    return current_tile;
  }
  void create_piece() {
    int tile_size;
    vector<string> tile_str_vec;
    string line;
    Tile* comp_tile;
    bool empty_tile = true;

    // read in the size
    cin >> tile_size;

    // read in strings
    if (tile_size > 0) {
      for (int i = 0; i < tile_size; i++) {
        cin >> line;
        tile_str_vec.push_back(line);
      }
    }
    // check tile rules

    string str = "";
    for (int i = 0; i < tile_str_vec.size(); i++) {
      str += tile_str_vec.at(i);
    }
    for (int i = 0; i < str.size(); i++) {
      if (str.at(i) == '*')
        empty_tile = false;
    }
    if (empty_tile) {
      cout << "invalid tile" << endl;
      return;
    }

    for (int i = 0; i < tile_str_vec.size(); i++) {
      line = tile_str_vec.at(i);
      // check if square
      if (line.length() != tile_size) {
        cout << "invalid tile" << endl;
        return;
      }
      // check characters and for empty tile
      for (int j = 0; j < line.length(); j++) {
        if (line.at(j) != '*' && line.at(j) != '.') {
          cout << "invalid tile" << endl;
          return;
        }
      }
    }
    // check disconnections
    for(int i = 0; i < tile_str_vec.size(); i++) {
      if(tile_str_vec.size() == 1) break;
      for(int j = 0; j < tile_str_vec.size(); j++) {
        if(tile_str_vec.at(i).at(j) == '*') {
          char check_up;
          char check_left;
          char check_right;
          char check_down;

          if (i == 0) {
            check_up = '.';
          } else {
            check_up = tile_str_vec.at(i - 1).at(j);
          }
          if (j == 0) {
            check_left = '.';
          } else {
            check_left = tile_str_vec.at(i).at(j - 1);
          }
          if (i == tile_str_vec.size() - 1) {
            check_down = '.';
          } else {
            check_down = tile_str_vec.at(i + 1).at(j);
          }
          if (j == tile_str_vec.size() - 1) {
            check_right = '.';
          } else {
            check_right = tile_str_vec.at(i).at(j + 1);
          }
          //cout << check_up << check_down << check_left << check_right << '\n';
          if (!(check_up == '*' || check_left == '*' || check_right == '*' || check_down == '*' )) {
            cout << "disconnected tile discarded" << endl;
            return;
          } else continue;
        }
      }
    }
    // make a Tile
    Tile* tile_ptr = new Tile(tile_str_vec, next_id);
    cout << "created tile " << next_id << endl;
    next_id++;

    // store it in a collection of Tiles
    tile_collection.push_back(tile_ptr);
  }

  void reset() {
    board.clear();
    tile_collection.clear();
    next_id = 100;
    cout << "game reset" << endl;
  }

  void show_tiles() const {
    Tile* tile_ptr;
    vector<string> tile;
    int tile_size;

    cout << "tile inventory" << endl;

    for (int i = 0; i < tile_collection.size(); i++) {
      tile_ptr = tile_collection.at(i);
      tile = tile_ptr->tile_specs;
      tile_size = tile.size();

      cout << tile_ptr->tile_id << endl;
      for (int j = 0; j < tile_size; j++) {
        cout << tile.at(j) << endl;
      }
    }
  }

  void show_board() const {
    for (int i = 0; i < board.size(); i++) {
      cout << board.at(i) << endl;
    }
  }

  // NEED TO IMPLEMENT
  void play_tile(TileID tile_id, int x, int y) {
    Tile* tile_ptr = find_tile(tile_id);
    vector<string> tile = tile_ptr->tile_specs;
    for (int i = 0; i < tile.size(); i++) {
      for (int j = 0; j < tile.size(); j++) {
        bool cond = (i + x > board.size() - 1 || j + y > board.size() - 1);
        if (tile.at(i).at(j) == '*' && cond) {
          cout << tile_id << " not played" << '\n';
          return;
        }
      }
    }
    for (int i = 0; i < tile.size(); i++) {
      for (int j = 0; j < tile.size(); j++) {
        if (tile.at(i).at(j) == '.')
          continue;
        board.at(i + x).at(j + y) = tile.at(i).at(j);
      }
    }
    cout << "played " << tile_id << "\n";
    return;
  }
  void set_size(int board_size) {
    if (board.size() == 0) {
      string empty_row;
      // create empty row string
      for (int i = 0; i < board_size; i++) {
        empty_row.append(".");
      }
      // create empty board
      for (int i = 0; i < board_size; i++) {
        board.push_back(empty_row);
      }
    }
    // Make bigger
    if (board_size > board.size()) {
      string rows = "";
      for (int i = 0; i < board.size(); i++) {
        for (int j = board.size(); j < board_size; j++) {
          board.at(i).push_back('.');
        }
      }
      for (int i = 0; i < board_size; i++) {
        rows.push_back('.');
      }
      for (int i = board.size(); i < board_size; i++) {
        board.push_back(rows);
      }
    }
  }
};
// MAIN. Do not change the below.

int main() {
  string command;
  Blokus b;

  while (true) {
    cin >> command;
    if (command == "quit")  {
      break;
    } else if (command == "//") {
      getline(cin, command);
    } else if (command == "board") {
      b.show_board();
    } else if (command == "create") {
      b.create_piece();
    } else if (command == "reset") {
      b.reset();
    } else if (command == "show") {
      string arg;
      cin >> arg;
      if (arg == "tiles") {
        b.show_tiles();
      } else {
        auto g = b.find_tile(std::stoi(arg));
        g->show();
      }
    } else if (command == "resize") {
      int newsize;
      cin >> newsize;
      b.set_size(newsize);
      b.show_board();
    } else if (command == "play") {
      TileID id;
      int row, col;
      cin >> id >> row >> col;
      b.play_tile(id, row, col);
    } else if (command == "rotate") {
      TileID id;
      cin >> id;
      auto g = b.find_tile(id);
      g->rotate();
      cout << "rotated " << id << "\n";
      g->show();
    } else if (command == "fliplr") {
      TileID id;
      cin >> id;
      auto g = b.find_tile(id);
      g->fliplr();
      cout << "fliplr " << id << "\n";
      g->show();
    } else if (command == "flipud") {
      TileID id;
      cin >> id;
      auto g = b.find_tile(id);
      g->flipud();
      cout << "flipud " << id << "\n";
      g->show();
    } else {
      cout << "command not understood.\n";
    }
  }
  cout << "Goodbye\n";
  return 0;
}
