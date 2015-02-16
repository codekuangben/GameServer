/*
 * This file is part of the CMaNGOS Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef NETWORK_THREAD_H
#define NETWORK_THREAD_H

#include "ProtocolDefinitions.h"

#include <boost/thread.hpp>
#include <boost/atomic.hpp>
#include <set>

class MClientThreadSafeData;
class MClientProcessData;

class NetworkThread : public boost::noncopyable
{
public:
    NetworkThread();

    virtual ~NetworkThread();

    void Stop();

    void Start();

    void Wait();

    long Connections() const
    {
        return m_Connections;
    }

    void AddSocket( const SocketPtr& sock );

    void RemoveSocket( const SocketPtr& sock );

    protocol::Service& service()
    {
        return m_networkingService;
    }

	void setMClientProcessData(MClientProcessData* pMClientProcessData);

private:

    virtual void svc();
	virtual void sendAndRecvData();

    typedef std::set<SocketPtr> SocketSet;

    boost::atomic_long m_Connections;
    std::auto_ptr<boost::thread> m_thread;
	std::auto_ptr<boost::thread> m_sendThread;		// 这个是发送数据的线程

    SocketSet m_Sockets;
    boost::mutex m_SocketsLock;

    protocol::Service m_networkingService;
    std::auto_ptr<protocol::Service::work> m_work;

	MClientThreadSafeData* m_pSocketBufferTSData;
	MClientProcessData* m_pMClientProcessData;
	bool m_bExitFlag;		// 线程是否退出
};


#endif