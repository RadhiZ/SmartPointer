//
//  main.cpp
//  SmartPointer
//
//  Created by Mohamed Radhi ZOUAOUI on 19/04/2015.
//  Copyright (c) 2015 Mohamed Radhi ZOUAOUI. All rights reserved.
//

/* Test code for the SmartPointer assignment.
 * Use this test suite to check your smart pointer assignment before submitting.  When grading
 * your submissions, I will use this code, plus some more rigorous tests, to check its validity.
 * Be sure to test beyond what I've provided here - what other tests can you come up with?
 */

#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <string>
#include "SmartPointer.h"

using namespace std;

static const int WIDTH = 75;

/* Class that tracks the number of active instances remaining. */
class Counter
{
public:
    Counter()  : value(0) { ++numInstances; }
    ~Counter() { --numInstances; }
    
    static int getNumInstances()
    {
        return numInstances;
    }
    
    static void resetNumInstances()
    {
        numInstances = 0;
    }
    
    int getValue() const
    {
        return value;
    }
    
    void setValue(int value)
    {
        this->value = value;
    }
    
private:
    static int numInstances;
    int value;
    
    /* Disallow copy */
    Counter(const Counter &other);
    Counter& operator =(const Counter &other);
};

int Counter::numInstances = 0;

void CheckCondition(bool shouldBeTrue, const string &message)
{
    if(!shouldBeTrue)
        cout << "FAILED: " << message << endl;
    else
        cout << "PASS:   " << message << endl;
}

void PrintTestHeader(const string &str)
{
    cout << endl << str << endl;
    cout << setw(WIDTH) << setfill('=') << "" << setfill(' ') << endl;
}

/* Checks to make sure the basic functionality works without stress. */
void BasicFunctionalityTest()
{
    PrintTestHeader("Basic Functionality Test");
    /* New scope to cause variables to deallocate in between. */
    {
        SmartPointer<Counter> basicPointer(new Counter);
        CheckCondition(basicPointer.getSharedCount() == 1, "Shared count of new pointer should be one.");
        CheckCondition(basicPointer.get() != NULL, "Stored resource of basic pointer should be non-NULL.");
        CheckCondition(Counter::getNumInstances() == 1, "There should be exactly one active instance of the Counter class.");
        
        basicPointer.get()->setValue(137);
        
        CheckCondition(basicPointer.get()->getValue() == 137, "Stored value should be 137");
    }
    CheckCondition(Counter::getNumInstances() == 0, "There should not be any instances of Counter remaining.");
    Counter::resetNumInstances();
    
    {
        SmartPointer<Counter> basicPointer(new Counter);
        SmartPointer<Counter> copyPointer = basicPointer;
        
        CheckCondition(Counter::getNumInstances() == 1, "There should only be one Counter class.");
        CheckCondition(basicPointer.getSharedCount() == 2, "There should be two shared pointers.");
        CheckCondition(copyPointer.getSharedCount() == 2, "The copy pointer should have reference count 2.");
        CheckCondition(basicPointer.get() == copyPointer.get(), "The two pointers should store the same resource.");
    }
    CheckCondition(Counter::getNumInstances() == 0, "There should not be any instances of Counter remaining.");
    Counter::resetNumInstances();
    
    {
        SmartPointer<Counter> basicPointer(new Counter);
        SmartPointer<Counter> copyPointer(new Counter);
        
        CheckCondition(Counter::getNumInstances() == 2, "There should only be two Counter classes.");
        copyPointer = basicPointer;
        CheckCondition(Counter::getNumInstances() == 1, "There should only be one Counter class.");
        
        CheckCondition(basicPointer.getSharedCount() == 2, "There should be two shared pointers.");
        CheckCondition(copyPointer.getSharedCount() == 2, "The copy pointer should have reference count 2.");
        CheckCondition(basicPointer.get() == copyPointer.get(), "The two pointers should store the same resource.");
    }
    
    CheckCondition(Counter::getNumInstances() == 0, "There should not be any instances of Counter remaining.");
    Counter::resetNumInstances();
    {
        SmartPointer<Counter> basicPointer(new Counter);
        SmartPointer<Counter> copyPointer = basicPointer;
        
        CheckCondition(Counter::getNumInstances() == 1, "There should only be one Counter class.");
        CheckCondition(basicPointer.getSharedCount() == 2, "There should be two shared pointers.");
        CheckCondition(copyPointer.getSharedCount() == 2, "The copy pointer should have reference count 2.");
        CheckCondition(basicPointer.get() == copyPointer.get(), "The two pointers should store the same resource.");
        
        basicPointer.reset(new Counter);
        
        CheckCondition(Counter::getNumInstances() == 2, "There should be two Counter classes.");
        CheckCondition(basicPointer.getSharedCount() == 1, "There should only be one reference count in original pointer.");
        CheckCondition(copyPointer.getSharedCount() == 1, "There should only be one reference count in copy pointer");
        CheckCondition(basicPointer.get() != copyPointer.get(), "The two pointers should not store the same resource.");
    }
    CheckCondition(Counter::getNumInstances() == 0, "There should not be any instances of Counter remaining.");
    Counter::resetNumInstances();
}

void ModerateFunctionalityTest()
{
    PrintTestHeader("Moderate Functionality Test");
    {
        SmartPointer<Counter> basicPointer(new Counter);
        CheckCondition(basicPointer.getSharedCount() == 1, "Shared count of new pointer should be one.");
        CheckCondition(basicPointer.get() != NULL, "Stored resource of basic pointer should be non-NULL.");
        CheckCondition(Counter::getNumInstances() == 1, "There should be exactly one active instance of the Counter class.");
        
        Counter *oldValue = basicPointer.get();
        basicPointer = basicPointer; // Self-assign.
        
        CheckCondition(basicPointer.getSharedCount() == 1, "Shared count of new pointer should be one.");
        CheckCondition(basicPointer.get() != NULL, "Stored resource of basic pointer should be non-NULL.");
        CheckCondition(Counter::getNumInstances() == 1, "There should be exactly one active instance of the Counter class.");
        CheckCondition(basicPointer.get() == oldValue, "The pointer after self-assignment should be the same as before.");
    }
    CheckCondition(Counter::getNumInstances() == 0, "There should not be any instances of Counter remaining.");
    Counter::resetNumInstances();
    
    {
        SmartPointer<Counter> basicPointer(new Counter);
        CheckCondition(basicPointer.getSharedCount() == 1, "Shared count of new pointer should be one.");
        CheckCondition(basicPointer.get() != NULL, "Stored resource of basic pointer should be non-NULL.");
        CheckCondition(Counter::getNumInstances() == 1, "There should be exactly one active instance of the Counter class.");
        
        Counter *oldValue = basicPointer.get();
        SmartPointer<Counter> two(new Counter), three(new Counter);
        CheckCondition(Counter::getNumInstances() == 3, "There should be exactly three active instances of the Counter class.");
        
        three = two = basicPointer; // Chained assign
        
        CheckCondition(basicPointer.getSharedCount() == 3, "Shared count of new pointer should be one.");
        CheckCondition(basicPointer.get() != NULL, "Stored resource of basic pointer should be non-NULL.");
        CheckCondition(Counter::getNumInstances() == 1, "There should be exactly one active instance of the Counter class.");
        CheckCondition(basicPointer.get() == three.get(), "First and third pointers should be equal.");
        CheckCondition(basicPointer.get() == two.get(), "First and second pointers should be equal.");
        
        oldValue = basicPointer.get();
        
        basicPointer.reset(new Counter);
        
        CheckCondition(Counter::getNumInstances() == 2, "There should be exactly two active instances of the Counter class.");
        CheckCondition(basicPointer.getSharedCount() == 1, "Shared count of original pointer should be one.");
        CheckCondition(basicPointer.get() != oldValue, "Original pointer should not have original value.");
        CheckCondition(two.get() == oldValue, "Second pointer should have same value as original.");
        CheckCondition(three.get() == oldValue, "Third pointer should have same value as original.");
        
        three.reset(new Counter);
        
        CheckCondition(Counter::getNumInstances() == 3, "There should be exactly three active instances of the Counter class.");
        CheckCondition(three.getSharedCount() == 1, "Shared count of third pointer should be one.");
        CheckCondition(two.getSharedCount() == 1, "Shared count of second pointer should be one.");
        CheckCondition(three.get() != oldValue, "Third pointer should not have original value.");
        CheckCondition(two.get() == oldValue, "Second pointer should have same value as original.");
    }
    CheckCondition(Counter::getNumInstances() == 0, "There should not be any instances of Counter remaining.");
    Counter::resetNumInstances();
}

int main()
{
    BasicFunctionalityTest();
    ModerateFunctionalityTest();
    return 0;	
}