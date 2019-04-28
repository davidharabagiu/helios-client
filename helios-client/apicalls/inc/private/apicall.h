#ifndef APICALL_H
#define APICALL_H

#include <vector>
#include <cstdint>
#include <memory>

class ApiCallVisitor;
enum class HttpStatus;

/**
 * @class ApiCall
 * @brief Service API call interface
 */
class ApiCall
{
public:
    /**
     * @brief Destructor
     */
    virtual ~ApiCall() = default;

    /**
     * @brief Send call
     */
    virtual void send(std::shared_ptr<ApiCallVisitor> visitor) = 0;

    /**
     * @brief Receive reply
     * @param status - Http reply status
     * @param reply - Http reply content
     */
    virtual void receive(HttpStatus status, const std::vector<uint8_t>& reply) = 0;
};

#endif  // APICALL_H
