#ifndef TEST_H
#define TEST_H
#include <QSharedData>

#include <QSharedDataPointer>
#include <QString>


class TestData : public QSharedData
{
public:
    QString pr;
    QString sd;
};

class Test
{
public:
    Test();
    Test(const Test &);
    Test &operator=(const Test &);
    ~Test();

    void setPr(QString bible);
    void setSd(QString bible);

    QString pr();
    QString sd();


private:
    QSharedDataPointer<TestData> data;
};


class EmployeeData : public QSharedData
{
  public:
    EmployeeData() : id(-1) { }
    EmployeeData(const EmployeeData &other)
        : QSharedData(other), id(other.id), name(other.name) { }
    ~EmployeeData() { }

    int id;
    QString name;
};

class Employee
{
  public:
    Employee() { d = new EmployeeData; }
    Employee(int id, QString name) {
        d = new EmployeeData;
        setId(id);
        setName(name);
    }
    Employee(const Employee &other)
          : d (other.d)
    {
    }
    void setId(int id) { d->id = id; }
    void setName(QString name) { d->name = name; }

    int id() const { return d->id; }
    QString name() const { return d->name; }

  private:
    QSharedDataPointer<EmployeeData> d;
};

class EMP : public Employee
{
public:
    Employee em;
};



#endif // TEST_H

