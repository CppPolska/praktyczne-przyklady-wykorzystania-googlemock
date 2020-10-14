#include <string>
#include <memory>
#include <exception>
#include <errno.h>

class SocketResource {

};

class SocketResourceFactoryInterface {

public:

    virtual SocketResource create(std::string host, std::string port) = 0;

};

class SocketConnection {

private:

    std::shared_ptr<SocketResourceFactoryInterface> resourceFactory;

    SocketResource resource;
    std::string errorMessage;

public:

    explicit SocketConnection(
            std::shared_ptr<SocketResourceFactoryInterface> resourceFactory
    ): resourceFactory(std::move(resourceFactory)) {}

    bool connect(std::string host, std::string port) {
        resource = resourceFactory->create(host, port);

        if (errno) {
            switch (errno) {
                case ETIMEDOUT: {
                    errorMessage = "Connection timeout";
                } break;
                case ECONNREFUSED: {
                    errorMessage = "Connection refused";
                } break;
                case EAUTH: {
                    errorMessage = "Authentication error";
                } break;
                default: {
                    errorMessage = "Unexpected error";
                }
            }
            return false;
        }

        return true;
    }

    std::string getErrorMessage() const {
        return errorMessage;
    }
};