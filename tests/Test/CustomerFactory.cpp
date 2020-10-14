#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <classes/CustomerFactory.hpp>

using ::testing::Return;
using ::testing::Throw;
using ::testing::_;

class UsernameGeneratorMock : public UsernameGeneratorInterface {

public:

    MOCK_METHOD2(generate, std::string(std::string, std::string));

};

TEST(CustomerFactoryTest, test_returning_customer) {
    std::string dummyName = "genuse1823";

    auto generatorMock = std::make_shared<UsernameGeneratorMock>();
    EXPECT_CALL(*generatorMock, generate(_, _))
            .WillOnce(Return(dummyName));

    CustomerFactory factory(generatorMock);
    auto customer = factory.create("John", "Doe", "john@doe.com", "secret");

    ASSERT_EQ(customer.getUsername(), dummyName);
}

TEST(CustomerFactoryTest, test_throwing_exception) {
    std::string dummyName = "genuse1823";

    auto generatorMock = std::make_shared<UsernameGeneratorMock>();
    EXPECT_CALL(*generatorMock, generate(_, _))
            .WillOnce(Throw(NameIsTooShortException("Name 'John' is too short")));

    CustomerFactory factory(generatorMock);
    ASSERT_THROW(factory.create("John", "Doe", "john@doe.com", "secret"), NameIsTooShortException);
}