#include "search_binary_tree.h"
#include <iostream>
#include <cassert>
using namespace std;

/**
@brief greater_than

Funtore usato per l'ordinamento decrescente.
**/
struct greater_than {
  unsigned int operator()(const unsigned int &a, const unsigned int &b) const {
    if (a > b) {
        return 1;
    }
    else if (a == b) {
        return 0;
    }
    else {
        return -1;
    }
  }
};

void testDefaultMethods() {
    cout << "Testing default constructor..." << endl;
    search_binary_tree<uint, greater_than> sbt1;
    cout << "  checking the size...";
    assert(sbt1.get_size() == 0);
    cout << sbt1.get_size() << " OK!"<< endl;

    cout << "Testing root constructor..." << endl;
    search_binary_tree<uint, greater_than> sbt2(42);
    cout << "  checking the size...";
    assert(sbt2.get_size() == 1);
    cout << sbt2.get_size() << " OK!" << endl;

    cout << "Testing operator=...";
    sbt2.insert(69);
    sbt2.insert(666);
    search_binary_tree<uint, greater_than> sbt3 = sbt2;
    sbt3.insert(420);
    assert(sbt2.get_size() == 3 && sbt3.get_size() == 4);
    assert(sbt3.search(42));
    assert(sbt3.search(69));
    assert(sbt3.search(666));
    assert(sbt3.search(420));
    assert(!sbt2.search(420));
    search_binary_tree<uint, greater_than> sbt4;
    sbt4.insert(1);
    sbt4.insert(2);
    sbt4.insert(3);
    sbt4 = sbt2;
    assert(!sbt4.search(1) && !sbt4.search(2) && !sbt4.search(3));
    assert(sbt4.search(42));
    assert(sbt4.search(69));
    assert(sbt4.search(666));
    assert(sbt4.get_size() == 3);
    cout << " OK!" << endl;

    cout << "Testing copy constructor..." << endl;
    sbt1.insert(42);
    sbt1.insert(10);
    sbt1.insert(100);
    sbt1.insert(80);
    sbt1.insert(200);
    sbt1.insert(5);
    sbt1.insert(250);
    sbt1.insert(300);
    sbt1.insert(120);
    sbt1.insert(90);
    search_binary_tree<uint, greater_than> sbt5(sbt1);
    cout << "  checking the size...";
    assert(sbt5.get_size() == 10);
    cout << sbt5.get_size() << " OK!" << endl;

    cout << "  checking the size after an insertion in the old tree...";
    sbt1.insert(85);
    assert(sbt5.get_size() == 10);
    cout << sbt5.get_size() << " OK!" << endl;
}

void testInserionSearch() {
    cout << "Testing insert method..." << endl;
    search_binary_tree<uint, greater_than> sbt1;
    sbt1.insert(42);
    sbt1.insert(10);
    sbt1.insert(100);
    sbt1.insert(80);
    sbt1.insert(200);
    sbt1.insert(5);
    sbt1.insert(250);
    sbt1.insert(300);
    sbt1.insert(120);
    sbt1.insert(90);
    sbt1.insert(85);
    sbt1.insert(70);
    sbt1.insert(75);
    sbt1.insert(400);
    cout << "  checking the size...";
    assert(sbt1.get_size() == 14);
    cout << sbt1.get_size() << " OK!" << endl;

    cout << "  checking the size after equal value insertion...";
    int duplicates = 0;
    try {
        sbt1.insert(10);
    } catch(std::runtime_error) {
        duplicates++;
    }
    try {
        sbt1.insert(42);
    } catch(std::runtime_error) {
        duplicates++;
    }
        sbt1.insert(50);
    try {
        sbt1.insert(100);
    } catch(std::runtime_error) {
        duplicates++;
    }
    assert(sbt1.get_size() == 15 && duplicates == 3);
    cout << sbt1.get_size() << " with " << duplicates << " duplicates. OK!" << endl;
    
    cout << "Testing search method on all the inserted values...";
    assert(sbt1.search(5) && sbt1.search(10) && sbt1.search(42) && sbt1.search(70) && sbt1.search(75));
    assert(sbt1.search(80) && sbt1.search(85) && sbt1.search(90) && sbt1.search(100) && sbt1.search(120));
    assert(sbt1.search(200) && sbt1.search(250) && sbt1.search(300) && sbt1.search(400) && sbt1.search(50));
    cout << " OK!" << endl;
    
    cout << "Testing search method on some random values...";
    assert(!sbt1.search(420) && !sbt1.search(666) && !sbt1.search(69));
    cout << " OK!" << endl;
}

void testSubtree() {
    cout << "Testing subtree method...";
    search_binary_tree<uint, greater_than> sbt1;
    sbt1.insert(42);
    sbt1.insert(10);
    sbt1.insert(100);
    sbt1.insert(80);
    sbt1.insert(200);
    sbt1.insert(5);
    sbt1.insert(250);
    sbt1.insert(300);
    sbt1.insert(120);
    sbt1.insert(90);
    sbt1.insert(85);
    sbt1.insert(70);
    sbt1.insert(75);
    sbt1.insert(400);
    search_binary_tree<uint, greater_than> sbt2 = sbt1.subtree(90);
    sbt2.insert(800);
    assert(sbt2.search(85));
    assert(sbt2.search(800));
    assert(!sbt2.search(75));
    assert(!sbt1.search(800));
    assert(sbt2.get_size() == 3);
    cout << sbt2.get_size() << " OK!" << endl;
    cout << "Printing a tree..." << endl << sbt1;
    cout << "Printing the subtree..." << endl << sbt2;
}

void testIterators() {
    cout << "Testing iterators...";
    search_binary_tree<uint, greater_than> sbt1;
    sbt1.insert(42);
    sbt1.insert(10);
    sbt1.insert(100);
    sbt1.insert(80);
    sbt1.insert(200);
    sbt1.insert(5);
    sbt1.insert(250);
    sbt1.insert(300);
    sbt1.insert(120);
    sbt1.insert(90);
    sbt1.insert(85);
    sbt1.insert(70);
    sbt1.insert(75);
    sbt1.insert(400);
    search_binary_tree<uint, greater_than>::const_iterator it, ite;
    it = sbt1.begin();
    ite = sbt1.end();
    while (it != ite) {
        cout << it.operator*().value << " ";
        it++;
    }
    cout << endl;
}

int main() {
  testDefaultMethods();
  testInserionSearch();
  testSubtree();
  testIterators();
}
