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
    LeftSaturationDefinition
        xNEDefinition = { -1.f, -.5f };
    TrapeziumDefinition
        xZEDefinition = { -1.f, -.5f, .5f, 1.f };
    RightSaturationDefinition
        xPODefinition = { .5f, 1.f };
    xBuilder
        << NamedVariable("NE", NewFunction(xNEDefinition))
        << NamedVariable("ZE", NewFunction(xZEDefinition))
        << NamedVariable("PO", NewFunction(xPODefinition));

    FuziBuilder dxBuilder;
    LeftSaturationDefinition
        dxNEDefinition = { -.5f, -.25f };
    TriangleDefinition
        dxZEDefinition = { -.5f, 0.f, .5f };
    RightSaturationDefinition
        dxPODefinition = { .25f, .5f };
    dxBuilder
        << NamedVariable("NE", NewFunction(dxNEDefinition))
        << NamedVariable("ZE", NewFunction(dxZEDefinition))
        << NamedVariable("PO", NewFunction(dxPODefinition));

    NamedFuzis ret;
    ret.insert(NamedFuzis::value_type("x", Fuzi(xBuilder)));
    ret.insert(NamedFuzis::value_type("dx", Fuzi(dxBuilder)));

    ret.insert(
            NamedFuzis::value_type("t", FuziBuilder()
                    << NamedVariable("a", NewFunction(dxPODefinition))
                    << NamedVariable("b", NewFunction(xPODefinition))));

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

    printf(" t: ");
    printNamedResults(fuzis["t"](0.7f));
    printf("\n");

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

    NamedFuzis fuzis = InitFuzi();

    FUZI_INPUT(input)
        FUZI_INPUT_("x", .6f)
        FUZI_INPUT_("dx", -.4f)
    FUZI_INPUT_END;

    FuzifiedValues results = FuzificationProblem(fuzis)(input);
    printResults(results);

    return 0;
}
