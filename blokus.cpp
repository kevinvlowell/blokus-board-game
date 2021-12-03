// Copyright 2021 you
#include <iostream>
#include <string>
#include <vector>
// allowed includes
// tuple, utility, vector, map, set, unordered_map,
// unordered_set, algorithm

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;

/* Blokus command glossary

Notes:
 - the first tile is numbered 100, and each new
   one is assigned the next available integer

 - the upper left corner is 0,0

 - first index is row, second index is column


COMMANDS

QUIT: exit the game
>quit
Goodbye

RESET: start the game from beginning
>reset
game reset

CREATE: create a new tile

>create 4
>..*.
>.**.
>.*..
>....
created tile 100


SHOW ALL TILES: show a tile inventory (unordered.)

>show tiles
tile inventory
102
*
101
**
**
100
***
**.
...


SHOW A TILE: show a tile by ID number
>show 100
***
**.
**.

PLAY A TILE: place a tile on the board at row and column numbers

>play 101 4 5
played 101

>play 99 0 0
99 not played

ROTATE A TILE: rotate a tile 90 degrees counterclockwise
>rotate 100
rotate 100 complete
**.
*..
*..

FLIP A TILE sideways (left becomes right)
>fliplr 100
fliplr 100 complete
**.
.*.
.*.

FLIP A TILE vertically (up becomes down)
>flipud 100
flipud 100 complete
.*.
.*.
**.

RESIZE: make the board bigger or smaller. When smaller,
        played pieces fall off the board. Show the new board.

>resize 5

- end of command glossary -
*/


typedef int TileID;

class Tile {

 public:
  vector<string> tile_specs;
  TileID tile_id;

  Tile(vector<string> tile_specs, TileID tile_id) {
      this->tile_specs = tile_shifter(tile_specs);
      this->tile_id = tile_id;
  }

  // print out tile in tilebox format
  void show() const {
      for (int i = 0; i < tile_specs.size(); i++) {
          cout << tile_specs.at(i) << endl;
      }
  }
  void rotate();
  void flipud();
  void fliplr();

  // helper method for shifting tile to top left corner
  vector<string> tile_shifter(vector<string> original_tile) {
      int topmost_index, leftmost_index;
      bool piece_found = false;
      string row, empty_row = "";

      //create empty row for later use
      for (int i = 0; i < original_tile.size(); i++) {
          empty_row = empty_row + ".";
      }
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
      // shift tile up
      for (int i = topmost_index; i > 0; i--) {
          original_tile.erase(original_tile.begin());
          original_tile.push_back(empty_row);
      }
      // shift tile left
      for (int i = 0; i < original_tile.size(); i++) {
          (original_tile.at(i)).erase(0,leftmost_index);
          (original_tile.at(i)).append(leftmost_index, '.');
      }
      return original_tile;
  }
};


class Blokus {
  // collection of Tiles

 public:
  vector<Tile*> tile_collection;
  TileID next_id;
  
  // constructor
  Blokus() {
    next_id = 100;
  }
  Tile* find_tile(TileID search_id) {
    
    // use binary search for efficiency, since list of tiles is ordered
    int min_index = 0, max_index = tile_collection.size() - 1;
    int midpoint = (min_index + max_index)/2;
    Tile* current_tile = tile_collection.at(midpoint);

    while (current_tile->tile_id != search_id) {
      if (current_tile->tile_id < search_id) {
        min_index = midpoint + 1;
      } else {
        max_index = midpoint - 1;
      }
      midpoint = (min_index + max_index)/2;
      current_tile = tile_collection.at(midpoint);
    }
    return current_tile;
  }
  void create_piece() {
    
    int tile_size;
    vector<string> tile_str_vec;
    string line;
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
    for (int i = 0; i < tile_str_vec.size(); i++) {
        line = tile_str_vec.at(i);
        // check if square
        if (line.length() != tile_size) {
            cout << "invalid tile" << endl;
            return;
        }
        //check characters and for empty tile
        for (int j = 0; j < line.length(); j++) {
            if (line.at(j) != '*' && line.at(j) != '.') {
                cout << "invalid tile" << endl;
                return;
            }
            if (line.at(j) == '*')
                empty_tile = false;
        }
        if (empty_tile) {
            cout << "invalid tile" << endl;
            return;
        }

        // check if a duplicate tile

        // check for disconnected *'s
    }
    // make a Tile
    Tile* tile_ptr = new Tile(tile_str_vec, next_id);
    cout << "created tile " << next_id << endl;
    next_id++;

    // store it in a collection of Tiles
    tile_collection.push_back(tile_ptr);
}

  void reset();
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
  void show_board() const;
  void play_tile(TileID, int, int);
  void set_size(int);
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
