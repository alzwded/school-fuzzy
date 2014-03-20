// Author: Vlad Mesco
// Date: Wed Mar 12 21:46:09 EET 2014
// Description: Fuzzy functions

#ifndef FUZI_H
#define FUZI_H

#include <vector>
#include <utility>
#include <string>

#include "functions.hpp"

typedef std::pair<std::string, IFuzzyFunction const*> NamedVariable;

class Fuzi;
class FuziBuilder
{
    typedef std::vector<NamedVariable> arrayOfFuncs;
    arrayOfFuncs functions_;
    friend class Fuzi;
public:
    FuziBuilder& operator<<(NamedVariable const& f)
    {
        functions_.push_back(f);
        return *this;
    }
private:
    arrayOfFuncs const& get() const
    {
        return functions_;
    }
};

class Fuzi
{
    typedef std::vector<NamedVariable> arrayOfFuncs;
    arrayOfFuncs functions_;

private:
    static void deleter(NamedVariable& v)
    {
        delete v.second;
    }

public:
    // Default constructor and operator= and ctor(&) needed for std::map
    Fuzi() {}
    Fuzi(FuziBuilder const& builder)
        : functions_(builder.get().size())
    {
        functions_ = builder.get();
    }

    ~Fuzi()
    {
        std::for_each(functions_.begin(), functions_.end(), deleter);
    }

    // Move constructor. In C++11 wthere would actually be just the
    //     move constructor and this would be private
    Fuzi(Fuzi const& other)
    {
        functions_ = other.functions_;
        const_cast<Fuzi&>(other).functions_.clear();
    }

    // Move behaviour. other will no longer contain any data after this
    //     function returns
    Fuzi& operator=(Fuzi& other)
    {
        functions_ = other.functions_;
        const_cast<Fuzi&>(other).functions_.clear();
        return *this;
    }

public:
    typedef std::map<std::string, float> NamedResults;
    typedef NamedResults::value_type NamedResult;

private:
    struct Generator {
        float const u_;

        explicit Generator(float const u)
            : u_(u)
        {}

        NamedResult operator()(arrayOfFuncs::value_type const& i) const
        {
            return NamedResult(
                    i.first,
                    (*i.second)(u_));
        }
    };

public:
    // Compute results for all parameters in point u
    NamedResults operator()(float const u) const
    {
        NamedResults ret;
        std::transform(functions_.begin(), functions_.end(), std::inserter(ret, ret.begin()), Generator(u));
        return ret;
    }
};

#endif
