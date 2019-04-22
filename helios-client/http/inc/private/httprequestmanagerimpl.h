#include <memory>
#include <unordered_map>
#include <utility>

#include "httprequestmanager.h"

class QNetworkAccessManager;
class QNetworkReply;
class HttpReplyListener;

/**
 * @class HttpRequestManager
 * @brief Provides a way to send http requests to a server and get back replies
 */
class HttpRequestManagerImpl : public HttpRequestManager
{
public:
    /**
     * @brief Constructor
     * @param baseUrl - http server base url
     */
    HttpRequestManagerImpl(const QString& baseUrl);

public:  // from HttpRequestManager
    void post(const QString& url, const HttpParams& headerParams, const HttpParams& params,
              const HttpReplyCallback& callback) override;

    void postMultiPart(const QString& url, const HttpParams& headerParams, const HttpParts& parts,
                       const HttpReplyCallback& callback) override;

    void get(const QString& url, const HttpParams& headerParams, const HttpParams& params,
             const HttpReplyCallback& callback) override;

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

private:
    /**
     * @brief Server base url for all requests
     */
    QString m_baseUrl;

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
};
