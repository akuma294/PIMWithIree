#include "iree/compiler/Codegen/Common/ReplaceMatMulPass.h"
#include "mlir/Dialect/Linalg/IR/Linalg.h"
#include "mlir/Pass/Pass.h"
#include "mlir/IR/PatternMatch.h"
#include "mlir/Transforms/DialectConversion.h"

#include "mlir/Dialect/Func/IR/FuncOps.h"         // for func::FuncOp
// #include "mlir/Dialect/Linalg/IR/Linalg.h"        // for linalg dialect
// #include "mlir/IR/PatternMatch.h"
// #include "mlir/Pass/Pass.h"
#include "mlir/Transforms/GreedyPatternRewriteDriver.h" // for applyPatternsAndFoldGreedily


using namespace mlir;

namespace mlir::iree_compiler{
    struct ReplaceMatmulPass : public PassWrapper<ReplaceMatmulPass, OperationPass<func::FuncOp>> {
        void runOnOperation() override {
            auto func = getOperation();
            MLIRContext *ctx = func.getContext();
            RewritePatternSet patterns(ctx);

            // Custom pattern to replace linalg.matmul
            struct ReplaceMatmulPattern : public RewritePattern {
                ReplaceMatmulPattern(MLIRContext *ctx)
                    : RewritePattern("linalg.matmul", 1, ctx) {}

                    LogicalResult matchAndRewrite(Operation *op, PatternRewriter &rewriter) const override {
                            // Get first operand
                            Value lhs = op->getOperand(0);
                            rewriter.replaceOp(op, lhs);
                            return success();
                    }
                };

            patterns.add<ReplaceMatmulPattern>(ctx);
            (void)applyPatternsAndFoldGreedily(func, std::move(patterns));
        }

        StringRef getArgument() const override { return "iree-replace-matmul"; }
        StringRef getDescription() const override {
          return "Replaces linalg.matmul with its lhs operand";
        }
    };


    std::unique_ptr<mlir::Pass> createReplaceMatmulPass() {
        return std::make_unique<ReplaceMatmulPass>();
      }

      
      // CLI registration
    //   static mlir::PassRegistration<ReplaceMatmulPass> pass(
    //       "iree-replace-matmul",
    //       "Replaces linalg.matmul with its lhs operand");  
    mlir::PassRegistration<ReplaceMatmulPass> registerPass; 

}

// Static registration
// static mlir::PassRegistration<ReplaceMatMulPass> pass(
//     "iree-replace-matmul",
//     "Replaces linalg.matmul with its lhs operand");

