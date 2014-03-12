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
#include "fuzi.h"

// main logic

typedef std::map<std::string, Fuzi> NamedFuzis;

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

void printNamedResults(Fuzi::NamedResults const& r)
{
    for(size_t i = 0; i < r.size(); ++i)
        printf("%s = %-7.2f", r[i].first.c_str(), r[i].second);
}

int main(
        int argc,
        char* argv[])
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

    return 0;
}
