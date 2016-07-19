//The following code is based on the paper "Dancing Links" by D. E. Knuth.
//See http://www-cs-faculty.stanford.edu/~uno/papers/dancing-color.ps.gz
#include "dlx.h"
#include <assert.h>
#include <limits.h>

namespace hxl {

	class Dlx::Impl
	{
		friend class Dlx;

		struct data_object //A module in the sparse matrix data structure.
		{
			data_object* L;                //Link to next object left.
			data_object* R;                //         "          right.
			data_object* U;                //         "          up.
			data_object* D;                //         "          down.
			data_object* C;                //Link to column header.
			int x;                         //In a column header: number of ones 
												//in the column. Otherwise: row index.
			void cover()                   //Covers a column.
			{
				data_object* i=D;
				data_object* j;
				R->L=L; L->R=R;
				while (i!=this)
				{
					j=i->R;
					while (j!=i)
					{
						j->D->U=j->U; j->U->D=j->D;
						j->C->x--;
						j=j->R;
					}
					i=i->D;
				}
			}
			void uncover()                 //Uncovers a column.
			{
				data_object* i=U;
				data_object* j;
				while (i!=this)
				{
					j=i->L;
					while (j!=i)
					{
						j->C->x++;
						j->D->U=j; j->U->D=j;
						j=j->L;
					}
					i=i->U;
				}
				R->L=this; L->R=this;
			}
		};

	public:
		Impl(const Matrix<char>& matrix);
		~Impl();
		template<class FUNC1> void search(int k, Dlx::FUNC func, FUNC1 choose_column);
		static data_object* Knuth_S_heuristic(data_object* root);

		void setLimits(int limit) { limit_ = limit; }
	private:
		data_object *root_;			//root
		int *res_;					//auxiliary stack for recursion
		int res_count_;				//result count
		int limit_;					//max result count
	};

	Dlx::Impl::Impl(const Matrix<char>& matrix)
		: root_(new data_object), res_count_(0)
	{
		int rows = matrix.getM(), cols = matrix.getN();
		res_ = new int[rows];
		data_object *P = root_, *Q;
		data_object** walkers=new data_object*[cols];

		for (int i=0; i<cols; i++)
		{
			(P->R=new data_object)->L = P;
			walkers[i] = Q = P = P->R;
			P->x = 0;
			for (int j=0; j<rows; j++)
				if (matrix.at(j, i) == '1')
				{
					(Q->D=new data_object)->U = Q;
					Q = Q->D;
					Q->C = P; P->x++;
					Q->x = j;
				}
			Q->D = P; P->U = Q;
		}
		P->R = root_; root_->L = P;

        //eliminate empty columns
        // P=root_;
        // for (int i=0; i<cols; i++)
        // {
        //     P=P->R;
        //     if (!P->x)
        //     {
        //         P->L->R=P->R;
        //         P->R->L=P->L;
        //     }
        // }

		//now construct the L/R links for the data objects.
		P=new data_object;
		for (int i=0; i<rows; i++)
		{
			Q=P;
			for (int j=0; j<cols; j++)
				if (matrix.at(i, j) == '1') //a one
				{
					//in _this_ row...
					walkers[j]=walkers[j]->D;
					//create L/R links
					(Q->R=walkers[j])->L=Q;
					//advance pointer
					Q=Q->R;
				}

			if (Q==P) continue;
			Q->R=P->R;       //link it to the first one in this row.
			P->R->L=Q;       //link the first one to the last one.
		}
		delete P;                //P is no longer needed
		delete []walkers;          //walkers are no longer needed

	}

	Dlx::Impl::~Impl()
	{
		data_object *P = root_->R, *Q;
		while (P != root_)
		{
			Q = P->D;
			while (Q != P)
			{
				Q = Q->D;
				delete Q->U;
			}
			P = P->R;
			delete P->L;
		}
		delete root_;

		delete []res_;
	}

	template<class FUNC1>
	void Dlx::Impl::search(int k, Dlx::FUNC send_row, FUNC1 choose_column)
	{
		data_object *r,*c,*j;
		if (root_->R==root_) //done - solution found
		{
			res_count_++;
			for (int i=0; i<k; i++)
				send_row(res_[i]);
			send_row(Dlx::nullrow);
			return;
		}
		//otherwise
		c=choose_column(root_); //choose a column to cover
		c->cover();         //cover it
		r=c->D;
		while (r!=c)
		{
			res_[k] = r->x;
			j=r->R;
			while (j!=r)
			{
				j->C->cover();
				j=j->R;
			}
			
			if (res_count_ < limit_) search(k+1, send_row, choose_column);
			//set r <- O[k], and c<- C[r], this is unnecessary
			j=r->L;
			while (j!=r)
			{
				j->C->uncover();
				j=j->L;
			}
			r=r->D;
		}
		c->uncover();
	}

	Dlx::Impl::data_object* Dlx::Impl::Knuth_S_heuristic(Dlx::Impl::data_object* root)
	{
		data_object* P=root->R;
		data_object* res = 0;
		int best=INT_MAX/2;
		while (P!=root)
		{
			if (P->x<best)
			{
				best=P->x;
				res=P;
			}
			P=P->R;
		}
		return res;
	}

	//////////////////////////////////////////////////////////////////////////
	Dlx::Dlx(const Matrix<char>& matrix, int limit)
		: pImpl(new Impl(matrix))
	{
		pImpl->setLimits(limit);
	}

	Dlx::~Dlx()
	{

	}

	void Dlx::Dance(FUNC send_row)
	{
		pImpl->search(0, send_row, Dlx::Impl::Knuth_S_heuristic);
	}

} // end namespace hxl

