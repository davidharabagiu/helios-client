#ifndef APICALLVISITOR_H
#define APICALLVISITOR_H

#include <memory>

class HttpRequestManager;
class RegisterUserCall;

/**
 * @class ApiCallVisitor
 * @brief Visits API calls, redirecting them to HttpRequestManager
 */
class ApiCallVisitor
{
public:
    /**
     * @brief Constructor
     * @param requestManager - HttpRequestManager
     */
    ApiCallVisitor(const std::weak_ptr<HttpRequestManager>& requestManager);

    /**
     * @brief Visit RegisterUserCall
     * @param call - RegisterUserCall
     */
    void visit(RegisterUserCall& call) const;

private:
    /**
     * @brief Http request manager
     */
    std::weak_ptr<HttpRequestManager> m_requestManager;
};

#endif  // APICALLVISITOR_H
