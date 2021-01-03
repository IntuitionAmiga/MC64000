<?php

namespace ABadCafe\MC64K;

const CLASS_MAP = [
  'ABadCafe\\MC64K\\IParser' => '/IParser.php',
  'ABadCafe\\MC64K\\Parser\\Utils\\TSignedDisplacementAware' => '/parser/utils/TSignedDisplacementAware.php',
  'ABadCafe\\MC64K\\Parser\\Utils\\Hex' => '/parser/utils/Hex.php',
  'ABadCafe\\MC64K\\Parser\\Instruction\\UnhandledCodeFoldException' => '/parser/instruction/CodeFoldException.php',
  'ABadCafe\\MC64K\\Parser\\Instruction\\IOperandSetParser' => '/parser/instruction/IOperandSetParser.php',
  'ABadCafe\\MC64K\\Parser\\Instruction\\Line' => '/parser/instruction/Line.php',
  'ABadCafe\\MC64K\\Parser\\Instruction\\CodeFoldException' => '/parser/instruction/UnhandledCodeFoldException.php',
  'ABadCafe\\MC64K\\Parser\\Instruction\\Operand\\BranchDisplacement' => '/parser/instruction/operand/BranchDisplacement.php',
  'ABadCafe\\MC64K\\Parser\\Instruction\\OperandSet\\IntegerMonadicBranch' => '/parser/instruction/operand_set/IntegerMonadicBranch.php',
  'ABadCafe\\MC64K\\Parser\\Instruction\\OperandSet\\TBranching' => '/parser/instruction/operand_set/TBranching.php',
  'ABadCafe\\MC64K\\Parser\\Instruction\\OperandSet\\IntegerMonadic' => '/parser/instruction/operand_set/IntegerMonadic.php',
  'ABadCafe\\MC64K\\Parser\\Instruction\\OperandSet\\IFixedBranch' => '/parser/instruction/operand_set/IFixedBranch.php',
  'ABadCafe\\MC64K\\Parser\\Instruction\\OperandSet\\FloatToIntegerDyadic' => '/parser/instruction/operand_set/FloatToIntegerDyadic.php',
  'ABadCafe\\MC64K\\Parser\\Instruction\\OperandSet\\FloatDyadic' => '/parser/instruction/operand_set/FloatDyadic.php',
  'ABadCafe\\MC64K\\Parser\\Instruction\\OperandSet\\FloatDyadicBranch' => '/parser/instruction/operand_set/FloatDyadicBranch.php',
  'ABadCafe\\MC64K\\Parser\\Instruction\\OperandSet\\Monadic' => '/parser/instruction/operand_set/Monadic.php',
  'ABadCafe\\MC64K\\Parser\\Instruction\\OperandSet\\IntegerDyadicBranch' => '/parser/instruction/operand_set/IntegerDyadicBranch.php',
  'ABadCafe\\MC64K\\Parser\\Instruction\\OperandSet\\None' => '/parser/instruction/operand_set/None.php',
  'ABadCafe\\MC64K\\Parser\\Instruction\\OperandSet\\FloatMonadicBranch' => '/parser/instruction/operand_set/FloatMonadicBranch.php',
  'ABadCafe\\MC64K\\Parser\\Instruction\\OperandSet\\Dyadic' => '/parser/instruction/operand_set/Dyadic.php',
  'ABadCafe\\MC64K\\Parser\\Instruction\\OperandSet\\IntegerDyadic' => '/parser/instruction/operand_set/IntegerDyadic.php',
  'ABadCafe\\MC64K\\Parser\\Instruction\\OperandSet\\IntegerToFloatDyadic' => '/parser/instruction/operand_set/IntegerToFloatDyadic.php',
  'ABadCafe\\MC64K\\Parser\\EffectiveAddress\\IntegerImmediate' => '/parser/effective_address/IntegerImmediate.php',
  'ABadCafe\\MC64K\\Parser\\EffectiveAddress\\GPRIndirectUpdating' => '/parser/effective_address/GPRIndirectUpdating.php',
  'ABadCafe\\MC64K\\Parser\\EffectiveAddress\\AllFloatReadable' => '/parser/effective_address/AllFloatReadable.php',
  'ABadCafe\\MC64K\\Parser\\EffectiveAddress\\AllFloatWriteable' => '/parser/effective_address/AllFloatWriteable.php',
  'ABadCafe\\MC64K\\Parser\\EffectiveAddress\\GPRIndirectDisplacement' => '/parser/effective_address/GPRIndirectDisplacement.php',
  'ABadCafe\\MC64K\\Parser\\EffectiveAddress\\GPRDirect' => '/parser/effective_address/GPRDirect.php',
  'ABadCafe\\MC64K\\Parser\\EffectiveAddress\\PCIndirectDisplacement' => '/parser/effective_address/PCIndirectDisplacement.php',
  'ABadCafe\\MC64K\\Parser\\EffectiveAddress\\AllIntegerWriteable' => '/parser/effective_address/AllIntegerWriteable.php',
  'ABadCafe\\MC64K\\Parser\\EffectiveAddress\\GPRIndirectIndexed' => '/parser/effective_address/GPRIndirectIndexed.php',
  'ABadCafe\\MC64K\\Parser\\EffectiveAddress\\FPRDirect' => '/parser/effective_address/FPRDirect.php',
  'ABadCafe\\MC64K\\Parser\\EffectiveAddress\\IParser' => '/parser/effective_address/IParser.php',
  'ABadCafe\\MC64K\\Parser\\EffectiveAddress\\TOperationSizeAware' => '/parser/effective_address/TOperationSizeAware.php',
  'ABadCafe\\MC64K\\Parser\\EffectiveAddress\\AllControlAddressing' => '/parser/effective_address/AllControlAddressing.php',
  'ABadCafe\\MC64K\\Parser\\EffectiveAddress\\GPRIndirectIndexedDisplacement' => '/parser/effective_address/GPRIndirectIndexedDisplacement.php',
  'ABadCafe\\MC64K\\Parser\\EffectiveAddress\\FloatImmediate' => '/parser/effective_address/FloatImmediate.php',
  'ABadCafe\\MC64K\\Parser\\EffectiveAddress\\AllIntegerReadable' => '/parser/effective_address/AllIntegerReadable.php',
  'ABadCafe\\MC64K\\Parser\\EffectiveAddress\\PCIndirectIndexedDisplacement' => '/parser/effective_address/PCIndirectIndexedDisplacement.php',
  'ABadCafe\\MC64K\\Parser\\EffectiveAddress\\GPRIndirect' => '/parser/effective_address/GPRIndirect.php',
  'ABadCafe\\MC64K\\Parser\\EffectiveAddress\\PCIndirectIndexed' => '/parser/effective_address/PCIndirectIndexed.php',
  'ABadCafe\\MC64K\\Parser\\EffectiveAddress\\Composite' => '/parser/effective_address/Composite.php',
  'ABadCafe\\MC64K\\Tokeniser\\Instruction' => '/tokeniser/Instruction.php',
  'ABadCafe\\MC64K\\Defs\\Register\\INames' => '/defs/register/INames.php',
  'ABadCafe\\MC64K\\Defs\\Register\\Enumerator' => '/defs/register/Enumerator.php',
  'ABadCafe\\MC64K\\Defs\\Mnemonic\\IDataMove' => '/defs/mnemonic/IDataMove.php',
  'ABadCafe\\MC64K\\Defs\\Mnemonic\\IControl' => '/defs/mnemonic/IControl.php',
  'ABadCafe\\MC64K\\Defs\\Mnemonic\\IByteCodeGroups' => '/defs/mnemonic/IByteCodeGroups.php',
  'ABadCafe\\MC64K\\Defs\\Mnemonic\\IArithmetic' => '/defs/mnemonic/IArithmetic.php',
  'ABadCafe\\MC64K\\Defs\\Mnemonic\\ILogical' => '/defs/mnemonic/ILogical.php',
  'ABadCafe\\MC64K\\Defs\\Mnemonic\\IMatches' => '/defs/mnemonic/IMatches.php',
  'ABadCafe\\MC64K\\Defs\\Mnemonic\\IOperandSizes' => '/defs/mnemonic/IOperandSizes.php',
  'ABadCafe\\MC64K\\Defs\\EffectiveAddress\\IRegisterDirect' => '/defs/effective_address/IRegisterDirect.php',
  'ABadCafe\\MC64K\\Defs\\EffectiveAddress\\IByteCodeGroups' => '/defs/effective_address/IByteCodeGroups.php',
  'ABadCafe\\MC64K\\Defs\\EffectiveAddress\\IByteCodeNames' => '/defs/effective_address/IByteCodeNames.php',
  'ABadCafe\\MC64K\\Defs\\EffectiveAddress\\IRegisterIndirect' => '/defs/effective_address/IRegisterIndirect.php',
  'ABadCafe\\MC64K\\Defs\\EffectiveAddress\\IMatches' => '/defs/effective_address/IMatches.php',
  'ABadCafe\\MC64K\\Defs\\EffectiveAddress\\ISameAsDestination' => '/defs/effective_address/ISameAsDestination.php',
  'ABadCafe\\MC64K\\Defs\\EffectiveAddress\\IRegisterIndirectIndexed' => '/defs/effective_address/IRegisterIndirectIndexed.php',
  'ABadCafe\\MC64K\\Defs\\EffectiveAddress\\IOther' => '/defs/effective_address/IOther.php',
];