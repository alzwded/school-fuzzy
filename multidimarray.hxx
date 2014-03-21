#ifndef MULTIDIMARRAY_HXX
#define MULTIDIMARRAY_HXX

#include <assert.h>
#include <map>
#include <string>
#include <deque>

template<typename T>
class MultiDimArray
{
    typedef T value_type;
    typedef MultiDimArray<value_type> my_type;
    typedef std::map<std::string, my_type> dimension_type;
    typedef typename dimension_type::value_type iterator_type;
    typedef typename std::map<std::string, my_type>::const_iterator dimension_type_const_iterator;
    value_type val_;
    dimension_type subrules_;
public:
    typedef std::deque<std::string> IndexSet;

    MultiDimArray() {}

    my_type& For(std::string const& name)
    {
        return subrules_[name];
    }

    void Set(value_type const& val)
    {
        val_ = val;
    }

    my_type& operator[](std::string const& name)
    {
        return For(name);
    }

    my_type const& operator[](std::string const& name) const
    {
        return Of(name);
    }

    my_type const& Of(std::string const& name) const
    {
        dimension_type_const_iterator i = subrules_.find(name);
        assert(i != subrules_.end());
        return i->second;
    }

    value_type const& Get() const
    {
        return val_;
    }

    value_type const& operator*() const
    {
        return Get();
    }

    value_type& At(IndexSet const& indexes)
    {
        my_type* self = this;
        for(IndexSet::const_iterator i = indexes.begin();
                i != indexes.end(); ++i)
        {
            self = &((*self).For(*i));
        }
        return self->val_;
    }

    value_type const& At(IndexSet const& indexes) const
    {
        my_type const* self = this;
        for(IndexSet::const_iterator i = indexes.begin();
                i != indexes.end(); ++i)
        {
            self = &((*self).Of(*i));
        }
        return **self;
    }

    struct iterator
    {
    private:
        mutable typename dimension_type::iterator i_;
    public:
        iterator(typename dimension_type::iterator const& i)
            : i_(i)
        {}

        iterator_type& operator*()
        {
            return *i_;
        }

        iterator_type* operator->()
        {
            return &*i_;
        }

        iterator operator++(int)
        {
            iterator ret(i_++);
            return ret;
        }

        iterator operator++()
        {
            ++i_;
            return *this;
        }

        bool operator!=(iterator const& other) const
        {
            return i_ != other.i_;
        }
    };
    typedef iterator const const_iterator;

    iterator begin()
    {
        return iterator(subrules_.begin());
    }

    const_iterator begin() const
    {
        return const_iterator(const_cast<my_type*>(this)->subrules_.begin());
    }

    iterator end()
    {
        return iterator(subrules_.end());
    }

    const_iterator end() const
    {
        return const_iterator(const_cast<my_type*>(this)->subrules_.end());
    }

    bool empty()
    {
        return subrules_.empty();
    }
};

typedef MultiDimArray<std::string> Rules;
typedef MultiDimArray<float> FloatMatrix;

#endif
