#include "sudoku.h"
#include "dlx.h"
#include <cmath>
#include <time.h>
#include <assert.h>

NS_HXL_BEGIN

std::ostream& operator<<(std::ostream& out, const Sudoku& sudoku)
{
    for (int r=0; r<sudoku.order_; r++)
    {
        for (int c=0; c<sudoku.order_; c++)
        {
            out << sudoku.data_->at(r, c) << " ";
        }
        out << "\n";
    }

    return out;
}

Sudoku::Sudoku(int order)
    : order_(order), base_(std::sqrt(order))
    , data_(new Matrix<int>(order, order, 0))
    , sloved_(0)
{
    // clock_t start = clock();
    
    // std::shared_ptr<Matrix<char> > mat = transferTo(*data_);
    // Dlx dlx(*mat, 10000);
    // dlx.Dance([&](int x) {
    //     int r, c, i;
    //     if (x != Dlx::nullrow)
    //     {
    //         i = x%order_; x/=order_;
    //         c = x%order_; r=x/order_;
    //         data_->at(r, c) = i+1;
    //     }
    // });

    // clock_t diff = clock() - start;
    // std::cout << "cost: " << diff << "  " << CLOCKS_PER_SEC  << std::endl;
    init();
}

Sudoku::~Sudoku()
{

}

void Sudoku::init()
{
    // 初始化第一行, 并打乱
    for (int c=0; c<order_; c++) data_->at(0, c) = c+1;

    for (int c=0; c<order_; c++)
    {
        while (1)
        {
            int random = rand() % order_;
            if (random != c)
            {
                int tmp = data_->at(0, c);
                data_->at(0, c) = data_->at(0, random);
                data_->at(0, random) = tmp;
                break;
            }
        }
    }

    // 任意可行解
    std::shared_ptr<Matrix<char> > mat = transferTo(*data_);
    Dlx dlx(*mat, 1);
    dlx.Dance([&](int x) {
        int r, c, i;
        if (x != Dlx::nullrow)
        {
            i = x%order_; x/=order_;
            c = x%order_; r=x/order_;
            data_->at(r, c) = i+1;
        }
    });
    sloved_ = new Matrix<int>(*data_);

    // 挖洞

}

void Sudoku::remove_numbers(int level)
{
    
}

int Sudoku::block(int r, int c, int base) 
{
    return base*(r/base) + c/base;
}

std::shared_ptr< Matrix<char> > Sudoku::transferTo(Matrix<int>& data) 
{
    assert(data.getM() == data.getN());
    int order = data.getM();
    int base  = std::sqrt(order);
    Matrix<char> *mat = new Matrix<char>(order*order*order, order*order*4, '0');
    int row = 0;

    for (int r=0; r<order; r++) {
        for(int c=0; c<order; c++) {
            for (int i=0; i<order; i++, row++)
            {
                int v = data.at(r, c);
                if (v>0 && v<=order && v != i+1) continue;
                
                // 约束条件1：每个格子只能填一个数字 
                mat->at(row, r*order+c) = '1'; 
                // 约束条件2：每行1-n的n个数字都得填一遍
                mat->at(row, r*order+i+order*order) = '1';
                // 约束条件3：每列1-n的这n个数字都得填一遍
                mat->at(row, c*order+i+order*order*2) = '1';
                // 约束条件4：每宫1-n的这n个数字都得填一遍
                mat->at(row, block(r,c,base)*order+i+order*order*3) = '1';
            }
        }
    }

    return std::shared_ptr<Matrix<char> >(mat);
}

NS_HXL_END