#ifndef HTTPREQUESTMANAGERIMPL_H
#define HTTPREQUESTMANAGERIMPL_H

#include <memory>
#include <unordered_map>
#include <utility>
#include <string>
#include <map>

#include "httprequestmanager.h"

class QNetworkAccessManager;
class QNetworkRequest;
class QNetworkReply;
class HttpReplyListener;
class UrlEncodedRequest;
class FormDataRequest;

/**
 * @class HttpRequestManager
 * @brief Provides a way to send http requests to a server and get back replies
 */
class HttpRequestManagerImpl : public HttpRequestManager
{
public:
    /**
     * @brief Constructor
     */
    HttpRequestManagerImpl();

public:  // from HttpRequestManager
    void post(std::unique_ptr<UrlEncodedRequest> request, const HttpReplyCallback& callback) override;
    void post(std::unique_ptr<FormDataRequest> request, const HttpReplyCallback& callback) override;
    void get(std::unique_ptr<UrlEncodedRequest> request, const HttpReplyCallback& callback) override;

private:
    /**
     * @brief Called when a reply is received
     * @param id - Request id
     */
    void repliedReceived(int id);

    /**
     * @brief Add a reply to the pending replies list
     * @param reply - Reply object
     * @param callback - Callback
     */
    void addPendingReply(const std::shared_ptr<QNetworkReply>& reply, const HttpReplyCallback& callback);

    /**
     * @brief Collect all the header values from a map in a QNetworkRequest object
     * @param values - Header values map
     * @param result - Output
     */
    void collectHeaderValues(const std::map<std::string, std::string>& values, QNetworkRequest& result);

private:
    /**
     * @brief Network access manager
     */
    std::unique_ptr<QNetworkAccessManager> m_networkAccessManager;

    /**
     * @brief Pending replies
     */
    std::unordered_map<int, std::pair<std::shared_ptr<HttpReplyListener>, HttpReplyCallback>> m_pendingReplies;

    /**
     * @brief Last assigned request id
     */
    int m_lastAssignedRequestId;

    /**
     * @brief Url separator
     */
    static const std::string s_kUrlSeparator;
};

#endif  // HTTPREQUESTMANAGERIMPL_H
