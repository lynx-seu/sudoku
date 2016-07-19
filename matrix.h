#ifndef MATRIX_H_
#define MATRIX_H_

#include <vector>
#include "common.h"

NS_HXL_BEGIN

template<typename T>
class Matrix
{
public:
    typedef T value_type;
    typedef Matrix<T> this_type;

    Matrix(int m, int n)
        :m_(m), n_(n), data_(m*n)
    {}

    Matrix(int m, int n, const value_type& val)
        :m_(m), n_(n), data_(m*n, val)
    {}

    value_type& at(int r, int c)
    {
        return data_[r*n_+c];
    }

    const value_type& at(int r, int c) const
    {
        return data_[r*n_+c];
    }

    int getM() const {return m_;}
    int getN() const {return n_;}

    template<typename FUNC>
    void Foreach(FUNC func)
    {
        for (int r=0; r<m_; r++) {
            for (int c=0; c<n_; c++) {
                if (func) func(r,c);
            }
        }
    }
    
private:
    int m_, n_;
    std::vector<value_type> data_;
};


NS_HXL_END

#endif
