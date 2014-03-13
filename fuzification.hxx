// Author: Vlad Mesco
// Date: Thu Mar 13 18:47:41 EET 2014
// Description: Fuzzyfication

#ifndef FUZIFICATION_H
#define FUZIFICATION_H

#include <map>
#include <string>

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
        FuzifiedValues& out_;
        FuzificationProblem const& self_;

        Computer(
                FuzificationProblem const& pb,
                FuzifiedValues& out)
            : self_(pb)
            , out_(out)
        {}

        void operator()(NamedInput::value_type const& v) const
        {
            NamedFuzis::const_iterator found = self_.fuzis_.find(v.first);
            if(found != self_.fuzis_.end()) {
                out_.insert(std::make_pair(v.first, (found->second)(v.second)));
            }
        }
    };
public:
    FuzificationProblem(NamedFuzis const& fuzis)
        : fuzis_(fuzis)
    {}

    FuzifiedValues operator()(NamedInput const& input)
    {
        FuzifiedValues ret;
        Computer pred(*this, ret);
        std::for_each(input.begin(), input.end(), pred);
        return ret;
    }
};

#define FUZI_INPUT(NAME) \
    NamedInput NAME; if(1) { \
        NamedInput& FUZI_input = NAME;

#define FUZI_INPUT_(X, V) \
        FUZI_input.insert(std::make_pair<std::string, float>((X), (V)));

#define FUZI_INPUT_END \
    } do{}while(0)

#endif
