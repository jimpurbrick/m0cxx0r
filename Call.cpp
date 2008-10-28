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

#include "Call.h"

#include "Param.h"

#include <iostream>

namespace m0cxx0r
{
	Call::Call(const std::string& name) :
	mName(name)
	{
	}

	Call::~Call()
	{
	}

	Call* Call::clone()
	{
		Call* result = new Call(mName);
		result->mIndex = mIndex;
		std::vector<Param*>::iterator i;
		for(i = mParameters.begin(); i != mParameters.end(); ++i)
		{
			result->addParameter((*i)->clone());
		}
		return result;
	}

	void Call::addParameter(Param* param)
	{
		mParameters.push_back(param);
	}

	size_t Call::getIndex()
	{
		return mIndex;
	}

	std::string Call::getName()
	{
		return mName;
	}

	void Call::setIndex(size_t index)
	{
		mIndex = index;
	}

	void Call::setParams(unsigned char* params)
	{
		ParamVector::iterator i;
		for(i = mParameters.begin(); i != mParameters.end(); ++i)
		{
			(*i)->setValue(params);
		}
	}

	bool Call::verify(Call* actual)
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

} // namespace m0cxx0r