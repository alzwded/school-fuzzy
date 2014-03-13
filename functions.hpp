// Author: Vlad Mesco
// Date: Wed Mar 12 21:46:09 EET 2014
// Description: Fuzzy Saturation Functions

#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

// base classes and stuff
class IFuzzyFunction
{
public:
    virtual float operator()(float const u) const =0;
    virtual ~IFuzzyFunction() {}
};

template<class T>
IFuzzyFunction* const
NewFunction(
        T const& parameters);

// LeftSaturation
typedef struct {
    float a, b;
} LeftSaturationDefinition;

template<>
IFuzzyFunction* const
NewFunction(
        LeftSaturationDefinition const&);
class LeftSaturation
: public IFuzzyFunction
{
    float const a_, b_;

    LeftSaturation(
            float const a,
            float const b)
        : a_(a)
        , b_(b)
    {}
    friend IFuzzyFunction* const NewFunction<LeftSaturationDefinition>(LeftSaturationDefinition const&);

public:
    virtual float operator()(float const u) const
    {
        return (u < a_) ? 1.f :
            (u < b_) ? ((b_ - u) / (b_ - a_)) :
            0.f;
    }
};

template<>
IFuzzyFunction* const
NewFunction(
        LeftSaturationDefinition const& parameters)
{
    return new LeftSaturation(
                parameters.a,
                parameters.b);
}

// Right Saturation
typedef struct {
    float a, b;
} RightSaturationDefinition;

template<>
IFuzzyFunction* const
NewFunction(
        RightSaturationDefinition const&);
class RightSaturation
: public IFuzzyFunction
{
    float const a_, b_;

    RightSaturation(
            float const a,
            float const b)
        : a_(a)
        , b_(b)
    {}
    friend IFuzzyFunction* const NewFunction<RightSaturationDefinition>(RightSaturationDefinition const&);

public:
    virtual float operator()(float const u) const
    {
        return (u < a_) ? 0.f :
            (u < b_) ? ((u - a_) / (b_ - a_)) :
            1.f;
    }
};

template<>
IFuzzyFunction* const
NewFunction(
        RightSaturationDefinition const& parameters)
{
    return new RightSaturation(
                parameters.a,
                parameters.b);
}

// Triangle
typedef struct {
    float a, b, c;
} TriangleDefinition;

template<>
IFuzzyFunction* const
NewFunction(
        TriangleDefinition const&);
class Triangle
: public IFuzzyFunction
{
    float const a_, b_, c_;

    Triangle(
            float const a,
            float const b,
            float const c)
        : a_(a)
        , b_(b)
        , c_(c)
    {}
    friend IFuzzyFunction* const NewFunction<TriangleDefinition>(TriangleDefinition const&);

public:
    virtual float operator()(float const u) const
    {
        return (u < a_) ? 0.f :
            (u < b_) ? ((u - a_) / (b_ - a_)) :
            (u < c_) ? ((c_ - u) / (c_ - b_)) :
            0.f;
    }
};

template<>
IFuzzyFunction* const
NewFunction(
        TriangleDefinition const& parameters)
{
    return new Triangle(
                parameters.a,
                parameters.b,
                parameters.c);
}

// Trapezium
typedef struct {
    float a, b, c, d;
} TrapeziumDefinition;

template<>
IFuzzyFunction* const
NewFunction(
        TrapeziumDefinition const&);
class Trapezium
: public IFuzzyFunction
{
    float const a_, b_, c_, d_;

    Trapezium(
            float const a,
            float const b,
            float const c,
            float const d)
        : a_(a)
        , b_(b)
        , c_(c)
        , d_(d)
    {}
    friend IFuzzyFunction* const NewFunction<TrapeziumDefinition>(TrapeziumDefinition const&);

public:
    virtual float operator()(float const u) const
    {
        return (u < a_) ? 0.f :
            (u < b_) ? ((u - a_) / (b_ - a_)) :
            (u < c_) ? 1.f :
            (u < d_) ? ((d_ - u) / (d_ - c_)) :
            0.f;
    }
};

template<>
IFuzzyFunction* const
NewFunction(
        TrapeziumDefinition const& parameters)
{
    return new Trapezium(
                parameters.a,
                parameters.b,
                parameters.c,
                parameters.d);
}

#endif
