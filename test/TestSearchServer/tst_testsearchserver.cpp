#include <QtTest>

// add necessary includes here
class TestSearchServer : public QObject
{
   Q_OBJECT

public:
   TestSearchServer();
   ~TestSearchServer();

private slots:
   void test_case1();
};

TestSearchServer::TestSearchServer() {}

TestSearchServer::~TestSearchServer() {}

void TestSearchServer::test_case1() {}

QTEST_APPLESS_MAIN(TestSearchServer)

#include "tst_testsearchserver.moc"
