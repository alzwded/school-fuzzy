// Author: Vlad Mesco
// Date: Wed Mar 12 21:46:09 EET 2014
// Description: First fuzzy logic assignment (implement 4 functions,
//              compute two instances for a pair of values)

#include <assert.h>
#include <algorithm>
#include <memory>
#include <cstdio>
#include <map>
#include <string>

#include "functions.hpp"
#include "fuzi.hxx"
#include "fuzification.hxx"

// main logic

NamedFuzis InitFuzi()
{
    FuziBuilder xBuilder;
    LeftSaturationParameters
        xNEParameters = { -1.f, -.5f };
    TrapeziumParameters
        xZEParameters = { -1.f, -.5f, .5f, 1.f };
    RightSaturationParameters
        xPOParameters = { .5f, 1.f };
    xBuilder
        << NamedVariable("NE", NewFunction(xNEParameters))
        << NamedVariable("ZE", NewFunction(xZEParameters))
        << NamedVariable("PO", NewFunction(xPOParameters));

    FuziBuilder dxBuilder;
    LeftSaturationParameters
        dxNEParameters = { -.5f, -.25f };
    TriangleParameters
        dxZEParameters = { -.5f, 0.f, .5f };
    RightSaturationParameters
        dxPOParameters = { .25f, .5f };
    dxBuilder
        << NamedVariable("NE", NewFunction(dxNEParameters))
        << NamedVariable("ZE", NewFunction(dxZEParameters))
        << NamedVariable("PO", NewFunction(dxPOParameters));

    NamedFuzis ret;
    ret.insert(std::make_pair(std::string("x"), Fuzi(xBuilder)));
    ret.insert(std::make_pair(std::string("dx"), Fuzi(dxBuilder)));

    return ret;
}

static void printResults(FuzifiedValues const& values)
{
    for(FuzifiedValues::const_iterator i = values.begin();
            i != values.end(); ++i)
    {
        printf("%s:\n\t", i->first.c_str());
        for(size_t idx = 0; idx < i->second.size(); ++idx) {
            printf("%s = %-7.2f",
                    i->second[idx].first.c_str(),
                    i->second[idx].second);
        }
        printf("\n");
    }
}

#ifdef TESTS
static void printNamedResults(Fuzi::NamedResults const& r)
{
    for(size_t i = 0; i < r.size(); ++i)
        printf("%s = %-7.2f", r[i].first.c_str(), r[i].second);
}

void test1()
{
    NamedFuzis fuzis = InitFuzi();

    Fuzi::NamedResults xResults = fuzis["x"](0.6f);
    Fuzi::NamedResults dxResults = fuzis["dx"](-0.4f);

    printf(" x: ");
    printNamedResults(xResults);
    printf("\n");

    printf("dx: ");
    printNamedResults(dxResults);
    printf("\n");
}
#endif


int main(
        int argc,
        char* argv[])
{
#ifdef TESTS
    test1();
#endif

    NamedInput input;
    input.insert(std::make_pair<std::string, float>("x", .6f));
    input.insert(std::make_pair<std::string, float>("dx", -.4f));

    NamedFuzis fuzis = InitFuzi();
    FuzificationProblem pb(fuzis);
    FuzifiedValues results = pb(input);
    printResults(results);

    return 0;
}
