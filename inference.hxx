// Author: Vlad Mesco
// Date: Sat Mar 22 11:17:10 EET 2014
// Description: Infer results matrix
#ifndef INFERENCE_HXX
#define INFERENCE_HXX

#include <vector>
#include <deque>

#include "fuzi.hxx"
#include "multidimmatrix.hpp"

class Inference
{
    FloatMatrix mat_;
    typedef std::vector<Fuzi::NamedResults const*> funcs_t;
    funcs_t funcs_;
public:
    Inference& operator<<(Fuzi::NamedResults const& func)
    {
        funcs_.push_back(&func);
        return *this;
    }

    FloatMatrix const& operator*()
    {
        std::deque<float> vals;
        Compute(mat_, vals, funcs_.begin());
        return mat_;
    }
private:
    void Compute(
            FloatMatrix& mat,
            std::deque<float>& vals,
            funcs_t::iterator i)
    {
        if(i == funcs_.end()) {
            std::deque<float>::iterator minimum =
                std::min_element(vals.begin(), vals.end());
            mat.Set(*minimum);
            return;
        }

        Fuzi::NamedResults const& r = **i;
        for(Fuzi::NamedResults::const_iterator j = r.begin();
                j != r.end(); ++j)
        {
            vals.push_back(j->second);
            Compute(mat.For(j->first), vals, i + 1);
            vals.pop_back();
        }
    }
};

#endif
