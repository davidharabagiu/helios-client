#ifndef URLENCODEDREQUESTPRIVATE_H
#define URLENCODEDREQUESTPRIVATE_H

#include <string>
#include <map>

class QUrlQuery;

/**
 * @class UrlEncodedRequestPrivate
 * @brief Private interface used for retrieving data from an url encoded request
 */
class UrlEncodedRequestPrivate
{
public:
    /**
     * @brief Destructor
     */
    virtual ~UrlEncodedRequestPrivate() = default;

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
     * @brief Url query in the form of param1=value1&param2=value2&....
     * @return const QUrlQuery&
     */
    virtual const QUrlQuery& urlQuery() const = 0;
};

#endif  // URLENCODEDREQUESTPRIVATE_H
