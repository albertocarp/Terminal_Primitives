// ***********************************************************************
// Assembly         : 
// Author           : Alberto-PC
// Created          : 05-13-2016
//
// Last Modified By : Alberto-PC
// Last Modified On : 05-13-2016
// ***********************************************************************
// <copyright file="TestCaseException.cpp" company="">
//     Copyright (c) . All rights reserved.
// </copyright>
// <summary></summary>
// ***********************************************************************
#include "TestCaseException.h"



/// <summary>
/// Initializes a new instance of the <see cref="TestCaseException"/> class.
/// </summary>
TestCaseException::TestCaseException()
{
}

/// <summary>
/// Initializes a new instance of the <see cref="TestCaseException"/> class.
/// </summary>
/// <param name="msg">The MSG.</param>
TestCaseException::TestCaseException(const std::string & msg)
	:std::exception(msg.c_str())
{
}

/// <summary>
/// Initializes a new instance of the <see cref="TestCaseException"/> class.
/// </summary>
/// <param name="msg">The MSG.</param>
/// <param name="code">The code.</param>
TestCaseException::TestCaseException(const std::string & msg, long code)
	: std::exception(msg.c_str(), code)
{
}


/// <summary>
/// Finalizes an instance of the <see cref="TestCaseException"/> class.
/// </summary>
TestCaseException::~TestCaseException()
{
}