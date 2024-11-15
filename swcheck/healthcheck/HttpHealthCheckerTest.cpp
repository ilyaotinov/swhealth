#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "HttpHealthChecker.h"

class MockHttpClient : public HttpClient {
public:
    MOCK_METHOD(long, getStatusCode, (const std::string&), (const, override));
};

class HttpHealthCheckerTest : public ::testing::Test {
protected:
    const std::string TEST_URL = "https://test.com/health";
    std::shared_ptr<MockHttpClient> mockClient = std::make_shared<MockHttpClient>();
    HttpHealthChecker checker{TEST_URL, mockClient};
};

TEST_F(HttpHealthCheckerTest, ReturnsTrueForSuccessfulResponse) {
    // Arrange
    EXPECT_CALL(*mockClient, getStatusCode(TEST_URL))
            .WillOnce(testing::Return(200));

    // Act
    bool result = checker.run();

    // Assert
    EXPECT_TRUE(result);
}

TEST_F(HttpHealthCheckerTest, ReturnsTrueForSuccessfulResponseWithContent) {
    // Arrange
    EXPECT_CALL(*mockClient, getStatusCode(TEST_URL))
            .WillOnce(testing::Return(201));

    // Act
    bool result = checker.run();

    // Assert
    EXPECT_TRUE(result);
}

TEST_F(HttpHealthCheckerTest, ReturnsFalseForRedirectResponse) {
    // Arrange
    EXPECT_CALL(*mockClient, getStatusCode(TEST_URL))
            .WillOnce(testing::Return(301));

    // Act
    bool result = checker.run();

    // Assert
    EXPECT_FALSE(result);
}

TEST_F(HttpHealthCheckerTest, ReturnsFalseForClientError) {
    // Arrange
    EXPECT_CALL(*mockClient, getStatusCode(TEST_URL))
            .WillOnce(testing::Return(404));

    // Act
    bool result = checker.run();

    // Assert
    EXPECT_FALSE(result);
}

TEST_F(HttpHealthCheckerTest, ReturnsFalseForServerError) {
    // Arrange
    EXPECT_CALL(*mockClient, getStatusCode(TEST_URL))
            .WillOnce(testing::Return(500));

    // Act
    bool result = checker.run();

    // Assert
    EXPECT_FALSE(result);
}

TEST_F(HttpHealthCheckerTest, ReturnsFalseForNetworkError) {
    // Arrange
    EXPECT_CALL(*mockClient, getStatusCode(TEST_URL))
            .WillOnce(testing::Return(0));  // Предполагаем, что 0 означает ошибку сети

    // Act
    bool result = checker.run();

    // Assert
    EXPECT_FALSE(result);
}