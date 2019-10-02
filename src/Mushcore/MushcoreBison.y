%{
//%Header {
/*****************************************************************************
*
* File: src/Mushcore/MushcoreBison.h
*
* This file contains original work by Andy Southgate.  Contact details can be
* found at http://www.mushware.com/.  This file was placed in the Public
* Domain by Andy Southgate and Mushware Limited in 2002-2003.
*
* This software carries NO WARRANTY of any kind.
*
****************************************************************************/
//%Header } /xS9B0abPPKxSkLhcISQjg
/*
 * $Id: MushcoreBison.y,v 1.11 2003/10/04 12:23:08 southa Exp $
 * $Log: MushcoreBison.y,v $
 * Revision 1.11  2003/10/04 12:23:08  southa
 * File renaming
 *
 * Revision 1.10  2003/09/17 19:40:35  southa
 * Source conditioning upgrades
 *
 * Revision 1.9  2003/08/21 23:09:06  southa
 * Fixed file headers
 *
 * Revision 1.8  2003/02/04 00:11:04  southa
 * Build fixes
 *
 * Revision 1.7  2003/01/20 17:03:21  southa
 * Command line expression evaluator enhancements
 *
 * Revision 1.6  2003/01/20 15:38:28  southa
 * Created MushcoreTest
 *
 * Revision 1.5  2003/01/20 10:45:28  southa
 * Singleton tidying
 *
 * Revision 1.4  2003/01/18 13:33:57  southa
 * Created MushcoreSingleton
 *
 * Revision 1.3  2003/01/14 20:46:10  southa
 * Post data handling
 *
 * Revision 1.2  2003/01/12 17:32:58  southa
 * Mushcore work
 *
 * Revision 1.1  2003/01/09 14:57:05  southa
 * Created Mushcore
 *
 * Revision 1.10  2002/12/29 20:59:51  southa
 * More build fixes
 *
 * Revision 1.9  2002/06/04 22:50:02  southa
 * Fixes to mac project
 *
 * Revision 1.8  2002/05/30 14:41:13  southa
 * InfernalData and loadtilemap command
 *
 */

/*****************************************************************************
 *
 *  MushcoreBison.cpp is autogenerated from the file MushcoreBison.y.
 *  Please edit that file in preference to MushcoreBison.cpp.
 *
 ****************************************************************************/
    
#include "MushcoreBison.h"
#include "MushcoreCommand.h"
#include "MushcoreEnv.h"

#include "MushcoreSTL.h"
    
#define YYSTYPE MushcoreScalar
#define YYPARSE_PARAM inBison
#define YYLEX_PARAM inBison
#define INBISON (static_cast<MushcoreBison *>(inBison))
#define MushcoreBison_lex INBISON->Lex
#define YYDEBUG 1
#define YYERROR_VERBOSE 1

using namespace std;
    
int yyerror( char *s);
%}

%pure_parser
%token NUMBER
%token VARIABLE
%token IDENTIFIER
%token STRING
%token EOS
%token END_OF_FILE

%left '+' '-'
%left '*' '/'
%left NEG

%% /* Grammar rules and actions follow */

input:    /* empty */
| input statement;
end: EOS
| END_OF_FILE
;
command: IDENTIFIER { INBISON->ClearParams(); }
| VARIABLE { $$=MushcoreEnv::Sgl().VariableGet($1.StringGet().substr(1, string::npos)).StringGet(); INBISON->ClearParams(); }
;
scalar: VARIABLE { $$=MushcoreEnv::Sgl().VariableGet($1.StringGet().substr(1, string::npos)); }
| STRING { $$ = $1.StringGet();}
| NUMBER { $$ = $1.ValGet();}
| scalar '+' scalar { $$ = $1+$3; }
| scalar '-' scalar { $$ = $1-$3; }
| scalar '*' scalar { $$ = $1*$3; }
| scalar '/' scalar { $$ = $1/$3; }
| '-' scalar %prec NEG { $$ = -$2.ValGet(); }
| '(' scalar ')' { $$ = $2; }
;
parameter: scalar {
    INBISON->PushParam($1);
}
;
parameter_list: /* empty */
| parameter
| parameter_list ',' parameter
;
command_tail: /* empty */
| '(' parameter_list ')'
;
statement: end
| command command_tail end {
    $$=INBISON->Despatch($1);
}
;
%%

int yyerror(char *s)
{
#ifdef MUSHCORE_DEBUG
    cerr << "Bison syntax error: " << s << endl;
#endif
    return 0;
}

int
MushcoreBison::Parse(MushcoreCommand& inCommand)
{
    m_command=&inCommand;
    return yyparse(this);
}

void
MushcoreBison::PushParam(MushcoreScalar& inParam)
{
    m_command->PushParam(inParam);
}

void
MushcoreBison::ClearParams(void)
{
    m_command->ClearParams();
}

MushcoreScalar 
MushcoreBison::Despatch(MushcoreScalar& inName)
{
    string name;
    inName.Get(name);
    m_command->NameSet(name);
    return m_command->Despatch();
}
