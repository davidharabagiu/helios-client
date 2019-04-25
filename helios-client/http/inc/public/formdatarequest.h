#ifndef FORMDATAREQUEST_H
#define FORMDATAREQUEST_H

#include <string>
#include <vector>
#include <cstdint>

/**
 * @class FormDataRequest
 * @brief Interface of an http request with url encoded parameters
 */
class FormDataRequest
{
public:
    /**
     * @brief Describes the content type of an http part
     */
    enum class HttpPartType
    {
        /**
         * @brief text/plain content type
         */
        TEXT,
        /**
         * @brief application/octet-stream content type
         */
        FILE
    };

public:
    /**
     * @brief Destructor
     */
    virtual ~FormDataRequest() = default;

    /**
     * @brief Setter for url
     * @param url - New url value
     */
    virtual void setUrl(const std::string& url) = 0;

    /**
     * @brief Set a value from the header. If this header already exists, its value will be replaced.
     * @param name - header field name
     * @param value - header field value
     */
    virtual void setHeaderValue(const std::string& name, const std::string& value) = 0;

    /**
     * @brief Set the value of a http part. If a part with this name already exists, its value will be replaced.
     * @param name - part name
     * @param type - part type
     * @param value - part value
     */
    virtual void setPart(const std::string& name, HttpPartType type, const std::vector<uint8_t>& value) = 0;
};

#endif  // FORMDATAREQUEST_H
