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

#ifndef M0CXX0R_MOCK_H
#define M0CXX0R_MOCK_H

#include "Call.h"

#include <iostream>
#include <vector>
#include <string>

namespace m0cxx0r
{	
	template<typename T>
    class Mock : public T
    {
    public:

        static Mock* create()
        {
            // Allocate memory for mock.
            size_t mockSize = sizeof(Mock<T>);
            unsigned char* mockData = new unsigned char[mockSize];
            memset(mockData, 0, mockSize);

            // Create VTableDonor object in the memory allocated for the mock to 
            // point the Mock VPtr at the VTableDonor VTable using placement new.
            Mock<T>* mock = reinterpret_cast<Mock<T>*>(new (mockData) VTableDonor);

            // Initialize mock data members.
            mock->mExpectedCalls = std::vector<Call*>();
            mock->mActualCalls = std::vector<Call*>();
            mock->mRecordingExpected = false;

            return mock;
        }

        static void destroy(Mock** mock)
        {
            // Delete byte array created for mock.
            unsigned char* mockData = reinterpret_cast<unsigned char*>(*mock);
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

        template<typename ReturnType, typename MemberFuncType, typename ParamType0>
        void expect(const std::string& name, ReturnType returnValue, MemberFuncType func, ParamType0 p0)
        {
            mRecordingExpected = true;
            Call* expectedCall = new Call(name);
			std::ptrdiff_t offset = returnOffset(returnValue.value());
            expectedCall->addParameter(p0.createParam(offset, reinterpret_cast<unsigned char*>(&p0)));
            mExpectedCalls.push_back(expectedCall);
            ((this)->*(func))(p0.value());
            mRecordingExpected = false;
        }

		/*
        template<typename MemberFuncType, typename ParamType0, typename ParamType1>
        void expect(const std::string& name, MemberFuncType func, ParamType0 p0, ParamType1 p1)
        {
            mRecordingExpected = true;
            Call* expectedCall = new Call(name);
            expectedCall->addParameter(p0.createParam(reinterpret_cast<unsigned char*>(&p0)));
            expectedCall->addParameter(p1.createParam(reinterpret_cast<unsigned char*>(&p0)));
            mExpectedCalls.push_back(expectedCall);
            ((this)->*(func))(p0.value(), p1.value());
            mRecordingExpected = false;
        }
		*/

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
            virtual void  f1(size_t p0) {((Mock*) this)->recordCall(1, (unsigned char*)(&p0));}
            virtual void  f2(size_t p0) {((Mock*) this)->recordCall(2, (unsigned char*)(&p0));}
            virtual void  f3(size_t p0) {((Mock*) this)->recordCall(3, (unsigned char*)(&p0));}
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
				Call* actualCall = expectedCall->clone();
				mActualCalls.push_back(actualCall);
				actualCall->setIndex(index);
                if(expectedCall->getIndex() == index)
                {
                    actualCall->setParams(params);   
                }
            }
        }

		template<typename T>
		T firstParamAddress(unsigned char firstParam, T result, unsigned char** address)
		{
			*address = &firstParam;
			return result;
		}

		template<typename T>
		std::ptrdiff_t returnOffset(T value)
		{
			unsigned char* paramAddress = NULL;
			unsigned char firstParam = 0;
			unsigned char local;
			firstParamAddress(firstParam, value, &paramAddress);
			return paramAddress - &local;
		}

        typedef std::vector<Call*> CallVector;
        CallVector mExpectedCalls;
        CallVector mActualCalls;
        bool mRecordingExpected;
    };
	
} // namespace m0cxx0r

#endif //M0CXX0R_MOCK_H