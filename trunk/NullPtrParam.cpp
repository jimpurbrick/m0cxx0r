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

#include "NullPtrParam.h"

#include <sstream>

namespace m0cxx0r
{
	NullPtrParam::NullPtrParam(unsigned char* firstParam, unsigned char* param) : 
		mOffset(param - firstParam),
		mValue(NULL)
	{
	}

	NullPtrParam::~NullPtrParam()
	{
	}

    Param* NullPtrParam::clone()
    {
        NullPtrParam* result = new NullPtrParam();
        result->mOffset = mOffset;
        result->mValue = mValue;
        return result;
    }

	void NullPtrParam::setValue(unsigned char* firstParam)
	{
		void** param = reinterpret_cast<void**>(firstParam + mOffset);
        mValue = *param;
	}

    bool NullPtrParam::verify(Param* actualParam)
	{
		NullPtrParam* nullPtrParam = dynamic_cast<NullPtrParam*>(actualParam);
		if(NULL == nullPtrParam)
		{
			return false;
		}
		return nullPtrParam->mValue == NULL;
	}

    std::string NullPtrParam::getString()
	{
        std::ostringstream stream;
		if(mValue)
		{
			stream << "0x" << mValue;
		}
		else
		{
			stream << "NULL";
		}
        return stream.str();
	}

} // namespace m0cxx0r
