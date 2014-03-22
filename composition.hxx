// Author: Vlad Mesco
// Date: Sat Mar 22 11:17:10 EET 2014
// Description: Compose fuzzy results
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

        FloatMatrix::IndexSet indexes;
        Compute(rules_,
                inferenceMatrix,
                indexes,
                ret);

        return ret;

    }
private:
    void Compute(
            Rules const& rules,
            FloatMatrix const& inferenceMatrix,
            FloatMatrix::IndexSet& indexes,
            Fuzi::NamedResults& ret)
    {
        if(rules.empty()) {
            Fuzi::NamedResults::iterator i = ret.find(*rules);
            if(i == ret.end()) {
                ret.insert(std::make_pair(*rules, *inferenceMatrix));
            } else {
                ret[*rules] = std::max(ret[*rules], *inferenceMatrix);
            }
            return;
        }

        for(Rules::const_iterator i = rules.begin();
                i != rules.end(); ++i)
        {
            indexes.push_back(i->first);
            Compute(rules[i->first],
                    inferenceMatrix[i->first],
                    indexes,
                    ret);
            indexes.pop_back();
        }
    }
};

#endif
