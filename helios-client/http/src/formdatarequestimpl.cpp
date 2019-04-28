#include <QDebug>
#include <QHttpMultiPart>

#include "formdatarequestimpl.h"
#include "typeconversions.h"

FormDataRequestImpl::FormDataRequestImpl(const std::string& url)
    : m_url(url)
    , m_multiPart(new QHttpMultiPart())
{
}

void FormDataRequestImpl::setUrl(const std::string& url)
{
    m_url = url;
}

void FormDataRequestImpl::setHeaderValue(const std::string& name, const std::string& value)
{
    m_header[name] = value;
}

void FormDataRequestImpl::setPart(const std::string& name, HttpPartType type, const std::vector<uint8_t>& value)
{
    QHttpPart httpPart;

    httpPart.setHeader(QNetworkRequest::ContentDispositionHeader,
                       QString(("form-data; name=\"" + name + "\"").c_str()));
    httpPart.setBody(QByteArray(reinterpret_cast<const char*>(value.data()), safe_integral_cast<int>(value.size())));

    if (type == HttpPartType::TEXT)
    {
        httpPart.setHeader(QNetworkRequest::ContentTypeHeader, "text/plain");
    }
    else if (type == HttpPartType::FILE)
    {
        httpPart.setHeader(QNetworkRequest::ContentTypeHeader, "application/octet-stream");
    }
    else
    {
        qWarning() << "Unknown http part type: " << static_cast<int>(type);
        return;
    }

    m_multiPart->append(httpPart);
}

void FormDataRequestImpl::setPart(const std::string& name, FormDataRequest::HttpPartType type, const std::string& value)
{
    std::vector<uint8_t> byteData(value.cbegin(), value.cend());
    setPart(name, type, byteData);
}

std::string FormDataRequestImpl::url() const
{
    return m_url;
}

std::map<std::string, std::string> FormDataRequestImpl::header() const
{
    return m_header;
}

std::shared_ptr<QHttpMultiPart> FormDataRequestImpl::multiPart() const
{
    return m_multiPart;
}
