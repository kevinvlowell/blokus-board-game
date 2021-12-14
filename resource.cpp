#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>

using std::cout;
using std::cin;
using std::vector;
using std::string;

bool correctCharacters(vector<string> tileSet) {
    for (int i{0}; i < tileSet.size(); ++i) {
        for (int j{0}; j < tileSet[i].size(); ++j) {
            if (tileSet[i][j] != '*' and tileSet[i][j] !='.' and
                tileSet[i][j] != ' ') return false;
            if (tileSet[i][j] == ' ' and
                tileSet[i][j] != tileSet[i][tileSet.size()-1]) return false;
        }
    }
    return true;
}

bool emptyLineAfterTile(vector<string> tileSet) {
    for (int i{0}; i < tileSet.size(); ++i) {
        if (tileSet[i].empty())
            if (tileSet[i+1].empty() and (i+1) < (tileSet.size()-1)) return false;
    }
    if (not tileSet[tileSet.size()-1].empty()) return false;
    return true;
}

bool correctSize(vector<string> tileSet, int N) {
    int rowTextBox{0};
    for (int i{0}; i < tileSet.size(); ++i) {
        if (not tileSet[i].empty()) {
            ++rowTextBox;
            if (tileSet[i].size() != N) return false;
        } else {
            if (rowTextBox != N) return false;
            else
                rowTextBox = 0;
        }
    }
    return true;
}

bool emptyTile(vector<string> tileSet, int N) {
    int count{0};
    for (int i{0}; i < tileSet.size(); ++i) {
        if (not tileSet[i].empty()) {
            for (int j{0}; j < N; ++j)
                if (tileSet[i][j] == '.') ++count;
        } else {
            if (count == N*N) return true;
            else
                count = 0;
        }
    }
    return false;
}

vector<int> numOfCells(char** tile, int rowN, int colN) {
    vector<int> countTile;
    int count{0};
    for (int a{0}; a < rowN; ++a) {
        for (int b{0}; b < colN; ++b) {
            if (a % (colN+1) != colN) {  //  not an empty line
                if (tile[a][b] == '*')
                    ++count;
            } else {  //  empty line
                countTile.push_back(count);
                count = 0;
                break;
            }
        }
    }
    return countTile;
}

void printArray(char** tile, int rowN, int colN, int argc, char* file) {
    std::ostream *outputstream;
    if (argc <=3) {
        outputstream = &cout;
    } else {
        std::ofstream output(file);
        outputstream = &output;
    }
    for (int a{0}; a < rowN; ++a) {
        for (int b{0}; b < colN; ++b) {
            if (tile[a][b] == 'X') {
                break;
            } else {
                *outputstream << tile[a][b];
            }
        }
        if (tile[a][0] != 'X')
            *outputstream << '\n';
    }
}

void connectedTiles(char** &tile, int row, int col, vector<int> countTile,
                    vector<int> empty) {
    bool connected = true;
    vector<std::pair<int, int>> tileIndex;
    std::pair <int, int> pos;
    int p;
    for (int n{0}; n < countTile.size(); ++n) {
        tileIndex.clear();
        if (countTile[n] != 1) {
            connected = true;
            for (int i{n*(col+1)}; i < empty[n]; ++i) {
                for (int j{0}; j < col; ++j) {
                    if (tile[i][j] == '*') {
                        pos = std::make_pair(i, j);
                        tileIndex.push_back(pos);
                    }
                }
            }
            for (int a{0}; a < tileIndex.size()-1; ++a) {
                int refrow = tileIndex[a].first;
                int refcol = tileIndex[a].second;
                int cmpcol = tileIndex[a+1].second;
                int cmprow = tileIndex[a+1].first;
                if (refcol == cmpcol) {
                    if (refrow + 1 != cmprow and refrow -1 != cmprow) {
                        if (cmprow - refrow >1) {
                            for (int k{0}; k < col; ++k) {
                                if (tile[refrow+1][k] == '*') {
                                    break;
                                }
                                if (k== col-1) {
                                    connected = false;
                                }
                            }
                        }
                        if (refrow - cmprow >1) {
                            for (int k{0}; k < col; ++k) {
                                if (tile[refrow-1][k] == '*') {
                                    break;
                                }
                                if (k== col-1) {
                                    connected = false;
                                }
                            }
                        }
                    }
                }
                if (refrow == cmprow) {
                    if (refcol + 1 != cmpcol and refcol -1 != cmpcol) {
                        if (cmpcol - refcol >1) {
                            for (int k{n*(col+1)}; k < empty[n]; ++k) {
                                if (tile[k][refcol+1] == '*') {
                                    break;
                                }
                                if (k== empty[n]-1) {
                                    connected = false;
                                }
                            }
                        }
                        if (refcol - cmpcol >1) {
                            for (int k{n*(col+1)}; k < empty[n]; ++k) {
                                if (tile[k][refcol-1] == '*') {
                                    break;
                                }
                                if (k == empty[n]-1) {
                                    connected = false;
                                }
                            }
                        }
                    }
                }
                if (refrow + 1 == cmprow or refrow -1 == cmprow) {
                    if (refcol + 1 == cmpcol or refcol -1 == cmpcol) {
                        if (tile[refrow][cmpcol] != '*' and
                            tile[cmprow][refcol] != '*') {
                            connected = false;
                        }
                    }
                    if (refcol - cmpcol > 1) {
                        if (tile[cmprow][refcol] == '*') {
                            for (int k{0}; k < refcol+1; ++k) {
                                if (tile[cmprow][k] != '*') {
                                    for (int p{cmprow}; p < empty[n]; ++p) {
                                        if (tile[p][k] =='*')
                                            continue;
                                    }
                                    if (p == empty[n]-1) {
                                        connected = false;
                                    }
                                }
                            }
                        } else {
                            connected = false;
                        }
                    }
                    if (cmpcol - refcol >1) {
                        if (tile[refrow][cmpcol] == '*') {
                            for (int k{0}; k < cmpcol+1; ++k) {
                                if (tile[refrow][k] != '*') {
                                    for (int p{refrow}; p < empty[n]; ++p) {
                                        if (tile[p][k] =='*')
                                            continue;
                                    }
                                    if (p == empty[n]-1) {
                                        connected = false;
                                    }
                                }
                            }
                        } else {
                            connected = false;
                        }
                    }
                }
                if (connected == true) {
                    if (cmprow - refrow >1 and refrow == cmpcol and refcol == cmprow) {
                        connected = false;
                    }
                }
            }
            if (not connected) {
                for (int c{0+n*(col+1)}; c <= empty[n]; ++c) {
                    for (int d{0}; d < col; ++d) {
                        tile[c][d] = 'X';
                    }
                }
            }
        }
    }
}

void cycleLeft(char** &tile, int ref, int row, int col) {
    char temp;
    int numRow = row - ref;
    int numBoxLeft = (numRow / (col+1)) - 1;
    for (int v{1}; v <= numBoxLeft; ++v) {
        for (int a{ref+v*(col+1)}; a < ref+(v+1)*(col+1)-1; ++a) {
            for (int b{0}; b < col-1; b++) {
                temp = tile[a][b];
                tile[a][b] = tile[a][b+1];;
                tile[a][b+1] = temp;
            }
        }
    }
}

void cycleUp(char** &tile, int ref, int row, int col) {
    char temp;
    int numRow = row - ref;
    int numBoxLeft = (numRow / (col+1)) - 1;
    for (int v{1}; v <= numBoxLeft; ++v) {
        for (int a{ref+v*(col+1)}; a < ref+(v+1)*(col+1)-2; ++a) {
            for (int b{0}; b < col; b++) {
                temp = tile[a][b];
                tile[a][b] = tile[a+1][b];
                tile[a+1][b] = temp;
            }
        }
    }
}

void flipTileRow(char** &tile, int ref, int row, int col) {
    char temp;
    int numRow = row - ref;
    int numBox = (numRow / (col+1)) - 1;
    for (int v{1}; v <= numBox; ++v) {
        for (int a{ref+v*(col+1)}; a < ref+(v+1)*(col+1)-1; ++a) {
            for (int b{0}; b < col/2; ++b) {
                temp = tile[a][col-b-1];
                tile[a][col-b-1] = tile[a][b];
                tile[a][b] = temp;
            }
        }
    }
}

void flipTileCol(char** &tile, int ref, int row, int col) {
    char temp;
    int numRow = row - ref;
    int numBox = (numRow / (col+1)) - 1;
    for (int v{1}; v <= numBox; ++v) {
        for (int a{ref+v*(col+1)}; a <= (ref+ v*(col+1) + col/2); ++a) {
            for (int b{0}; b < col; ++b) {
                temp = tile[a][b];
                int nextempty = ref+(v+1)*(col+1)-1;
                int realrow = nextempty - a-1;
                tile[a][b] = tile[realrow + ref+v*(col+1)][b];
                tile[realrow + ref+v*(col+1)][b] = temp;
            }
        }
    }
}

void rotate90Tile(char** &tile, int ref, int row, int col) {
    int temp, tempindrow, tempindcol, i{0}, realrow, tempcol;
    int numRow = row - ref;
    int numBox = (numRow / (col+1)) - 1;
    for (int v{1}; v <= numBox; ++v) {
        for (int a{ref+v*(col+1)}; a < ref+(v+1)*(col+1)-1; ++a) {
            for (int b{0}; b < a - (ref+v*(col+1)-1); ++b) {
                temp = tile[a][b];
                realrow = a - (ref+v*(col+1));
                tempcol = b + ref+v*(col+1);
                tempindrow = tempcol;
                tempindcol = realrow;
                tile[a][b] = tile[tempindrow][tempindcol];
                tile[tempindrow][tempindcol] = temp;
            }
        }
    }
    flipTileRow(tile, ref, row, col);
}

void sameTileSet(char** &tile, int ref, int row, int col) {
    bool same{false};
    vector<std::pair<int, int>> refIndex;
    vector<std::pair<int, int>> tileIndex;
    std::pair <int, int> pos;
    for (int i{ref}; i < ref+col+1; ++i) {  //  make reference to compare
        for (int j{0}; j < col; ++j) {
            if (tile[i][j] == '*') {
                pos = std::make_pair(i, j);
                refIndex.push_back(pos);
            }
        }
    }  //  one tile at a time
    int numRow = row - ref;
    int numBox = (numRow / (col+1))-1;
    int v{1};
    while (v <= numBox)  {
        for (int i{ref+v*(col+1)}; i < ref+(v+1)*(col+1)-1; ++i) {
            for (int j{0}; j < col; ++j) {
                if (tile[i][j] == '*') {
                    pos = std::make_pair(i, j);
                    tileIndex.push_back(pos);
                }
            }
        }
        if (refIndex.size() == tileIndex.size()) {
            bool same{true};
            for (int l{0}; l < refIndex.size(); ++l) {
                same = same and ((tileIndex[l].first - refIndex[l].first)
                                 % (col+1) == 0) and (tileIndex[l].second == refIndex[l].second);
            }
            if (same) {
                for (int y{ref+v*(col+1)}; y < ref+(v+1)*(col+1); ++y) {
                    for (int z{0}; z < col; ++z)
                        tile[y][z] = 'X';
                }
            }
        }
        tileIndex.clear();
        ++v;
    }
}
