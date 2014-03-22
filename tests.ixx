// Author: Vlad Mesco
// Date: Sat Mar 22 11:17:10 EET 2014
// Description: tests...
static void test1()
{
    NamedFuzis fuzis = InitFuzi();

    Fuzi::NamedResults xResults = fuzis["x"](0.6f);
    Fuzi::NamedResults dxResults = fuzis["dx"](-0.4f);

    printf(" t: ");
    Utilities::printNamedResults(fuzis["t"](0.7f));
    printf("\n");

    printf(" x: ");
    Utilities::printNamedResults(xResults);
    printf("\n");

    printf("dx: ");
    Utilities::printNamedResults(dxResults);
    printf("\n");
}

static void test2()
{
    NamedFuzis fuzis = InitFuzi();

    RightSaturationDefinition
        dxPODefinition = { .25f, .5f };
    RightSaturationDefinition
        xPODefinition = { .5f, 1.f };
    fuzis.insert(
            NamedFuzis::value_type("t", FuziBuilder()
                    << NamedVariable("a", NewFunction(dxPODefinition))
                    << NamedVariable("b", NewFunction(xPODefinition))));

    FUZI_INPUT(input)
        FUZI_INPUT_("x", .6f)
        FUZI_INPUT_("dx", -.4f)
    FUZI_INPUT_END;

    FuzifiedValues results = FuzificationProblem(fuzis)(input);
    Utilities::printResults(results);

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

    printf("%s\n", (*rules["NE"]["NE"]).c_str());
    Rules::IndexSet indexes;
    indexes.push_back("PO");
    indexes.push_back("NE");
    printf("%s\n", rules.At(indexes).c_str());

    FloatMatrix testInferenceMatrix;
    testInferenceMatrix.For("ZE").For("ZE").Set(std::min(results["x"]["ZE"], results["dx"]["ZE"]));
    printf("%7.2f\n", *testInferenceMatrix["ZE"]["ZE"]);

    testInferenceMatrix.At(indexes) = std::min(results["x"]["PO"], results["dx"]["NE"]);
    printf("%7.2f\n", testInferenceMatrix.Of("PO").Of("NE").Get());
    printf("%7.2f\n", testInferenceMatrix.At(indexes));

    printf("\n");
    Utilities::printRulesMatrix(results, rules);
    printf("\n");

    Inference infComputer;
    infComputer << results["dx"] << results["x"];
    FloatMatrix const& inferenceMatrix = *infComputer;
    Utilities::printInferenceMatrix(results, inferenceMatrix);

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

    
    Fuzi::NamedResults composed = CompositionProblem(rules)(inferenceMatrix);
    printf("\ny:\n");
    Utilities::printNamedResults(composed);
    printf("\n");
}
