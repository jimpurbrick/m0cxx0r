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
// * Neither the name of the organisation nor the names of its contributors
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

#include "Any.h"
#include "AnyRef.h"
#include "Equal.h"
#include "Mock.h"
#include "NotNullPtr.h"
#include "NullPtr.h"

class ProductionClass
{
public:

    ProductionClass() {;}

    virtual ProductionClass foo(size_t p0) {return ProductionClass();}
    virtual ProductionClass* bar(size_t p0, size_t p1) {return new ProductionClass();}
	virtual void baz(const std::string&, size_t*) {;}
};

using namespace m0cxx0r;

m0cxx0r::Mock<ProductionClass>* m0cxx0r::Mock<ProductionClass>::self;

int main()
{
	int i = sizeof(ProductionClass);
    typedef Mock<ProductionClass> MockClass;
    MockClass* mock = MockClass::create();
	mock->expect<ProductionClass>("foo", &ProductionClass::foo, Equal<int>(42));
    mock->expect<ProductionClass*>("bar", &ProductionClass::bar, Equal<int>(42), Any<int>());
	mock->expect<void>("baz", &ProductionClass::baz, AnyRef<const std::string>(), NullPtr<size_t>());
	mock->expect<void>("baz", &ProductionClass::baz, AnyRef<const std::string>(), NotNullPtr<size_t>());
    mock->foo(42);
    mock->bar(3, 4); // bad parameter 0
	mock->baz("test", NULL);
	mock->baz("test", new size_t);
    mock->verify();
    MockClass::destroy(&mock);
}
