// Author: Vlad Mesco
// Date: Thu Mar 13 18:47:41 EET 2014
// Description: Fuzzyfication

#ifndef FUZIFICATION_H
#define FUZIFICATION_H

#include <map>
#include <string>
#include <limits>

#include "functions.hpp"
#include "fuzi.hxx"

typedef std::map<std::string, Fuzi> NamedFuzis;
typedef std::map<std::string, float> NamedInput;
typedef std::map<std::string, Fuzi::NamedResults> FuzifiedValues;

class FuzificationProblem
{
    NamedFuzis const& fuzis_;

    struct Computer
    {
        FuzificationProblem const& self_;

        explicit Computer(FuzificationProblem const& pb)
            : self_(pb)
        {}

        FuzifiedValues::value_type operator()(NamedInput::value_type const& v) const
        {
            NamedFuzis::const_iterator found = self_.fuzis_.find(v.first);
            if(found != self_.fuzis_.end()) {
                return FuzifiedValues::value_type(v.first, (found->second)(v.second));
            }
            return std::make_pair(v.first, FuzifiedValues::value_type::second_type());
        }
    };
public:
    FuzificationProblem(NamedFuzis const& fuzis)
        : fuzis_(fuzis)
    {}

    FuzifiedValues operator()(NamedInput const& input)
    {
        FuzifiedValues ret;
        std::transform(input.begin(), input.end(), std::inserter(ret, ret.begin()), Computer(*this));
        return ret;
    }
};

#define FUZI_INPUT(NAME) \
    NamedInput NAME; if(1) { \
        NamedInput& FUZI_input = NAME;

#define FUZI_INPUT_(X, V) \
        FUZI_input.insert(NamedInput::value_type((X), (V)));

#define FUZI_INPUT_END \
    } do{}while(0)

#endif
