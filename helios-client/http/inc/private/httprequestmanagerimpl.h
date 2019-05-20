#ifndef HTTPREQUESTMANAGERIMPL_H
#define HTTPREQUESTMANAGERIMPL_H

#include <memory>

#include "httprequestmanager.h"

// Forward declarations
class QHttpRequestManagerImpl;

/**
 * @class HttpRequestManagerImpl
 * @brief Implementation of HttpRequestManager. Wrapper over QHttpRequestManagerImpl.
 */
class HttpRequestManagerImpl : public HttpRequestManager
{
public:
    /**
     * @brief Constructor
     */
    HttpRequestManagerImpl();

public:  // from HttpRequestManager
    void post(std::shared_ptr<UrlEncodedRequest> request, const HttpReplyCallback& callback) override;
    void post(std::shared_ptr<FormDataRequest> request, const HttpReplyCallback& callback) override;
    void get(std::shared_ptr<UrlEncodedRequest> request, const HttpReplyCallback& callback) override;
    void setIgnoreSslErrors(bool value) override;

private:
    std::shared_ptr<QHttpRequestManagerImpl> m_privateImpl;
};

#endif  // HTTPREQUESTMANAGERIMPL_H
