#ifndef FORMDATAREQUESTIMPL_H
#define FORMDATAREQUESTIMPL_H

#include <vector>
#include <QBuffer>

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

    ~FormDataRequestImpl() override;

public:  // from FormDataRequest
    void setUrl(const std::string& url) override;
    void setHeaderValue(const std::string& name, const std::string& value) override;
    void setPart(const std::string& name, HttpPartType type, const std::vector<uint8_t>& value) override;
    void setPart(const std::string& name, HttpPartType type, const std::string& value) override;

public:  // from UrlEncodedRequestPrivate
    std::string                        url() const override;
    std::map<std::string, std::string> header() const override;
    QHttpMultiPart*                    multiPart() const override;

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
    QHttpMultiPart* m_multiPart;

    /**
     * @brief Vector which holds the transmitted data while it's sent
     */
    std::vector<QByteArray*> m_sourceData;
};

#endif  // FORMDATAREQUESTIMPL_H
