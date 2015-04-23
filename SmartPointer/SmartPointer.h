//
//  SmartPointer.h
//  SmartPointer
//
//  Created by Mohamed Radhi ZOUAOUI on 19/04/2015.
//  Copyright (c) 2015 Mohamed Radhi ZOUAOUI. All rights reserved.
//

#ifndef __SmartPointer__SmartPointer__
#define __SmartPointer__SmartPointer__

template<typename T>
class SmartPointer
{
public:
    explicit SmartPointer(T* memory);
    SmartPointer(const SmartPointer& other);
    SmartPointer& operator=(const SmartPointer& other);
    ~SmartPointer();
    
    T& operator *  () const;
    T* operator -> () const;
    
    T* get() const;
    void reset(T* memory);
    int getSharedCount() const;
    
private:
    struct Intermediary
    {
        T* resource;
        int nbRefs;
    };
    
    Intermediary* intermed;
    
    void copyOther(const SmartPointer& other);
    void clear();
};
//
//
template<typename T>
SmartPointer<T>::SmartPointer(T* memory)
{
    intermed = new Intermediary;
    intermed->resource = memory;
    intermed->nbRefs = 1;
}
//
template<typename T>
int SmartPointer<T>::getSharedCount() const
{
    return intermed->nbRefs;
}
//
template<typename T>
T* SmartPointer<T>::get() const
{
    return intermed->resource;
}
//
template<typename T>
void SmartPointer<T>::clear()
{
    if (getSharedCount()<=1) {
        delete intermed->resource;
        delete intermed;
    }
    else
    {
        --intermed->nbRefs;
    }
}
//
template<typename T>
void SmartPointer<T>::copyOther(const SmartPointer& other)
{
    intermed = other.intermed;
    ++intermed->nbRefs;
}
//
template<typename T>
SmartPointer<T>::SmartPointer(const SmartPointer& other)
{
    copyOther(other);
}
//
template<typename T>
SmartPointer<T>& SmartPointer<T>::operator=(const SmartPointer& other)
{
    if(this!=&other)
    {
        clear();
        copyOther(other);
    }
    return *this;
}
//
template<typename T>
SmartPointer<T>::~SmartPointer()
{
    clear();
}
//
template<typename T>
void SmartPointer<T>::reset(T* memory)
{
    clear();
    intermed = new Intermediary;
    intermed->resource = memory;
    intermed->nbRefs = 1;
}
//
template <typename T> T& SmartPointer<T>::operator * () const
{
    return *intermed->resource;
}
//
template <typename T> T* SmartPointer<T>::operator -> () const
{
    return intermed->resource;
}
#endif /* defined(__SmartPointer__SmartPointer__) */
