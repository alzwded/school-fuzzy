// Author: Vlad Mesco
// Date: Wed Mar 12 21:46:09 EET 2014
// Description: Fuzzy logic lab assignment

#include <assert.h>
#include <algorithm>
#include <memory>
#include <cstdio>
#include <map>
#include <string>

#include "functions.hpp"
#include "fuzi.hxx"
#include "fuzification.hxx"
#include "multidimmatrix.hpp"
#include "composition.hxx"
#include "inference.hxx"

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

#ifdef TESTS
    ret.insert(
            NamedFuzis::value_type("t", FuziBuilder()
                    << NamedVariable("a", NewFunction(dxPODefinition))
                    << NamedVariable("b", NewFunction(xPODefinition))));
#endif

    return ret;
}

static void printResults(FuzifiedValues const& values)
{
    for(FuzifiedValues::const_iterator i = values.begin();
            i != values.end(); ++i)
    {
        printf("%s:\n\t", i->first.c_str());
        for(Fuzi::NamedResults::const_iterator j = i->second.begin();
                j != i->second.end(); ++j)
        {
            printf("%s = %-7.2f",
                    j->first.c_str(),
                    j->second);
        }
        printf("\n");
    }
}

static void printNamedResults(Fuzi::NamedResults const& r)
{
    for(Fuzi::NamedResults::const_iterator i = r.begin();
            i != r.end(); ++i)
    {
        printf("%s = %-7.2f", i->first.c_str(), i->second);
    }
}

#ifdef TESTS
static void test1()
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

static void printRulesMatrix(
        FuzifiedValues& results,
        Rules const& mat)
{
    FloatMatrix::IndexSet indexes;

    printf("dx\\x");
    for(Fuzi::NamedResults::const_iterator j = results["x"].begin();
            j != results["x"].end(); ++j)
    {
        printf("%7s", j->first.c_str());
    }
    printf("\n");

    for(Fuzi::NamedResults::const_iterator i = results["dx"].begin();
            i != results["dx"].end(); ++i)
    {
        printf("%-3s ", i->first.c_str());
        indexes.push_back(i->first);
        for(Fuzi::NamedResults::const_iterator j = results["x"].begin();
                j != results["x"].end(); ++j)
        {
            indexes.push_back(j->first);
            printf("%7s", mat.At(indexes).c_str());
            indexes.pop_back();
        }
        indexes.pop_back();
        printf("\n");
    }
}

static void printInferenceMatrix(
        FuzifiedValues& results,
        FloatMatrix const& mat)
{
    FloatMatrix::IndexSet indexes;

    printf("dx\\x");
    for(Fuzi::NamedResults::const_iterator j = results["x"].begin();
            j != results["x"].end(); ++j)
    {
        printf("%7s", j->first.c_str());
    }
    printf("\n");

    for(Fuzi::NamedResults::const_iterator i = results["dx"].begin();
            i != results["dx"].end(); ++i)
    {
        printf("%-3s ", i->first.c_str());
        indexes.push_back(i->first);
        for(Fuzi::NamedResults::const_iterator j = results["x"].begin();
                j != results["x"].end(); ++j)
        {
            indexes.push_back(j->first);
            printf("%7.2f", mat.At(indexes));
            indexes.pop_back();
        }
        indexes.pop_back();
        printf("\n");
    }
}

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

#ifdef TESTS
    printf("%s\n", (*rules["NE"]["NE"]).c_str());
    Rules::IndexSet indexes;
    indexes.push_back("PO");
    indexes.push_back("NE");
    printf("%s\n", rules.At(indexes).c_str());
#endif

#ifdef TESTS
    FloatMatrix testInferenceMatrix;
    testInferenceMatrix.For("ZE").For("ZE").Set(std::min(results["x"]["ZE"], results["dx"]["ZE"]));
    printf("%7.2f\n", *testInferenceMatrix["ZE"]["ZE"]);

    testInferenceMatrix.At(indexes) = std::min(results["x"]["PO"], results["dx"]["NE"]);
    printf("%7.2f\n", testInferenceMatrix.Of("PO").Of("NE").Get());
    printf("%7.2f\n", testInferenceMatrix.At(indexes));
#endif

    printf("\n");
    printRulesMatrix(results, rules);
    printf("\n");

    Inference infComputer;
    infComputer << results["dx"] << results["x"];
    FloatMatrix const& inferenceMatrix = *infComputer;
    printInferenceMatrix(results, inferenceMatrix);

#ifdef TESTS
    printf("\ndx*x\n");
    for(Rules::iterator i = rules.begin(); i != rules.end(); ++i)
    {
        for(Rules::iterator j = i->second.begin(); j != i->second.end(); ++j)
        {
            printf("%sx%s: %s\n",
                    i->first.c_str(),
                    j->first.c_str(),
                    (*j->second).c_str());
        }
    }
#endif

    
    Fuzi::NamedResults composed = CompositionProblem(rules)(inferenceMatrix);
    printf("\ny:\n");
    printNamedResults(composed);
    printf("\n");

    return 0;
}
