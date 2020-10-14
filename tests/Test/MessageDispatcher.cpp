#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <classes/MessageDispatcher.hpp>

using ::testing::Return;
using ::testing::AllOf;
using ::testing::Invoke;
using ::testing::StartsWith;
using ::testing::_;

class DispatcherMock : public DispatcherInterface {

public:

    MOCK_METHOD1(performDispatch, bool(Message));

};

class LoggerMock : public LoggerInterface {

public:

    MOCK_METHOD1(error, void(std::string));

};

TEST(MessageDispatcher, test_logger_logs_on_3_negative_dispatches) {
    auto dispatcherMock = std::make_shared<DispatcherMock>();
    auto loggerMock = std::make_shared<LoggerMock>();

    ON_CALL(*dispatcherMock, performDispatch(_))
            .WillByDefault(Return(false));

    EXPECT_CALL(*loggerMock, error(StartsWith("Message cannot be dispatcher: ")))
            .Times(1);

    MessageDispatcher dispatcher(dispatcherMock, loggerMock);
    dispatcher.dispatch(Message("Hello world"));
}

TEST(MessageDispatcher, test_logger_doesnt_log_when_only_2_negative_dispatches) {
    int invokeCounter = 0;
    auto incrementFunction = [&invokeCounter](auto message) {
        invokeCounter++;
    };

    auto dispatcherMock = std::make_shared<DispatcherMock>();
    auto loggerMock = std::make_shared<LoggerMock>();

    EXPECT_CALL(*dispatcherMock, performDispatch(_))
        .WillOnce(Return(false))
        .WillOnce(Return(false))
        .WillOnce(Return(true));

    ON_CALL(*loggerMock, error(StartsWith("Message cannot be dispatcher: ")))
            .WillByDefault(Invoke(incrementFunction));

    MessageDispatcher dispatcher(dispatcherMock, loggerMock);
    dispatcher.dispatch(Message("Hello world"));

    EXPECT_EQ(invokeCounter, 0);
}