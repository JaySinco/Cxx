#pragma once
#include "vars.h"
#include <algorithm>
#include <cassert>
#include <iomanip>
#include <iostream>
#include <vector>

/*
3 * 3 board looks like:
  0 1 2
 ------- Col
0|0 1 2
1|3 4 5
2|6 7 8
 |
Row  => move z(5) = (x(1), y(2))
*/

#define ON_BOARD(row, col) (row >= 0 && row < BOARD_MAX_ROW && col >= 0 && col < BOARD_MAX_COL)

enum class Color : int8_t {
    Empty = 0,
    Black = -1,
    White = 1,
};

inline Color operator~(const Color c)
{
    assert(c != Color::Empty);
    return Color(-1 * (int)c);
}

std::ostream& operator<<(std::ostream& out, Color c);

class Move {
public:
    Move(int z)
        : index(z)
    {
        assert((z >= 0 && z < BOARD_SIZE) || z == NO_MOVE_YET);
    }
    Move(int row, int col)
        : Move(row * BOARD_MAX_COL + col)
    {
        assert(ON_BOARD(row, col));
    }
    Move(const Move& mv)
        : Move(mv.z())
    {
    }
    int z() const { return index; }
    int r() const { return index / BOARD_MAX_COL; }
    int c() const { return index % BOARD_MAX_COL; }
    bool operator<(const Move& rhs) const { return index < rhs.index; }
    bool operator==(const Move& rhs) const { return index == rhs.index; }

    friend std::ostream& operator<<(std::ostream& out, Move mv)
    {
        return out << "(" << std::setw(2) << mv.r() << ", " << std::setw(2) << mv.c() << ")";
    }

private:
    bsize_t index;
};

class Board {
public:
    Board()
        : grid { Color::Empty }
    {
    }
    Color get(Move mv) const { return grid[mv.z()]; }
    void push_valid(std::vector<Move>& set) const;
    bool win_from(Move mv) const;

    void put(Move mv, Color c)
    {
        assert(get(mv) == Color::Empty && c != Color::Empty);
        grid[mv.z()] = c;
    }

    friend std::ostream& operator<<(std::ostream& out, const Board& board);

private:
    Color grid[BOARD_SIZE];
};

class State {
public:
    State()
        : last(NO_MOVE_YET)
        , winner(Color::Empty)
    {
        board.push_valid(opts);
    }
    Move get_last() const { return last; }
    Color get_winner() const { return winner; }
    bool first_hand() const { return current() == Color::Black; }
    void fill_feature_array(float data[INPUT_FEATURE_NUM * BOARD_SIZE]) const;
    bool valid(Move mv) const { return std::find(opts.cbegin(), opts.cend(), mv) != opts.end(); }
    bool over() const { return winner != Color::Empty || opts.size() == 0; }
    Color current() const;
    void next(Move mv);
    Color next_rand_till_end();

    const std::vector<Move>& get_options() const
    {
        assert(!over());
        return opts;
    };

    friend std::ostream& operator<<(std::ostream& out, const State& state)
    {
        if (state.last.z() == NO_MOVE_YET)
            return out << state.board << "last move: None";
        else
            return out << state.board << "last move: " << ~state.current() << state.last;
    }

private:
    Board board;
    Move last;
    Color winner;
    std::vector<Move> opts;
};

struct Player {
    static Player& play(Player& p1, Player& p2, bool silent = true);
    static float benchmark(Player& p1, Player& p2, int round, bool silent = true);
    Player() {}
    virtual void reset() = 0;
    virtual const std::string& name() const = 0;
    virtual Move play(const State& state) = 0;
    virtual ~Player() {};
};

class RandomPlayer : public Player {
public:
    RandomPlayer(const std::string& name)
        : id(name)
    {
    }
    void reset() override {}
    const std::string& name() const override { return id; }
    Move play(const State& state) override { return state.get_options()[0]; }
    ~RandomPlayer() {};

private:
    std::string id;
};

class HumanPlayer : public Player {
public:
    HumanPlayer(const std::string& name)
        : id(name)
    {
    }
    void reset() override {}
    const std::string& name() const override { return id; }
    Move play(const State& state) override;
    ~HumanPlayer() {};

private:
    std::string id;
    bool get_move(int& row, int& col);
};