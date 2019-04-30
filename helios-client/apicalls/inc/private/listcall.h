#ifndef LISTCALL_H
#define LISTCALL_H

#include <string>
#include <vector>
#include <QString>

#include "apicall.h"
#include "apicalldefs.h"
#include "urlencodedrequest.h"

/**
 * @class ListCall
 * @brief Manages a file listing call
 */
class ListCall : public ApiCall
{
public:
    ListCall(const std::string& authToken, const std::string& path, const ApiCallbacks::ListCallback& callback);

public:
    /**
     * @brief Returns the http request
     * @return std::shared_ptr<UrlEncodedRequest>
     */
    std::shared_ptr<UrlEncodedRequest> request();

public:  // from ApiCall
    void send(std::shared_ptr<ApiCallVisitor> visitor) override;
    void receive(HttpStatus status, const std::vector<uint8_t>& reply) override;

private:
    /**
     * @brief Http request
     */
    std::shared_ptr<UrlEncodedRequest> m_request;

    /**
     * @brief Callback
     */
    ApiCallbacks::ListCallback m_callback;

    /**
     * @brief Request url
     */
    static const std::string s_kUrl;

    /**
     * @brief Authentication token header parameter name
     */
    static const std::string s_kTokenParam;

    /**
     * @brief File path parameter name
     */
    static const std::string s_kPathParam;

    /**
     * @brief Name of the JSON field representing the file or directory name in the reply
     */
    static const QString s_kFileNameJsonField;

    /**
     * @brief Name of the JSON field representing the "is directory" flag in the reply
     */
    static const QString s_kIsDirJsonField;

    /**
     * @brief Invalid path error message
     */
    static const std::string s_kErrorInvalidPath;
};

#endif  // LISTCALL_H
