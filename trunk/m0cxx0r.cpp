// m0cxx0r - Compiler Generated Mock Objects For C++
// Copyright (c) 2008, Jim Purbrick
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice, 
//   this list of conditions and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
// * Neither the name of the <ORGANIZATION> nor the names of its contributors
//   may be used to endorse or promote products derived from this software 
//   without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
// POSSIBILITY OF SUCH DAMAGE.

#include <iostream>
#include <vector>
#include <string>
#include <sstream>

namespace m0cxx0r
{
  class IParam
  {
  public:
    IParam() {;}
    virtual ~IParam() {;}
    virtual IParam* clone() = 0;
    virtual void setValue(unsigned char* p0) = 0;
    virtual bool isEqual(IParam* iparam) = 0;
    virtual std::string getString() = 0;
  };

  template<typename ParamType>
  struct Param : public IParam
  {
  public:

    Param(unsigned char* p0, ParamType* param) :
      mOffset((unsigned char*)(param) - p0),
      mParam(*param)
    {
    }

    virtual ~Param() {;}

    virtual IParam* clone()
    {
      Param* result = new Param();
      result->mOffset = mOffset;
      result->mParam = mParam;
      return result;
    }

    virtual void setValue(unsigned char* p0)
    {
      ParamType* param = (ParamType*)(p0 + mOffset);
      mParam = *param;
    }

    virtual bool isEqual(IParam* iparam)
    {
      Param* param = static_cast<Param*>(iparam);
      return mParam == param->mParam;
    }

    virtual std::string getString()
    {
      std::ostringstream stream;
      stream << mParam << std::ends;
      return stream.str();
    }

  private:

    Param() {;}

    int mOffset;
    ParamType mParam;
  };

  class Call
  {
  public:
    Call(const std::string& name) :
      mName(name)
    {
    }

    virtual ~Call() {;}

    virtual Call* clone()
    {
      Call* result = new Call(mName);
      result->mIndex = mIndex;
      std::vector<IParam*>::iterator i;
      for(i = mParameters.begin(); i != mParameters.end(); ++i)
      {
	result->addParameter((*i)->clone());
      }
      return result;
    }

    void addParameter(IParam* param) {mParameters.push_back(param);}

    size_t getIndex()
    {
      return mIndex;
    }

    std::string getName()
    {
      return mName;
    }

    void setIndex(size_t index)
    {
      mIndex = index;
    }

    void setParams(unsigned char* params)
    {
      ParamVector::iterator i;
      for(i = mParameters.begin(); i != mParameters.end(); ++i)
      {
	(*i)->setValue(params);
      }
    }

    bool verify(Call* actual)
    {
      bool result = true;
      size_t index = 0;
      ParamVector::const_iterator expectedIter = mParameters.begin();
      ParamVector::const_iterator actualIter = actual->mParameters.begin();
      while(expectedIter != mParameters.end())
      {
	if(! (*expectedIter)->isEqual(*actualIter))
	{
	  result = false;
	  std::cerr << "Failure! m0cxx0r"
		    << " expected " << (*expectedIter)->getString()
	            << " as parameter " << index
		    << " of " << getName() << " call"
		    << " (actual " << (*actualIter)->getString() << ')'
		    << std::endl;
	}
	++index;
	++expectedIter;
	++actualIter;
      }
      return result;
    }

  private:
    std::string mName;
    size_t mIndex;
    typedef std::vector<IParam*> ParamVector;
    ParamVector mParameters;
  };

template<typename T>
class Mock : public T
{
public:
  
  static Mock* create()
  {
    // Allocate memory for mock.
    size_t mockSize = sizeof(Mock<T>);
    unsigned char* mockData = new unsigned char[mockSize];
    Mock<T>* mock = reinterpret_cast<Mock<T>*>(mockData);
    
    // Point mock vptr to donor vtable.
    VTableDonor donor;
    *(void**) mock = *(void**)&donor;

    // Initialize mock data members.
    mock->mExpectedCalls = std::vector<Call*>();
    mock->mActualCalls = std::vector<Call*>();
    mock->mRecordingExpected = false;

    return mock;
  }

  static void destroy(Mock** mock)
  {
    // Delete byte array created for mock.
    unsigned char* mockData = (unsigned char*) (*mock);
    delete[] mockData;

    // Null given pointer.
    mock = NULL;
  }

  template<typename MemberFuncType>
  void expect(const std::string& name, MemberFuncType func)
  {
    mRecordingExpected = true;
    Call* expectedCall = new Call(name);
    mExpectedCalls.push_back(expectedCall);
    ((this)->*(func))();
    mRecordingExpected = false;
  }

  template<typename MemberFuncType, typename ParamType1>
  void expect(const std::string& name, MemberFuncType func, ParamType1 p1)
  {
    mRecordingExpected = true;
    Call* expectedCall = new Call(name);
    expectedCall->addParameter(new Param<ParamType1>((unsigned char*)(&p1), &p1));
    mExpectedCalls.push_back(expectedCall);
    ((this)->*(func))(p1);
    mRecordingExpected = false;
  }

  bool verify()
  {
    bool result = true;
    CallVector::const_iterator actualIter = mActualCalls.begin();
    CallVector::const_iterator expectedIter = mExpectedCalls.begin();
    while(actualIter != mActualCalls.end())
    {
      result &= (*expectedIter)->verify(*actualIter);
      ++actualIter;
      ++expectedIter;
    }
    while(expectedIter != mExpectedCalls.end())
    {
      std::cerr << "Failure! m0cxx0r expected "
		<< (*expectedIter)->getName() << " call" << std::endl;
      ++expectedIter;
    }
    return result;
  }

private:

  Mock()
  {
    std::cerr << "Error! m0cxx0r::Mock objects must not be constructed" 
	      << std::endl;
  }

  virtual ~Mock()
  {
    std::cerr << "Error! m0cxx0r::Mock objects must not be deleted" 
	      << std::endl;
  }

  class VTableDonor
  {
    virtual void f1(size_t p0) {((Mock*) this)->recordCall(1, (unsigned char*)(&p0));}
    virtual void f2(size_t p0) {((Mock*) this)->recordCall(2, (unsigned char*)(&p0));}
    virtual void f3(size_t p0) {((Mock*) this)->recordCall(3, (unsigned char*)(&p0));}
    // TODO: Add more virtual methods as needed.
  };

  void recordCall(size_t index, unsigned char* params)
  {
    if(mRecordingExpected)
    {
      Call* call = mExpectedCalls.back();
      call->setIndex(index);
      call->setParams(params);
    }
    else
    {
      size_t callIndex = mActualCalls.size();
      Call* expectedCall = mExpectedCalls[callIndex];
      if(expectedCall->getIndex() == index)
      {
	Call* actualCall = expectedCall->clone();
	actualCall->setParams(params);
	mActualCalls.push_back(actualCall);
      }
    }
  }

  typedef std::vector<Call*> CallVector;
  CallVector mExpectedCalls;
  CallVector mActualCalls;
  bool mRecordingExpected;
};

} // namespace m0cxx0r

class ProductionClass
{
public:
  ProductionClass(size_t param)
  {
    std::cerr << "Error! ProductionClass ctor should never be called" << std::endl;
  }
  
  virtual void foo() {;}
  virtual void bar(size_t) {;}
  virtual void baz() {;}
};

int main()
{
  typedef m0cxx0r::Mock<ProductionClass> MockClass;
  MockClass* mock = MockClass::create();
  mock->expect("foo", &ProductionClass::foo);
  mock->expect("bar", &ProductionClass::bar, 42);
  mock->expect("baz", &ProductionClass::baz);
  mock->foo();
  mock->bar(3); // bad parameter
  // missing call to baz
  mock->verify();
  MockClass::destroy(&mock);
}
