/*
**  Copyright (C) - Triton
**
**  This program is under the terms of the LGPLv3 License.
*/

#ifndef LIGHT_VERSION

#include <iostream>
#include <sstream>
#include <stdexcept>

#include <SubIRBuilder.h>
#include <Registers.h>
#include <SMT2Lib.h>
#include <SymbolicExpression.h>


SubIRBuilder::SubIRBuilder(__uint address, const std::string &disassembly):
  BaseIRBuilder(address, disassembly) {
}


void SubIRBuilder::regImm(AnalysisProcessor &ap, Inst &inst) const {
  SymbolicExpression *se;
  smt2lib::smtAstAbstractNode *expr, *op1, *op2;
  auto reg = this->operands[0].getReg();
  auto imm = this->operands[1].getImm().getValue();
  auto regSize = this->operands[0].getReg().getSize();

  /* Create the SMT semantic */
  op1 = ap.buildSymbolicRegOperand(reg, regSize);
  op2 = smt2lib::bv(imm, regSize * BYTE_SIZE_BIT);

  /* Finale expr */
  expr = smt2lib::bvsub(op1, op2);

  /* Create the symbolic expression */
  se = ap.createRegSE(inst, expr, reg, regSize);

  /* Apply the taint */
  ap.aluSpreadTaintRegImm(se, reg);

  /* Add the symbolic flags expression to the current inst */
  EflagsBuilder::af(inst, se, ap, regSize, op1, op2);
  EflagsBuilder::cfSub(inst, se, ap, regSize, op1, op2);
  EflagsBuilder::ofSub(inst, se, ap, regSize, op1, op2);
  EflagsBuilder::pf(inst, se, ap, regSize);
  EflagsBuilder::sf(inst, se, ap, regSize);
  EflagsBuilder::zf(inst, se, ap, regSize);
}


void SubIRBuilder::regReg(AnalysisProcessor &ap, Inst &inst) const {
  SymbolicExpression *se;
  smt2lib::smtAstAbstractNode *expr, *op1, *op2;
  auto reg1 = this->operands[0].getReg();
  auto reg2 = this->operands[1].getReg();
  auto regSize1 = this->operands[0].getReg().getSize();
  auto regSize2 = this->operands[1].getReg().getSize();

  /* Create the SMT semantic */
  op1 = ap.buildSymbolicRegOperand(reg1, regSize1);
  op2 = ap.buildSymbolicRegOperand(reg2, regSize2);

  // Final expr
  expr = smt2lib::bvsub(op1, op2);

  /* Create the symbolic expression */
  se = ap.createRegSE(inst, expr, reg1, regSize1);

  /* Apply the taint */
  ap.aluSpreadTaintRegReg(se, reg1, reg2);

  /* Add the symbolic flags expression to the current inst */
  EflagsBuilder::af(inst, se, ap, regSize1, op1, op2);
  EflagsBuilder::cfSub(inst, se, ap, regSize1, op1, op2);
  EflagsBuilder::ofSub(inst, se, ap, regSize1, op1, op2);
  EflagsBuilder::pf(inst, se, ap, regSize1);
  EflagsBuilder::sf(inst, se, ap, regSize1);
  EflagsBuilder::zf(inst, se, ap, regSize1);
}


void SubIRBuilder::regMem(AnalysisProcessor &ap, Inst &inst) const {
  SymbolicExpression *se;
  smt2lib::smtAstAbstractNode *expr, *op1, *op2;
  auto mem = this->operands[1].getMem();
  auto memSize = this->operands[1].getMem().getSize();
  auto reg = this->operands[0].getReg();
  auto regSize = this->operands[0].getReg().getSize();

  /* Create the SMT semantic */
  op1 = ap.buildSymbolicRegOperand(reg, regSize);
  op2 = ap.buildSymbolicMemOperand(mem, memSize);

  // Final expr
  expr = smt2lib::bvsub(op1, op2);

  /* Create the symbolic expression */
  se = ap.createRegSE(inst, expr, reg, regSize);

  /* Apply the taint */
  ap.aluSpreadTaintRegMem(se, reg, mem, memSize);

  /* Add the symbolic flags expression to the current inst */
  EflagsBuilder::af(inst, se, ap, regSize, op1, op2);
  EflagsBuilder::cfSub(inst, se, ap, regSize, op1, op2);
  EflagsBuilder::ofSub(inst, se, ap, regSize, op1, op2);
  EflagsBuilder::pf(inst, se, ap, regSize);
  EflagsBuilder::sf(inst, se, ap, regSize);
  EflagsBuilder::zf(inst, se, ap, regSize);
}


void SubIRBuilder::memImm(AnalysisProcessor &ap, Inst &inst) const {
  SymbolicExpression *se;
  smt2lib::smtAstAbstractNode *expr, *op1, *op2;
  auto mem = this->operands[0].getMem();
  auto memSize = this->operands[0].getMem().getSize();
  auto imm = this->operands[1].getImm().getValue();

  /* Create the SMT semantic */
  op1 = ap.buildSymbolicMemOperand(mem, memSize);
  op2 = smt2lib::bv(imm, memSize * BYTE_SIZE_BIT);

  /* Final expr */
  expr = smt2lib::bvsub(op1, op2);

  /* Create the symbolic expression */
  se = ap.createMemSE(inst, expr, mem, memSize);

  /* Apply the taint */
  ap.aluSpreadTaintMemImm(se, mem, memSize);

  /* Add the symbolic flags expression to the current inst */
  EflagsBuilder::af(inst, se, ap, memSize, op1, op2);
  EflagsBuilder::cfSub(inst, se, ap, memSize, op1, op2);
  EflagsBuilder::ofSub(inst, se, ap, memSize, op1, op2);
  EflagsBuilder::pf(inst, se, ap, memSize);
  EflagsBuilder::sf(inst, se, ap, memSize);
  EflagsBuilder::zf(inst, se, ap, memSize);
}


void SubIRBuilder::memReg(AnalysisProcessor &ap, Inst &inst) const {
  SymbolicExpression *se;
  smt2lib::smtAstAbstractNode *expr, *op1, *op2;
  auto mem = this->operands[0].getMem();
  auto memSize = this->operands[0].getMem().getSize();
  auto reg = this->operands[1].getReg();
  auto regSize = this->operands[1].getReg().getSize();

  /* Create the SMT semantic */
  op1 = ap.buildSymbolicMemOperand(mem, memSize);
  op2 = ap.buildSymbolicRegOperand(reg, regSize);

  // Final expr
  expr = smt2lib::bvsub(op1, op2);

  /* Create the symbolic expression */
  se = ap.createMemSE(inst, expr, mem, memSize);

  /* Apply the taint */
  ap.aluSpreadTaintMemReg(se, mem, reg, memSize);

  /* Add the symbolic flags expression to the current inst */
  EflagsBuilder::af(inst, se, ap, memSize, op1, op2);
  EflagsBuilder::cfSub(inst, se, ap, memSize, op1, op2);
  EflagsBuilder::ofSub(inst, se, ap, memSize, op1, op2);
  EflagsBuilder::pf(inst, se, ap, memSize);
  EflagsBuilder::sf(inst, se, ap, memSize);
  EflagsBuilder::zf(inst, se, ap, memSize);
}


Inst *SubIRBuilder::process(AnalysisProcessor &ap) const {
  this->checkSetup();

  Inst *inst = new Inst(ap.getThreadID(), this->address, this->disas);

  try {
    this->templateMethod(ap, *inst, this->operands, "SUB");
    ap.incNumberOfExpressions(inst->numberOfExpressions()); /* Used for statistics */
    ControlFlow::rip(*inst, ap, this->nextAddress);
  }
  catch (std::exception &e) {
    delete inst;
    throw;
  }

  return inst;
}

#endif /* LIGHT_VERSION */

