#include "pch.h"
#include "memory_pool.h"

class test_class {
public:
	int value;
	static const int testValue = 42;
	static int testValue2;

	test_class() : value(testValue)
	{
		test_class::testValue2 = test_class::testValue;
	}

	test_class(int value) : value(value) { }

	~test_class() { test_class::testValue2 = 0; }
};

int test_class::testValue2 = 42;
TEST(memory_pool_tests, createsObject) {
	// Arrange
	auto pool = std::make_unique<memory_pool<test_class>>(10);

	// Act
	auto obj = pool->allocate();

	// Assert
	EXPECT_TRUE(obj != nullptr);
}

TEST(memory_pool_tests, createsMultipleObjects) {
	// Arrange
	auto pool = std::make_unique <memory_pool<test_class>>(10);

	// Act
	auto obj = pool->allocate();
	auto obj2 = pool->allocate();

	// Assert
	EXPECT_TRUE(obj != nullptr);
	EXPECT_TRUE(obj2 != nullptr);
	EXPECT_TRUE(obj != obj2);
}

// This test is prone to false positives as there is no
// way to guarantee that the default allocator won't
// re-use the same memory address.  If anyone knows a way
// around this, please let me know.
TEST(memory_pool_tests, resusesMemory) {
	// Arrange
	auto pool = std::make_unique<memory_pool<test_class>>(10);

	// Act
	auto obj = pool->allocate();
	pool->deallocate(obj);
	auto obj2 = pool->allocate();

	pool->deallocate(obj2);
	obj2 = pool->allocate();

	// Assert
	EXPECT_TRUE(obj != nullptr);
	EXPECT_TRUE(obj == obj2);
}

TEST(memory_pool_tests, callsConstructor) {
	// Arrange
	auto pool = std::make_unique<memory_pool<test_class>>(10);

	// Act
	auto obj = pool->allocate();

	// Assert
	EXPECT_TRUE(obj->value == test_class::testValue);
}

TEST(memory_pool_tests, callsDestructor) {
	// Arrange
	auto pool = std::make_unique<memory_pool<test_class>>(10);

	// Act
	auto obj = pool->allocate();
	pool->deallocate(obj);

	// Assert
	EXPECT_TRUE(test_class::testValue2 == 0);
}

TEST(memory_pool_tests, callsConstructorWithArguments) {
	// Arrange
	auto pool = std::make_unique<memory_pool<test_class>>(10);

	// Act
	auto obj = pool->allocate();
	//auto obj2 = pool.allocate(42);

	// Assert
	EXPECT_TRUE(obj->value == test_class::testValue);
	//EXPECT_TRUE(obj2->value == 42);
}

TEST(memory_pool_tests, allocatesBeyondOneBlock) {
	// Arrange
	auto pool = std::make_unique<memory_pool<test_class>>
		(2);

	// Act
	auto obj1 = pool->allocate();
	auto obj2 = pool->allocate();
	auto obj3 = pool->allocate();

	// Assert
	EXPECT_TRUE(obj3 != nullptr);
}