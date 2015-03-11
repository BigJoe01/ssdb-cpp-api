#ifndef SSDB_ASYNC_CLIENT_H
#define SSDB_ASYNC_CLIENT_H

#include <string>
#include <thread>

#include "ssdb_client.h"
#include "msgqueue.h"

/*  �첽ssdb�ͻ��ˣ��ӿڷ��̰߳�ȫ */
class SSDBAsyncClient
{
public:
    enum SSDB_CONNECT_STATUS
    {
        SSDB_CONNECT_NONE,
        SSDB_CONNECT_POST,  /*  ��Ͷ�ݿ���db�߳�(��ʼ����ssdb������)    */
        SSDB_CONNECT_OK,    /*  ���ӳɹ�    */
        SSDB_CONNECT_CLOSE, /*  ���ӶϿ�    */
    };

public:
    SSDBAsyncClient();
    ~SSDBAsyncClient();

    /*  �߼��̴߳���db���  */
    void                    pollDBReply(int ms);

    /*  ����db�߳�  */
    void                    postStartDBThread(std::string ip, int port);
    /*  �ر�db�߳�  */
    void                    closeDBThread();
    SSDB_CONNECT_STATUS     getConnectStatus() const;

    void                    set(const std::string& key, const std::string& value, const std::function<void(const Status&)>& callback);
    void                    get(const std::string& key, const std::function<void(const std::string&, const Status&)>& callback);

    void		            hset(const std::string& name, const std::string& key, const std::string& val,
        const std::function<void(const Status&)>& callback);

    void		            hget(const std::string& name, const std::string& key,
        const std::function<void(const std::string&, const Status&)>& callback);

    void		            zset(const std::string& name, const std::string& key, int64_t score,
        const std::function<void(const Status&)>& callback);

    void		            zget(const std::string& name, const std::string& key, const std::function<void(int64_t, const Status&)>& callback);

    void		            zsize(const std::string& name, const std::function<void(int64_t, const Status&)>& callback);

    void		            zkeys(const std::string& name, const std::string& key_start, int64_t score_start, int64_t score_end,
        uint64_t limit, const std::function<void(const std::vector<std::string>&, const Status&)>& callback);

    void		            zscan(const std::string& name, const std::string& key_start, int64_t score_start, int64_t score_end,
        uint64_t limit, const std::function<void(const std::vector<std::string>&, const Status&)>& callback);

    void		            zclear(const std::string& name, const std::function<void(const Status&)>& callback);

private:
    void                    dbThread(std::string ip, int port);
    void                    waitCloseDBThread();
private:
    SSDB_CONNECT_STATUS                     mConnectStatus;
    bool                                    mCloseDBThread;

    std::thread*                            mDBThread;
    MsgQueue<std::function<void(void)>>     mDBFunctorMQ;
    MsgQueue<std::function<void(void)>>     mLogicFunctorMQ;

    SSDBClient                              mSSDBClient;
};

#endif