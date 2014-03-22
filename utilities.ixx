// Author: Vlad Mesco
// Date: Sat Mar 22 11:17:10 EET 2014
// Description: print utilities
#include <cstdio>
#include "fuzi.hxx"
#include "fuzification.hxx"
#include "multidimmatrix.hpp"

namespace Utilities {

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

} // namespace
