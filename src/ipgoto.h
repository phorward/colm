/*
 *  Copyright 2001-2014 Adrian Thurston <thurston@complang.org>
 */

#ifndef _CDIPGOTO_H
#define _CDIPGOTO_H

#include <iostream>
#include "goto.h"

/* Forwards. */
struct CodeGenData;

/*
 * class FGotoCodeGen
 */
class IpGoto
	: public Goto
{
public:
	IpGoto( const CodeGenArgs &args ) 
			: Goto(args) {}

	std::ostream &EXIT_STATES();
	std::ostream &TRANS_GOTO( RedTransAp *trans, int level );
	std::ostream &COND_GOTO( RedCondAp *trans, int level );
	std::ostream &FINISH_CASES();
	std::ostream &AGAIN_CASES();
	std::ostream &STATE_GOTOS();
	std::ostream &STATE_GOTO_CASES();

	void GOTO( ostream &ret, int gotoDest, bool inFinish );
	void CALL( ostream &ret, int callDest, int targState, bool inFinish );
	void NCALL( ostream &ret, int callDest, int targState, bool inFinish );
	void NEXT( ostream &ret, int nextDest, bool inFinish );
	void GOTO_EXPR( ostream &ret, GenInlineItem *ilItem, bool inFinish );
	void NEXT_EXPR( ostream &ret, GenInlineItem *ilItem, bool inFinish );
	void CALL_EXPR( ostream &ret, GenInlineItem *ilItem, int targState, bool inFinish );
	void NCALL_EXPR( ostream &ret, GenInlineItem *ilItem, int targState, bool inFinish );
	void RET( ostream &ret, bool inFinish );
	void NRET( ostream &ret, bool inFinish );
	void CURS( ostream &ret, bool inFinish );
	void TARGS( ostream &ret, bool inFinish, int targState );
	void BREAK( ostream &ret, int targState, bool csForced );
	void NBREAK( ostream &ret, int targState, bool csForced );

	virtual void genAnalysis();
	virtual void writeData();
	virtual void writeExec();

protected:
	bool useAgainLabel();

	/* Called from Goto::STATE_GOTOS just before writing the gotos for
	 * each state. */
	bool IN_TRANS_ACTIONS( RedStateAp *state );
	void GOTO_HEADER( RedStateAp *state );
	void STATE_GOTO_ERROR();

	/* Set up labelNeeded flag for each state. */
	void setLabelsNeeded( GenInlineList *inlineList );
	void setLabelsNeeded();
};

#endif