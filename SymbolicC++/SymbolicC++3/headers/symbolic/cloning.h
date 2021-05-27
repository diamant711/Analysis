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


// cloning.h

#ifndef SYMBOLIC_CPLUSPLUS_CLONING
#define SYMBOLIC_CPLUSPLUS_CLONING

using namespace std;

class Cloning
{
 public: Cloning();
         Cloning(const Cloning&);
         virtual Cloning *clone() const = 0;
	 template <class T> static Cloning *clone(const T&);
         virtual ~Cloning() {}
         virtual Cloning *reference(int) const;
         virtual int unreference() const;
         int refcount;
};

class CloningPtr
{
 private: Cloning *value;
 public:  CloningPtr();
          CloningPtr(const Cloning&,int=0);
          CloningPtr(const CloningPtr&,int=0);
          ~CloningPtr();

          CloningPtr &operator=(const CloningPtr&);
          CloningPtr &operator=(const Cloning&);

          Cloning *operator->() const;
          Cloning &operator*() const;
};

template <class T>
class CastPtr: public CloningPtr
{
 public:  CastPtr();
          CastPtr(const Cloning&,int=0);
          CastPtr(const CloningPtr&,int=0);
          ~CastPtr();

          T *operator->() const;
          T &operator*() const;
};

///////////////////////////////
// Cloning Implementation    //
///////////////////////////////

Cloning::Cloning() : refcount(0) {}

Cloning::Cloning(const Cloning &c) : refcount(0) {}

template <class T> Cloning *Cloning::clone(const T &t)
{ return new T(t); }

Cloning *Cloning::reference(int forceclone) const
{
 Cloning *c;
 if(refcount == 0 || forceclone)
 {
  c = clone();
  c->refcount = 1;
 }
 else
 {
  // we have to cast away the constness,
  // but we do know *this was dynamically allocated
  c = const_cast<Cloning*>(this);
  (c->refcount)++;
 }
 return c;
}

int Cloning::unreference() const
{
 if(refcount == 0) return 0;
 if(refcount == 1) return 1;
 // we have to cast away the constness,
 // but we do know *this was dynamically allocated
 (const_cast<Cloning*>(this)->refcount)--;
 return 0;
}

///////////////////////////////
// CloningPtr Implementation //
///////////////////////////////

CloningPtr::CloningPtr() : value(NULL) {}

CloningPtr::CloningPtr(const Cloning &p,int forceclone)
{ value = p.reference(forceclone); }

CloningPtr::CloningPtr(const CloningPtr &p,int forceclone)
{
 if(p.value == NULL) value = NULL;
 else value = p.value->reference(forceclone);
}

CloningPtr::~CloningPtr()
{
 if(value != NULL)
  if(value->unreference())
   delete value;
}

CloningPtr &CloningPtr::operator=(const Cloning &p)
{
 Cloning *v = value;
 if(value == &p) return *this;
 value = p.reference(0);
 if(v != NULL && v->unreference()) delete v;
 return *this;
}

CloningPtr &CloningPtr::operator=(const CloningPtr &p)
{
 Cloning *v = value;
 if(this == &p) return *this;
 if(value == p.value) return *this;
 if(p.value == NULL) value = NULL;
 else value = p.value->reference(0);
 if(v != NULL && v->unreference()) delete v;
 return *this;
}

Cloning *CloningPtr::operator->() const
{ return value; }

Cloning &CloningPtr::operator*() const
{ return *value; }

///////////////////////////////
// CastPtr Implementation    //
///////////////////////////////

template <class T> CastPtr<T>::CastPtr() : CloningPtr() {}

template <class T> CastPtr<T>::CastPtr(const Cloning &p,int forceclone)
 : CloningPtr(p,forceclone) {}

template <class T> CastPtr<T>::CastPtr(const CloningPtr &p,int forceclone)
 : CloningPtr(p,forceclone) {}

template <class T> CastPtr<T>::~CastPtr() {}

template <class T> T *CastPtr<T>::operator->() const
{ return dynamic_cast<T*>(CloningPtr::operator->()); }

template <class T> T &CastPtr<T>::operator*() const
{ return *operator->(); }

#endif
