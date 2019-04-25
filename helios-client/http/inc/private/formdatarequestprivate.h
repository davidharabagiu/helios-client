#ifndef FORMDATAREQUESTPRIVATE_H
#define FORMDATAREQUESTPRIVATE_H

#include <string>
#include <map>
#include <memory>

class QHttpMultiPart;

/**
 * @class FormDataRequestPrivate
 * @brief Private interface used for retrieving data from a form data request
 */
class FormDataRequestPrivate
{
public:
    /**
     * @brief Destructor
     */
    virtual ~FormDataRequestPrivate() = default;

    /**
     * @brief Request url
     * @return std::string
     */
    virtual std::string url() const = 0;

    /**
     * @brief Request header, name - value pairs
     * @return std::map<std::string, std::string>
     */
    virtual std::map<std::string, std::string> header() const = 0;

    /**
     * @brief Http multi-part
     * @return std::shared_ptr<QHttpMultiPart>
     */
    virtual std::shared_ptr<QHttpMultiPart> multiPart() const = 0;
};

#endif  // FORMDATAREQUESTPRIVATE_H
