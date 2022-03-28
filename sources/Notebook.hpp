#ifndef EX2
#define EX2

#include <iostream>
#include "Direction.hpp"
#include <string>
#include <stdexcept>
#include <map>

namespace ariel
{
    class Notebook
    {

    public:
        std::map<int, std::map<int, std::string>> notebook;
        void write(int page, int row, int column, Direction dir, std::string const &txt);
        std::string read(int page, int row, int column, Direction dir, int length);
        void erase(int page, int row, int column, Direction dir, int length);
        void show(int page);
    };

};

#endif // EX2