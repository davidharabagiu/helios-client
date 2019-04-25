#ifndef URLENCODEDREQUESTIMPL_H
#define URLENCODEDREQUESTIMPL_H

#include <QUrlQuery>

#include "urlencodedrequest.h"
#include "urlencodedrequestprivate.h"

/**
 * @brief UrlEncodedRequest implementation
 */
class UrlEncodedRequestImpl : public UrlEncodedRequest, public UrlEncodedRequestPrivate
{
public:
    /**
     * @brief Constructor
     * @param url - request url
     */
    UrlEncodedRequestImpl(const std::string& url);

public:  // from UrlEncodedRequest
    void setUrl(const std::string& url) override;
    void setHeaderValue(const std::string& name, const std::string& value) override;
    void setParameter(const std::string& name, const std::string& value) override;

public:  // from UrlEncodedRequestPrivate
    std::string                        url() const override;
    std::map<std::string, std::string> header() const override;
    const QUrlQuery&                   urlQuery() const override;

private:
    /**
     * @brief Request url
     */
    std::string m_url;

    /**
     * @brief Request header values
     */
    std::map<std::string, std::string> m_header;

    /**
     * @brief Request parameters in form of an url query
     */
    QUrlQuery m_urlQuery;
};

#endif  // URLENCODEDREQUESTIMPL_H
