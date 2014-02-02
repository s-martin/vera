//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include "StatementOfDefine.h"
#include "IsTokenWithName.h"

#include <vector>
#include <map>
#include <algorithm>

#define WITHOUT_STATEMENT_SCOPE "The statement not contain a scope associated."
#define TOKEN_NAME  "pp_define"
#define IDENTIFIER_TOKEN_NAME  "identifier"
#define STRINGLIT_TOKEN_NAME  "stringlit"
#define LEFTPAREN_TOKEN_NAME  "leftparen"
#define RIGHTPAREN_TOKEN_NAME  "rightparen"
#define NEWLINE_TOKEN_NAME  "newline"

#define IS_EQUAL_RETURN(left, right) \
  {\
    if (left == right) \
    { \
      return;\
    }\
  }
#define IS_EQUAL_BREAK(left, right) \
  {\
    if (left == right) \
    { \
      break;\
    }\
  }

#define IS_EQUAL_RETURN_FALSE(left, right) \
  {\
    if (left == right) \
    { \
      return false;\
    }\
  }

namespace Vera
{
namespace Structures
{

StatementOfDefine::StatementOfDefine(Statement& statement,
  Tokens::TokenSequence::const_iterator& it,
  Tokens::TokenSequence::const_iterator& end)
: StatementsBuilder(statement)
, scope_(NULL)
{
  statement.type_ = Statement::TYPE_ITEM_STATEMENT_OF_DEFINE;
  initialize(it, end);
}

void
StatementOfDefine::initialize(Tokens::TokenSequence::const_iterator& it,
    Tokens::TokenSequence::const_iterator& end)
{
  Statement& current = getCurrentStatement();

  push(*it);

  ++it;

  Tokens::TokenSequence::const_iterator endMatched = std::find_if(it,
    end,
    IsTokenWithName(NEWLINE_TOKEN_NAME));

  IS_EQUAL_RETURN(endMatched, end);

  addEachInvalidToken(it, endMatched);

  if (it->name_.compare(IDENTIFIER_TOKEN_NAME) == 0)
  {
    push(*it);
    name_ = it->value_;
  }

  ++it;

  IS_EQUAL_RETURN(it, end);

  if (it->name_.compare(LEFTPAREN_TOKEN_NAME) == 0)
  {
    Tokens::TokenSequence::const_iterator rightParenMatched = std::find_if(it + 1,
      end,
      EndsWithCorrectPattern(LEFTPAREN_TOKEN_NAME, RIGHTPAREN_TOKEN_NAME));

    if (rightParenMatched < end)
    {
      ++rightParenMatched;
    }

    parseScope(it, rightParenMatched);
  }

  addEachInvalidToken(it, end);

  IS_EQUAL_RETURN(it, end);

  StatementsBuilder branches(add());

  for (; it < endMatched; ++it)
  {
    branches.push(*it);
  }

  IS_EQUAL_RETURN(it, end);
}

bool
StatementOfDefine::isValid(
    Tokens::TokenSequence::const_iterator it,
    Tokens::TokenSequence::const_iterator end)
{
  return it->name_.compare(TOKEN_NAME) == 0;
}

bool
StatementOfDefine::create(Statement& statement,
    Tokens::TokenSequence::const_iterator& it,
    Tokens::TokenSequence::const_iterator& end)
{
  StatementOfDefine builder(statement.add(), it, end);


  return true;
}

} // Vera namespace
} // Structures namespace
