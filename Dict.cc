// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME Dict

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "RConfig.h"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Since CINT ignores the std namespace, we need to do so in this file.
namespace std {} using namespace std;

// Header files passed as explicit arguments
#include "Hit.h"
#include "Cluster.h"
#include "Cbc.h"
#include "Stub.h"
#include "Track.h"
#include "TelescopeEvent.h"
#include "Event.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void *new_tbeamcLcLhit(void *p = 0);
   static void *newArray_tbeamcLcLhit(Long_t size, void *p);
   static void delete_tbeamcLcLhit(void *p);
   static void deleteArray_tbeamcLcLhit(void *p);
   static void destruct_tbeamcLcLhit(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::tbeam::hit*)
   {
      ::tbeam::hit *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::tbeam::hit >(0);
      static ::ROOT::TGenericClassInfo 
         instance("tbeam::hit", ::tbeam::hit::Class_Version(), "Hit.h", 7,
                  typeid(::tbeam::hit), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::tbeam::hit::Dictionary, isa_proxy, 4,
                  sizeof(::tbeam::hit) );
      instance.SetNew(&new_tbeamcLcLhit);
      instance.SetNewArray(&newArray_tbeamcLcLhit);
      instance.SetDelete(&delete_tbeamcLcLhit);
      instance.SetDeleteArray(&deleteArray_tbeamcLcLhit);
      instance.SetDestructor(&destruct_tbeamcLcLhit);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::tbeam::hit*)
   {
      return GenerateInitInstanceLocal((::tbeam::hit*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::tbeam::hit*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_tbeamcLcLcluster(void *p = 0);
   static void *newArray_tbeamcLcLcluster(Long_t size, void *p);
   static void delete_tbeamcLcLcluster(void *p);
   static void deleteArray_tbeamcLcLcluster(void *p);
   static void destruct_tbeamcLcLcluster(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::tbeam::cluster*)
   {
      ::tbeam::cluster *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::tbeam::cluster >(0);
      static ::ROOT::TGenericClassInfo 
         instance("tbeam::cluster", ::tbeam::cluster::Class_Version(), "Cluster.h", 7,
                  typeid(::tbeam::cluster), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::tbeam::cluster::Dictionary, isa_proxy, 4,
                  sizeof(::tbeam::cluster) );
      instance.SetNew(&new_tbeamcLcLcluster);
      instance.SetNewArray(&newArray_tbeamcLcLcluster);
      instance.SetDelete(&delete_tbeamcLcLcluster);
      instance.SetDeleteArray(&deleteArray_tbeamcLcLcluster);
      instance.SetDestructor(&destruct_tbeamcLcLcluster);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::tbeam::cluster*)
   {
      return GenerateInitInstanceLocal((::tbeam::cluster*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::tbeam::cluster*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_tbeamcLcLcbc(void *p = 0);
   static void *newArray_tbeamcLcLcbc(Long_t size, void *p);
   static void delete_tbeamcLcLcbc(void *p);
   static void deleteArray_tbeamcLcLcbc(void *p);
   static void destruct_tbeamcLcLcbc(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::tbeam::cbc*)
   {
      ::tbeam::cbc *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::tbeam::cbc >(0);
      static ::ROOT::TGenericClassInfo 
         instance("tbeam::cbc", ::tbeam::cbc::Class_Version(), "Cbc.h", 7,
                  typeid(::tbeam::cbc), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::tbeam::cbc::Dictionary, isa_proxy, 4,
                  sizeof(::tbeam::cbc) );
      instance.SetNew(&new_tbeamcLcLcbc);
      instance.SetNewArray(&newArray_tbeamcLcLcbc);
      instance.SetDelete(&delete_tbeamcLcLcbc);
      instance.SetDeleteArray(&deleteArray_tbeamcLcLcbc);
      instance.SetDestructor(&destruct_tbeamcLcLcbc);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::tbeam::cbc*)
   {
      return GenerateInitInstanceLocal((::tbeam::cbc*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::tbeam::cbc*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_tbeamcLcLstub(void *p = 0);
   static void *newArray_tbeamcLcLstub(Long_t size, void *p);
   static void delete_tbeamcLcLstub(void *p);
   static void deleteArray_tbeamcLcLstub(void *p);
   static void destruct_tbeamcLcLstub(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::tbeam::stub*)
   {
      ::tbeam::stub *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::tbeam::stub >(0);
      static ::ROOT::TGenericClassInfo 
         instance("tbeam::stub", ::tbeam::stub::Class_Version(), "Stub.h", 7,
                  typeid(::tbeam::stub), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::tbeam::stub::Dictionary, isa_proxy, 4,
                  sizeof(::tbeam::stub) );
      instance.SetNew(&new_tbeamcLcLstub);
      instance.SetNewArray(&newArray_tbeamcLcLstub);
      instance.SetDelete(&delete_tbeamcLcLstub);
      instance.SetDeleteArray(&deleteArray_tbeamcLcLstub);
      instance.SetDestructor(&destruct_tbeamcLcLstub);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::tbeam::stub*)
   {
      return GenerateInitInstanceLocal((::tbeam::stub*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::tbeam::stub*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_tbeamcLcLTrack(void *p = 0);
   static void *newArray_tbeamcLcLTrack(Long_t size, void *p);
   static void delete_tbeamcLcLTrack(void *p);
   static void deleteArray_tbeamcLcLTrack(void *p);
   static void destruct_tbeamcLcLTrack(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::tbeam::Track*)
   {
      ::tbeam::Track *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::tbeam::Track >(0);
      static ::ROOT::TGenericClassInfo 
         instance("tbeam::Track", ::tbeam::Track::Class_Version(), "Track.h", 7,
                  typeid(::tbeam::Track), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::tbeam::Track::Dictionary, isa_proxy, 4,
                  sizeof(::tbeam::Track) );
      instance.SetNew(&new_tbeamcLcLTrack);
      instance.SetNewArray(&newArray_tbeamcLcLTrack);
      instance.SetDelete(&delete_tbeamcLcLTrack);
      instance.SetDeleteArray(&deleteArray_tbeamcLcLTrack);
      instance.SetDestructor(&destruct_tbeamcLcLTrack);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::tbeam::Track*)
   {
      return GenerateInitInstanceLocal((::tbeam::Track*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::tbeam::Track*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_tbeamcLcLEvent(void *p = 0);
   static void *newArray_tbeamcLcLEvent(Long_t size, void *p);
   static void delete_tbeamcLcLEvent(void *p);
   static void deleteArray_tbeamcLcLEvent(void *p);
   static void destruct_tbeamcLcLEvent(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::tbeam::Event*)
   {
      ::tbeam::Event *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::tbeam::Event >(0);
      static ::ROOT::TGenericClassInfo 
         instance("tbeam::Event", ::tbeam::Event::Class_Version(), "Event.h", 23,
                  typeid(::tbeam::Event), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::tbeam::Event::Dictionary, isa_proxy, 4,
                  sizeof(::tbeam::Event) );
      instance.SetNew(&new_tbeamcLcLEvent);
      instance.SetNewArray(&newArray_tbeamcLcLEvent);
      instance.SetDelete(&delete_tbeamcLcLEvent);
      instance.SetDeleteArray(&deleteArray_tbeamcLcLEvent);
      instance.SetDestructor(&destruct_tbeamcLcLEvent);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::tbeam::Event*)
   {
      return GenerateInitInstanceLocal((::tbeam::Event*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::tbeam::Event*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace tbeam {
//______________________________________________________________________________
atomic_TClass_ptr hit::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *hit::Class_Name()
{
   return "tbeam::hit";
}

//______________________________________________________________________________
const char *hit::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::tbeam::hit*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int hit::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::tbeam::hit*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *hit::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::tbeam::hit*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *hit::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::tbeam::hit*)0x0)->GetClass(); }
   return fgIsA;
}

} // namespace tbeam
namespace tbeam {
//______________________________________________________________________________
atomic_TClass_ptr cluster::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *cluster::Class_Name()
{
   return "tbeam::cluster";
}

//______________________________________________________________________________
const char *cluster::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::tbeam::cluster*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int cluster::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::tbeam::cluster*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *cluster::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::tbeam::cluster*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *cluster::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::tbeam::cluster*)0x0)->GetClass(); }
   return fgIsA;
}

} // namespace tbeam
namespace tbeam {
//______________________________________________________________________________
atomic_TClass_ptr cbc::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *cbc::Class_Name()
{
   return "tbeam::cbc";
}

//______________________________________________________________________________
const char *cbc::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::tbeam::cbc*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int cbc::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::tbeam::cbc*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *cbc::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::tbeam::cbc*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *cbc::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::tbeam::cbc*)0x0)->GetClass(); }
   return fgIsA;
}

} // namespace tbeam
namespace tbeam {
//______________________________________________________________________________
atomic_TClass_ptr stub::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *stub::Class_Name()
{
   return "tbeam::stub";
}

//______________________________________________________________________________
const char *stub::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::tbeam::stub*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int stub::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::tbeam::stub*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *stub::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::tbeam::stub*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *stub::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::tbeam::stub*)0x0)->GetClass(); }
   return fgIsA;
}

} // namespace tbeam
namespace tbeam {
//______________________________________________________________________________
atomic_TClass_ptr Track::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *Track::Class_Name()
{
   return "tbeam::Track";
}

//______________________________________________________________________________
const char *Track::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::tbeam::Track*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int Track::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::tbeam::Track*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *Track::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::tbeam::Track*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *Track::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::tbeam::Track*)0x0)->GetClass(); }
   return fgIsA;
}

} // namespace tbeam
namespace tbeam {
//______________________________________________________________________________
atomic_TClass_ptr Event::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *Event::Class_Name()
{
   return "tbeam::Event";
}

//______________________________________________________________________________
const char *Event::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::tbeam::Event*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int Event::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::tbeam::Event*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *Event::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::tbeam::Event*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *Event::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::tbeam::Event*)0x0)->GetClass(); }
   return fgIsA;
}

} // namespace tbeam
namespace tbeam {
//______________________________________________________________________________
void hit::Streamer(TBuffer &R__b)
{
   // Stream an object of class tbeam::hit.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(tbeam::hit::Class(),this);
   } else {
      R__b.WriteClassBuffer(tbeam::hit::Class(),this);
   }
}

} // namespace tbeam
namespace ROOT {
   // Wrappers around operator new
   static void *new_tbeamcLcLhit(void *p) {
      return  p ? new(p) ::tbeam::hit : new ::tbeam::hit;
   }
   static void *newArray_tbeamcLcLhit(Long_t nElements, void *p) {
      return p ? new(p) ::tbeam::hit[nElements] : new ::tbeam::hit[nElements];
   }
   // Wrapper around operator delete
   static void delete_tbeamcLcLhit(void *p) {
      delete ((::tbeam::hit*)p);
   }
   static void deleteArray_tbeamcLcLhit(void *p) {
      delete [] ((::tbeam::hit*)p);
   }
   static void destruct_tbeamcLcLhit(void *p) {
      typedef ::tbeam::hit current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::tbeam::hit

namespace tbeam {
//______________________________________________________________________________
void cluster::Streamer(TBuffer &R__b)
{
   // Stream an object of class tbeam::cluster.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(tbeam::cluster::Class(),this);
   } else {
      R__b.WriteClassBuffer(tbeam::cluster::Class(),this);
   }
}

} // namespace tbeam
namespace ROOT {
   // Wrappers around operator new
   static void *new_tbeamcLcLcluster(void *p) {
      return  p ? new(p) ::tbeam::cluster : new ::tbeam::cluster;
   }
   static void *newArray_tbeamcLcLcluster(Long_t nElements, void *p) {
      return p ? new(p) ::tbeam::cluster[nElements] : new ::tbeam::cluster[nElements];
   }
   // Wrapper around operator delete
   static void delete_tbeamcLcLcluster(void *p) {
      delete ((::tbeam::cluster*)p);
   }
   static void deleteArray_tbeamcLcLcluster(void *p) {
      delete [] ((::tbeam::cluster*)p);
   }
   static void destruct_tbeamcLcLcluster(void *p) {
      typedef ::tbeam::cluster current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::tbeam::cluster

namespace tbeam {
//______________________________________________________________________________
void cbc::Streamer(TBuffer &R__b)
{
   // Stream an object of class tbeam::cbc.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(tbeam::cbc::Class(),this);
   } else {
      R__b.WriteClassBuffer(tbeam::cbc::Class(),this);
   }
}

} // namespace tbeam
namespace ROOT {
   // Wrappers around operator new
   static void *new_tbeamcLcLcbc(void *p) {
      return  p ? new(p) ::tbeam::cbc : new ::tbeam::cbc;
   }
   static void *newArray_tbeamcLcLcbc(Long_t nElements, void *p) {
      return p ? new(p) ::tbeam::cbc[nElements] : new ::tbeam::cbc[nElements];
   }
   // Wrapper around operator delete
   static void delete_tbeamcLcLcbc(void *p) {
      delete ((::tbeam::cbc*)p);
   }
   static void deleteArray_tbeamcLcLcbc(void *p) {
      delete [] ((::tbeam::cbc*)p);
   }
   static void destruct_tbeamcLcLcbc(void *p) {
      typedef ::tbeam::cbc current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::tbeam::cbc

namespace tbeam {
//______________________________________________________________________________
void stub::Streamer(TBuffer &R__b)
{
   // Stream an object of class tbeam::stub.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(tbeam::stub::Class(),this);
   } else {
      R__b.WriteClassBuffer(tbeam::stub::Class(),this);
   }
}

} // namespace tbeam
namespace ROOT {
   // Wrappers around operator new
   static void *new_tbeamcLcLstub(void *p) {
      return  p ? new(p) ::tbeam::stub : new ::tbeam::stub;
   }
   static void *newArray_tbeamcLcLstub(Long_t nElements, void *p) {
      return p ? new(p) ::tbeam::stub[nElements] : new ::tbeam::stub[nElements];
   }
   // Wrapper around operator delete
   static void delete_tbeamcLcLstub(void *p) {
      delete ((::tbeam::stub*)p);
   }
   static void deleteArray_tbeamcLcLstub(void *p) {
      delete [] ((::tbeam::stub*)p);
   }
   static void destruct_tbeamcLcLstub(void *p) {
      typedef ::tbeam::stub current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::tbeam::stub

namespace tbeam {
//______________________________________________________________________________
void Track::Streamer(TBuffer &R__b)
{
   // Stream an object of class tbeam::Track.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(tbeam::Track::Class(),this);
   } else {
      R__b.WriteClassBuffer(tbeam::Track::Class(),this);
   }
}

} // namespace tbeam
namespace ROOT {
   // Wrappers around operator new
   static void *new_tbeamcLcLTrack(void *p) {
      return  p ? new(p) ::tbeam::Track : new ::tbeam::Track;
   }
   static void *newArray_tbeamcLcLTrack(Long_t nElements, void *p) {
      return p ? new(p) ::tbeam::Track[nElements] : new ::tbeam::Track[nElements];
   }
   // Wrapper around operator delete
   static void delete_tbeamcLcLTrack(void *p) {
      delete ((::tbeam::Track*)p);
   }
   static void deleteArray_tbeamcLcLTrack(void *p) {
      delete [] ((::tbeam::Track*)p);
   }
   static void destruct_tbeamcLcLTrack(void *p) {
      typedef ::tbeam::Track current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::tbeam::Track

namespace tbeam {
//______________________________________________________________________________
void Event::Streamer(TBuffer &R__b)
{
   // Stream an object of class tbeam::Event.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(tbeam::Event::Class(),this);
   } else {
      R__b.WriteClassBuffer(tbeam::Event::Class(),this);
   }
}

} // namespace tbeam
namespace ROOT {
   // Wrappers around operator new
   static void *new_tbeamcLcLEvent(void *p) {
      return  p ? new(p) ::tbeam::Event : new ::tbeam::Event;
   }
   static void *newArray_tbeamcLcLEvent(Long_t nElements, void *p) {
      return p ? new(p) ::tbeam::Event[nElements] : new ::tbeam::Event[nElements];
   }
   // Wrapper around operator delete
   static void delete_tbeamcLcLEvent(void *p) {
      delete ((::tbeam::Event*)p);
   }
   static void deleteArray_tbeamcLcLEvent(void *p) {
      delete [] ((::tbeam::Event*)p);
   }
   static void destruct_tbeamcLcLEvent(void *p) {
      typedef ::tbeam::Event current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::tbeam::Event

namespace ROOT {
   static TClass *vectorlEunsignedsPshortgR_Dictionary();
   static void vectorlEunsignedsPshortgR_TClassManip(TClass*);
   static void *new_vectorlEunsignedsPshortgR(void *p = 0);
   static void *newArray_vectorlEunsignedsPshortgR(Long_t size, void *p);
   static void delete_vectorlEunsignedsPshortgR(void *p);
   static void deleteArray_vectorlEunsignedsPshortgR(void *p);
   static void destruct_vectorlEunsignedsPshortgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<unsigned short>*)
   {
      vector<unsigned short> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<unsigned short>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<unsigned short>", -2, "vector", 214,
                  typeid(vector<unsigned short>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEunsignedsPshortgR_Dictionary, isa_proxy, 4,
                  sizeof(vector<unsigned short>) );
      instance.SetNew(&new_vectorlEunsignedsPshortgR);
      instance.SetNewArray(&newArray_vectorlEunsignedsPshortgR);
      instance.SetDelete(&delete_vectorlEunsignedsPshortgR);
      instance.SetDeleteArray(&deleteArray_vectorlEunsignedsPshortgR);
      instance.SetDestructor(&destruct_vectorlEunsignedsPshortgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<unsigned short> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<unsigned short>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEunsignedsPshortgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<unsigned short>*)0x0)->GetClass();
      vectorlEunsignedsPshortgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEunsignedsPshortgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEunsignedsPshortgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<unsigned short> : new vector<unsigned short>;
   }
   static void *newArray_vectorlEunsignedsPshortgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<unsigned short>[nElements] : new vector<unsigned short>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEunsignedsPshortgR(void *p) {
      delete ((vector<unsigned short>*)p);
   }
   static void deleteArray_vectorlEunsignedsPshortgR(void *p) {
      delete [] ((vector<unsigned short>*)p);
   }
   static void destruct_vectorlEunsignedsPshortgR(void *p) {
      typedef vector<unsigned short> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<unsigned short>

namespace ROOT {
   static TClass *vectorlEtbeamcLcLstubgR_Dictionary();
   static void vectorlEtbeamcLcLstubgR_TClassManip(TClass*);
   static void *new_vectorlEtbeamcLcLstubgR(void *p = 0);
   static void *newArray_vectorlEtbeamcLcLstubgR(Long_t size, void *p);
   static void delete_vectorlEtbeamcLcLstubgR(void *p);
   static void deleteArray_vectorlEtbeamcLcLstubgR(void *p);
   static void destruct_vectorlEtbeamcLcLstubgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<tbeam::stub>*)
   {
      vector<tbeam::stub> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<tbeam::stub>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<tbeam::stub>", -2, "vector", 214,
                  typeid(vector<tbeam::stub>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEtbeamcLcLstubgR_Dictionary, isa_proxy, 4,
                  sizeof(vector<tbeam::stub>) );
      instance.SetNew(&new_vectorlEtbeamcLcLstubgR);
      instance.SetNewArray(&newArray_vectorlEtbeamcLcLstubgR);
      instance.SetDelete(&delete_vectorlEtbeamcLcLstubgR);
      instance.SetDeleteArray(&deleteArray_vectorlEtbeamcLcLstubgR);
      instance.SetDestructor(&destruct_vectorlEtbeamcLcLstubgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<tbeam::stub> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<tbeam::stub>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEtbeamcLcLstubgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<tbeam::stub>*)0x0)->GetClass();
      vectorlEtbeamcLcLstubgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEtbeamcLcLstubgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEtbeamcLcLstubgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<tbeam::stub> : new vector<tbeam::stub>;
   }
   static void *newArray_vectorlEtbeamcLcLstubgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<tbeam::stub>[nElements] : new vector<tbeam::stub>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEtbeamcLcLstubgR(void *p) {
      delete ((vector<tbeam::stub>*)p);
   }
   static void deleteArray_vectorlEtbeamcLcLstubgR(void *p) {
      delete [] ((vector<tbeam::stub>*)p);
   }
   static void destruct_vectorlEtbeamcLcLstubgR(void *p) {
      typedef vector<tbeam::stub> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<tbeam::stub>

namespace ROOT {
   static TClass *vectorlEtbeamcLcLstubmUgR_Dictionary();
   static void vectorlEtbeamcLcLstubmUgR_TClassManip(TClass*);
   static void *new_vectorlEtbeamcLcLstubmUgR(void *p = 0);
   static void *newArray_vectorlEtbeamcLcLstubmUgR(Long_t size, void *p);
   static void delete_vectorlEtbeamcLcLstubmUgR(void *p);
   static void deleteArray_vectorlEtbeamcLcLstubmUgR(void *p);
   static void destruct_vectorlEtbeamcLcLstubmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<tbeam::stub*>*)
   {
      vector<tbeam::stub*> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<tbeam::stub*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<tbeam::stub*>", -2, "vector", 214,
                  typeid(vector<tbeam::stub*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEtbeamcLcLstubmUgR_Dictionary, isa_proxy, 4,
                  sizeof(vector<tbeam::stub*>) );
      instance.SetNew(&new_vectorlEtbeamcLcLstubmUgR);
      instance.SetNewArray(&newArray_vectorlEtbeamcLcLstubmUgR);
      instance.SetDelete(&delete_vectorlEtbeamcLcLstubmUgR);
      instance.SetDeleteArray(&deleteArray_vectorlEtbeamcLcLstubmUgR);
      instance.SetDestructor(&destruct_vectorlEtbeamcLcLstubmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<tbeam::stub*> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<tbeam::stub*>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEtbeamcLcLstubmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<tbeam::stub*>*)0x0)->GetClass();
      vectorlEtbeamcLcLstubmUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEtbeamcLcLstubmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEtbeamcLcLstubmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<tbeam::stub*> : new vector<tbeam::stub*>;
   }
   static void *newArray_vectorlEtbeamcLcLstubmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<tbeam::stub*>[nElements] : new vector<tbeam::stub*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEtbeamcLcLstubmUgR(void *p) {
      delete ((vector<tbeam::stub*>*)p);
   }
   static void deleteArray_vectorlEtbeamcLcLstubmUgR(void *p) {
      delete [] ((vector<tbeam::stub*>*)p);
   }
   static void destruct_vectorlEtbeamcLcLstubmUgR(void *p) {
      typedef vector<tbeam::stub*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<tbeam::stub*>

namespace ROOT {
   static TClass *vectorlEtbeamcLcLhitgR_Dictionary();
   static void vectorlEtbeamcLcLhitgR_TClassManip(TClass*);
   static void *new_vectorlEtbeamcLcLhitgR(void *p = 0);
   static void *newArray_vectorlEtbeamcLcLhitgR(Long_t size, void *p);
   static void delete_vectorlEtbeamcLcLhitgR(void *p);
   static void deleteArray_vectorlEtbeamcLcLhitgR(void *p);
   static void destruct_vectorlEtbeamcLcLhitgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<tbeam::hit>*)
   {
      vector<tbeam::hit> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<tbeam::hit>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<tbeam::hit>", -2, "vector", 214,
                  typeid(vector<tbeam::hit>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEtbeamcLcLhitgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<tbeam::hit>) );
      instance.SetNew(&new_vectorlEtbeamcLcLhitgR);
      instance.SetNewArray(&newArray_vectorlEtbeamcLcLhitgR);
      instance.SetDelete(&delete_vectorlEtbeamcLcLhitgR);
      instance.SetDeleteArray(&deleteArray_vectorlEtbeamcLcLhitgR);
      instance.SetDestructor(&destruct_vectorlEtbeamcLcLhitgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<tbeam::hit> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<tbeam::hit>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEtbeamcLcLhitgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<tbeam::hit>*)0x0)->GetClass();
      vectorlEtbeamcLcLhitgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEtbeamcLcLhitgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEtbeamcLcLhitgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<tbeam::hit> : new vector<tbeam::hit>;
   }
   static void *newArray_vectorlEtbeamcLcLhitgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<tbeam::hit>[nElements] : new vector<tbeam::hit>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEtbeamcLcLhitgR(void *p) {
      delete ((vector<tbeam::hit>*)p);
   }
   static void deleteArray_vectorlEtbeamcLcLhitgR(void *p) {
      delete [] ((vector<tbeam::hit>*)p);
   }
   static void destruct_vectorlEtbeamcLcLhitgR(void *p) {
      typedef vector<tbeam::hit> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<tbeam::hit>

namespace ROOT {
   static TClass *vectorlEtbeamcLcLclustergR_Dictionary();
   static void vectorlEtbeamcLcLclustergR_TClassManip(TClass*);
   static void *new_vectorlEtbeamcLcLclustergR(void *p = 0);
   static void *newArray_vectorlEtbeamcLcLclustergR(Long_t size, void *p);
   static void delete_vectorlEtbeamcLcLclustergR(void *p);
   static void deleteArray_vectorlEtbeamcLcLclustergR(void *p);
   static void destruct_vectorlEtbeamcLcLclustergR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<tbeam::cluster>*)
   {
      vector<tbeam::cluster> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<tbeam::cluster>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<tbeam::cluster>", -2, "vector", 214,
                  typeid(vector<tbeam::cluster>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEtbeamcLcLclustergR_Dictionary, isa_proxy, 4,
                  sizeof(vector<tbeam::cluster>) );
      instance.SetNew(&new_vectorlEtbeamcLcLclustergR);
      instance.SetNewArray(&newArray_vectorlEtbeamcLcLclustergR);
      instance.SetDelete(&delete_vectorlEtbeamcLcLclustergR);
      instance.SetDeleteArray(&deleteArray_vectorlEtbeamcLcLclustergR);
      instance.SetDestructor(&destruct_vectorlEtbeamcLcLclustergR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<tbeam::cluster> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<tbeam::cluster>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEtbeamcLcLclustergR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<tbeam::cluster>*)0x0)->GetClass();
      vectorlEtbeamcLcLclustergR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEtbeamcLcLclustergR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEtbeamcLcLclustergR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<tbeam::cluster> : new vector<tbeam::cluster>;
   }
   static void *newArray_vectorlEtbeamcLcLclustergR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<tbeam::cluster>[nElements] : new vector<tbeam::cluster>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEtbeamcLcLclustergR(void *p) {
      delete ((vector<tbeam::cluster>*)p);
   }
   static void deleteArray_vectorlEtbeamcLcLclustergR(void *p) {
      delete [] ((vector<tbeam::cluster>*)p);
   }
   static void destruct_vectorlEtbeamcLcLclustergR(void *p) {
      typedef vector<tbeam::cluster> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<tbeam::cluster>

namespace ROOT {
   static TClass *vectorlEtbeamcLcLclustermUgR_Dictionary();
   static void vectorlEtbeamcLcLclustermUgR_TClassManip(TClass*);
   static void *new_vectorlEtbeamcLcLclustermUgR(void *p = 0);
   static void *newArray_vectorlEtbeamcLcLclustermUgR(Long_t size, void *p);
   static void delete_vectorlEtbeamcLcLclustermUgR(void *p);
   static void deleteArray_vectorlEtbeamcLcLclustermUgR(void *p);
   static void destruct_vectorlEtbeamcLcLclustermUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<tbeam::cluster*>*)
   {
      vector<tbeam::cluster*> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<tbeam::cluster*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<tbeam::cluster*>", -2, "vector", 214,
                  typeid(vector<tbeam::cluster*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEtbeamcLcLclustermUgR_Dictionary, isa_proxy, 4,
                  sizeof(vector<tbeam::cluster*>) );
      instance.SetNew(&new_vectorlEtbeamcLcLclustermUgR);
      instance.SetNewArray(&newArray_vectorlEtbeamcLcLclustermUgR);
      instance.SetDelete(&delete_vectorlEtbeamcLcLclustermUgR);
      instance.SetDeleteArray(&deleteArray_vectorlEtbeamcLcLclustermUgR);
      instance.SetDestructor(&destruct_vectorlEtbeamcLcLclustermUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<tbeam::cluster*> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<tbeam::cluster*>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEtbeamcLcLclustermUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<tbeam::cluster*>*)0x0)->GetClass();
      vectorlEtbeamcLcLclustermUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEtbeamcLcLclustermUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEtbeamcLcLclustermUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<tbeam::cluster*> : new vector<tbeam::cluster*>;
   }
   static void *newArray_vectorlEtbeamcLcLclustermUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<tbeam::cluster*>[nElements] : new vector<tbeam::cluster*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEtbeamcLcLclustermUgR(void *p) {
      delete ((vector<tbeam::cluster*>*)p);
   }
   static void deleteArray_vectorlEtbeamcLcLclustermUgR(void *p) {
      delete [] ((vector<tbeam::cluster*>*)p);
   }
   static void destruct_vectorlEtbeamcLcLclustermUgR(void *p) {
      typedef vector<tbeam::cluster*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<tbeam::cluster*>

namespace ROOT {
   static TClass *vectorlEtbeamcLcLcbcgR_Dictionary();
   static void vectorlEtbeamcLcLcbcgR_TClassManip(TClass*);
   static void *new_vectorlEtbeamcLcLcbcgR(void *p = 0);
   static void *newArray_vectorlEtbeamcLcLcbcgR(Long_t size, void *p);
   static void delete_vectorlEtbeamcLcLcbcgR(void *p);
   static void deleteArray_vectorlEtbeamcLcLcbcgR(void *p);
   static void destruct_vectorlEtbeamcLcLcbcgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<tbeam::cbc>*)
   {
      vector<tbeam::cbc> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<tbeam::cbc>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<tbeam::cbc>", -2, "vector", 214,
                  typeid(vector<tbeam::cbc>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEtbeamcLcLcbcgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<tbeam::cbc>) );
      instance.SetNew(&new_vectorlEtbeamcLcLcbcgR);
      instance.SetNewArray(&newArray_vectorlEtbeamcLcLcbcgR);
      instance.SetDelete(&delete_vectorlEtbeamcLcLcbcgR);
      instance.SetDeleteArray(&deleteArray_vectorlEtbeamcLcLcbcgR);
      instance.SetDestructor(&destruct_vectorlEtbeamcLcLcbcgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<tbeam::cbc> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<tbeam::cbc>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEtbeamcLcLcbcgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<tbeam::cbc>*)0x0)->GetClass();
      vectorlEtbeamcLcLcbcgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEtbeamcLcLcbcgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEtbeamcLcLcbcgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<tbeam::cbc> : new vector<tbeam::cbc>;
   }
   static void *newArray_vectorlEtbeamcLcLcbcgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<tbeam::cbc>[nElements] : new vector<tbeam::cbc>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEtbeamcLcLcbcgR(void *p) {
      delete ((vector<tbeam::cbc>*)p);
   }
   static void deleteArray_vectorlEtbeamcLcLcbcgR(void *p) {
      delete [] ((vector<tbeam::cbc>*)p);
   }
   static void destruct_vectorlEtbeamcLcLcbcgR(void *p) {
      typedef vector<tbeam::cbc> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<tbeam::cbc>

namespace ROOT {
   static TClass *vectorlEtbeamcLcLTrackgR_Dictionary();
   static void vectorlEtbeamcLcLTrackgR_TClassManip(TClass*);
   static void *new_vectorlEtbeamcLcLTrackgR(void *p = 0);
   static void *newArray_vectorlEtbeamcLcLTrackgR(Long_t size, void *p);
   static void delete_vectorlEtbeamcLcLTrackgR(void *p);
   static void deleteArray_vectorlEtbeamcLcLTrackgR(void *p);
   static void destruct_vectorlEtbeamcLcLTrackgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<tbeam::Track>*)
   {
      vector<tbeam::Track> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<tbeam::Track>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<tbeam::Track>", -2, "vector", 214,
                  typeid(vector<tbeam::Track>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEtbeamcLcLTrackgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<tbeam::Track>) );
      instance.SetNew(&new_vectorlEtbeamcLcLTrackgR);
      instance.SetNewArray(&newArray_vectorlEtbeamcLcLTrackgR);
      instance.SetDelete(&delete_vectorlEtbeamcLcLTrackgR);
      instance.SetDeleteArray(&deleteArray_vectorlEtbeamcLcLTrackgR);
      instance.SetDestructor(&destruct_vectorlEtbeamcLcLTrackgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<tbeam::Track> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<tbeam::Track>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEtbeamcLcLTrackgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<tbeam::Track>*)0x0)->GetClass();
      vectorlEtbeamcLcLTrackgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEtbeamcLcLTrackgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEtbeamcLcLTrackgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<tbeam::Track> : new vector<tbeam::Track>;
   }
   static void *newArray_vectorlEtbeamcLcLTrackgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<tbeam::Track>[nElements] : new vector<tbeam::Track>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEtbeamcLcLTrackgR(void *p) {
      delete ((vector<tbeam::Track>*)p);
   }
   static void deleteArray_vectorlEtbeamcLcLTrackgR(void *p) {
      delete [] ((vector<tbeam::Track>*)p);
   }
   static void destruct_vectorlEtbeamcLcLTrackgR(void *p) {
      typedef vector<tbeam::Track> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<tbeam::Track>

namespace ROOT {
   static TClass *vectorlEintgR_Dictionary();
   static void vectorlEintgR_TClassManip(TClass*);
   static void *new_vectorlEintgR(void *p = 0);
   static void *newArray_vectorlEintgR(Long_t size, void *p);
   static void delete_vectorlEintgR(void *p);
   static void deleteArray_vectorlEintgR(void *p);
   static void destruct_vectorlEintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<int>*)
   {
      vector<int> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<int>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<int>", -2, "vector", 214,
                  typeid(vector<int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEintgR_Dictionary, isa_proxy, 4,
                  sizeof(vector<int>) );
      instance.SetNew(&new_vectorlEintgR);
      instance.SetNewArray(&newArray_vectorlEintgR);
      instance.SetDelete(&delete_vectorlEintgR);
      instance.SetDeleteArray(&deleteArray_vectorlEintgR);
      instance.SetDestructor(&destruct_vectorlEintgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<int> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<int>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<int>*)0x0)->GetClass();
      vectorlEintgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEintgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<int> : new vector<int>;
   }
   static void *newArray_vectorlEintgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<int>[nElements] : new vector<int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEintgR(void *p) {
      delete ((vector<int>*)p);
   }
   static void deleteArray_vectorlEintgR(void *p) {
      delete [] ((vector<int>*)p);
   }
   static void destruct_vectorlEintgR(void *p) {
      typedef vector<int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<int>

namespace ROOT {
   static TClass *vectorlEdoublegR_Dictionary();
   static void vectorlEdoublegR_TClassManip(TClass*);
   static void *new_vectorlEdoublegR(void *p = 0);
   static void *newArray_vectorlEdoublegR(Long_t size, void *p);
   static void delete_vectorlEdoublegR(void *p);
   static void deleteArray_vectorlEdoublegR(void *p);
   static void destruct_vectorlEdoublegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<double>*)
   {
      vector<double> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<double>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<double>", -2, "vector", 214,
                  typeid(vector<double>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEdoublegR_Dictionary, isa_proxy, 4,
                  sizeof(vector<double>) );
      instance.SetNew(&new_vectorlEdoublegR);
      instance.SetNewArray(&newArray_vectorlEdoublegR);
      instance.SetDelete(&delete_vectorlEdoublegR);
      instance.SetDeleteArray(&deleteArray_vectorlEdoublegR);
      instance.SetDestructor(&destruct_vectorlEdoublegR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<double> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<double>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEdoublegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<double>*)0x0)->GetClass();
      vectorlEdoublegR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEdoublegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEdoublegR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<double> : new vector<double>;
   }
   static void *newArray_vectorlEdoublegR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<double>[nElements] : new vector<double>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEdoublegR(void *p) {
      delete ((vector<double>*)p);
   }
   static void deleteArray_vectorlEdoublegR(void *p) {
      delete [] ((vector<double>*)p);
   }
   static void destruct_vectorlEdoublegR(void *p) {
      typedef vector<double> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<double>

namespace ROOT {
   static TClass *maplEunsignedsPintcOvectorlEtbeamcLcLcbcgRsPgR_Dictionary();
   static void maplEunsignedsPintcOvectorlEtbeamcLcLcbcgRsPgR_TClassManip(TClass*);
   static void *new_maplEunsignedsPintcOvectorlEtbeamcLcLcbcgRsPgR(void *p = 0);
   static void *newArray_maplEunsignedsPintcOvectorlEtbeamcLcLcbcgRsPgR(Long_t size, void *p);
   static void delete_maplEunsignedsPintcOvectorlEtbeamcLcLcbcgRsPgR(void *p);
   static void deleteArray_maplEunsignedsPintcOvectorlEtbeamcLcLcbcgRsPgR(void *p);
   static void destruct_maplEunsignedsPintcOvectorlEtbeamcLcLcbcgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<unsigned int,vector<tbeam::cbc> >*)
   {
      map<unsigned int,vector<tbeam::cbc> > *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<unsigned int,vector<tbeam::cbc> >));
      static ::ROOT::TGenericClassInfo 
         instance("map<unsigned int,vector<tbeam::cbc> >", -2, "map", 96,
                  typeid(map<unsigned int,vector<tbeam::cbc> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEunsignedsPintcOvectorlEtbeamcLcLcbcgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(map<unsigned int,vector<tbeam::cbc> >) );
      instance.SetNew(&new_maplEunsignedsPintcOvectorlEtbeamcLcLcbcgRsPgR);
      instance.SetNewArray(&newArray_maplEunsignedsPintcOvectorlEtbeamcLcLcbcgRsPgR);
      instance.SetDelete(&delete_maplEunsignedsPintcOvectorlEtbeamcLcLcbcgRsPgR);
      instance.SetDeleteArray(&deleteArray_maplEunsignedsPintcOvectorlEtbeamcLcLcbcgRsPgR);
      instance.SetDestructor(&destruct_maplEunsignedsPintcOvectorlEtbeamcLcLcbcgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<unsigned int,vector<tbeam::cbc> > >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<unsigned int,vector<tbeam::cbc> >*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEunsignedsPintcOvectorlEtbeamcLcLcbcgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<unsigned int,vector<tbeam::cbc> >*)0x0)->GetClass();
      maplEunsignedsPintcOvectorlEtbeamcLcLcbcgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEunsignedsPintcOvectorlEtbeamcLcLcbcgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEunsignedsPintcOvectorlEtbeamcLcLcbcgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<unsigned int,vector<tbeam::cbc> > : new map<unsigned int,vector<tbeam::cbc> >;
   }
   static void *newArray_maplEunsignedsPintcOvectorlEtbeamcLcLcbcgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<unsigned int,vector<tbeam::cbc> >[nElements] : new map<unsigned int,vector<tbeam::cbc> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEunsignedsPintcOvectorlEtbeamcLcLcbcgRsPgR(void *p) {
      delete ((map<unsigned int,vector<tbeam::cbc> >*)p);
   }
   static void deleteArray_maplEunsignedsPintcOvectorlEtbeamcLcLcbcgRsPgR(void *p) {
      delete [] ((map<unsigned int,vector<tbeam::cbc> >*)p);
   }
   static void destruct_maplEunsignedsPintcOvectorlEtbeamcLcLcbcgRsPgR(void *p) {
      typedef map<unsigned int,vector<tbeam::cbc> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<unsigned int,vector<tbeam::cbc> >

namespace ROOT {
   static TClass *maplEunsignedsPintcOunsignedsPintgR_Dictionary();
   static void maplEunsignedsPintcOunsignedsPintgR_TClassManip(TClass*);
   static void *new_maplEunsignedsPintcOunsignedsPintgR(void *p = 0);
   static void *newArray_maplEunsignedsPintcOunsignedsPintgR(Long_t size, void *p);
   static void delete_maplEunsignedsPintcOunsignedsPintgR(void *p);
   static void deleteArray_maplEunsignedsPintcOunsignedsPintgR(void *p);
   static void destruct_maplEunsignedsPintcOunsignedsPintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<unsigned int,unsigned int>*)
   {
      map<unsigned int,unsigned int> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<unsigned int,unsigned int>));
      static ::ROOT::TGenericClassInfo 
         instance("map<unsigned int,unsigned int>", -2, "map", 96,
                  typeid(map<unsigned int,unsigned int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEunsignedsPintcOunsignedsPintgR_Dictionary, isa_proxy, 0,
                  sizeof(map<unsigned int,unsigned int>) );
      instance.SetNew(&new_maplEunsignedsPintcOunsignedsPintgR);
      instance.SetNewArray(&newArray_maplEunsignedsPintcOunsignedsPintgR);
      instance.SetDelete(&delete_maplEunsignedsPintcOunsignedsPintgR);
      instance.SetDeleteArray(&deleteArray_maplEunsignedsPintcOunsignedsPintgR);
      instance.SetDestructor(&destruct_maplEunsignedsPintcOunsignedsPintgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<unsigned int,unsigned int> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<unsigned int,unsigned int>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEunsignedsPintcOunsignedsPintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<unsigned int,unsigned int>*)0x0)->GetClass();
      maplEunsignedsPintcOunsignedsPintgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEunsignedsPintcOunsignedsPintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEunsignedsPintcOunsignedsPintgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<unsigned int,unsigned int> : new map<unsigned int,unsigned int>;
   }
   static void *newArray_maplEunsignedsPintcOunsignedsPintgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<unsigned int,unsigned int>[nElements] : new map<unsigned int,unsigned int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEunsignedsPintcOunsignedsPintgR(void *p) {
      delete ((map<unsigned int,unsigned int>*)p);
   }
   static void deleteArray_maplEunsignedsPintcOunsignedsPintgR(void *p) {
      delete [] ((map<unsigned int,unsigned int>*)p);
   }
   static void destruct_maplEunsignedsPintcOunsignedsPintgR(void *p) {
      typedef map<unsigned int,unsigned int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<unsigned int,unsigned int>

namespace ROOT {
   static TClass *maplEstringcOvectorlEunsignedsPshortgRsPgR_Dictionary();
   static void maplEstringcOvectorlEunsignedsPshortgRsPgR_TClassManip(TClass*);
   static void *new_maplEstringcOvectorlEunsignedsPshortgRsPgR(void *p = 0);
   static void *newArray_maplEstringcOvectorlEunsignedsPshortgRsPgR(Long_t size, void *p);
   static void delete_maplEstringcOvectorlEunsignedsPshortgRsPgR(void *p);
   static void deleteArray_maplEstringcOvectorlEunsignedsPshortgRsPgR(void *p);
   static void destruct_maplEstringcOvectorlEunsignedsPshortgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<string,vector<unsigned short> >*)
   {
      map<string,vector<unsigned short> > *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<string,vector<unsigned short> >));
      static ::ROOT::TGenericClassInfo 
         instance("map<string,vector<unsigned short> >", -2, "map", 96,
                  typeid(map<string,vector<unsigned short> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEstringcOvectorlEunsignedsPshortgRsPgR_Dictionary, isa_proxy, 4,
                  sizeof(map<string,vector<unsigned short> >) );
      instance.SetNew(&new_maplEstringcOvectorlEunsignedsPshortgRsPgR);
      instance.SetNewArray(&newArray_maplEstringcOvectorlEunsignedsPshortgRsPgR);
      instance.SetDelete(&delete_maplEstringcOvectorlEunsignedsPshortgRsPgR);
      instance.SetDeleteArray(&deleteArray_maplEstringcOvectorlEunsignedsPshortgRsPgR);
      instance.SetDestructor(&destruct_maplEstringcOvectorlEunsignedsPshortgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<string,vector<unsigned short> > >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<string,vector<unsigned short> >*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEstringcOvectorlEunsignedsPshortgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<string,vector<unsigned short> >*)0x0)->GetClass();
      maplEstringcOvectorlEunsignedsPshortgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEstringcOvectorlEunsignedsPshortgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEstringcOvectorlEunsignedsPshortgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<string,vector<unsigned short> > : new map<string,vector<unsigned short> >;
   }
   static void *newArray_maplEstringcOvectorlEunsignedsPshortgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<string,vector<unsigned short> >[nElements] : new map<string,vector<unsigned short> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEstringcOvectorlEunsignedsPshortgRsPgR(void *p) {
      delete ((map<string,vector<unsigned short> >*)p);
   }
   static void deleteArray_maplEstringcOvectorlEunsignedsPshortgRsPgR(void *p) {
      delete [] ((map<string,vector<unsigned short> >*)p);
   }
   static void destruct_maplEstringcOvectorlEunsignedsPshortgRsPgR(void *p) {
      typedef map<string,vector<unsigned short> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<string,vector<unsigned short> >

namespace ROOT {
   static TClass *maplEstringcOvectorlEtbeamcLcLstubgRsPgR_Dictionary();
   static void maplEstringcOvectorlEtbeamcLcLstubgRsPgR_TClassManip(TClass*);
   static void *new_maplEstringcOvectorlEtbeamcLcLstubgRsPgR(void *p = 0);
   static void *newArray_maplEstringcOvectorlEtbeamcLcLstubgRsPgR(Long_t size, void *p);
   static void delete_maplEstringcOvectorlEtbeamcLcLstubgRsPgR(void *p);
   static void deleteArray_maplEstringcOvectorlEtbeamcLcLstubgRsPgR(void *p);
   static void destruct_maplEstringcOvectorlEtbeamcLcLstubgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<string,vector<tbeam::stub> >*)
   {
      map<string,vector<tbeam::stub> > *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<string,vector<tbeam::stub> >));
      static ::ROOT::TGenericClassInfo 
         instance("map<string,vector<tbeam::stub> >", -2, "map", 96,
                  typeid(map<string,vector<tbeam::stub> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEstringcOvectorlEtbeamcLcLstubgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(map<string,vector<tbeam::stub> >) );
      instance.SetNew(&new_maplEstringcOvectorlEtbeamcLcLstubgRsPgR);
      instance.SetNewArray(&newArray_maplEstringcOvectorlEtbeamcLcLstubgRsPgR);
      instance.SetDelete(&delete_maplEstringcOvectorlEtbeamcLcLstubgRsPgR);
      instance.SetDeleteArray(&deleteArray_maplEstringcOvectorlEtbeamcLcLstubgRsPgR);
      instance.SetDestructor(&destruct_maplEstringcOvectorlEtbeamcLcLstubgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<string,vector<tbeam::stub> > >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<string,vector<tbeam::stub> >*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEstringcOvectorlEtbeamcLcLstubgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<string,vector<tbeam::stub> >*)0x0)->GetClass();
      maplEstringcOvectorlEtbeamcLcLstubgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEstringcOvectorlEtbeamcLcLstubgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEstringcOvectorlEtbeamcLcLstubgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<string,vector<tbeam::stub> > : new map<string,vector<tbeam::stub> >;
   }
   static void *newArray_maplEstringcOvectorlEtbeamcLcLstubgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<string,vector<tbeam::stub> >[nElements] : new map<string,vector<tbeam::stub> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEstringcOvectorlEtbeamcLcLstubgRsPgR(void *p) {
      delete ((map<string,vector<tbeam::stub> >*)p);
   }
   static void deleteArray_maplEstringcOvectorlEtbeamcLcLstubgRsPgR(void *p) {
      delete [] ((map<string,vector<tbeam::stub> >*)p);
   }
   static void destruct_maplEstringcOvectorlEtbeamcLcLstubgRsPgR(void *p) {
      typedef map<string,vector<tbeam::stub> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<string,vector<tbeam::stub> >

namespace ROOT {
   static TClass *maplEstringcOvectorlEtbeamcLcLhitgRsPgR_Dictionary();
   static void maplEstringcOvectorlEtbeamcLcLhitgRsPgR_TClassManip(TClass*);
   static void *new_maplEstringcOvectorlEtbeamcLcLhitgRsPgR(void *p = 0);
   static void *newArray_maplEstringcOvectorlEtbeamcLcLhitgRsPgR(Long_t size, void *p);
   static void delete_maplEstringcOvectorlEtbeamcLcLhitgRsPgR(void *p);
   static void deleteArray_maplEstringcOvectorlEtbeamcLcLhitgRsPgR(void *p);
   static void destruct_maplEstringcOvectorlEtbeamcLcLhitgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<string,vector<tbeam::hit> >*)
   {
      map<string,vector<tbeam::hit> > *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<string,vector<tbeam::hit> >));
      static ::ROOT::TGenericClassInfo 
         instance("map<string,vector<tbeam::hit> >", -2, "map", 96,
                  typeid(map<string,vector<tbeam::hit> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEstringcOvectorlEtbeamcLcLhitgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(map<string,vector<tbeam::hit> >) );
      instance.SetNew(&new_maplEstringcOvectorlEtbeamcLcLhitgRsPgR);
      instance.SetNewArray(&newArray_maplEstringcOvectorlEtbeamcLcLhitgRsPgR);
      instance.SetDelete(&delete_maplEstringcOvectorlEtbeamcLcLhitgRsPgR);
      instance.SetDeleteArray(&deleteArray_maplEstringcOvectorlEtbeamcLcLhitgRsPgR);
      instance.SetDestructor(&destruct_maplEstringcOvectorlEtbeamcLcLhitgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<string,vector<tbeam::hit> > >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<string,vector<tbeam::hit> >*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEstringcOvectorlEtbeamcLcLhitgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<string,vector<tbeam::hit> >*)0x0)->GetClass();
      maplEstringcOvectorlEtbeamcLcLhitgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEstringcOvectorlEtbeamcLcLhitgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEstringcOvectorlEtbeamcLcLhitgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<string,vector<tbeam::hit> > : new map<string,vector<tbeam::hit> >;
   }
   static void *newArray_maplEstringcOvectorlEtbeamcLcLhitgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<string,vector<tbeam::hit> >[nElements] : new map<string,vector<tbeam::hit> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEstringcOvectorlEtbeamcLcLhitgRsPgR(void *p) {
      delete ((map<string,vector<tbeam::hit> >*)p);
   }
   static void deleteArray_maplEstringcOvectorlEtbeamcLcLhitgRsPgR(void *p) {
      delete [] ((map<string,vector<tbeam::hit> >*)p);
   }
   static void destruct_maplEstringcOvectorlEtbeamcLcLhitgRsPgR(void *p) {
      typedef map<string,vector<tbeam::hit> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<string,vector<tbeam::hit> >

namespace ROOT {
   static TClass *maplEstringcOvectorlEtbeamcLcLclustergRsPgR_Dictionary();
   static void maplEstringcOvectorlEtbeamcLcLclustergRsPgR_TClassManip(TClass*);
   static void *new_maplEstringcOvectorlEtbeamcLcLclustergRsPgR(void *p = 0);
   static void *newArray_maplEstringcOvectorlEtbeamcLcLclustergRsPgR(Long_t size, void *p);
   static void delete_maplEstringcOvectorlEtbeamcLcLclustergRsPgR(void *p);
   static void deleteArray_maplEstringcOvectorlEtbeamcLcLclustergRsPgR(void *p);
   static void destruct_maplEstringcOvectorlEtbeamcLcLclustergRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<string,vector<tbeam::cluster> >*)
   {
      map<string,vector<tbeam::cluster> > *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<string,vector<tbeam::cluster> >));
      static ::ROOT::TGenericClassInfo 
         instance("map<string,vector<tbeam::cluster> >", -2, "map", 96,
                  typeid(map<string,vector<tbeam::cluster> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEstringcOvectorlEtbeamcLcLclustergRsPgR_Dictionary, isa_proxy, 4,
                  sizeof(map<string,vector<tbeam::cluster> >) );
      instance.SetNew(&new_maplEstringcOvectorlEtbeamcLcLclustergRsPgR);
      instance.SetNewArray(&newArray_maplEstringcOvectorlEtbeamcLcLclustergRsPgR);
      instance.SetDelete(&delete_maplEstringcOvectorlEtbeamcLcLclustergRsPgR);
      instance.SetDeleteArray(&deleteArray_maplEstringcOvectorlEtbeamcLcLclustergRsPgR);
      instance.SetDestructor(&destruct_maplEstringcOvectorlEtbeamcLcLclustergRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<string,vector<tbeam::cluster> > >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<string,vector<tbeam::cluster> >*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEstringcOvectorlEtbeamcLcLclustergRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<string,vector<tbeam::cluster> >*)0x0)->GetClass();
      maplEstringcOvectorlEtbeamcLcLclustergRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEstringcOvectorlEtbeamcLcLclustergRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEstringcOvectorlEtbeamcLcLclustergRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<string,vector<tbeam::cluster> > : new map<string,vector<tbeam::cluster> >;
   }
   static void *newArray_maplEstringcOvectorlEtbeamcLcLclustergRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<string,vector<tbeam::cluster> >[nElements] : new map<string,vector<tbeam::cluster> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEstringcOvectorlEtbeamcLcLclustergRsPgR(void *p) {
      delete ((map<string,vector<tbeam::cluster> >*)p);
   }
   static void deleteArray_maplEstringcOvectorlEtbeamcLcLclustergRsPgR(void *p) {
      delete [] ((map<string,vector<tbeam::cluster> >*)p);
   }
   static void destruct_maplEstringcOvectorlEtbeamcLcLclustergRsPgR(void *p) {
      typedef map<string,vector<tbeam::cluster> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<string,vector<tbeam::cluster> >

namespace ROOT {
   static TClass *maplEstringcOvectorlEtbeamcLcLclustermUgRsPgR_Dictionary();
   static void maplEstringcOvectorlEtbeamcLcLclustermUgRsPgR_TClassManip(TClass*);
   static void *new_maplEstringcOvectorlEtbeamcLcLclustermUgRsPgR(void *p = 0);
   static void *newArray_maplEstringcOvectorlEtbeamcLcLclustermUgRsPgR(Long_t size, void *p);
   static void delete_maplEstringcOvectorlEtbeamcLcLclustermUgRsPgR(void *p);
   static void deleteArray_maplEstringcOvectorlEtbeamcLcLclustermUgRsPgR(void *p);
   static void destruct_maplEstringcOvectorlEtbeamcLcLclustermUgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<string,vector<tbeam::cluster*> >*)
   {
      map<string,vector<tbeam::cluster*> > *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<string,vector<tbeam::cluster*> >));
      static ::ROOT::TGenericClassInfo 
         instance("map<string,vector<tbeam::cluster*> >", -2, "map", 96,
                  typeid(map<string,vector<tbeam::cluster*> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEstringcOvectorlEtbeamcLcLclustermUgRsPgR_Dictionary, isa_proxy, 4,
                  sizeof(map<string,vector<tbeam::cluster*> >) );
      instance.SetNew(&new_maplEstringcOvectorlEtbeamcLcLclustermUgRsPgR);
      instance.SetNewArray(&newArray_maplEstringcOvectorlEtbeamcLcLclustermUgRsPgR);
      instance.SetDelete(&delete_maplEstringcOvectorlEtbeamcLcLclustermUgRsPgR);
      instance.SetDeleteArray(&deleteArray_maplEstringcOvectorlEtbeamcLcLclustermUgRsPgR);
      instance.SetDestructor(&destruct_maplEstringcOvectorlEtbeamcLcLclustermUgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<string,vector<tbeam::cluster*> > >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<string,vector<tbeam::cluster*> >*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEstringcOvectorlEtbeamcLcLclustermUgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<string,vector<tbeam::cluster*> >*)0x0)->GetClass();
      maplEstringcOvectorlEtbeamcLcLclustermUgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEstringcOvectorlEtbeamcLcLclustermUgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEstringcOvectorlEtbeamcLcLclustermUgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<string,vector<tbeam::cluster*> > : new map<string,vector<tbeam::cluster*> >;
   }
   static void *newArray_maplEstringcOvectorlEtbeamcLcLclustermUgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<string,vector<tbeam::cluster*> >[nElements] : new map<string,vector<tbeam::cluster*> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEstringcOvectorlEtbeamcLcLclustermUgRsPgR(void *p) {
      delete ((map<string,vector<tbeam::cluster*> >*)p);
   }
   static void deleteArray_maplEstringcOvectorlEtbeamcLcLclustermUgRsPgR(void *p) {
      delete [] ((map<string,vector<tbeam::cluster*> >*)p);
   }
   static void destruct_maplEstringcOvectorlEtbeamcLcLclustermUgRsPgR(void *p) {
      typedef map<string,vector<tbeam::cluster*> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<string,vector<tbeam::cluster*> >

namespace ROOT {
   static TClass *maplEstringcOvectorlEintgRsPgR_Dictionary();
   static void maplEstringcOvectorlEintgRsPgR_TClassManip(TClass*);
   static void *new_maplEstringcOvectorlEintgRsPgR(void *p = 0);
   static void *newArray_maplEstringcOvectorlEintgRsPgR(Long_t size, void *p);
   static void delete_maplEstringcOvectorlEintgRsPgR(void *p);
   static void deleteArray_maplEstringcOvectorlEintgRsPgR(void *p);
   static void destruct_maplEstringcOvectorlEintgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<string,vector<int> >*)
   {
      map<string,vector<int> > *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<string,vector<int> >));
      static ::ROOT::TGenericClassInfo 
         instance("map<string,vector<int> >", -2, "map", 96,
                  typeid(map<string,vector<int> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEstringcOvectorlEintgRsPgR_Dictionary, isa_proxy, 4,
                  sizeof(map<string,vector<int> >) );
      instance.SetNew(&new_maplEstringcOvectorlEintgRsPgR);
      instance.SetNewArray(&newArray_maplEstringcOvectorlEintgRsPgR);
      instance.SetDelete(&delete_maplEstringcOvectorlEintgRsPgR);
      instance.SetDeleteArray(&deleteArray_maplEstringcOvectorlEintgRsPgR);
      instance.SetDestructor(&destruct_maplEstringcOvectorlEintgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<string,vector<int> > >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<string,vector<int> >*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEstringcOvectorlEintgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<string,vector<int> >*)0x0)->GetClass();
      maplEstringcOvectorlEintgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEstringcOvectorlEintgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEstringcOvectorlEintgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<string,vector<int> > : new map<string,vector<int> >;
   }
   static void *newArray_maplEstringcOvectorlEintgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<string,vector<int> >[nElements] : new map<string,vector<int> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEstringcOvectorlEintgRsPgR(void *p) {
      delete ((map<string,vector<int> >*)p);
   }
   static void deleteArray_maplEstringcOvectorlEintgRsPgR(void *p) {
      delete [] ((map<string,vector<int> >*)p);
   }
   static void destruct_maplEstringcOvectorlEintgRsPgR(void *p) {
      typedef map<string,vector<int> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<string,vector<int> >

namespace ROOT {
   static TClass *maplEstringcOvectorlEdoublegRsPgR_Dictionary();
   static void maplEstringcOvectorlEdoublegRsPgR_TClassManip(TClass*);
   static void *new_maplEstringcOvectorlEdoublegRsPgR(void *p = 0);
   static void *newArray_maplEstringcOvectorlEdoublegRsPgR(Long_t size, void *p);
   static void delete_maplEstringcOvectorlEdoublegRsPgR(void *p);
   static void deleteArray_maplEstringcOvectorlEdoublegRsPgR(void *p);
   static void destruct_maplEstringcOvectorlEdoublegRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<string,vector<double> >*)
   {
      map<string,vector<double> > *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<string,vector<double> >));
      static ::ROOT::TGenericClassInfo 
         instance("map<string,vector<double> >", -2, "map", 96,
                  typeid(map<string,vector<double> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEstringcOvectorlEdoublegRsPgR_Dictionary, isa_proxy, 4,
                  sizeof(map<string,vector<double> >) );
      instance.SetNew(&new_maplEstringcOvectorlEdoublegRsPgR);
      instance.SetNewArray(&newArray_maplEstringcOvectorlEdoublegRsPgR);
      instance.SetDelete(&delete_maplEstringcOvectorlEdoublegRsPgR);
      instance.SetDeleteArray(&deleteArray_maplEstringcOvectorlEdoublegRsPgR);
      instance.SetDestructor(&destruct_maplEstringcOvectorlEdoublegRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<string,vector<double> > >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<string,vector<double> >*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEstringcOvectorlEdoublegRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<string,vector<double> >*)0x0)->GetClass();
      maplEstringcOvectorlEdoublegRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEstringcOvectorlEdoublegRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEstringcOvectorlEdoublegRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<string,vector<double> > : new map<string,vector<double> >;
   }
   static void *newArray_maplEstringcOvectorlEdoublegRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<string,vector<double> >[nElements] : new map<string,vector<double> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEstringcOvectorlEdoublegRsPgR(void *p) {
      delete ((map<string,vector<double> >*)p);
   }
   static void deleteArray_maplEstringcOvectorlEdoublegRsPgR(void *p) {
      delete [] ((map<string,vector<double> >*)p);
   }
   static void destruct_maplEstringcOvectorlEdoublegRsPgR(void *p) {
      typedef map<string,vector<double> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<string,vector<double> >

namespace ROOT {
   static TClass *maplEstringcOintgR_Dictionary();
   static void maplEstringcOintgR_TClassManip(TClass*);
   static void *new_maplEstringcOintgR(void *p = 0);
   static void *newArray_maplEstringcOintgR(Long_t size, void *p);
   static void delete_maplEstringcOintgR(void *p);
   static void deleteArray_maplEstringcOintgR(void *p);
   static void destruct_maplEstringcOintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<string,int>*)
   {
      map<string,int> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<string,int>));
      static ::ROOT::TGenericClassInfo 
         instance("map<string,int>", -2, "map", 96,
                  typeid(map<string,int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEstringcOintgR_Dictionary, isa_proxy, 4,
                  sizeof(map<string,int>) );
      instance.SetNew(&new_maplEstringcOintgR);
      instance.SetNewArray(&newArray_maplEstringcOintgR);
      instance.SetDelete(&delete_maplEstringcOintgR);
      instance.SetDeleteArray(&deleteArray_maplEstringcOintgR);
      instance.SetDestructor(&destruct_maplEstringcOintgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<string,int> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<string,int>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEstringcOintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<string,int>*)0x0)->GetClass();
      maplEstringcOintgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEstringcOintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEstringcOintgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<string,int> : new map<string,int>;
   }
   static void *newArray_maplEstringcOintgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<string,int>[nElements] : new map<string,int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEstringcOintgR(void *p) {
      delete ((map<string,int>*)p);
   }
   static void deleteArray_maplEstringcOintgR(void *p) {
      delete [] ((map<string,int>*)p);
   }
   static void destruct_maplEstringcOintgR(void *p) {
      typedef map<string,int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<string,int>

namespace {
  void TriggerDictionaryInitialization_Dict_Impl() {
    static const char* headers[] = {
"Hit.h",
"Cluster.h",
"Cbc.h",
"Stub.h",
"Track.h",
"TelescopeEvent.h",
"Event.h",
0
    };
    static const char* includePaths[] = {
"/cvmfs/cms.cern.ch/slc6_amd64_gcc630/lcg/root/6.10.04-ghjeda/include",
"/afs/cern.ch/work/a/ade/public/CMSSW_9_3_0/src/TestBeamSoftware/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "Dict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
namespace tbeam{class __attribute__((annotate("$clingAutoload$Cluster.h")))  cluster;}
namespace std{template <typename _Tp> class __attribute__((annotate("$clingAutoload$bits/allocator.h")))  __attribute__((annotate("$clingAutoload$string")))  allocator;
}
namespace tbeam{class __attribute__((annotate("$clingAutoload$Stub.h")))  stub;}
namespace tbeam{class __attribute__((annotate("$clingAutoload$Hit.h")))  hit;}
namespace tbeam{class __attribute__((annotate("$clingAutoload$Cbc.h")))  cbc;}
namespace tbeam{class __attribute__((annotate("$clingAutoload$Track.h")))  Track;}
namespace std{template <class _CharT> struct __attribute__((annotate("$clingAutoload$bits/char_traits.h")))  __attribute__((annotate("$clingAutoload$string")))  char_traits;
}
namespace tbeam{class __attribute__((annotate("$clingAutoload$Event.h")))  Event;}
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "Dict dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "Hit.h"
#include "Cluster.h"
#include "Cbc.h"
#include "Stub.h"
#include "Track.h"
#include "TelescopeEvent.h"
#include "Event.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"tbeam::Event", payloadCode, "@",
"tbeam::Track", payloadCode, "@",
"tbeam::cbc", payloadCode, "@",
"tbeam::cluster", payloadCode, "@",
"tbeam::hit", payloadCode, "@",
"tbeam::stub", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("Dict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_Dict_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_Dict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_Dict() {
  TriggerDictionaryInitialization_Dict_Impl();
}
