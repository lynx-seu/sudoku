#ifndef SUDOKU_H_
#define SUDOKU_H_

#include "common.h"
#include "matrix.h"
#include <memory>
#include <iostream>

NS_HXL_BEGIN

class Sudoku
{
    friend std::ostream& operator<<(std::ostream&, const Sudoku&);
public:
    Sudoku(int order);
    ~Sudoku();

    void init();
    void remove_numbers(int level);

    static int block(int r, int c, int base);
    static std::shared_ptr< Matrix<char> > transferTo(Matrix<int>& data);
private:
    int base_;
    int order_;
    Matrix<int> *data_;
    Matrix<int> *sloved_;
};

NS_HXL_END

#endif