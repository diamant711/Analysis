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


// product.h

#ifndef SYMBOLIC_CPLUSPLUS_PRODUCT

#include <algorithm>
#include <list>
#include <vector>
using namespace std;

#ifdef  SYMBOLIC_FORWARD
#ifndef SYMBOLIC_CPLUSPLUS_PRODUCT_FORWARD
#define SYMBOLIC_CPLUSPLUS_PRODUCT_FORWARD

class Product;

#endif
#endif

#ifdef  SYMBOLIC_DECLARE
#ifndef SYMBOLIC_CPLUSPLUS_PRODUCT_DECLARE
#define SYMBOLIC_CPLUSPLUS_PRODUCT_DECLARE

class Product: public CloningSymbolicInterface
{
 public: list<Symbolic> factors;
         Product();
         Product(const Product&);
         Product(const Symbolic&,const Symbolic&);
         ~Product();

         Product &operator=(const Product&);
         int printsNegative() const;

         void print(ostream&) const;
         Symbolic subst(const Symbolic&,const Symbolic&,int &n) const;
         Simplified simplify() const;
         int compare(const Symbolic&) const;
         Symbolic df(const Symbolic&) const;
         Symbolic integrate(const Symbolic&) const;
         Symbolic coeff(const Symbolic&) const;
         Expanded expand() const;
         int commute(const Symbolic&) const;

         Cloning *clone() const { return Cloning::clone(*this); }
};

#endif
#endif

#ifdef  SYMBOLIC_DEFINE
#ifndef SYMBOLIC_CPLUSPLUS_PRODUCT_DEFINE
#define SYMBOLIC_CPLUSPLUS_PRODUCT_DEFINE
#define SYMBOLIC_CPLUSPLUS_PRODUCT

Product::Product() {}

Product::Product(const Product &s)
 : CloningSymbolicInterface(s), factors(s.factors) {}

Product::Product(const Symbolic &s1,const Symbolic &s2)
{
 if(s1.type() == typeid(Product))
  factors = CastPtr<const Product>(s1)->factors;
 else factors.push_back(s1);
 if(s2.type() == typeid(Product))
 {
  CastPtr<const Product> p(s2);
  factors.insert(factors.end(),p->factors.begin(),p->factors.end());
 }
 else factors.push_back(s2);
}

Product::~Product() {}

Product &Product::operator=(const Product &p)
{
 if(this != &p) factors = p.factors;
 return *this;
}

int Product::printsNegative() const
{
 return factors.size() > 1                        &&
        factors.front().type() == typeid(Numeric) &&
        CastPtr<const Numeric>(factors.front())->isNegative();
}

void Product::print(ostream &o) const
{
 if(factors.empty()) o << 1;
 if(factors.size() == 1) factors.begin()->print(o);
 else
  for(list<Symbolic>::const_iterator i=factors.begin();i!=factors.end();i++)
  {
   o << ((i==factors.begin()) ? "":"*");
   if(*i == -1) { o << "-"; i++; }
   if(i->type() != typeid(Product)    &&
      i->type() != typeid(Power)      &&
      i->type() != typeid(Sin)        &&
      i->type() != typeid(Cos)        &&
      i->type() != typeid(Log)        &&
      i->type() != typeid(Derivative) &&
      i->type() != typeid(Symbol)     &&
      i->type() != typeid(Numeric)) 
    { o << "("; i->print(o); o << ")"; }
   else i->print(o);
  }
}

Symbolic Product::subst(const Symbolic &x,const Symbolic &y,int &n) const
{
 if(x.type() == type())
 {
  CastPtr<const Product> p(x);
  // vector<T>::iterator has ordering comparisons
  // while list<T>::iterator does not
  list<Symbolic> u;
  vector<Symbolic> v;
  list<Symbolic>::const_iterator i;
  list<Symbolic>::const_iterator i1;
  vector<Symbolic>::iterator j, insert;
  list< vector<Symbolic>::iterator >::iterator k;
  // we store lists of locations (iterators) in v in the list l,
  // each list in l should describe a path of unique locations through v
  list<list< vector<Symbolic>::iterator > > l;
  list<list< vector<Symbolic>::iterator > >::iterator li;

  // expand positive integer powers to match each factor individually
  for(i=p->factors.begin();i!=p->factors.end();i++)
  {
   if(i->type() == typeid(Power))
   {
    CastPtr<const Power> p(*i);
    if(p->parameters.back().type() == typeid(Numeric) &&
       Number<void>(p->parameters.back()).numerictype() == typeid(int))
    {
     int n = CastPtr<const Number<int> >(p->parameters.back())->n;
     if(n>0) for(int m=0;m<n;m++) u.push_back(p->parameters.front());
     else u.push_back(*i);
    }
    else u.push_back(*i);
   }
   else u.push_back(*i);
  }

  // expand positive integer powers to match each factor individually
  for(i1=factors.begin();i1!=factors.end();i1++)
  {
   if(i1->type() == typeid(Power))
   {
    CastPtr<const Power> p(*i1);
    if(p->parameters.back().type() == typeid(Numeric) &&
       Number<void>(p->parameters.back()).numerictype() == typeid(int))
    {
     int n = CastPtr<const Number<int> >(p->parameters.back())->n;
     if(n>0) for(int m=0;m<n;m++) v.push_back(p->parameters.front());
     else v.push_back(*i1);
    }
    else v.push_back(*i1);
   }
   else v.push_back(*i1);
  }

  i = u.begin();
  
  // initialize each path in l to begin with the location
  // of the first factor of x (copied in p)
  // there could be none, one or many such paths
  for(j=v.begin();j!=v.end();j++)
   if(*j == *i) 
   {
    l.push_back(list< vector<Symbolic>::iterator >());
    l.back().push_back(j);
   }

  // build each path by considering all possible
  // locations of subsequent factors of x and creating
  // all possible paths
  for(i++;i!=u.end();i++)
  {
   list<list< vector<Symbolic>::iterator > > l2;
   for(j=v.begin();j!=v.end();j++)
    if(*j == *i) 
     for(li=l.begin();li!=l.end();li++)
      if(find(li->begin(),li->end(),j) == li->end())
      {
       l2.push_back(*li);
       l2.back().push_back(j);
      }
   l = l2;
  }

  // erase paths that are too short
  for(li=l.begin();li!=l.end();)
   if(li->size() != u.size()) l.erase(li++);
   else li++;

  // search for a path which may be substituted
  for(li=l.begin();li!=l.end();li++)
  {
   for(k=li->begin();k!=li->end();k++)
   {
    list< vector<Symbolic>::iterator >::iterator k1 = k;
    // when consecutive values are in the wrong order
    // check that they commute
    for((k1=k)++;k1!=li->end();k1++)
     if(*k1 < *k && !(*k)->commute(**k1)) break;
    if(k1!=li->end()) break;
   }
   // values cannot commute to the right order
   if(k != li->end()) continue;

   // try to find a position that all values
   // can commute to, and set that as the place for substitution
   for(insert = v.begin();insert!=v.end();insert++)
   {
    for(k=li->begin();k!=li->end();k++)
    {
     vector<Symbolic>::iterator beg, end;
     if(insert <= *k) { beg = insert; end = *k; }
     else { beg = *k + 1; end = insert; }
     for(j=beg;j!=end;j++)
      if(find(li->begin(),li->end(),j) == li->end() &&
         !j->commute(**k)) break;
     if(j != end) break;
    }
    if(k == li->end()) break;
   }
   if(insert != v.end()) break;
  }

  // found a match
  if(li != l.end() && insert != v.end())
  {
   Product resultl, resultr;

   // if the term did not play a role in the substitution just copy it
   for(j=v.begin();j!=insert;j++)
    if(find(li->begin(),li->end(),j) == li->end())
     if(j->commute(x)) resultr.factors.push_back(*j);
     else              resultl.factors.push_back(*j);

   // perform the substitution
   resultl.factors.push_back(y);
   n++;

   for(;j!=v.end();j++)
    if(find(li->begin(),li->end(),j) == li->end())
     resultr.factors.push_back(*j);

   return resultl*resultr.subst(x,y,n);
  }
 }
  
 // product does not contain expression for substitution
 // try to substitute in each factor
 Product p;
 for(list<Symbolic>::const_iterator i=factors.begin();i!=factors.end();i++)
  p.factors.push_back(i->subst(x,y,n));
 return p;
}

Simplified Product::simplify() const
{
 list<Symbolic>::const_iterator i;
 list<Symbolic>::iterator j, k, k1;
 Product r;

 // 1-element product:  (a) -> a
 if(factors.size() == 1) return factors.front().simplify();

 for(i=factors.begin();i!=factors.end();i++)
 {
  // absorb product of product: a * (a * a) * a -> a * a * a * a
  Simplified s = i->simplify();
  if(s.type() == typeid(Product))
  {
   CastPtr<const Product> product(s);
   r.factors.insert(r.factors.end(),product->factors.begin(),
                    product->factors.end());
  }
  else r.factors.push_back(s);
 }

 // if any matrices appear in the product,
 // pull everything (commutative) into the matrix
 SymbolicMatrix m(1,1,1);
 for(j=r.factors.begin();j!=r.factors.end();)
 {
  // found a matrix
  if(j->type() == typeid(SymbolicMatrix))
  {
   int br = 0;
   list<Symbolic>::iterator k;
   m = *CastPtr<SymbolicMatrix>(*j);
   // some terms preceding the matrix must be brought in from the left
   k = j;
   if(j!=r.factors.begin())
    for(k--;1;)
    {
     // this is the last element so end the loop
     if(k == r.factors.begin()) br = 1;
     // only multiply with elements that commute, i.e. "scalars"
     if(k->commute(m))
     {
      m = *k * m;
      k1 = k;
      if(k != r.factors.begin()) k--;
      r.factors.erase(k1);
     }
     else br = 1;
     if(br) break;
    }

   // some terms following the matrix must be brought in from the right
   k = j;
   for(k++;k!=r.factors.end();)
    // multiply matrices with matrices
    if(k->type() == typeid(SymbolicMatrix))
    {
     m = m * *CastPtr<SymbolicMatrix>(*k);
     r.factors.erase(k++);
    }
    // only multiply with elements that commute, i.e. "scalars"
    else if(k->commute(m))
    {
     m = m * *k;
     r.factors.erase(k++);
    }
    else break;
   
   // set *j to the resulting matrix
   *j = m.simplify();
   // set j to the next element after the last non-commuting element
   j = k;
  }
  else j++;
 }

 // group common terms
 for(j=r.factors.begin();j!=r.factors.end();j++)
 {
  // numbers will be grouped later
  if(j->type() == typeid(Numeric)) continue;

  Symbolic n = 1;
  Symbolic j1 = *j;

  // the exponent in products must be ignored in grouping comparisons
  if(j1.type() == typeid(Power))
  {
   CastPtr<Power> j2 = j1;
   n = j2->parameters.back();
   j1 = j2->parameters.front();
  }

  for((k=j)++;k!=r.factors.end() && j->commute(*k);)
  {
   // numbers will be grouped later
   if(k->type() == typeid(Numeric)) { k++; continue; }

   Symbolic k1 = *k;
   Symbolic power = 1;
   // the exponent in products must be ignored in grouping comparisons
   if(k1.type() == typeid(Power))
   {
    CastPtr<Power> k2 = k1;
    power = k2->parameters.back();
    k1 = k2->parameters.front();
   }

   if(j1 == k1)
   {
    n = n + power;
    r.factors.erase(k++);
   }
   else k++;
  }
  if(n == 0)
   *j = 1;
  else
   if(n == 1)
    *j = j1;
   else
    *j = (j1 ^ n).simplify();
 }

 // move numbers to the front
 Number<void> n = Number<int>(1);
 for(j=r.factors.begin();j!=r.factors.end();)
 {
  if(j->type() == typeid(Numeric))
  {
   n = n * Number<void>(*j);
   if(n.isZero()) return n;
   r.factors.erase(j++);
  }
  else j++;
 }

 if(!n.isOne()) r.factors.push_front(n->simplify());
 if(r.factors.size()==0) return Number<int>(1);
 if(r.factors.size()==1) return r.factors.front();
 return r;
}

int Product::compare(const Symbolic &s) const
{
 int c = 0;
 if(type() != s.type()) return 0;

 return (subst(s,Symbolic(1),c) == 1 && c == 1);
}

Symbolic Product::df(const Symbolic &s) const
{
 list<Symbolic>::iterator i;
 Product p(*this);
 Sum r;

 for(i=p.factors.begin();i!=p.factors.end();i++)
 {
  Symbolic t = *i;
  *i = i->df(s);
  r.summands.push_back(p);
  *i = t;
 }
 return r;
}

Symbolic Product::integrate(const Symbolic &s) const
{
 int count = 0;
 list<Symbolic>::const_iterator i, i1;

 for(i=factors.begin();i!=factors.end();i++)
  if(i->df(s) != 0) { count++; i1 = i; }

 if(count == 1)
 {
  Product p;
  for(i=factors.begin();i!=factors.end();i++)
   if(i == i1) p.factors.push_back(i->integrate(s));
   else        p.factors.push_back(*i);
  return p;
 }

 if(count == 0) return *this * s;
 return Integral(*this,s);
}

Symbolic Product::coeff(const Symbolic &s) const
{
 int c = 0;
 Symbolic result = subst(s,1,c);

 if(c != 1) return 0;

 if(s.type() == typeid(Product))
 {
  CastPtr<const Product> p(s);
  list<Symbolic>::const_iterator i;
  for(i=p->factors.begin();i!=p->factors.end();i++)
  {
   result.subst(*i,1,c);
   if(c != 1) return 0;
  }
 }

 if(s.type() == typeid(Power))
 {
  CastPtr<const Power> p(s);
  result.subst(p->parameters.front(),1,c);
  if(c != 1) return 0;
 }

 if(s * result == *this) return result;

 return 0;
}

Expanded Product::expand() const
{
 list<Symbolic>::const_iterator i, k;
 list<Symbolic>::iterator j;
 Product r;

 for(i=factors.begin();i!=factors.end();i++)
 {
  Symbolic s = i->expand();
  if(s.type() == typeid(Sum))
  {
   // make a copy of s
   CastPtr<Sum> sum(s,1);
   k = i; k++;
   for(j=sum->summands.begin();j!=sum->summands.end();j++)
   {
    Product p = r;
    p.factors.push_back(*j);
    p.factors.insert(p.factors.end(),k,factors.end());
    *j = p;
   }
   return sum->expand();
  }
  else r.factors.push_back(s);
 }
 return r;
}

int Product::commute(const Symbolic &s) const
{
 // Optimize the case for numbers
 if(s.type() == typeid(Numeric)) return 1;

 list<Symbolic>::const_iterator i;
 for(i=factors.begin();i!=factors.end();i++)
  if(!i->commute(s)) return 0;
 return 1;
}

#endif
#endif
#endif
