// Author: Vlad Mesco
// Date: Wed Mar 12 21:46:09 EET 2014
// Description: Fuzzy logic lab assignment

#include <assert.h>
#include <algorithm>
#include <memory>
#include <map>
#include <cstdio>
#include <string>

#include "functions.hpp"
#include "fuzi.hxx"
#include "fuzification.hxx"
#include "multidimmatrix.hpp"
#include "composition.hxx"
#include "inference.hxx"

#include "utilities.ixx"

// main logic

static NamedFuzis InitFuzi()
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


    return ret;
}

static Rules InitRules()
{
    Rules rules;
    //        dx        x         y
    rules.For("NE").For("NE").Set("PO");
    rules.For("NE").For("ZE").Set("ZE");
    rules.For("NE").For("PO").Set("ZE");
    rules.For("ZE").For("NE").Set("PO");
    rules.For("ZE").For("ZE").Set("ZE");
    rules.For("ZE").For("PO").Set("NE");
    rules.For("PO").For("NE").Set("ZE");
    rules.For("PO").For("ZE").Set("NE");
    rules.For("PO").For("PO").Set("NE");

    return rules;
}

void clean_main()
{
    NamedFuzis fuzis = InitFuzi();

    FUZI_INPUT(input)
        FUZI_INPUT_("x", .6f)
        FUZI_INPUT_("dx", -.4f)
    FUZI_INPUT_END;

    FuzifiedValues results = FuzificationProblem(fuzis)(input);
    Utilities::printResults(results);

    Rules rules = InitRules();

    printf("\n");
    Utilities::printRulesMatrix(results, rules);
    printf("\n");

    Inference infComputer;
    infComputer << results["dx"] << results["x"];
    FloatMatrix const& inferenceMatrix = *infComputer;
    Utilities::printInferenceMatrix(results, inferenceMatrix);
 
    Fuzi::NamedResults composed = CompositionProblem(rules)(inferenceMatrix);
    printf("\ny:\n");
    Utilities::printNamedResults(composed);
    printf("\n");
}

#ifdef TESTS
#include "tests.ixx"
#endif

int main(
        int argc,
        char* argv[])
{
#ifdef TESTS
    test1();
    test2();
#endif

    clean_main();

    return 0;
}
