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

#ifndef M0CXX0R_EQUALPARAM_H
#define M0CXX0R_EQUALPARAM_H

#include "Param.h"

#include <sstream>

namespace m0cxx0r
{
	template<typename ParamType>
    struct EqualParam : public Param
    {
    public:

        EqualParam(unsigned char* p0, ParamType* param) : 
			mOffset(reinterpret_cast<unsigned char*>(param) - p0),
            mValue(*param)
        {
        }

        virtual ~EqualParam() {;}

        virtual Param* clone()
        {
            EqualParam* result = new EqualParam();
            result->mOffset = mOffset;
            result->mValue = mValue;
            return result;
        }

        virtual void setValue(unsigned char* p0)
        {
            ParamType* param = reinterpret_cast<ParamType*>(p0 + mOffset);
            mValue = *param;
        }

        virtual bool isEqual(Param* param)
        {
			// TODO: dynamic cast when param may not be an EqualParam.
            EqualParam* equalParam = static_cast<EqualParam*>(param);
            return mValue == equalParam->mValue;
        }

        virtual std::string getString()
        {
            std::ostringstream stream;
            stream << mValue;
            return stream.str();
        }

    private:

        EqualParam() {;}
		std::ptrdiff_t mOffset;
        ParamType mValue;
    };

} // namespace m0cxx0r

#endif // M0CXX0R_EQUALPARAM_H
