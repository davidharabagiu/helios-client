#ifndef FORMDATAREQUESTIMPL_H
#define FORMDATAREQUESTIMPL_H

#include "formdatarequest.h"
#include "formdatarequestprivate.h"

/**
 * @brief FormDataRequest implementation
 */
class FormDataRequestImpl : public FormDataRequest, public FormDataRequestPrivate
{
public:
    /**
     * @brief Constructor
     * @param url - request url
     */
    FormDataRequestImpl(const std::string& url);

public:  // from FormDataRequest
    void setUrl(const std::string& url) override;
    void setHeaderValue(const std::string& name, const std::string& value) override;
    void setPart(const std::string& name, HttpPartType type, const std::vector<uint8_t>& value) override;
    void setPart(const std::string& name, HttpPartType type, const std::string& value) override;

public:  // from UrlEncodedRequestPrivate
    std::string                        url() const override;
    std::map<std::string, std::string> header() const override;
    std::shared_ptr<QHttpMultiPart>    multiPart() const override;

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
     * @brief Request multi-part
     */
    std::shared_ptr<QHttpMultiPart> m_multiPart;
};

#endif  // FORMDATAREQUESTIMPL_H
