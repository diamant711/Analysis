/*
    SymbolicC++ : An object oriented computer algebra system written in C++

    Copyright (C) 2008 Yorick Hardy and Willi-Hans Steeb

    This library is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/


// symbolicc++.h

// normal include headers
#include <iostream>
#include <iterator>
#include <list>
#include "cloning.h"
#include "identity.h"

// phased include headers
// according to class hierarchy
#include "symbolic.h"  // SymbolicInterface, Symbolic ...
#include "equation.h"  //   Equation : CloningSymbolicInterface
#include "number.h"    //   Numeric  : CloningSymbolicInterface
#include "product.h"   //   Product  : CloningSymbolicInterface
#include "sum.h"       //   Sum      : CloningSymbolicInterface
#include "symbol.h"    //   Symbol   : CloningSymbolicInterface
#include "functions.h" //     Sin    : Symbol ...
#include "symmatrix.h" //   SymbolicMatrix : CloningSymbolicInterface
#include "symerror.h"  //   SymbolicError  : CloningSymbolicInterface
#include "constants.h"

#ifndef SYMBOLIC_CPLUSPLUS
#define SYMBOLIC_CPLUSPLUS

// Include the relevant classes in 3 phases
//  Phase 1 ensures that every class has a forward
//  declaration for use in phase 2.
//  Phase 2 ensures that every constructor and method
//  has a forward declaration for use in phase 3.
// 1. Forward declarations: class X;
// 2. Declaraions:          class X { ... };
// 3. Definitions:          X::X() ...

// This overcomes mutual recursion in dependencies,
// for example class Sum needs class Product
// and class Product needs class Sum.

// forward declarations of all classes first
#define SYMBOLIC_FORWARD
#include "symbolicc++.h"
#undef  SYMBOLIC_FORWARD

// declarations of classes without definitions
#define SYMBOLIC_DECLARE
#include "symbolicc++.h"
#undef  SYMBOLIC_DECLARE

// definitions for non-member functions
// also used in definition phase for clarity

typedef list<Equation> Equations;
typedef list<Symbolic> SymbolicList;

Symbolic expand(const SymbolicInterface &s)
{ return s.expand(); }

ostream &operator<<(ostream &o,const Symbolic &s)
{ s.print(o); return o; }

ostream &operator<<(ostream &o,const Equation &s)
{ s.print(o); return o; }

Symbolic operator+(const Symbolic &s)
{ return s; }

Symbolic operator+(const Symbolic &s1,const Symbolic &s2)
{ return Sum(s1,s2); }

Symbolic operator+(const int &s1,const Symbolic &s2)
{ return Symbolic(Number<int>(s1)) + s2; }

Symbolic operator+(const Symbolic &s1,const int &s2)
{ return s1 + Symbolic(Number<int>(s2)); }

Symbolic operator+(const double &s1,const Symbolic &s2)
{ return Symbolic(Number<double>(s1)) + s2; }

Symbolic operator+(const Symbolic &s1,const double &s2)
{ return s1 + Symbolic(Number<double>(s2)); }

Symbolic operator++(Symbolic &s)
{ return s = s + 1; }

Symbolic operator++(Symbolic &s,int)
{ Symbolic t = s; ++s; return t; }

Symbolic operator-(const Symbolic &s)
{ return Product(Number<int>(-1),s); }

Symbolic operator-(const Symbolic &s1,const Symbolic &s2)
{ return Sum(s1,-s2); }

Symbolic operator-(const int &s1,const Symbolic &s2)
{ return Symbolic(s1) - s2; }

Symbolic operator-(const Symbolic &s1,const int &s2)
{ return s1 - Symbolic(s2); }

Symbolic operator-(const double &s1,const Symbolic &s2)
{ return Symbolic(s1) - s2; }

Symbolic operator-(const Symbolic &s1,const double &s2)
{ return s1 - Symbolic(s2); }

Symbolic operator--(Symbolic &s)
{ return s = s - 1; }

Symbolic operator--(Symbolic &s,int)
{ Symbolic t = s; --s; return t; }

Symbolic operator*(const Symbolic &s1,const Symbolic &s2)
{ return Product(s1,s2); }

Symbolic operator*(const int &s1,const Symbolic &s2)
{ return Symbolic(s1) * s2; }

Symbolic operator*(const Symbolic &s1,const int &s2)
{ return s1 * Symbolic(s2); }

Symbolic operator*(const double &s1,const Symbolic &s2)
{ return Symbolic(s1) * s2; }

Symbolic operator*(const Symbolic &s1,const double &s2)
{ return s1 * Symbolic(s2); }

Symbolic operator/(const Symbolic &s1,const Symbolic &s2)
{ return Product(s1,Power(s2,Number<int>(-1))); }

Symbolic operator/(const int &s1,const Symbolic &s2)
{ return Symbolic(s1) / s2; }

Symbolic operator/(const Symbolic &s1,const int &s2)
{ return s1 / Symbolic(s2); }

Symbolic operator/(const double &s1,const Symbolic &s2)
{ return Symbolic(s1) / s2; }

Symbolic operator/(const Symbolic &s1,const double &s2)
{ return s1 / Symbolic(s2); }

Symbolic operator+=(Symbolic &s1,const Symbolic &s2)
{ return s1 = s1 + s2; }

Symbolic operator+=(Symbolic &s1,const int &s2)
{ return s1 = s1 + s2; }

Symbolic operator+=(Symbolic &s1,const double &s2)
{ return s1 = s1 + s2; }

Symbolic operator-=(Symbolic &s1,const Symbolic &s2)
{ return s1 = s1 - s2; }

Symbolic operator-=(Symbolic &s1,const int &s2)
{ return s1 = s1 - s2; }

Symbolic operator-=(Symbolic &s1,const double &s2)
{ return s1 = s1 - s2; }

Symbolic operator*=(Symbolic &s1,const Symbolic &s2)
{ return s1 = s1 * s2; }

Symbolic operator*=(Symbolic &s1,const int &s2)
{ return s1 = s1 * s2; }

Symbolic operator*=(Symbolic &s1,const double &s2)
{ return s1 = s1 * s2; }

Symbolic operator/=(Symbolic &s1,const Symbolic &s2)
{ return s1 = s1 / s2; }

Symbolic operator/=(Symbolic &s1,const int &s2)
{ return s1 = s1 / s2; }

Symbolic operator/=(Symbolic &s1,const double &s2)
{ return s1 = s1 / s2; }

Equation operator==(const Symbolic &s1,const Symbolic &s2)
{ return Equation(s1,s2); }

Equation operator==(const Symbolic &s1,int i)
{ return s1 == Symbolic(i); }

Equation operator==(int i,const Symbolic &s1)
{ return Symbolic(i) == s1; }

Equation operator==(const Symbolic &s1,double d)
{ return s1 == Symbolic(d); }

Equation operator==(double d,const Symbolic &s1)
{ return Symbolic(d) == s1; }

int operator!=(const Symbolic &s1,const Symbolic &s2)
{ return !s1.compare(s2); }

int operator!=(const Symbolic &s1,int i)
{ return s1 != Symbolic(i); }

int operator!=(int i,const Symbolic &s1)
{ return Symbolic(i) != s1; }

int operator!=(const Symbolic &s1,double d)
{ return s1 != Symbolic(d); }

int operator!=(double d,const Symbolic &s1)
{ return Symbolic(d) != s1; }

Symbolic sin(const Symbolic &s)
{ return Sin(s); }

Symbolic cos(const Symbolic &s)
{ return Cos(s); }

Symbolic tan(const Symbolic &s)
{ return sin(s) / cos(s); }

Symbolic cot(const Symbolic &s)
{ return cos(s) / sin(s); }

Symbolic sec(const Symbolic &s)
{ return 1 / cos(s); }

Symbolic csc(const Symbolic &s)
{ return 1 / sin(s); }

Symbolic sinh(const Symbolic &s)
{ return Sinh(s); }

Symbolic cosh(const Symbolic &s)
{ return Cosh(s); }

Symbolic ln(const Symbolic &s)
{ return Log(SymbolicConstant::e,s); }

Symbolic log(const Symbolic &a,const Symbolic &b)
{ return Log(a,b); }

Symbolic pow(const Symbolic &s,const Symbolic &n)
{ return Power(s,n); }

Symbolic operator^(const Symbolic &s,const Symbolic &n)
{ return Power(s,n); }

Symbolic operator^(const Symbolic &s,int i)
{ return Power(s,Symbolic(i)); }

Symbolic operator^(int i,const Symbolic &s)
{ return Power(Symbolic(i),i); }

Symbolic operator^(const Symbolic &s,double d)
{ return Power(s,Symbolic(d)); }

Symbolic operator^(double d,const Symbolic &s)
{ return Power(Symbolic(d),s); }

Symbolic exp(const Symbolic &s)
{ return SymbolicConstant::e ^ s; }

Symbolic sqrt(const Symbolic &s)
{ return s ^ (Number<int>(1) / 2); }

Symbolic df(const Symbolic &s,const Symbolic &x)
{ return s.df(x); }

Symbolic integrate(const Symbolic &s,const Symbolic &x)
{ return s.integrate(x); }

Symbolic df(const Symbolic &s,const Symbolic &x,unsigned int i)
{
 Symbolic r = s;
 while(i-- > 0) r = r.df(x);
 return r;
}

Symbolic integrate(const Symbolic &s,const Symbolic &x,unsigned int i)
{
 Symbolic r = s;
 while(i-- > 0) r = r.integrate(x);
 return r;
}

Symbolic &rhs(list<Equation> &l,const Symbolic &lhs)
{
 list<Equation>::iterator i = l.begin();
 for(i=l.begin();i!=l.end();i++)
  if(i->lhs == lhs) return i->rhs;
 cerr << "Equation list does not contain lhs " << lhs << endl;
 throw SymbolicError(SymbolicError::NoMatch);
 return i->rhs;
}

Symbolic &lhs(list<Equation> &l,const Symbolic &rhs)
{
 list<Equation>::iterator i = l.begin();
 for(i=l.begin();i!=l.end();i++)
  if(i->rhs == rhs) return i->lhs;
 cerr << "Equation list does not contain rhs " << rhs << endl;
 throw SymbolicError(SymbolicError::NoMatch);
 return i->lhs;
}

template<> Symbolic zero(Symbolic) { return Number<int>(0); }

template<> Symbolic one(Symbolic) { return Number<int>(1); }

list<Equation>
operator,(const Equation &x,const Equation &y)
{
 list<Equation> l;
 l.push_back(x); l.push_back(y);
 return l;
}

list<Equation>
operator,(const list<Equation> &x,const Equation &y)
{
 list<Equation> l(x);
 l.push_back(y);
 return l;
}

list<Equation>
operator,(const Equation &x,const list<Equation> &y)
{
 list<Equation> l(y);
 l.push_front(x);
 return l;
}

list<Symbolic>
operator,(const Symbolic &x,const Symbolic &y)
{
 list<Symbolic> l;
 l.push_back(x);
 l.push_back(y);
 return l;
}

list<Symbolic> operator,(const int &x,const Symbolic &y)
{ return (Symbolic(x), y); }

list<Symbolic> operator,(const double &x,const Symbolic &y)
{ return (Symbolic(x), y); }

list<Symbolic> operator,(const Symbolic &x,const int &y)
{ return (x,Symbolic(y)); }

list<Symbolic> operator,(const Symbolic &x,const double &y)
{ return (x,Symbolic(y)); }

list<Symbolic>
operator,(const list<Symbolic> &x,const Symbolic &y)
{
 list<Symbolic> l(x);
 l.push_back(y);
 return l;
}

list<Symbolic> operator,(const list<Symbolic> &x,const int &y)
{ return (x, Symbolic(y)); }

list<Symbolic> operator,(const list<Symbolic> &x,const double &y)
{ return (x, Symbolic(y)); }

list<Symbolic>
operator,(const Symbolic &x,const list<Symbolic> &y)
{
 list<Symbolic> l(y);
 l.push_front(x);
 return l;
}

list<Symbolic> operator,(const int &x,const list<Symbolic> &y)
{ return (Symbolic(x), y); }

list<Symbolic> operator,(const double &x,const list<Symbolic> &y)
{ return (Symbolic(x), y); }

list<list<Symbolic> >
operator,(const list<Symbolic> &x,const list<Symbolic> &y)
{
 list<list<Symbolic> > l;
 l.push_back(x); l.push_back(y);
 return l;
}

list<list<Symbolic> >
operator,(const list<list<Symbolic> > &x,const list<Symbolic> &y)
{
 list<list<Symbolic> > l(x);
 l.push_back(y);
 return l;
}

list<list<Symbolic> >
operator,(const list<Symbolic> &x,const list<list<Symbolic> > &y)
{
 list<list<Symbolic> > l(y);
 l.push_front(x);
 return l;
}

ostream &operator<<(ostream &o,const Equations &e)
{
 Equations::const_iterator i = e.begin();
 o << "[ ";
 while(i != e.end())
 {
  o << *(i++);
  if(i != e.end()) o << ",\n  ";
 }
 o << " ]" << endl;
 return o;
}

ostream &operator<<(ostream &o,const SymbolicList &e)
{
 SymbolicList::const_iterator i = e.begin();
 o << "[ ";
 while(i != e.end())
 {
  o << *(i++);
  if(i != e.end()) o << ", ";
 }
 o << " ]" << endl;
 return o;
}

Symbolic tr(const Symbolic &x) { return x.trace(); }

Symbolic trace(const Symbolic &x) { return x.trace(); }

Symbolic det(const Symbolic &x) { return x.determinant(); }

Symbolic determinant(const Symbolic &x) { return x.determinant(); }

Symbolic kron(const Symbolic &x,const Symbolic &y)
{ return x.kron(y); }

Symbolic dsum(const Symbolic &x,const Symbolic &y)
{ return x.dsum(y); }

Symbolic hadamard(const Symbolic &x,const Symbolic &y)
{ return x.hadamard(y); }

// definitions for classes, member functions
#define SYMBOLIC_DEFINE
#include "symbolicc++.h"
#undef  SYMBOLIC_DEFINE

#endif
