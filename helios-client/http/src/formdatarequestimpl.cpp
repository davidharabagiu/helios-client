#include <QDebug>
#include <QHttpMultiPart>
#include <QBuffer>

#include "formdatarequestimpl.h"
#include "typeconversions.h"

FormDataRequestImpl::FormDataRequestImpl(const std::string& url)
    : m_url(url)
    , m_multiPart(new QHttpMultiPart(QHttpMultiPart::FormDataType))
{
}

FormDataRequestImpl::~FormDataRequestImpl()
{
    for (auto p : m_sourceData)
    {
        delete p;
    }
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

    httpPart.setHeader(QNetworkRequest::ContentDispositionHeader, ("form-data; name=\"" + name + "\"").c_str());

    auto bytes = new QByteArray(reinterpret_cast<const char*>(value.data()), safe_integral_cast<int>(value.size()));

    if (type == HttpPartType::TEXT)
    {
        httpPart.setHeader(QNetworkRequest::ContentTypeHeader, "text/plain");
        httpPart.setBody(*bytes);
        delete bytes;
    }
    else if (type == HttpPartType::FILE)
    {
        httpPart.setHeader(QNetworkRequest::ContentDispositionHeader,
                           ("form-data; name=\"" + name + "\"; filename=\"pula\"").c_str());
        httpPart.setHeader(QNetworkRequest::ContentTypeHeader, "application/zip");
        httpPart.setHeader(QNetworkRequest::ContentLengthHeader, bytes->length());

        auto buffer = new QBuffer(bytes);
        buffer->open(QBuffer::ReadOnly);
        httpPart.setBodyDevice(buffer);

        // Keep data alive while it's being sent
        m_sourceData.push_back(bytes);
        buffer->setParent(m_multiPart);
    }
    else
    {
        qWarning() << "Unknown http part type: " << static_cast<int>(type);
        delete bytes;
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

QHttpMultiPart* FormDataRequestImpl::multiPart() const
{
    return m_multiPart;
}
