#include <QObject>
#include <memory>
#include <functional>

class QNetworkReply;

/**
 * @class HttpReplyListener
 * @brief The HttpReplyListener class
 */
class HttpReplyListener : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Constructor
     * @param id - Request id
     * @param reply - QNetworkReply
     * @param callback - Function to be called when a reply arrives. The callback parameter is the given request id.
     */
    HttpReplyListener(int id, const std::shared_ptr<QNetworkReply>& reply, std::function<void(int)> callback);

    /**
     * @brief Returns the reply object
     * @return std::shared_ptr<QNetworkReply>
     */
    std::shared_ptr<QNetworkReply> reply() const;

private slots:
    /**
     * @brief Called when the response is received
     */
    void onReplyReceived() const;

private:
    /**
     * @brief Request id
     */
    int m_id;

    /**
     * @brief Reply object
     */
    std::shared_ptr<QNetworkReply> m_reply;

    /**
     * @brief Callback
     */
    std::function<void(int)> m_callback;
};
