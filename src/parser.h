/*
 *  Copyright 2013 Adrian Thurston <thurston@complang.org>
 */

#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include "avltree.h"
#include "compiler.h"
#include "parser.h"

#define PROPERTY_REDUCE_FIRST 0x1

struct BaseParser
{
	BaseParser( Compiler *pd )
		: pd(pd), enterRl(false)
	{}

	virtual ~BaseParser() {}

	Compiler *pd;

	RegionSetVect regionStack;
	NamespaceVect namespaceStack;
	ContextStack contextStack;
	ObjectDef *curLocalFrame;
	ObjNameScope *curScope;

	bool enterRl;

	bool insideRegion()
		{ return regionStack.length() > 0; }

	Context *curContext()
		{ return contextStack.length() == 0 ? 0 : contextStack.top(); }

	Namespace *curNspace()
		{ return namespaceStack.top(); }

	/* Lexical feedback. */

	void init();
	void addRegularDef( const InputLoc &loc, Namespace *nspace, 
			const String &name, LexJoin *join );
	TokenRegion *createRegion( const InputLoc &loc, RegionImpl *impl );
	Namespace *createRootNamespace();
	Namespace *createNamespace( const InputLoc &loc, const String &name );
	void pushRegionSet( const InputLoc &loc );
	void popRegionSet();
	void addProduction( const InputLoc &loc, const String &name, 
			ProdElList *prodElList, bool commit, CodeBlock *redBlock, LangEl *predOf );
	void addArgvList();
	LexJoin *literalJoin( const InputLoc &loc, const String &data );

	void defineToken( const InputLoc &loc, String name, LexJoin *join, ObjectDef *objectDef,
			CodeBlock *transBlock, bool ignore, bool noPreIgnore, bool noPostIgnore );

	void zeroDef( const InputLoc &loc, const String &name );
	void literalDef( const InputLoc &loc, const String &data,
			bool noPreIgnore, bool noPostIgnore );

	ObjectDef *blockOpen();
	void blockClose();
	void functionDef( StmtList *stmtList, ObjectDef *localFrame,
			ParameterList *paramList, TypeRef *typeRef, const String &name, bool exprt );
	void iterDef( StmtList *stmtList, ObjectDef *localFrame,
			ParameterList *paramList, const String &name );
	LangStmt *globalDef( ObjectField *objField, LangExpr *expr, LangStmt::Type assignType );
	void cflDef( NtDef *ntDef, ObjectDef *objectDef, LelDefList *defList );
	ReOrBlock *lexRegularExprData( ReOrBlock *reOrBlock, ReOrItem *reOrItem );

	int lexFactorRepNum( const InputLoc &loc, const String &data );
	LexFactor *lexRlFactorName( const String &data, const InputLoc &loc );
	LexFactorAug *lexFactorLabel( const InputLoc &loc, const String &data,
			LexFactorAug *factorAug );
	LexJoin *lexOptJoin( LexJoin *join, LexJoin *context );
	LangExpr *send( const InputLoc &loc, LangVarRef *varRef, ConsItemList *list, bool eof );
	LangExpr *sendTree( const InputLoc &loc, LangVarRef *varRef, ConsItemList *list, bool eof );
	LangExpr *parseCmd( const InputLoc &loc, bool tree, bool stop, ObjectField *objField,
			TypeRef *typeRef, FieldInitVect *fieldInitVect, ConsItemList *list );
	PatternItemList *consPatternEl( LangVarRef *varRef, PatternItemList *list );
	PatternItemList *patternElNamed( const InputLoc &loc, LangVarRef *varRef,
			NamespaceQual *nspaceQual, const String &data, RepeatType repeatType );
	PatternItemList *patternElType( const InputLoc &loc, LangVarRef *varRef,
			NamespaceQual *nspaceQual, const String &data, RepeatType repeatType );
	PatternItemList *patListConcat( PatternItemList *list1, PatternItemList *list2 );
	ConsItemList *consListConcat( ConsItemList *list1, ConsItemList *list2 );
	LangStmt *forScope( const InputLoc &loc, const String &data,
			ObjNameScope *scope, TypeRef *typeRef, IterCall *iterCall, StmtList *stmtList );
	void preEof( const InputLoc &loc, StmtList *stmtList, ObjectDef *localFrame );

	ProdEl *prodElName( const InputLoc &loc, const String &data,
			NamespaceQual *nspaceQual, ObjectField *objField, RepeatType repeatType,
			bool commit );
	ProdEl *prodElLiteral( const InputLoc &loc, const String &data,
			NamespaceQual *nspaceQual, ObjectField *objField, RepeatType repeatType,
			bool commit );
	ConsItemList *consElLiteral( const InputLoc &loc, TypeRef *consTypeRef,
			const String &data, NamespaceQual *nspaceQual );
	Production *production( const InputLoc &loc, ProdElList *prodElList,
			String name, bool commit, CodeBlock *codeBlock, LangEl *predOf );
	void objVarDef( ObjectDef *objectDef, ObjectField *objField );
	LelDefList *prodAppend( LelDefList *defList, Production *definition );

	LangExpr *construct( const InputLoc &loc, ObjectField *objField,
			ConsItemList *list, TypeRef *typeRef, FieldInitVect *fieldInitVect );
	LangExpr *match( const InputLoc &loc, LangVarRef *varRef,
			PatternItemList *list );
	LangStmt *varDef( ObjectField *objField,
			LangExpr *expr, LangStmt::Type assignType );
	LangStmt *exportStmt( ObjectField *objField, LangStmt::Type assignType, LangExpr *expr );

	LangExpr *require( const InputLoc &loc, LangVarRef *varRef, PatternItemList *list );
	void contextVarDef( const InputLoc &loc, ObjectField *objField );
	void contextHead( const InputLoc &loc, const String &data );
	StmtList *appendStatement( StmtList *stmtList, LangStmt *stmt );
	ParameterList *appendParam( ParameterList *paramList, ObjectField *objField );
	ObjectField *addParam( const InputLoc &loc, TypeRef *typeRef, const String &name );
	PredDecl *predTokenName( const InputLoc &loc, NamespaceQual *qual, const String &data );
	PredDecl *predTokenLit( const InputLoc &loc, const String &data,
			NamespaceQual *nspaceQual );
	void alias( const InputLoc &loc, const String &data, TypeRef *typeRef );
	void precedenceStmt( PredType predType, PredDeclList *predDeclList );
	ProdElList *appendProdEl( ProdElList *prodElList, ProdEl *prodEl );

	void pushScope();
	void popScope();

	virtual void go( long activeRealm ) = 0;
};

#endif