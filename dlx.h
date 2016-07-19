#ifndef LOGIC_H_
#define LOGIC_H_

#include <memory>
#include <functional>
#include "matrix.h"
#include "limits.h"

namespace hxl {

////////////////////////////////////////////////////////////////////////////////
// class Dlx
// Use to slove exact cover problem
////////////////////////////////////////////////////////////////////////////////
	class Dlx
	{
	public:
		enum { nullrow = -1 };
		typedef std::function<void (int)> FUNC;

		Dlx(const Matrix<char>& matrix, int limit=INT_MAX);
		~Dlx();

		void Dance(FUNC send_row);
	private:
		Dlx(const Dlx&);
		Dlx& operator=(const Dlx&);
	private:
		class Impl;
		std::unique_ptr<Impl> pImpl;
	};

}


#endif
