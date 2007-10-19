/* Generated file, do not edit */

#ifndef CXXTEST_RUNNING
#define CXXTEST_RUNNING
#endif

#define _CXXTEST_HAVE_STD
#include <cxxtest/TestListener.h>
#include <cxxtest/TestTracker.h>
#include <cxxtest/TestRunner.h>
#include <cxxtest/RealDescriptions.h>
#include <cxxtest/ErrorPrinter.h>

int main() {
 return CxxTest::ErrorPrinter().run();
}
#include "test/ordered_list_test.h"

static TestOrderedList suite_TestOrderedList;

static CxxTest::List Tests_TestOrderedList = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_TestOrderedList( "test/ordered_list_test.h", 6, "TestOrderedList", suite_TestOrderedList, Tests_TestOrderedList );

static class TestDescription_TestOrderedList_testPushPop : public CxxTest::RealTestDescription {
public:
 TestDescription_TestOrderedList_testPushPop() : CxxTest::RealTestDescription( Tests_TestOrderedList, suiteDescription_TestOrderedList, 9, "testPushPop" ) {}
 void runTest() { suite_TestOrderedList.testPushPop(); }
} testDescription_TestOrderedList_testPushPop;

#include "test/slot_test.h"

static TestSlot suite_TestSlot;

static CxxTest::List Tests_TestSlot = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_TestSlot( "test/slot_test.h", 28, "TestSlot", suite_TestSlot, Tests_TestSlot );

static class TestDescription_TestSlot_testSingleConnection : public CxxTest::RealTestDescription {
public:
 TestDescription_TestSlot_testSingleConnection() : CxxTest::RealTestDescription( Tests_TestSlot, suiteDescription_TestSlot, 31, "testSingleConnection" ) {}
 void runTest() { suite_TestSlot.testSingleConnection(); }
} testDescription_TestSlot_testSingleConnection;

static class TestDescription_TestSlot_testManyConnections : public CxxTest::RealTestDescription {
public:
 TestDescription_TestSlot_testManyConnections() : CxxTest::RealTestDescription( Tests_TestSlot, suiteDescription_TestSlot, 42, "testManyConnections" ) {}
 void runTest() { suite_TestSlot.testManyConnections(); }
} testDescription_TestSlot_testManyConnections;

#include <cxxtest/Root.cpp>
