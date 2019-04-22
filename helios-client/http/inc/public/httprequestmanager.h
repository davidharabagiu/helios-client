#ifndef HTTPREQUESTMANAGER_H
#define HTTPREQUESTMANAGER_H

#include <QString>
#include <QByteArray>
#include <functional>
#include <map>

#include "httpstatus.h"

/**
 * @brief Http reply callback containing the reply data
 * @param status - Reply status
 * @param content - Reply content
 */
using HttpReplyCallback = std::function<void(HttpStatus status, const QByteArray& content)>;

/**
 * @brief Http request parameters dictionary
 */
using HttpParams = std::map<QString, QString>;

/**
 * @brief Describes the content disposition of an http part
 */
enum class HttpPartType
{
    TEXT,
    FILE
};

/**
 * @brief Http multipart request parts dictionary
 */
using HttpParts = std::map<QString, std::pair<HttpPartType, QByteArray>>;

/**
 * @class HttpRequestManager
 * @brief Provides a way to send http requests to a server and get back replies
 */
class HttpRequestManager
{
public:
    /**
     * @brief Destructor
     */
    virtual ~HttpRequestManager() = default;

    /**
     * @brief Send an http post request with url encoded parameters
     * @param url - Relative url
     * @param headerParams - Request header parameters
     * @param bodyParams - Request body parameters
     * @param callback - Reply callback
     */
    virtual void post(const QString& url, const HttpParams& headerParams, const HttpParams& params,
                      const HttpReplyCallback& callback) = 0;

    /**
     * @brief Send an http post request with multipart form data parameters
     * @param url - Relative url
     * @param headerParams - Request header parameters
     * @param parts - Request parts of the multipart object
     * @param callback - Reply callback
     */
    virtual void postMultiPart(const QString& url, const HttpParams& headerParams, const HttpParts& parts,
                               const HttpReplyCallback& callback) = 0;

    /**
     * @brief Send an http get request with url encoded parameters
     * @param url - Relative url
     * @param headerParams - Request header parameters
     * @param params - Request parts of the multipart object
     * @param callback - Reply callback
     */
    virtual void get(const QString& url, const HttpParams& headerParams, const HttpParams& params,
                     const HttpReplyCallback& callback) = 0;
};

#endif  // HTTPREQUESTMANAGER_H
