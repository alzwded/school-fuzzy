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

    Fuzi(Fuzi const& other)
    {
        functions_ = other.functions_;
        const_cast<Fuzi&>(other).functions_.clear();
    }

    Fuzi& operator=(Fuzi& other)
    {
        functions_ = other.functions_;
        const_cast<Fuzi&>(other).functions_.clear();
        return *this;
    }

public:
    typedef std::pair<std::string, float> NamedResult;
    typedef std::vector<NamedResult> NamedResults;

private:
    struct Generator {
        float const u_;
        arrayOfFuncs::const_iterator i_;

        Generator(arrayOfFuncs::const_iterator i, float const u)
            : u_(u)
            , i_(i) 
        {}

        NamedResult operator()()
        {
            NamedResult ret(
                    i_->first,
                    (*i_->second)(u_));
            ++i_;
            return ret;
        }
    };

public:
    NamedResults operator()(float const u)
    {
        NamedResults ret(functions_.size());
        Generator gen(functions_.begin(), u);
        std::generate(ret.begin(), ret.end(), gen);
        return ret;
    }
};

#endif
