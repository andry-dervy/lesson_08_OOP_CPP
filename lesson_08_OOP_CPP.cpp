//============================================================================
// Name        : lesson_08_OOP_CPP.cpp
// Author      : andry antonenko
// IDE         : Qt Creator 4.14.2 based on Qt 5.15.2
// Description : lesson 08 of the object-oriented programming on C++ course
//============================================================================
#include <QCoreApplication>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <memory>
using namespace std;
//----------------------------------------------------------------------------
//#define NDEBUG
#include <cassert>
//----------------------------------------------------------------------------
/*
1. Написать шаблонную функцию div,
которая должна вычислять результат деления двух параметров и
запускать исключение DivisionByZero,
если второй параметр равен 0.
В функции main выводить результат вызова функции div в консоль,
а также ловить исключения.
//*/

template<typename T>
T div(T a, T b)
{
  if(b == 0)
    throw "Divider equals zero!";
  return a/b;
}

void task_1()
{
  cout << "Task 1\n" << endl;


  cout << "Enter number a:" << endl;
  double a;
  cin >> a;

  cout << "Enter number b:" << endl;
  double b;
  cin >> b;

  double c;
  try {
    c = div(a,b);
    cout << "c = " << c << endl;
  }  catch (const char* ex) {
    cout << ex << endl;
  }

  cout << endl;
}

//----------------------------------------------------------------------------
/*
2. Написать класс Ex,
хранящий вещественное число x и имеющий конструктор по вещественному числу,
инициализирующий x значением параметра.
Написать класс Bar, хранящий вещественное число y
(конструктор по умолчанию инициализирует его нулем) и
имеющий метод set с единственным вещественным параметром a.
Если y + a > 100, возбуждается исключение типа Ex с данными a*y,
иначе в y заносится значение a.
В функции main завести переменную класса Bar и
в цикле в блоке try вводить с клавиатуры целое n.
Использовать его в качестве параметра метода set до тех пор,
пока не будет введено 0.
В обработчике исключения выводить сообщение об ошибке,
содержащее данные объекта исключения.
//*/

class Ex
{
public:
  double x;
  Ex(double a_x = 0)
    :x(a_x){}
};

class Bar
{
private:
  double y;
public:
  Bar(double a_y = 0)
    :y(a_y){}
  void set(double a)
  {
    if(y+a > 100)
      throw Ex(a*y);
    y = a;
  }
};

void task_2()
{
  cout << "Task 2\n" << endl;

  Bar bar;

  int a;

  do
  {
    cout << "Enter number a:" << endl;
    cin >> a;
    try {
      bar.set(a);
    }  catch (const Ex ex) {
      cout << "Exception: " << ex.x << endl;
    }
  }
  while (a != 0);

  cout << endl;
}

//----------------------------------------------------------------------------
/*
3. Написать класс «робот»,
моделирующий перемещения робота по сетке 10x10,
у которого есть метод,
означающий задание переместиться на соседнюю позицию.
Эти методы должны запускать классы-исключения OffTheField,
если робот должен уйти с сетки, и IllegalCommand,
если подана неверная команда
(направление не находится в нужном диапазоне).
Объект исключения должен содержать всю необходимую информацию —
текущую позицию и направление движения.
Написать функцию main,
пользующуюся этим классом и перехватывающую все исключения от его методов,
а также выводящую подробную информацию о всех возникающих ошибках.
//*/

enum Direction
{
  UP,
  DOWN,
  LEFT,
  RIGHT,
};

class Field
{
public:
  const int xSize;
  const int ySize;
  Field(int aXSize = 10, int aYSize = 10)
    :xSize(aXSize),ySize(aYSize){}
};

class OffTheField
{
public:
  int x,y;
  Direction dir;
  OffTheField(int aX,int aY,Direction aDir)
    :x(aX),y(aY),dir(aDir){}
  friend const std::ostream& operator<<(std::ostream& out, const OffTheField &otf);
};

const std::ostream& operator<<(std::ostream& out, const OffTheField &otf)
{
  out << "Exeption OffTheField: x = " << otf.x <<
         " y = " << otf.y <<
         " direction = ";
  switch(otf.dir)
  {
  case UP: out << "UP"; break;
  case DOWN: out << "DOWN"; break;
  case LEFT: out << "LEFT"; break;
  case RIGHT: out << "RIGHT"; break;
  }

  out << std::endl;

  return out;
}

class IllegalCommand
{
public:
  Field *field;
  int x,y;
  IllegalCommand(Field *pField,int aX,int aY)
    :field(pField),x(aX),y(aY){
  }
  friend const std::ostream& operator<<(std::ostream& out, const IllegalCommand &ic);
};

const std::ostream& operator<<(std::ostream& out, const IllegalCommand &ic)
{
  out << "Exeption IllegalCommand: ";

  if(ic.x < 0)
  {
    out << "aX < 0; ";
  }
  else if(ic.x >= ic.field->xSize)
  {
    out << "aX >= " << ic.field->xSize << "; ";
  }

  if(ic.y < 0)
  {
    out << "aY < 0; ";
  }
  else if(ic.y >= ic.field->ySize)
  {
    out << "aY >= " << ic.field->ySize << "; ";
  }
  out << std::endl;
  return out;
}


class Robot
{
private:
  Field *field;
  int x,y;
public:
  Robot(Field *pField,int aX = 0, int aY = 0)
    :field(pField),x(aX),y(aY){
    assert(pField != nullptr);
  }
  ~Robot()
  {
    if(field != nullptr)
      delete field;
  }
  void move(Direction dir)
  {
    switch(dir)
    {
    case UP:
      if(y == 0)
        throw OffTheField(x,y,dir);
      cout << "\nmoving up" << endl;
      --y;
      break;
    case DOWN:
      if(y == field->ySize-1)
        throw OffTheField(x,y,dir);
      cout << "\nmoving down" << endl;
      ++y;
      break;
    case LEFT:
      if(x == 0)
        throw OffTheField(x,y,dir);
      cout << "\nmoving left" << endl;
      --x;
      break;
    case RIGHT:
      if(x == field->xSize-1)
        throw OffTheField(x,y,dir);
      cout << "\nmoving right" << endl;
      ++x;
      break;
    }
  }
  void move(int aX, int aY)
  {
    if(x == aX && y == aY)
      return;

    if(aX < 0 || aX >= field->xSize ||
       aY < 0 || aY >= field->ySize)
      throw IllegalCommand(field,aX,aY);

    cout << "\nmoving to (" << aX << "," <<
            aY << ")" << endl;

    x = aX;
    y = aY;
  }
  friend const std::ostream& operator<<(std::ostream& out, const Robot &r);
};

const std::ostream& operator<<(std::ostream& out, const Robot &r)
{
  out << "Robot's position:";
  out << " x = " << r.x;
  out << " y = " << r.y;
  out << std::endl;
  return out;
}


void task_3()
{
  cout << "Task 3\n" << endl;

  Robot rob(new Field());
  cout << rob;

  try {
    rob.move(LEFT);
  }  catch (OffTheField otf) {
    cout << otf;
  }
  cout << rob;

  try {
    rob.move(RIGHT);
  }  catch (OffTheField otf) {
    cout << otf;
  }
  cout << rob;

  try {
    rob.move(UP);
  }  catch (OffTheField otf) {
    cout << otf;
  }
  cout << rob;

  try {
    rob.move(DOWN);
  }  catch (OffTheField otf) {
    cout << otf;
  }
  cout << rob;

  try {
    rob.move(9,9);
  }  catch (IllegalCommand ic) {
    cout << ic;
  }
  cout << rob;

  try {
    rob.move(10,4);
  }  catch (IllegalCommand ic) {
    cout << ic;
  }
  cout << rob;

  try {
    rob.move(2,-15);
  }  catch (IllegalCommand ic) {
    cout << ic;
  }
  cout << rob;

  cout << endl;
}
//----------------------------------------------------------------------------
int main(int argc, char *argv[])
{
  QCoreApplication a(argc, argv);
  //--------------------------------------------------------------------------
  // Task 1
  //*
  task_1();
  //*/
  //--------------------------------------------------------------------------
  // Task 2
  //*
  task_2();
  //*/
  //--------------------------------------------------------------------------
  // Task 3
  //*
  task_3();
  //*/
  //--------------------------------------------------------------------------
  return a.exec();
}
