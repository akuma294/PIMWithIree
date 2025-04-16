#ifndef IREE_COMPILER_CODEGEN_COMMON_REPLACEMATMULPASS_H_
#define IREE_COMPILER_CODEGEN_COMMON_REPLACEMATMULPASS_H_

#include "mlir/Pass/Pass.h"

namespace mlir::iree_compiler {
std::unique_ptr<mlir::Pass> createReplaceMatmulPass();
}  // namespace mlir::iree_compiler

#endif  // IREE_COMPILER_CODEGEN_COMMON_REPLACEMATMULPASS_H_
