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
} LeftSaturationParameters;

template<>
IFuzzyFunction* const
NewFunction(
        LeftSaturationParameters const&);
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
    friend IFuzzyFunction* const NewFunction<LeftSaturationParameters>(LeftSaturationParameters const&);

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
        LeftSaturationParameters const& parameters)
{
    return new LeftSaturation(
                parameters.a,
                parameters.b);
}

// Right Saturation
typedef struct {
    float a, b;
} RightSaturationParameters;

template<>
IFuzzyFunction* const
NewFunction(
        RightSaturationParameters const&);
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
    friend IFuzzyFunction* const NewFunction<RightSaturationParameters>(RightSaturationParameters const&);

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
        RightSaturationParameters const& parameters)
{
    return new RightSaturation(
                parameters.a,
                parameters.b);
}

// Triangle
typedef struct {
    float a, b, c;
} TriangleParameters;

template<>
IFuzzyFunction* const
NewFunction(
        TriangleParameters const&);
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
    friend IFuzzyFunction* const NewFunction<TriangleParameters>(TriangleParameters const&);

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
        TriangleParameters const& parameters)
{
    return new Triangle(
                parameters.a,
                parameters.b,
                parameters.c);
}

// Trapezium
typedef struct {
    float a, b, c, d;
} TrapeziumParameters;

template<>
IFuzzyFunction* const
NewFunction(
        TrapeziumParameters const&);
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
    friend IFuzzyFunction* const NewFunction<TrapeziumParameters>(TrapeziumParameters const&);

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
        TrapeziumParameters const& parameters)
{
    return new Trapezium(
                parameters.a,
                parameters.b,
                parameters.c,
                parameters.d);
}

#endif
