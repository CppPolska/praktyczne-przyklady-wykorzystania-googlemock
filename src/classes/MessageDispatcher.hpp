#include <string>

class Message {

private:

    std::string message;

public:

    Message(std::string message): message(message) {

    }

    std::string serialize() const {
        return "Serialized: " + message;
    }

};

class DispatcherInterface {

public:

    virtual bool performDispatch(Message message) = 0;

};

class LoggerInterface {

public:

    virtual void error(std::string message) = 0;

};

class MessageDispatcher {

private:

    std::shared_ptr<DispatcherInterface> dispatcher;
    std::shared_ptr<LoggerInterface> logger;

public:

    MessageDispatcher(
            std::shared_ptr<DispatcherInterface> dispatcher,
            std::shared_ptr<LoggerInterface> logger
    ) : logger(std::move(logger)), dispatcher(std::move(dispatcher)) {

    }

    void dispatch(Message message) {
        bool dispatched = false;
        int dispatchCount = 0;
        do {
            dispatched = dispatcher->performDispatch(message);
            dispatchCount++;
        } while (!dispatched && dispatchCount < 3);

        if (!dispatched) {
            logger->error(std::string(
                "Message cannot be dispatcher: " + message.serialize()
            ));
        }
    }
};