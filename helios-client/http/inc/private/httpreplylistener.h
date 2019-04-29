#ifndef HTTPREPLYLISTENER_H
#define HTTPREPLYLISTENER_H

#include <QObject>
#include <functional>

#include <QSSlError>

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
    HttpReplyListener(int id, QNetworkReply* reply, const std::function<void(int)>& callback);

    /**
     * @brief Destructor
     */
    ~HttpReplyListener();

    /**
     * @brief Returns the reply object
     * @return std::shared_ptr<QNetworkReply>
     */
    QNetworkReply* reply() const;

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
    QNetworkReply* m_reply;

    /**
     * @brief Callback
     */
    std::function<void(int)> m_callback;
};

#endif  // HTTPREPLYLISTENER_H
