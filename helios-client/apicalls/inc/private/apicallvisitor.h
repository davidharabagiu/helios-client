#ifndef APICALLVISITOR_H
#define APICALLVISITOR_H

#include <memory>

class HttpRequestManager;
class RegisterUserCall;
class LoginCall;
class LogoutCall;
class CheckTokenCall;
class GetUserKeyCall;
class SetUserKeyCall;
class CreateDirectoryCall;
class BeginUploadCall;
class UploadCall;
class BeginDownloadCall;
class DownloadCall;
class EndTransferCall;
class GetFileSizeCall;
class ListCall;
class RemoveCall;
class MoveCall;
class IsDirCall;
class ShareKeyCall;
class AcceptKeyCall;
class ShareCall;
class NotificationsCall;
class DismissNotificationCall;
class DismissAllNotificationsCall;

/**
 * @class ApiCallVisitor
 * @brief Visits API calls, redirecting them to HttpRequestManager
 */
class ApiCallVisitor
{
public:
    /**
     * @brief Constructor
     * @param requestManager - HttpRequestManager
     */
    ApiCallVisitor(const std::weak_ptr<HttpRequestManager>& requestManager);

    /**
     * @brief Visit RegisterUserCall
     * @param call - RegisterUserCall
     */
    void visit(RegisterUserCall* call) const;

    /**
     * @brief Visit LoginCall
     * @param call - LoginCall
     */
    void visit(LoginCall* call) const;

    /**
     * @brief Visit LogoutCall
     * @param call - LogoutCall
     */
    void visit(LogoutCall* call) const;

    /**
     * @brief Visit CreateDirectoryCall
     * @param call - CreateDirectoryCall
     */
    void visit(CreateDirectoryCall* call) const;

    /**
     * @brief Visit GetUserKeyCall
     * @param call - GetUserKeyCall
     */
    void visit(GetUserKeyCall* call) const;

    /**
     * @brief Visit SetUserKeyCall
     * @param call - SetUserKeyCall
     */
    void visit(SetUserKeyCall* call) const;

    /**
     * @brief Visit BeginUploadCall
     * @param call - BeginUploadCall
     */
    void visit(BeginUploadCall* call) const;

    /**
     * @brief Visit UploadCall
     * @param call - UploadCall
     */
    void visit(UploadCall* call) const;

    /**
     * @brief Visit BeginDownloadCall
     * @param call - BeginDownloadCall
     */
    void visit(BeginDownloadCall* call) const;

    /**
     * @brief Visit DownloadCall
     * @param call - DownloadCall
     */
    void visit(DownloadCall* call) const;

    /**
     * @brief Visit EndTransferCall
     * @param call - EndTransferCall
     */
    void visit(EndTransferCall* call) const;

    /**
     * @brief Visit GetFileSizeCall
     * @param call - GetFileSizeCall
     */
    void visit(GetFileSizeCall* call) const;

    /**
     * @brief Visit ListCall
     * @param call - ListCall
     */
    void visit(ListCall* call) const;

    /**
     * @brief Visit RemoveCall
     * @param call - RemoveCall
     */
    void visit(RemoveCall* call) const;

    /**
     * @brief Visit MoveCall
     * @param call - MoveCall
     */
    void visit(MoveCall* call) const;

    /**
     * @brief Visit CheckTokenCall
     * @param call - CheckTokenCall
     */
    void visit(CheckTokenCall* call) const;

    /**
     * @brief Visit IsDirCall
     * @param call - IsDirCall
     */
    void visit(IsDirCall* call) const;

    /**
     * @brief Visit ShareKeyCall
     * @param call - ShareKeyCall
     */
    void visit(ShareKeyCall* call) const;

    /**
     * @brief Visit AcceptKeyCall
     * @param call - AcceptKeyCall
     */
    void visit(AcceptKeyCall* call) const;

    /**
     * @brief Visit ShareCall
     * @param call - ShareCall
     */
    void visit(ShareCall* call) const;

    /**
     * @brief NotificationsCall
     * @param call - NotificationsCall
     */
    void visit(NotificationsCall* call) const;

    /**
     * @brief DismissNotificationCall
     * @param call - DismissNotificationCall
     */
    void visit(DismissNotificationCall* call) const;

    /**
     * @brief DismissAllNotificationsCall
     * @param call - DismissAllNotificationsCall
     */
    void visit(DismissAllNotificationsCall* call) const;

private:
    /**
     * @brief Handle calls that encapsulate post requests
     * @param call - Generic call
     * @tparam Call - Call type
     */
    template <typename Call>
    void handlePost(Call* call) const;

    /**
     * @brief Handle calls that encapsulate get requests
     * @param call - Generic call
     * @tparam Call - Call type
     */
    template <typename Call>
    void handleGet(Call* call) const;

private:
    /**
     * @brief Http request manager
     */
    std::weak_ptr<HttpRequestManager> m_requestManager;
};

#include "apicallvisitor.tpp"

#endif  // APICALLVISITOR_H
