#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "../VnocMessageHandler.hpp"
#include "MockTcpConnection.hpp"

class VnocMessageHandlerTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE( VnocMessageHandlerTest );
    CPPUNIT_TEST( testRVC );
    CPPUNIT_TEST_SUITE_END();
    MockTcpConnection *conn_;
public:
    void setUp()
    {
        conn_ = new MockTcpConnection;
    }
    void tearDown()
    {
        delete conn_;
    }
public:
    void testRVC()
    {
        VnocMessageHandler<MockTcpConnection> handler(conn_);
        handler.start();
        char rvc[]={0x55,
            0x00,
            0x00,0x01,
            0x00,0x00,0x00,0x35,

            0x02,0x02,0x02,0x02,
            0x02,0x02,0x02,0x02,
            0x02,0x02,0x02,0x02,
            0x02,0x02,0x02,0x02,

            0x14,
            0x00,0x00,0x00,0x00,
            0x01,

            0x00,0x00,0x00,0x10,

            0x00,0x00,0x00,0x00,
            0x00,0x00,0x00,0x00,
            0x00,0x00,0x00,0x00,
            0x00,0x00,0x00,0x00,

            0x00,0x00,
            0x43};
        conn_->setRecv(rvc, sizeof(rvc));
        const char *sendBuf = conn_->getSendBuf();
        //return a AVC message
        CPPUNIT_ASSERT(sendBuf[24]==0x15);
    }

};
CPPUNIT_TEST_SUITE_REGISTRATION ( VnocMessageHandlerTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(VnocMessageHandlerTest, "VnocMessageHandlerTest");
