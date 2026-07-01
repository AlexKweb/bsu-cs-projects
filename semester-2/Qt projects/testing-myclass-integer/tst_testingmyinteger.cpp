#include <QTest>

#include "../unit-test-class-integer/MyInteger.h"

class testingMyInteger : public QObject
{
    Q_OBJECT

public:
    testingMyInteger();
    ~testingMyInteger();

private slots:
    void test_case1();
    void test_case2();
    void test_case3();
    void test_case4();
};

testingMyInteger::testingMyInteger() {}

testingMyInteger::~testingMyInteger() = default;

void testingMyInteger::test_case1()
{
    QCOMPARE(MyInteger().nod(), 0);
    QCOMPARE(MyInteger(12, 18).nod(), 6);
    QCOMPARE(MyInteger(0, 18).nod(), 18);
    QCOMPARE(MyInteger(0, 0).nod(), 0);
    QCOMPARE(MyInteger(-12, 18).nod(), 6);
    QCOMPARE(MyInteger(2147483647, 1).nod(), 1);
    QCOMPARE(MyInteger(-2147483648, 1).nod(), 1);
}

void testingMyInteger::test_case2()
{
    QCOMPARE(MyInteger().nok(), 0);
    QCOMPARE(MyInteger(12, 18).nok(), 36);
    QCOMPARE(MyInteger(0, 0).nok(), 0);
    QCOMPARE(MyInteger(-12, 18).nok(), 36LL);
    QCOMPARE(MyInteger(2147483647, 1).nok(), 2147483647);
    QCOMPARE(MyInteger(-2147483648, 1).nok(), 2147483648);
}

void testingMyInteger::test_case3()
{
    {
        MyInteger forTesting;
        QVERIFY(!forTesting.isPrimeA());
        QVERIFY(!forTesting.isPrimeB());
    }

    {
        MyInteger forTesting(7, 13);
        QVERIFY(forTesting.isPrimeA());
        QVERIFY(forTesting.isPrimeB());
    }

    {
        MyInteger forTesting(1, 12);
        QVERIFY(!forTesting.isPrimeA());
        QVERIFY(!forTesting.isPrimeB());
    }

    {
        MyInteger forTesting(-7, -12);
        QVERIFY(forTesting.isPrimeA());
        QVERIFY(!forTesting.isPrimeB());
    }

    {
        MyInteger forTesting(2147483647, -2147483648);
        QVERIFY(forTesting.isPrimeA());
        QVERIFY(!forTesting.isPrimeB());
    }
}

void testingMyInteger::test_case4()
{
    {
        MyInteger forTesting(12, 18);
        QCOMPARE(forTesting.divisorsA(), QVector<int>({2, 2, 3}));
        QCOMPARE(forTesting.divisorsB(), QVector<int>({2, 3, 3}));
    }

    {
        MyInteger forTesting(-12, 0);
        QCOMPARE(forTesting.divisorsA(), QVector<int>({2, 2, 3}));
        QCOMPARE(forTesting.divisorsB(), QVector<int>());
    }

    {
        MyInteger forTesting(1, 7);
        QCOMPARE(forTesting.divisorsA(), QVector<int>());
        QCOMPARE(forTesting.divisorsB(), QVector<int>({7}));
    }

    {
        MyInteger forTesting(2147483647, 0);
        QCOMPARE(forTesting.divisorsA(), QVector<int>({2147483647}));
        QCOMPARE(forTesting.divisorsB(), QVector<int>());
    }

    {
        MyInteger forTesting(-2147483648, 0);
        QVector<int> expected;

        for (int i = 0; i < 31; ++i) {
            expected.push_back(2);
        }

        QCOMPARE(forTesting.divisorsA(), expected);
        QCOMPARE(forTesting.divisorsB(), QVector<int>());
    }
}

QTEST_APPLESS_MAIN(testingMyInteger)

#include "tst_testingmyinteger.moc"
