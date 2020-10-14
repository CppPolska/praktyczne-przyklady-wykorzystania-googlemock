#include <string>
#include <memory>
#include <exception>

class NameIsTooShortException : public std::exception {

private:

    std::string message;

public:

    explicit NameIsTooShortException(std::string message): message(message) {

    }

    const char * what() const noexcept override {
        return message.c_str();
    }
};

class Customer {

private:

    std::string username;
    std::string firstName;
    std::string lastName;
    std::string emailAddress;
    std::string password;

public:

    Customer(
        std::string username,
        std::string firstName,
        std::string lastName,
        std::string emailAddress,
        std::string password
    ): username(username), firstName(firstName), lastName(lastName), emailAddress(emailAddress), password(password) {

    }

    std::string getUsername() const {
        return username;
    }

    std::string getFirstName() const {
        return firstName;
    }

    std::string getLastName() const {
        return lastName;
    }

    std::string getEmailAddress() const {
        return emailAddress;
    }

    std::string getPassword() const {
        return password;
    }
};

class UsernameGeneratorInterface {

public:

    virtual std::string generate(std::string firstName, std::string lastName) = 0;
};

class CustomerFactory {

private:

    std::shared_ptr<UsernameGeneratorInterface> usernameGenerator;

public:

    explicit CustomerFactory(
        std::shared_ptr<UsernameGeneratorInterface> usernameGenerator
    ) : usernameGenerator(std::move(usernameGenerator)) {
    }


    Customer create(
        std::string firstName,
        std::string lastName,
        std::string emailAddress,
        std::string password
    ) {
        std::string username = this->usernameGenerator->generate(firstName, lastName);

        return Customer(
            username, firstName, lastName, emailAddress, password
        );
    }
};