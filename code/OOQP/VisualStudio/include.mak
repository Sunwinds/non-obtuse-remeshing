HEADERS =  \
 ..\include\Ma27Solver.h \
 ..\include\Ma57Solver.h \
 ..\include\Data.h \
 ..\include\DoubleLinearSolver.h \
 ..\include\DoubleMatrix.h \
 ..\include\DoubleMatrixHandle.h \
 ..\include\DoubleMatrixTypes.h \
 ..\include\IotrRefCount.h \
 ..\include\LinearAlgebraPackage.h \
 ..\include\LinearSystem.h \
 ..\include\OoqpMonitorData.h \
 ..\include\OoqpMonitor.h \
 ..\include\OoqpPetscMonitor.h \
 ..\include\OoqpStartStrategy.h \
 ..\include\OoqpVersion.h \
 ..\include\ProblemFormulation.h \
 ..\include\Residuals.h \
 ..\include\SmartPointer.h \
 ..\include\Solver.h \
 ..\include\Status.h \
 ..\include\Variables.h \
 ..\include\OoqpBlas.h \
 ..\include\OoqpVector.h \
 ..\include\OoqpVectorHandle.h \
 ..\include\SimpleVector.h \
 ..\include\SimpleVectorHandle.h \
 ..\include\VectorUtilities.h \
 ..\include\cBounds.h \
 ..\include\cMpsReader.h \
 ..\include\cMpsReaderPetsc.h \
 ..\include\cQpBoundDense.h \
 ..\include\cQpBound.h \
 ..\include\cQpBoundPetsc.h \
 ..\include\cQpGenDense.h \
 ..\include\cQpGen.h \
 ..\include\cQpGenPetsc.h \
 ..\include\cQpGenSparse.h \
 ..\include\DenseGenMatrix.h \
 ..\include\DenseGenMatrixHandle.h \
 ..\include\DenseLinearAlgebraPackage.h \
 ..\include\DenseStorage.h \
 ..\include\DenseStorageHandle.h \
 ..\include\DenseSymMatrix.h \
 ..\include\DenseSymMatrixHandle.h \
 ..\include\DeSymIndefSolver.h \
 ..\include\DeSymPSDSolver.h \
 ..\include\HuberData.h \
 ..\include\Huber.h \
 ..\include\HuberLinsys.h \
 ..\include\HuberResiduals.h \
 ..\include\HuberVars.h \
 ..\include\QpBoundData.h \
 ..\include\QpBoundDense.h \
 ..\include\QpBoundDenseLinsys.h \
 ..\include\QpBound.h \
 ..\include\QpBoundLinsys.h \
 ..\include\QpBoundPetsc.h \
 ..\include\QpBoundResiduals.h \
 ..\include\QpBoundVars.h \
 ..\include\QpGenData.h \
 ..\include\QpGenDense.h \
 ..\include\QpGenDenseLinsys.h \
 ..\include\QpGenDriver.h \
 ..\include\QpGen.h \
 ..\include\QpGenLinsys.h \
 ..\include\QpGenPetsc.h \
 ..\include\QpGenResiduals.h \
 ..\include\QpGenSparseLinsys.h \
 ..\include\QpGenSparseMa27.h \
 ..\include\QpGenSparseMa57.h \
 ..\include\QpGenSparseOblio.h \
 ..\include\QpGenSparseSeq.h \
 ..\include\QpGenSparseSuperLu.h \
 ..\include\QpGenVars.h \
 ..\include\GondzioSolver.h \
 ..\include\MehrotraSolver.h \
 ..\include\hash.h \
 ..\include\MpsReader.h \
 ..\include\SparseGenMatrix.h \
 ..\include\SparseGenMatrixHandle.h \
 ..\include\SparseLinearAlgebraPackage.h \
 ..\include\SparseStorage.h \
 ..\include\SparseStorageHandle.h \
 ..\include\SparseSymMatrix.h \
 ..\include\SparseSymMatrixHandle.h \
 ..\include\SvmData.h \
 ..\include\Svm.h \
 ..\include\SvmLinsys.h \
 ..\include\SvmResiduals.h \
 ..\include\SvmVars.h

all : $(HEADERS)

..\include\Ma27Solver.h : ..\src\LinearSolvers\Ma27Solver\Ma27Solver.h
	copy ..\src\LinearSolvers\Ma27Solver\Ma27Solver.h ..\include\Ma27Solver.h

..\include\Ma57Solver.h : ..\src\LinearSolvers\Ma57Solver\Ma57Solver.h
	copy ..\src\LinearSolvers\Ma57Solver\Ma57Solver.h ..\include\Ma57Solver.h

..\include\Data.h : ..\src\Abstract\Data.h
	copy ..\src\Abstract\Data.h ..\include\Data.h

..\include\DoubleLinearSolver.h : ..\src\Abstract\DoubleLinearSolver.h
	copy ..\src\Abstract\DoubleLinearSolver.h ..\include\DoubleLinearSolver.h

..\include\DoubleMatrix.h : ..\src\Abstract\DoubleMatrix.h
	copy ..\src\Abstract\DoubleMatrix.h ..\include\DoubleMatrix.h

..\include\DoubleMatrixHandle.h : ..\src\Abstract\DoubleMatrixHandle.h
	copy ..\src\Abstract\DoubleMatrixHandle.h ..\include\DoubleMatrixHandle.h

..\include\DoubleMatrixTypes.h : ..\src\Abstract\DoubleMatrixTypes.h
	copy ..\src\Abstract\DoubleMatrixTypes.h ..\include\DoubleMatrixTypes.h

..\include\IotrRefCount.h : ..\src\Abstract\IotrRefCount.h
	copy ..\src\Abstract\IotrRefCount.h ..\include\IotrRefCount.h

..\include\LinearAlgebraPackage.h : ..\src\Abstract\LinearAlgebraPackage.h
	copy ..\src\Abstract\LinearAlgebraPackage.h ..\include\LinearAlgebraPackage.h

..\include\LinearSystem.h : ..\src\Abstract\LinearSystem.h
	copy ..\src\Abstract\LinearSystem.h ..\include\LinearSystem.h

..\include\OoqpMonitorData.h : ..\src\Abstract\OoqpMonitorData.h
	copy ..\src\Abstract\OoqpMonitorData.h ..\include\OoqpMonitorData.h

..\include\OoqpMonitor.h : ..\src\Abstract\OoqpMonitor.h
	copy ..\src\Abstract\OoqpMonitor.h ..\include\OoqpMonitor.h

..\include\OoqpPetscMonitor.h : ..\src\Abstract\OoqpPetscMonitor.h
	copy ..\src\Abstract\OoqpPetscMonitor.h ..\include\OoqpPetscMonitor.h

..\include\OoqpStartStrategy.h : ..\src\Abstract\OoqpStartStrategy.h
	copy ..\src\Abstract\OoqpStartStrategy.h ..\include\OoqpStartStrategy.h

..\include\OoqpVersion.h : ..\src\Abstract\OoqpVersion.h
	copy ..\src\Abstract\OoqpVersion.h ..\include\OoqpVersion.h

..\include\ProblemFormulation.h : ..\src\Abstract\ProblemFormulation.h
	copy ..\src\Abstract\ProblemFormulation.h ..\include\ProblemFormulation.h

..\include\Residuals.h : ..\src\Abstract\Residuals.h
	copy ..\src\Abstract\Residuals.h ..\include\Residuals.h

..\include\SmartPointer.h : ..\src\Abstract\SmartPointer.h
	copy ..\src\Abstract\SmartPointer.h ..\include\SmartPointer.h

..\include\Solver.h : ..\src\Abstract\Solver.h
	copy ..\src\Abstract\Solver.h ..\include\Solver.h

..\include\Status.h : ..\src\Abstract\Status.h
	copy ..\src\Abstract\Status.h ..\include\Status.h

..\include\Variables.h : ..\src\Abstract\Variables.h
	copy ..\src\Abstract\Variables.h ..\include\Variables.h

..\include\OoqpBlas.h : ..\src\Utilities\OoqpBlas.h
	copy ..\src\Utilities\OoqpBlas.h ..\include\OoqpBlas.h

..\include\OoqpVector.h : ..\src\Vector\OoqpVector.h
	copy ..\src\Vector\OoqpVector.h ..\include\OoqpVector.h

..\include\OoqpVectorHandle.h : ..\src\Vector\OoqpVectorHandle.h
	copy ..\src\Vector\OoqpVectorHandle.h ..\include\OoqpVectorHandle.h

..\include\SimpleVector.h : ..\src\Vector\SimpleVector.h
	copy ..\src\Vector\SimpleVector.h ..\include\SimpleVector.h

..\include\SimpleVectorHandle.h : ..\src\Vector\SimpleVectorHandle.h
	copy ..\src\Vector\SimpleVectorHandle.h ..\include\SimpleVectorHandle.h

..\include\VectorUtilities.h : ..\src\Vector\VectorUtilities.h
	copy ..\src\Vector\VectorUtilities.h ..\include\VectorUtilities.h

..\include\cBounds.h : ..\src\CInterface\cBounds.h
	copy ..\src\CInterface\cBounds.h ..\include\cBounds.h

..\include\cMpsReader.h : ..\src\CInterface\cMpsReader.h
	copy ..\src\CInterface\cMpsReader.h ..\include\cMpsReader.h

..\include\cMpsReaderPetsc.h : ..\src\CInterface\cMpsReaderPetsc.h
	copy ..\src\CInterface\cMpsReaderPetsc.h ..\include\cMpsReaderPetsc.h

..\include\cQpBoundDense.h : ..\src\CInterface\cQpBoundDense.h
	copy ..\src\CInterface\cQpBoundDense.h ..\include\cQpBoundDense.h

..\include\cQpBound.h : ..\src\CInterface\cQpBound.h
	copy ..\src\CInterface\cQpBound.h ..\include\cQpBound.h

..\include\cQpBoundPetsc.h : ..\src\CInterface\cQpBoundPetsc.h
	copy ..\src\CInterface\cQpBoundPetsc.h ..\include\cQpBoundPetsc.h

..\include\cQpGenDense.h : ..\src\CInterface\cQpGenDense.h
	copy ..\src\CInterface\cQpGenDense.h ..\include\cQpGenDense.h

..\include\cQpGen.h : ..\src\CInterface\cQpGen.h
	copy ..\src\CInterface\cQpGen.h ..\include\cQpGen.h

..\include\cQpGenPetsc.h : ..\src\CInterface\cQpGenPetsc.h
	copy ..\src\CInterface\cQpGenPetsc.h ..\include\cQpGenPetsc.h

..\include\cQpGenSparse.h : ..\src\CInterface\cQpGenSparse.h
	copy ..\src\CInterface\cQpGenSparse.h ..\include\cQpGenSparse.h

..\include\DenseGenMatrix.h : ..\src\DenseLinearAlgebra\DenseGenMatrix.h
	copy ..\src\DenseLinearAlgebra\DenseGenMatrix.h ..\include\DenseGenMatrix.h

..\include\DenseGenMatrixHandle.h : ..\src\DenseLinearAlgebra\DenseGenMatrixHandle.h
	copy ..\src\DenseLinearAlgebra\DenseGenMatrixHandle.h ..\include\DenseGenMatrixHandle.h

..\include\DenseLinearAlgebraPackage.h : ..\src\DenseLinearAlgebra\DenseLinearAlgebraPackage.h
	copy ..\src\DenseLinearAlgebra\DenseLinearAlgebraPackage.h ..\include\DenseLinearAlgebraPackage.h

..\include\DenseStorage.h : ..\src\DenseLinearAlgebra\DenseStorage.h
	copy ..\src\DenseLinearAlgebra\DenseStorage.h ..\include\DenseStorage.h

..\include\DenseStorageHandle.h : ..\src\DenseLinearAlgebra\DenseStorageHandle.h
	copy ..\src\DenseLinearAlgebra\DenseStorageHandle.h ..\include\DenseStorageHandle.h

..\include\DenseSymMatrix.h : ..\src\DenseLinearAlgebra\DenseSymMatrix.h
	copy ..\src\DenseLinearAlgebra\DenseSymMatrix.h ..\include\DenseSymMatrix.h

..\include\DenseSymMatrixHandle.h : ..\src\DenseLinearAlgebra\DenseSymMatrixHandle.h
	copy ..\src\DenseLinearAlgebra\DenseSymMatrixHandle.h ..\include\DenseSymMatrixHandle.h

..\include\DeSymIndefSolver.h : ..\src\DenseLinearAlgebra\DeSymIndefSolver.h
	copy ..\src\DenseLinearAlgebra\DeSymIndefSolver.h ..\include\DeSymIndefSolver.h

..\include\DeSymPSDSolver.h : ..\src\DenseLinearAlgebra\DeSymPSDSolver.h
	copy ..\src\DenseLinearAlgebra\DeSymPSDSolver.h ..\include\DeSymPSDSolver.h

..\include\HuberData.h : ..\src\Huber\HuberData.h
	copy ..\src\Huber\HuberData.h ..\include\HuberData.h

..\include\Huber.h : ..\src\Huber\Huber.h
	copy ..\src\Huber\Huber.h ..\include\Huber.h

..\include\HuberLinsys.h : ..\src\Huber\HuberLinsys.h
	copy ..\src\Huber\HuberLinsys.h ..\include\HuberLinsys.h

..\include\HuberResiduals.h : ..\src\Huber\HuberResiduals.h
	copy ..\src\Huber\HuberResiduals.h ..\include\HuberResiduals.h

..\include\HuberVars.h : ..\src\Huber\HuberVars.h
	copy ..\src\Huber\HuberVars.h ..\include\HuberVars.h

..\include\QpBoundData.h : ..\src\QpBound\QpBoundData.h
	copy ..\src\QpBound\QpBoundData.h ..\include\QpBoundData.h

..\include\QpBoundDense.h : ..\src\QpBound\QpBoundDense.h
	copy ..\src\QpBound\QpBoundDense.h ..\include\QpBoundDense.h

..\include\QpBoundDenseLinsys.h : ..\src\QpBound\QpBoundDenseLinsys.h
	copy ..\src\QpBound\QpBoundDenseLinsys.h ..\include\QpBoundDenseLinsys.h

..\include\QpBound.h : ..\src\QpBound\QpBound.h
	copy ..\src\QpBound\QpBound.h ..\include\QpBound.h

..\include\QpBoundLinsys.h : ..\src\QpBound\QpBoundLinsys.h
	copy ..\src\QpBound\QpBoundLinsys.h ..\include\QpBoundLinsys.h

..\include\QpBoundPetsc.h : ..\src\QpBound\QpBoundPetsc.h
	copy ..\src\QpBound\QpBoundPetsc.h ..\include\QpBoundPetsc.h

..\include\QpBoundResiduals.h : ..\src\QpBound\QpBoundResiduals.h
	copy ..\src\QpBound\QpBoundResiduals.h ..\include\QpBoundResiduals.h

..\include\QpBoundVars.h : ..\src\QpBound\QpBoundVars.h
	copy ..\src\QpBound\QpBoundVars.h ..\include\QpBoundVars.h

..\include\QpGenData.h : ..\src\QpGen\QpGenData.h
	copy ..\src\QpGen\QpGenData.h ..\include\QpGenData.h

..\include\QpGenDense.h : ..\src\QpGen\QpGenDense.h
	copy ..\src\QpGen\QpGenDense.h ..\include\QpGenDense.h

..\include\QpGenDenseLinsys.h : ..\src\QpGen\QpGenDenseLinsys.h
	copy ..\src\QpGen\QpGenDenseLinsys.h ..\include\QpGenDenseLinsys.h

..\include\QpGenDriver.h : ..\src\QpGen\QpGenDriver.h
	copy ..\src\QpGen\QpGenDriver.h ..\include\QpGenDriver.h

..\include\QpGen.h : ..\src\QpGen\QpGen.h
	copy ..\src\QpGen\QpGen.h ..\include\QpGen.h

..\include\QpGenLinsys.h : ..\src\QpGen\QpGenLinsys.h
	copy ..\src\QpGen\QpGenLinsys.h ..\include\QpGenLinsys.h

..\include\QpGenPetsc.h : ..\src\QpGen\QpGenPetsc.h
	copy ..\src\QpGen\QpGenPetsc.h ..\include\QpGenPetsc.h

..\include\QpGenResiduals.h : ..\src\QpGen\QpGenResiduals.h
	copy ..\src\QpGen\QpGenResiduals.h ..\include\QpGenResiduals.h

..\include\QpGenSparseLinsys.h : ..\src\QpGen\QpGenSparseLinsys.h
	copy ..\src\QpGen\QpGenSparseLinsys.h ..\include\QpGenSparseLinsys.h

..\include\QpGenSparseMa27.h : ..\src\QpGen\QpGenSparseMa27.h
	copy ..\src\QpGen\QpGenSparseMa27.h ..\include\QpGenSparseMa27.h

..\include\QpGenSparseMa57.h : ..\src\QpGen\QpGenSparseMa57.h
	copy ..\src\QpGen\QpGenSparseMa57.h ..\include\QpGenSparseMa57.h

..\include\QpGenSparseOblio.h : ..\src\QpGen\QpGenSparseOblio.h
	copy ..\src\QpGen\QpGenSparseOblio.h ..\include\QpGenSparseOblio.h

..\include\QpGenSparseSeq.h : ..\src\QpGen\QpGenSparseSeq.h
	copy ..\src\QpGen\QpGenSparseSeq.h ..\include\QpGenSparseSeq.h

..\include\QpGenSparseSuperLu.h : ..\src\QpGen\QpGenSparseSuperLu.h
	copy ..\src\QpGen\QpGenSparseSuperLu.h ..\include\QpGenSparseSuperLu.h

..\include\QpGenVars.h : ..\src\QpGen\QpGenVars.h
	copy ..\src\QpGen\QpGenVars.h ..\include\QpGenVars.h

..\include\GondzioSolver.h : ..\src\QpSolvers\GondzioSolver.h
	copy ..\src\QpSolvers\GondzioSolver.h ..\include\GondzioSolver.h

..\include\MehrotraSolver.h : ..\src\QpSolvers\MehrotraSolver.h
	copy ..\src\QpSolvers\MehrotraSolver.h ..\include\MehrotraSolver.h

..\include\hash.h : ..\src\Readers\hash.h
	copy ..\src\Readers\hash.h ..\include\hash.h

..\include\MpsReader.h : ..\src\Readers\MpsReader.h
	copy ..\src\Readers\MpsReader.h ..\include\MpsReader.h

..\include\SparseGenMatrix.h : ..\src\SparseLinearAlgebra\SparseGenMatrix.h
	copy ..\src\SparseLinearAlgebra\SparseGenMatrix.h ..\include\SparseGenMatrix.h

..\include\SparseGenMatrixHandle.h : ..\src\SparseLinearAlgebra\SparseGenMatrixHandle.h
	copy ..\src\SparseLinearAlgebra\SparseGenMatrixHandle.h ..\include\SparseGenMatrixHandle.h

..\include\SparseLinearAlgebraPackage.h : ..\src\SparseLinearAlgebra\SparseLinearAlgebraPackage.h
	copy ..\src\SparseLinearAlgebra\SparseLinearAlgebraPackage.h ..\include\SparseLinearAlgebraPackage.h

..\include\SparseStorage.h : ..\src\SparseLinearAlgebra\SparseStorage.h
	copy ..\src\SparseLinearAlgebra\SparseStorage.h ..\include\SparseStorage.h

..\include\SparseStorageHandle.h : ..\src\SparseLinearAlgebra\SparseStorageHandle.h
	copy ..\src\SparseLinearAlgebra\SparseStorageHandle.h ..\include\SparseStorageHandle.h

..\include\SparseSymMatrix.h : ..\src\SparseLinearAlgebra\SparseSymMatrix.h
	copy ..\src\SparseLinearAlgebra\SparseSymMatrix.h ..\include\SparseSymMatrix.h

..\include\SparseSymMatrixHandle.h : ..\src\SparseLinearAlgebra\SparseSymMatrixHandle.h
	copy ..\src\SparseLinearAlgebra\SparseSymMatrixHandle.h ..\include\SparseSymMatrixHandle.h

..\include\SvmData.h : ..\src\Svm\SvmData.h
	copy ..\src\Svm\SvmData.h ..\include\SvmData.h

..\include\Svm.h : ..\src\Svm\Svm.h
	copy ..\src\Svm\Svm.h ..\include\Svm.h

..\include\SvmLinsys.h : ..\src\Svm\SvmLinsys.h
	copy ..\src\Svm\SvmLinsys.h ..\include\SvmLinsys.h

..\include\SvmResiduals.h : ..\src\Svm\SvmResiduals.h
	copy ..\src\Svm\SvmResiduals.h ..\include\SvmResiduals.h

..\include\SvmVars.h : ..\src\Svm\SvmVars.h
	copy ..\src\Svm\SvmVars.h ..\include\SvmVars.h

clean:
    del $(HEADERS)
