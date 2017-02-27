//Steeng: Sun Feb 26 16:01:28 EST 2017
#ifndef INCLUDED_VECTOR
#define INCLUDED_VECTOR
#include <Arduino.h> 

//Very basic implementation of C++ STL vector
template <class T>
class Vector
{
public:
    Vector():d_size(0), d_capacity(0), d_data(NULL){}
    
    ~Vector()
    {
        free(d_data);
        d_data = NULL;
    }
    
    Vector(const Vector& o):
    d_size(o.d_size), d_capacity(o.d_capacity), d_data(NULL)
    {
        d_data = (T*)malloc(d_capacity * sizeof(T));
        memcpy(d_data, o.d_data, d_size * sizeof(T));
    }

    Vector& operator=(const Vector& o)
    {
        d_size      = o.d_size;
        d_capacity  = o.d_capacity;
        d_data = (T*)malloc(d_capacity * sizeof(T));
        memcpy(d_data, o.d_data, d_size * sizeof(T));
        return *this;
    }
    
    size_t size()
    {
        return d_size; 
    }

    void push_back(const T& t)
    {
        if (d_size == d_capacity)
        {
            resize();
        }
        d_data[d_size++] = t;
    }

    const T& operator[](size_t i) const
    {
        return d_data[i];
    }
    
    T& operator[](size_t i)
    {
        return d_data[i];
    }
private:
    size_t  d_size;
    size_t  d_capacity;
    T*      d_data;

    void resize()
    {
        d_capacity = d_capacity ? d_capacity * 2 : 1; 
        T *newdata = (T*)malloc(d_capacity*sizeof(T)); 
        memcpy(newdata, d_data, d_size * sizeof(T)); 
        free(d_data); 
        d_data = newdata;
    }
};

#endif
//------ end of VECTOR_LIB.h ------
