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
        FloatMatrix::IndexSet indexes;
        std::deque<float> vals;
        Compute(mat_, indexes, vals, funcs_.begin());
        return mat_;
    }
private:
    void Compute(
            FloatMatrix& mat,
            FloatMatrix::IndexSet& indexes,
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
            indexes.push_back(j->first);
            vals.push_back(j->second);
            Compute(mat.For(j->first), indexes, vals, i + 1);
            vals.pop_back();
            indexes.pop_back();
        }
    }
};

#endif
