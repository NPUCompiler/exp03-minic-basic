
// Generated from MiniC.g4 by ANTLR 4.12.0

#pragma once


#include "antlr4-runtime.h"
#include "MiniCVisitor.h"


/**
 * This class provides an empty implementation of MiniCVisitor, which can be
 * extended to create a visitor which only needs to handle a subset of the available methods.
 */
class  MiniCBaseVisitor : public MiniCVisitor {
public:

  virtual std::any visitCompileUnit(MiniCParser::CompileUnitContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFuncDef(MiniCParser::FuncDefContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitBlock(MiniCParser::BlockContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitBlockItemList(MiniCParser::BlockItemListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitBlockItem(MiniCParser::BlockItemContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitReturnStatement(MiniCParser::ReturnStatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExpr(MiniCParser::ExprContext *ctx) override {
    return visitChildren(ctx);
  }


};

