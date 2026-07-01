#pragma once 
#include <iostream>
////////////////////// Абстрактные классы ////////////////////////////
// абстрактный базовый класс Итератор
class AbstractIterator
{
public:
  virtual ~AbstractIterator() {};
  virtual bool InRange() const = 0;    // индекс в допустимых пределах? 
  virtual void Reset() = 0;      // сбросить индекс в начало
  virtual int& operator *() const = 0;    // разыменование (чтение элемента)
  virtual void operator ++() = 0;      // сдвиг на элемент
};
// абстрактный базовый класс Контейнер
class AbstractContainer
{
public:
  virtual ~AbstractContainer() {};
  virtual bool IsEmpty() const = 0;    // контейнер пуст
  virtual bool IsFull()  const = 0;    // контейнер полный
};
// абстрактный базовый класс Стек
class AbstractStack: public AbstractContainer
{
public:
  virtual void push(const int& n) = 0;    // втолкнуть в стек
  virtual void pop(int& n)  = 0;      // вытолкнуть из стека
};
// абстрактный базовый класс Очередь
class AbstractQueue: public AbstractContainer
{
public:
  virtual void push(const int& n) = 0;  // втолкнуть в очередь 
  virtual void del(int& n) = 0;      // удалить из очереди
};

//////////////////// Конкретные классы ///////////////////////////////
class StackIterator;    // предваряющее объявление
class ArrayStack;     // предваряющее объявление
class QueueIterator;    // предваряющее объявление

// класс Стек на базе массива
class ArrayStack: public AbstractStack
{
protected:
  int    size;  // размерность массива
  int*  p;    // указатель на массив
  int    top;  // верхушка стека
public:
  ArrayStack(int _size = 16);
  ArrayStack(ArrayStack &s);
  ~ArrayStack();
  void push(const int& n);    // втолкнуть в стек
  void pop(int& n);      // вытолкнуть из стека
  bool IsEmpty() const;
  bool IsFull() const ;
  void print() const;
  friend class ArrayStackIterator;
};

// класс стек на базе списка - определить самостоятельно

// класс Кольцевая очередь
class ArrayQueue: public AbstractQueue 
{
protected:
  int    size;      // размерность массива
  int*  p;        // указатель на массив
  int    head;      // индекс первого занятого элемента
  int    n;      // количество элементов в очереди
public:
  ArrayQueue(int _size = 16);
  ArrayQueue(ArrayQueue &q);
  ~ArrayQueue();
  void push(const int& n);    // втолкнуть в очередь 
  void del(int& n);      // удалить из очереди
  bool IsEmpty() const;
  bool IsFull() const;
  friend class QueueIterator;
};

// класс Дек
class ArrayDequeue: public ArrayQueue
{
public:
  ArrayDequeue(int _size = 16);
  ArrayDequeue(const ArrayDequeue &d);
  ~ArrayDequeue();
  virtual void pop(int& n);    // вытолкнуть из дека со стороны push
  virtual void ins(const int& n);    // вставить в дек со стороны del
};

// класс Итератор стека
class ArrayStackIterator: public AbstractIterator
{
  ArrayStack  &a;      // ссылка на стек
  int    pos;      // текущая позиция итератора
  int StackIterator();
public:
  ArrayStackIterator(const ArrayStack& _a);
  bool InRange() const;      // индекс в допустимых пределах
  void Reset();        // сбросить индекс в начало
  int& operator *() const;    // разыменование (чтение элемента)
  void operator ++();      // сдвиг на элемент
};

// класс Итератор кольцевой очереди
class QueueIterator: public AbstractIterator
{
  ArrayQueue  &a;      // ссылка на очередь
  int    pos;      // текущая позиция итератора
  QueueIterator();
public:
  QueueIterator(ArrayQueue& _a);
  bool InRange();        // индекс в допустимых пределах
  void Reset();        // сбросить индекс в начало
  int& operator *() const;    // разыменование (чтение элемента)
  void operator ++();      // сдвиг на элемент
};
