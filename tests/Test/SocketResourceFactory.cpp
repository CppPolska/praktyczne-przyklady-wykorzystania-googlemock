#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <exception>

#include <classes/SocketResourceFactory.hpp>

using ::testing::_;
using ::testing::Return;
using ::testing::SetErrnoAndReturn;

class SocketResourceFactoryMock : public SocketResourceFactoryInterface {

public:

    MOCK_METHOD2(create, SocketResource(std::string, std::string));

};

TEST(SocketResourceFactory, test_resource_created_successfully) {
    auto socketFactoryMock = std::make_shared<SocketResourceFactoryMock>();

    EXPECT_CALL(*socketFactoryMock, create(_, _))
            .WillOnce(SetErrnoAndReturn(0, SocketResource()));

    SocketConnection connection(socketFactoryMock);

    ASSERT_TRUE(connection.connect("http://example.com", "80"));
    ASSERT_EQ(connection.getErrorMessage(), "");
}

TEST(SocketResourceFactory, test_connection_timeout) {
    auto socketFactoryMock = std::make_shared<SocketResourceFactoryMock>();

    EXPECT_CALL(*socketFactoryMock, create(_, _))
            .WillOnce(SetErrnoAndReturn(ETIMEDOUT, SocketResource()));

    SocketConnection connection(socketFactoryMock);

    ASSERT_FALSE(connection.connect("http://example.com", "80"));
    ASSERT_EQ(connection.getErrorMessage(), "Connection timeout");
}

TEST(SocketResourceFactory, test_connection_refused) {
    auto socketFactoryMock = std::make_shared<SocketResourceFactoryMock>();

    EXPECT_CALL(*socketFactoryMock, create(_, _))
            .WillOnce(SetErrnoAndReturn(ECONNREFUSED, SocketResource()));

    SocketConnection connection(socketFactoryMock);

    ASSERT_FALSE(connection.connect("http://example.com", "80"));
    ASSERT_EQ(connection.getErrorMessage(), "Connection refused");
}

TEST(SocketResourceFactory, test_authentication_error) {
    auto socketFactoryMock = std::make_shared<SocketResourceFactoryMock>();

    EXPECT_CALL(*socketFactoryMock, create(_, _))
            .WillOnce(SetErrnoAndReturn(EAUTH, SocketResource()));

    SocketConnection connection(socketFactoryMock);

    ASSERT_FALSE(connection.connect("http://example.com", "80"));
    ASSERT_EQ(connection.getErrorMessage(), "Authentication error");
}

TEST(SocketResourceFactory, test_unexpected_error) {
    auto socketFactoryMock = std::make_shared<SocketResourceFactoryMock>();

    EXPECT_CALL(*socketFactoryMock, create(_, _))
            .WillOnce(SetErrnoAndReturn(ECONNABORTED, SocketResource()));

    SocketConnection connection(socketFactoryMock);

    ASSERT_FALSE(connection.connect("http://example.com", "80"));
    ASSERT_EQ(connection.getErrorMessage(), "Unexpected error");
}