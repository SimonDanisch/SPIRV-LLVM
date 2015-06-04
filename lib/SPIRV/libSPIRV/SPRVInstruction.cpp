//===- SPRVInstruction.cpp -Class to represent SPIR-V instruction - C++ -*-===//
//
//                     The LLVM/SPIRV Translator
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
// Copyright (c) 2014 Advanced Micro Devices, Inc. All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal with the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// Redistributions of source code must retain the above copyright notice,
// this list of conditions and the following disclaimers.
// Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following disclaimers in the documentation
// and/or other materials provided with the distribution.
// Neither the names of Advanced Micro Devices, Inc., nor the names of its
// contributors may be used to endorse or promote products derived from this
// Software without specific prior written permission.
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// CONTRIBUTORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS WITH
// THE SOFTWARE.
//
//===----------------------------------------------------------------------===//
/// \file
///
/// This file implements SPIR-V instructions.
///
//===----------------------------------------------------------------------===//

#include "SPRVInstruction.h"
#include "SPRVBasicBlock.h"
#include "SPRVFunction.h"

using namespace SPRV;

// Complete constructor for instruction with type and id
SPRVInstruction::SPRVInstruction(unsigned TheWordCount, SPRVOpCode TheOC,
    SPRVType *TheType, SPRVId TheId, SPRVBasicBlock *TheBB)
  :SPRVValue(TheBB->getModule(), TheWordCount, TheOC, TheType, TheId),
   BB(TheBB){
  validate();
}

SPRVInstruction::SPRVInstruction(unsigned TheWordCount, SPRVOpCode TheOC,
  SPRVType *TheType, SPRVId TheId, SPRVBasicBlock *TheBB, SPRVModule *TheBM)
  : SPRVValue(TheBM, TheWordCount, TheOC, TheType, TheId), BB(TheBB){
  validate();
}

// Complete constructor for instruction with id but no type
SPRVInstruction::SPRVInstruction(unsigned TheWordCount, SPRVOpCode TheOC,
    SPRVId TheId, SPRVBasicBlock *TheBB)
  :SPRVValue(TheBB->getModule(), TheWordCount, TheOC, TheId), BB(TheBB){
  validate();
}
// Complete constructor for instruction without type and id
SPRVInstruction::SPRVInstruction(unsigned TheWordCount, SPRVOpCode TheOC,
    SPRVBasicBlock *TheBB)
  :SPRVValue(TheBB->getModule(), TheWordCount, TheOC), BB(TheBB){
  validate();
}
// Complete constructor for instruction with type but no id
SPRVInstruction::SPRVInstruction(unsigned TheWordCount, SPRVOpCode TheOC,
    SPRVType *TheType, SPRVBasicBlock *TheBB)
  :SPRVValue(TheBB->getModule(), TheWordCount, TheOC, TheType), BB(TheBB){
  validate();
}

void
SPRVInstruction::setParent(SPRVBasicBlock *TheBB) {
  assert(TheBB && "Invalid BB");
  if (BB == TheBB)
    return;
  assert(BB == NULL && "BB cannot change parent");
  BB = TheBB;
}

void
SPRVInstruction::setScope(SPRVEntry *Scope) {
  assert(Scope && Scope->getOpCode() == SPRVOC_OpLabel && "Invalid scope");
  setParent(static_cast<SPRVBasicBlock*>(Scope));
}

SPRVFunctionCall::SPRVFunctionCall(SPRVId TheId, SPRVFunction *TheFunction,
    const std::vector<SPRVValue *> &TheArgs, SPRVBasicBlock *BB)
  :SPRVFunctionCallGeneric(
      TheFunction->getFunctionType()->getReturnType(),
      TheId, TheArgs, BB), FunctionId(TheFunction->getId()){
  validate();
}

void
SPRVFunctionCall::validate()const {
  SPRVFunctionCallGeneric::validate();
}

// ToDo: Each instruction should implement this function
std::vector<SPRVValue *>
SPRVInstruction::getOperands() {
  std::vector<SPRVValue *> Empty;
  assert(0 && "not supported");
  return Empty;
}

std::vector<SPRVType*>
SPRVInstruction::getOperandTypes() {
  std::vector<SPRVType*> Tys;
  for (auto& I : getOperands()) {
    SPRVType* Ty = nullptr;
    if (I->getOpCode() == SPRVOC_OpFunction)
      Ty = reinterpret_cast<SPRVFunction*>(I)->getFunctionType();
    else
      Ty = I->getType();

    Tys.push_back(Ty);
  }
  return Tys;
}
