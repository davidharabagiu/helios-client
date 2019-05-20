#ifndef QHTTPREQUESTMANAGERIMPL_H
#define QHTTPREQUESTMANAGERIMPL_H

#include <memory>
#include <unordered_map>
#include <tuple>
#include <string>
#include <map>
#include <QNetworkAccessManager>
#include <QObject>
#include <QEvent>

#include "httprequestmanager.h"

class QNetworkRequest;
class QNetworkReply;
class HttpReplyListener;
class UrlEncodedRequest;
class FormDataRequest;

/**
 * @class HttpRequestManager
 * @brief Provides a way to send http requests to a server and get back replies
 */
class QHttpRequestManagerImpl : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Constructor
     */
    QHttpRequestManagerImpl();

public:  // Forwarded from HttpRequestManagerImpl
    void post(std::shared_ptr<UrlEncodedRequest> request, const HttpReplyCallback& callback);
    void post(std::shared_ptr<FormDataRequest> request, const HttpReplyCallback& callback);
    void get(std::shared_ptr<UrlEncodedRequest> request, const HttpReplyCallback& callback);
    void setIgnoreSslErrors(bool value);

public:  // from QObject
    bool event(QEvent* event) override;

private slots:
    void onSslErrors(QNetworkReply* reply, const QList<QSslError>& errors) const;

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
    void addPendingReply(QNetworkReply* reply, const HttpReplyCallback& callback);

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
    std::unordered_map<int, std::tuple<std::shared_ptr<HttpReplyListener>, HttpReplyCallback>> m_pendingReplies;

    /**
     * @brief Last assigned request id
     */
    int m_lastAssignedRequestId;

    /**
     * @brief Setting this value to true will ignore SSL errors regarding security issues (e.g. untrusted certificate)
     */
    bool m_ignoreSslErrors;

public:
    /**
     * @brief Event type for submitting a request
     */
    static const QEvent::Type SendRequestEventType;
};

#endif  // QHTTPREQUESTMANAGERIMPL_H
