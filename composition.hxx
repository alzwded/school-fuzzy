#ifndef COMPOSITION_HXX
#define COMPOSITION_HXX

#include "fuzi.hxx"
#include "multidimmatrix.hpp"

class CompositionProblem
{
    Rules const& rules_;
public:
    CompositionProblem(Rules const& rules)
        : rules_(rules)
    {}
    Fuzi::NamedResults operator()(FloatMatrix const& inferenceMatrix)
    {
        Fuzi::NamedResults ret;

        for(Rules::iterator i = rules_.begin(); i != rules_.end(); ++i)
        {
            for(Rules::iterator j = i->second.begin(); j != i->second.end(); ++j)
            {
                ret[(*j->second).c_str()] =
                    std::max(
                            ret[(*j->second).c_str()],
                            *inferenceMatrix
                            .Of(i->first.c_str())
                            .Of(j->first.c_str()));
            }
        }

        return ret;

    }
};

#endif
