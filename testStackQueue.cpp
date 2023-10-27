#define CATCH_CONFIG_MAIN
#include <iostream>
#include "cs221util/catch.hpp"
#include "stack.h"
#include "queue.h"

//using namespace cs221util;
using namespace std;

TEST_CASE("stack::basic functions","[weight=1][part=stack]"){
    //cout << "Testing Stack..." << endl;
    Stack<int> intStack;
    vector<int> result;
    vector<int> expected;
    for (int i = 10; i > 0; i--) {
        expected.push_back(i);
    }
    for (int i = 1; i <= 10; i++) {
        intStack.push(i);
    }
    while (!intStack.isEmpty()) {
        result.push_back(intStack.pop());
    }
    REQUIRE( result == expected);
}
TEST_CASE("queue::basic functions","[weight=1][part=queue]"){
    Queue<int> intQueue;
    vector<int> result;
    vector<int> expected;
    for (int i = 1; i <= 10; i++) {
        expected.push_back(i);
    }
    for (int i = 1; i <= 10; i++) {
        intQueue.enqueue(i);
    }
    while (!intQueue.isEmpty()) {
        result.push_back(intQueue.dequeue());
    }
    REQUIRE( result == expected);
}

TEST_CASE("deque::resizing pushpopLR") {
    Deque<int> deque;

    // pushing elements to the right.
    for (int i = 1; i <= 10; i++) {
        deque.pushR(i);
    }

    // deque = {1, 2, 3, ... 10}

    // popping 5 elements from the left.
    for (int i = 1; i <= 5; i++) {
        int item = deque.popL();
        REQUIRE(item == i);  
    }

    // deque = {6, 7, 8, 9, 10}


    for (int i = 11; i <= 20; i++) {
        deque.pushR(i);
    }

    // deque = {6, 7, 8, 9, 10, 11, ... 20}

    // popping a few elements from the right
    for (int i = 20; i >= 15; i--) {
        int item = deque.popR();
        REQUIRE(item == i);  
    }

    // deque = {6, 7, 8, 9, 10, 11, 12, 13, 14}

    std::cout << "All test cases passed!" << std::endl;
}

TEST_CASE("deque::resizing refill") {
    Deque<int> deque;

    // pushing elements to the right
    for (int i = 1; i <= 10; i++) {
        deque.pushR(i);
    }

    // deque = {1, 2, 3, ... 10}

    // popping all elements from the left to potentially cause a resize
    for (int i = 1; i <= 10; i++) {
        int item = deque.popL();
        assert(item == i);  
    }

    // deque should be empty
    assert(deque.isEmpty());

    // Refill the deque.
    for (int i = 11; i <= 20; i++) {
        deque.pushR(i);
    }

    // deque = {11, 12, ... 20}

    // check  contents by popping from the left
    for (int i = 11; i <= 20; i++) {
        int item = deque.popL();
        assert(item == i);  
    }

    std::cout << "All test cases passed!" << std::endl;
}


TEST_CASE("stack::big miscellaneous ops") { 

    vector<int> expected;
    vector<int> result;
    Stack<int> inputStack;
   

    for(int i = 0; i < 10; i++) {
        int r1 = (rand() % 4);
       
        expected.push_back(i);
        inputStack.push(i);
        int r2 = (rand() % 4);
        if( r2 == r1) {
         expected.pop_back();
         inputStack.pop();
        }
    }

    reverse(expected.begin(), expected.end());

    for(unsigned int i = 0; i < expected.size(); i++){
        result.push_back(inputStack.pop());
    }
    REQUIRE(result == expected);
}
