#include "common.h"
#include "jsRadPose.h"

using std::chrono::microseconds;
using std::chrono::duration_cast;
typedef std::chrono::high_resolution_clock Clock;


const char* JSRadPose::kName = "jsRadPose";
MTypeId JSRadPose::kId(0x00000005);

// Static node attrs
MObject JSRadPose::aFunctionType;
MObject JSRadPose::aHeight;
MObject JSRadPose::aGlobalSigma;
MObject JSRadPose::aUseGlobalSigma;
MObject JSRadPose::aPoseRotateOffset;
MObject JSRadPose::aRotateLocatorsVisible;

MObject JSRadPose::aIncludeTranslateX;
MObject JSRadPose::aIncludeTranslateY;
MObject JSRadPose::aIncludeTranslateZ;
MObject JSRadPose::aIncludeRotate;
MObject JSRadPose::aIncludeScaleX;
MObject JSRadPose::aIncludeScaleY;
MObject JSRadPose::aIncludeScaleZ;
MObject JSRadPose::aIncludeR;
MObject JSRadPose::aIncludeG;
MObject JSRadPose::aIncludeB;
MObject JSRadPose::aIncludeA;
MObject JSRadPose::aInputIncludes;

MObject JSRadPose::aPoseMatrix;
MObject JSRadPose::aPoseRotateLocX;
MObject JSRadPose::aPoseRotateLocY;
MObject JSRadPose::aPoseRotateLocZ;
MObject JSRadPose::aPoseColor;
MObject JSRadPose::aPoseTransparency;

MObject JSRadPose::aTargetMatrix;
MObject JSRadPose::aRotateLocX;
MObject JSRadPose::aRotateLocY;
MObject JSRadPose::aRotateLocZ;
MObject JSRadPose::aTargetColor;
MObject JSRadPose::aTargetTransparency;
MObject JSRadPose::aTargetSigma;
MObject JSRadPose::aOutputSigma;
MObject JSRadPose::aTarget;

MObject JSRadPose::aOutputTranslateX;
MObject JSRadPose::aOutputTranslateY;
MObject JSRadPose::aOutputTranslateZ;
MObject JSRadPose::aOutputRotateX;
MObject JSRadPose::aOutputRotateY;
MObject JSRadPose::aOutputRotateZ;
MObject JSRadPose::aOutputScaleX;
MObject JSRadPose::aOutputScaleY;
MObject JSRadPose::aOutputScaleZ;
MObject JSRadPose::aOutputColor;
MObject JSRadPose::aOutputTransparency;
MObject JSRadPose::aOutputInterpolate;


// Constructor / destructor / creator functions 
JSRadPose::JSRadPose() {}
JSRadPose::~JSRadPose() {}
void* JSRadPose::creator() { return new JSRadPose; }

MStatus JSRadPose::initialize()
{
	MStatus status;

	MFnCompoundAttribute fnCompound;
	MFnEnumAttribute fnEnum;
	MFnMatrixAttribute fnMatrix;
	MFnNumericAttribute fnNumeric;
	MFnTypedAttribute fnTyped;

	// RBF Solver attrs
	aFunctionType = fnEnum.create("functionType", "functionType");
	fnEnum.addField("Gaussian", 0);
	fnEnum.addField("Multiquadratic", 1);
	fnEnum.addField("Inverse Multiquadratic", 2);
	fnEnum.addField("Linear", 3);
	fnEnum.setKeyable(true);
	fnEnum.setStorable(true);
	addAttribute(aFunctionType);

	aHeight = fnNumeric.create("height", "height", MFnNumericData::kDouble, 1.0);
	fnNumeric.setKeyable(true);
	fnNumeric.setStorable(true);
	fnNumeric.setMin(0.00001);
	addAttribute(aHeight);

	aGlobalSigma = fnNumeric.create("globalSigma", "globalSigma", MFnNumericData::kDouble, 0.1);
	fnNumeric.setKeyable(true);
	fnNumeric.setStorable(true);
	fnNumeric.setMin(0.00001);
	addAttribute(aGlobalSigma);

	aUseGlobalSigma = fnNumeric.create("useGlobalSigma", "useGlobalSigma", MFnNumericData::kBoolean, true);
	fnNumeric.setKeyable(true);
	fnNumeric.setStorable(true);
	fnNumeric.setMin(0.00000001);
	addAttribute(aUseGlobalSigma);

	aPoseRotateOffset = fnNumeric.create("rotateLocatorOffset", "rotateLocatorOffset", MFnNumericData::kDouble, 15.0);
	addAttribute(aPoseRotateOffset);

	aRotateLocatorsVisible = fnNumeric.create("rotateLocatorsVisible", "rotateLocatorsVisible", MFnNumericData::kBoolean);
	fnNumeric.setDefault(true);
	addAttribute(aRotateLocatorsVisible);

	// Input include booleans
	aIncludeTranslateX = fnNumeric.create("includeInputTranslateX", "includeInputTranslateX", MFnNumericData::kBoolean, true);
	fnNumeric.setKeyable(true);
	fnNumeric.setStorable(true);

	aIncludeTranslateY = fnNumeric.create("includeInputTranslateY", "includeInputTranslateY", MFnNumericData::kBoolean, true);
	fnNumeric.setKeyable(true);
	fnNumeric.setStorable(true);
	
	aIncludeTranslateZ = fnNumeric.create("includeInputTranslateZ", "includeInputTranslateZ", MFnNumericData::kBoolean, true);
	fnNumeric.setKeyable(true);
	fnNumeric.setStorable(true);

	aIncludeRotate = fnNumeric.create("includeInputRotate", "includeInputRotate", MFnNumericData::kBoolean, true);
	fnNumeric.setKeyable(true);
	fnNumeric.setStorable(true);
	
	aIncludeR = fnNumeric.create("includeInputRed", "includeInputRed", MFnNumericData::kBoolean, true);
	fnNumeric.setKeyable(true);
	fnNumeric.setStorable(true);
	
	aIncludeG = fnNumeric.create("includeInputGreen", "includeInputGreen", MFnNumericData::kBoolean, true);
	fnNumeric.setKeyable(true);
	fnNumeric.setStorable(true);
	
	aIncludeB = fnNumeric.create("includeInputBlue", "includeInputBlue", MFnNumericData::kBoolean, true);
	fnNumeric.setKeyable(true);
	fnNumeric.setStorable(true);
	
	aInputIncludes = fnCompound.create("inputIncludes", "inputIncludes");
	fnCompound.addChild(aIncludeTranslateX);
	fnCompound.addChild(aIncludeTranslateY);
	fnCompound.addChild(aIncludeTranslateZ);
	fnCompound.addChild(aIncludeRotate);
	fnCompound.addChild(aIncludeR);
	fnCompound.addChild(aIncludeG);
	fnCompound.addChild(aIncludeB);
	addAttribute(aInputIncludes);

	// Input pose connections
	aPoseMatrix = fnMatrix.create("poseMatrix", "poseMatrix");
	fnMatrix.setKeyable(true);
	fnMatrix.setStorable(true);
	fnMatrix.setHidden(true);
	addAttribute(aPoseMatrix);

	aPoseRotateLocX = fnMatrix.create("poseRotateLocX", "poseRotateLocX");
	fnMatrix.setHidden(true);
	addAttribute(aPoseRotateLocX);

	aPoseRotateLocY = fnMatrix.create("poseRotateLocY", "poseRotateLocY");
	fnMatrix.setHidden(true);
	addAttribute(aPoseRotateLocY);

	aPoseRotateLocZ = fnMatrix.create("poseRotateLocZ", "poseRotateLocZ");
	fnMatrix.setHidden(true);
	addAttribute(aPoseRotateLocZ);

	aPoseColor = fnNumeric.create("poseColor", "poseColor", MFnNumericData::k3Double);
	fnNumeric.setKeyable(true);
	fnNumeric.setStorable(true);
	fnNumeric.setHidden(true);
	addAttribute(aPoseColor);

	aPoseTransparency = fnNumeric.create("poseTransparency", "poseTransparency", MFnNumericData::k3Double);
	fnNumeric.setKeyable(true);
	fnNumeric.setStorable(true);
	fnNumeric.setHidden(true);
	addAttribute(aPoseTransparency);
	
	// Input target connections
	aTargetMatrix = fnMatrix.create("targetMatrix", "targetMatrix");
	fnMatrix.setKeyable(true);
	fnMatrix.setStorable(true);

	aRotateLocX = fnMatrix.create("rotateLocX", "rotateLocX");
	fnMatrix.setKeyable(true);
	fnMatrix.setStorable(true);

	aRotateLocY = fnMatrix.create("rotateLocY", "rotateLocY");
	fnMatrix.setKeyable(true);
	fnMatrix.setStorable(true);

	aRotateLocZ = fnMatrix.create("rotateLocZ", "rotateLocZ");
	fnMatrix.setKeyable(true);
	fnMatrix.setStorable(true);

	aTargetColor = fnNumeric.create("targetColor", "targetColor", MFnNumericData::k3Double);
	fnNumeric.setKeyable(true);
	fnNumeric.setStorable(true);

	aTargetTransparency = fnNumeric.create("targetTransparency", "targetTransparency", MFnNumericData::k3Double);
	fnNumeric.setKeyable(true);
	fnNumeric.setStorable(true);

	aTargetSigma = fnNumeric.create("targetSigma", "targetSigma", MFnNumericData::kDouble, 0.05);
	fnNumeric.setKeyable(true);
	fnNumeric.setStorable(true);
	fnNumeric.setMin(0.00000001);

	aTarget = fnCompound.create("target", "target");
	fnCompound.addChild(aTargetMatrix);
	fnCompound.addChild(aRotateLocX);
	fnCompound.addChild(aRotateLocY);
	fnCompound.addChild(aRotateLocZ);
	fnCompound.addChild(aTargetColor);
	fnCompound.addChild(aTargetTransparency);
	fnCompound.addChild(aTargetSigma);
	fnCompound.setArray(true);
	addAttribute(aTarget);

	// Output connections
	aOutputTranslateX = fnNumeric.create("outputTranslateX", "outputTranslateX", MFnNumericData::kDouble);
	fnNumeric.setKeyable(false);
	fnNumeric.setStorable(false);
	fnNumeric.setWritable(false);
	addAttribute(aOutputTranslateX);

	aOutputTranslateY = fnNumeric.create("outputTranslateY", "outputTranslateY", MFnNumericData::kDouble);
	fnNumeric.setKeyable(false);
	fnNumeric.setStorable(false);
	fnNumeric.setWritable(false);
	addAttribute(aOutputTranslateY);

	aOutputTranslateZ = fnNumeric.create("outputTranslateZ", "outputTranslateZ", MFnNumericData::kDouble);
	fnNumeric.setKeyable(false);
	fnNumeric.setStorable(false);
	fnNumeric.setWritable(false);
	addAttribute(aOutputTranslateZ);

	aOutputColor = fnNumeric.create("outputColor", "outputColor", MFnNumericData::k3Double);
	fnNumeric.setKeyable(false);
	fnNumeric.setStorable(false);
	fnNumeric.setWritable(false);
	addAttribute(aOutputColor);

	aOutputInterpolate = fnNumeric.create("outputInterpolate", "outputInterpolate", MFnNumericData::kDouble);
	fnNumeric.setKeyable(false);
	fnNumeric.setKeyable(false);
	fnNumeric.setWritable(false);
	fnNumeric.setArray(true);
	fnNumeric.setUsesArrayDataBuilder(true);
	addAttribute(aOutputInterpolate);

	aOutputSigma = fnNumeric.create("outputSigma", "outputSigma", MFnNumericData::kDouble);
	fnNumeric.setKeyable(false);
	fnNumeric.setStorable(false);
	fnNumeric.setWritable(false);
	fnNumeric.setMin(0.00000001);
	fnNumeric.setArray(true);
	fnNumeric.setUsesArrayDataBuilder(true);
	addAttribute(aOutputSigma);


	// Attribute affects
	attributeAffects(aFunctionType, aOutputTranslateX);
	attributeAffects(aHeight, aOutputTranslateX);
	attributeAffects(aGlobalSigma, aOutputTranslateX);
	attributeAffects(aUseGlobalSigma, aOutputTranslateX);
	attributeAffects(aPoseRotateOffset, aOutputTranslateX);
	attributeAffects(aIncludeTranslateX, aOutputTranslateX);
	attributeAffects(aIncludeTranslateY, aOutputTranslateX);
	attributeAffects(aIncludeTranslateZ, aOutputTranslateX);
	attributeAffects(aIncludeRotate, aOutputTranslateX);
	attributeAffects(aIncludeScaleX, aOutputTranslateX);
	attributeAffects(aIncludeScaleY, aOutputTranslateX);
	attributeAffects(aIncludeScaleZ, aOutputTranslateX);
	attributeAffects(aIncludeR, aOutputTranslateX);
	attributeAffects(aIncludeG, aOutputTranslateX);
	attributeAffects(aIncludeB, aOutputTranslateX);
	attributeAffects(aIncludeA, aOutputTranslateX);
	attributeAffects(aInputIncludes, aOutputTranslateX);
	attributeAffects(aPoseMatrix, aOutputTranslateX);
	attributeAffects(aPoseColor, aOutputTranslateX);
	attributeAffects(aPoseTransparency, aOutputTranslateX);
	attributeAffects(aTargetMatrix, aOutputTranslateX);
	attributeAffects(aTargetColor, aOutputTranslateX);
	attributeAffects(aTargetSigma, aOutputTranslateX);

	attributeAffects(aFunctionType, aOutputTranslateY);
	attributeAffects(aHeight, aOutputTranslateY);
	attributeAffects(aGlobalSigma, aOutputTranslateY);
	attributeAffects(aUseGlobalSigma, aOutputTranslateY);
	attributeAffects(aPoseRotateOffset, aOutputTranslateY);
	attributeAffects(aIncludeTranslateX, aOutputTranslateY);
	attributeAffects(aIncludeTranslateY, aOutputTranslateY);
	attributeAffects(aIncludeTranslateZ, aOutputTranslateY);
	attributeAffects(aIncludeRotate, aOutputTranslateY);
	attributeAffects(aIncludeScaleX, aOutputTranslateY);
	attributeAffects(aIncludeScaleY, aOutputTranslateY);
	attributeAffects(aIncludeScaleZ, aOutputTranslateY);
	attributeAffects(aIncludeR, aOutputTranslateY);
	attributeAffects(aIncludeG, aOutputTranslateY);
	attributeAffects(aIncludeB, aOutputTranslateY);
	attributeAffects(aIncludeA, aOutputTranslateY);
	attributeAffects(aInputIncludes, aOutputTranslateY);
	attributeAffects(aPoseMatrix, aOutputTranslateY);
	attributeAffects(aPoseColor, aOutputTranslateY);
	attributeAffects(aPoseTransparency, aOutputTranslateY);
	attributeAffects(aTargetMatrix, aOutputTranslateY);
	attributeAffects(aTargetColor, aOutputTranslateY);
	attributeAffects(aTargetSigma, aOutputTranslateY);

	attributeAffects(aFunctionType, aOutputTranslateZ);
	attributeAffects(aHeight, aOutputTranslateZ);
	attributeAffects(aGlobalSigma, aOutputTranslateZ);
	attributeAffects(aUseGlobalSigma, aOutputTranslateZ);
	attributeAffects(aPoseRotateOffset, aOutputTranslateZ);
	attributeAffects(aIncludeTranslateX, aOutputTranslateZ);
	attributeAffects(aIncludeTranslateY, aOutputTranslateZ);
	attributeAffects(aIncludeTranslateZ, aOutputTranslateZ);
	attributeAffects(aIncludeRotate, aOutputTranslateZ);
	attributeAffects(aIncludeScaleX, aOutputTranslateZ);
	attributeAffects(aIncludeScaleY, aOutputTranslateZ);
	attributeAffects(aIncludeScaleZ, aOutputTranslateZ);
	attributeAffects(aIncludeR, aOutputTranslateZ);
	attributeAffects(aIncludeG, aOutputTranslateZ);
	attributeAffects(aIncludeB, aOutputTranslateZ);
	attributeAffects(aIncludeA, aOutputTranslateZ);
	attributeAffects(aInputIncludes, aOutputTranslateZ);
	attributeAffects(aPoseMatrix, aOutputTranslateZ);
	attributeAffects(aPoseColor, aOutputTranslateZ);
	attributeAffects(aPoseTransparency, aOutputTranslateZ);
	attributeAffects(aTargetMatrix, aOutputTranslateZ);
	attributeAffects(aTargetColor, aOutputTranslateZ);
	attributeAffects(aTargetSigma, aOutputTranslateZ);

	attributeAffects(aFunctionType, aOutputColor);
	attributeAffects(aHeight, aOutputColor);
	attributeAffects(aGlobalSigma, aOutputColor);
	attributeAffects(aUseGlobalSigma, aOutputColor);
	attributeAffects(aPoseRotateOffset, aOutputColor);
	attributeAffects(aIncludeTranslateX, aOutputColor);
	attributeAffects(aIncludeTranslateY, aOutputColor);
	attributeAffects(aIncludeTranslateZ, aOutputColor);
	attributeAffects(aIncludeRotate, aOutputColor);
	attributeAffects(aIncludeScaleX, aOutputColor);
	attributeAffects(aIncludeScaleY, aOutputColor);
	attributeAffects(aIncludeScaleZ, aOutputColor);
	attributeAffects(aIncludeR, aOutputColor);
	attributeAffects(aIncludeG, aOutputColor);
	attributeAffects(aIncludeB, aOutputColor);
	attributeAffects(aIncludeA, aOutputColor);
	attributeAffects(aInputIncludes, aOutputColor);
	attributeAffects(aPoseMatrix, aOutputColor);
	attributeAffects(aPoseColor, aOutputColor);
	attributeAffects(aTargetMatrix, aOutputColor);
	attributeAffects(aTargetColor, aOutputColor);
	attributeAffects(aTargetSigma, aOutputColor);

	attributeAffects(aFunctionType, aOutputInterpolate);
	attributeAffects(aHeight, aOutputInterpolate);
	attributeAffects(aGlobalSigma, aOutputInterpolate);
	attributeAffects(aUseGlobalSigma, aOutputInterpolate);
	attributeAffects(aPoseRotateOffset, aOutputInterpolate);
	attributeAffects(aIncludeTranslateX, aOutputInterpolate);
	attributeAffects(aIncludeTranslateY, aOutputInterpolate);
	attributeAffects(aIncludeTranslateZ, aOutputInterpolate);
	attributeAffects(aIncludeRotate, aOutputInterpolate);
	attributeAffects(aIncludeScaleX, aOutputInterpolate);
	attributeAffects(aIncludeScaleY, aOutputInterpolate);
	attributeAffects(aIncludeScaleZ, aOutputInterpolate);
	attributeAffects(aIncludeR, aOutputInterpolate);
	attributeAffects(aIncludeG, aOutputInterpolate);
	attributeAffects(aIncludeB, aOutputInterpolate);
	attributeAffects(aIncludeA, aOutputInterpolate);
	attributeAffects(aPoseMatrix, aOutputInterpolate);
	attributeAffects(aPoseColor, aOutputInterpolate);
	attributeAffects(aPoseTransparency, aOutputInterpolate);
	attributeAffects(aTargetMatrix, aOutputInterpolate);
	attributeAffects(aTargetColor, aOutputInterpolate);
	attributeAffects(aTargetSigma, aOutputInterpolate);

	attributeAffects(aFunctionType, aOutputSigma);
	attributeAffects(aHeight, aOutputSigma);
	attributeAffects(aGlobalSigma, aOutputSigma);
	attributeAffects(aUseGlobalSigma, aOutputSigma);
	attributeAffects(aPoseRotateOffset, aOutputSigma);
	attributeAffects(aIncludeTranslateX, aOutputSigma);
	attributeAffects(aIncludeTranslateY, aOutputSigma);
	attributeAffects(aIncludeTranslateZ, aOutputSigma);
	attributeAffects(aIncludeRotate, aOutputSigma);
	attributeAffects(aIncludeScaleX, aOutputSigma);
	attributeAffects(aIncludeScaleY, aOutputSigma);
	attributeAffects(aIncludeScaleZ, aOutputSigma);
	attributeAffects(aIncludeR, aOutputSigma);
	attributeAffects(aIncludeG, aOutputSigma);
	attributeAffects(aIncludeB, aOutputSigma);
	attributeAffects(aIncludeA, aOutputSigma);
	attributeAffects(aPoseMatrix, aOutputSigma);
	attributeAffects(aPoseColor, aOutputSigma);
	attributeAffects(aPoseTransparency, aOutputSigma);
	attributeAffects(aTargetMatrix, aOutputSigma);
	attributeAffects(aTargetColor, aOutputSigma);
	attributeAffects(aTargetSigma, aOutputSigma);

	
	return MS::kSuccess;
}

MStatus JSRadPose::compute(const MPlug &plug, MDataBlock &data)
{
	auto t0 = Clock::now(); //////////////////////////// BENCHMARK /////////////////////////////
	MStatus status;
	// Check plug
	if ( plug != aOutputColor && plug != aOutputInterpolate && plug != aOutputSigma)
	{
		return MS::kInvalidParameter;
	}
	
	// Hard code maximum input dimensions for now
	maxDim_ = 15;

	// Get include values and build vector to multiply against actual inputs, acts as a gate for passing values
	double inclTrsX = static_cast<double>(data.inputValue(aIncludeTranslateX).asBool());
	double inclTrsY = static_cast<double>(data.inputValue(aIncludeTranslateY).asBool());
	double inclTrsZ = static_cast<double>(data.inputValue(aIncludeTranslateZ).asBool());
	double inclRot = static_cast<double>(data.inputValue(aIncludeRotate).asBool());
	double inclR = static_cast<double>(data.inputValue(aIncludeR).asBool());
	double inclG = static_cast<double>(data.inputValue(aIncludeG).asBool());
	double inclB = static_cast<double>(data.inputValue(aIncludeB).asBool());
	includeVec_.resize(maxDim_);
	includeVec_ << inclTrsX, inclTrsY, inclTrsZ, inclRot, inclRot, inclRot, inclRot, inclRot, inclRot, inclRot, inclRot, inclRot, inclR, inclG, inclB;


	// Get RBF values
	ftype_ = data.inputValue(aFunctionType).asShort();
	height_ = data.inputValue(aHeight).asDouble();
	globalSigma = data.inputValue(aGlobalSigma).asDouble();
	usesGlobalSigma_ = data.inputValue(aUseGlobalSigma).asBool();
	rotateOffset_ = data.inputValue(aPoseRotateOffset).asDouble();

	// Get pose matrix and color values
	MMatrix poseMatrix = data.inputValue(aPoseMatrix).asMatrix();
	MMatrix poseRotLocMtxX = data.inputValue(aPoseRotateLocX).asMatrix();
	MMatrix poseRotLocMtxY = data.inputValue(aPoseRotateLocY).asMatrix();
	MMatrix poseRotLocMtxZ = data.inputValue(aPoseRotateLocZ).asMatrix();

	double poseTrsX = poseMatrix[3][0]; double poseTrsY = poseMatrix[3][1]; double poseTrsZ = poseMatrix[3][2];
	double poseRotXX = poseRotLocMtxX[3][0]; double poseRotXY = poseRotLocMtxX[3][1]; double poseRotXZ = poseRotLocMtxX[3][2];
	double poseRotYX = poseRotLocMtxY[3][0]; double poseRotYY = poseRotLocMtxY[3][1]; double poseRotYZ = poseRotLocMtxY[3][2];
	double poseRotZX = poseRotLocMtxZ[3][0]; double poseRotZY = poseRotLocMtxZ[3][2]; double poseRotZZ = poseRotLocMtxZ[3][2];

	double3 &poseColor = data.inputValue(aPoseColor).asDouble3();

	poseVec_.resize(maxDim_);
	poseVec_ << poseTrsX, poseTrsY, poseTrsZ, poseRotXX, poseRotXY, poseRotXZ, poseRotYX, poseRotYY, poseRotYZ, poseRotZX, poseRotZY, poseRotZZ, poseColor[0], poseColor[1], poseColor[2];

	// Get target matrix and color array handles + target count, resize vectors and arrays
	MArrayDataHandle hArrayTarget = data.inputArrayValue(aTarget);
	targetCount_ = hArrayTarget.elementCount();
	targetVecs_.resize(targetCount_);
	bVec_.resize(targetCount_);
	weightVec_.resize(targetCount_);
	sigmas_.setLength(targetCount_);
	rbfValues_.setLength(targetCount_);
		
	for (unsigned int i = 0; i < maxDim_; ++i)
	{
		// Apply include vector, acts as a gate for passing values
		poseVec_[i] = poseVec_[i] * includeVec_[i];
	}

	// Declare eigen matrix
	Eigen::MatrixXd mtx(targetCount_, targetCount_);

	for (unsigned int i = 0; i < targetCount_; ++i)
	{
		// Get target matrix and color handles
		hArrayTarget.jumpToElement(i);
		MDataHandle hTarget = hArrayTarget.outputValue();
		double3 &color = hTarget.child(aTargetColor).asDouble3();
		double localSigma = hTarget.child(aTargetSigma).asDouble();
		MMatrix matrix = hTarget.child(aTargetMatrix).asMatrix();
		MMatrix locMtxX = hTarget.child(aRotateLocX).asMatrix();
		MMatrix locMtxY = hTarget.child(aRotateLocY).asMatrix();
		MMatrix locMtxZ = hTarget.child(aRotateLocZ).asMatrix();

		double trsX = matrix[3][0];	double trsY = matrix[3][1]; double trsZ = matrix[3][2];
		double locRotXX = locMtxX[3][0]; double locRotXY = locMtxX[3][1]; double locRotXZ = locMtxX[3][2];
		double locRotYX = locMtxY[3][0]; double locRotYY = locMtxY[3][1]; double locRotYZ = locMtxY[3][2];
		double locRotZX = locMtxZ[3][0]; double locRotZY = locMtxZ[3][2]; double locRotZZ = locMtxZ[3][2];

		Eigen::VectorXd vec(maxDim_);
		vec << trsX, trsY, trsZ, locRotXX, locRotXY, locRotXZ, locRotYX, locRotYY, locRotYZ, locRotZX, locRotZY, locRotZZ, color[0], color[1], color[2];

		// Apply include vector, acts as a gate for passing values
		for (unsigned int j = 0; j < maxDim_; ++j)
		{
			vec[j] = vec[j] * includeVec_[j];
		}
		targetVecs_[i] = vec;

		// Populate the b-vector using height * 1.0
		bVec_[i] = height_;

		if (!usesGlobalSigma_)
		{
			sigmas_[i] = localSigma;
		}
		else
		{
			sigmas_[i] = globalSigma;
		}

	}

	// Populate the eigenlib matrix
	for (unsigned int i = 0; i < targetCount_; ++i)
	{		
		for (unsigned int j = 0; j < targetCount_; ++j)
		{
			Eigen::VectorXd jvec = targetVecs_[j];
			Eigen::VectorXd diffVec = targetVecs_[i] - jvec;
			double diffNorm = diffVec.norm();
			mtx(i, j) = RBF(diffNorm, height_, sigmas_[i], ftype_);
		}
	}

	// Solve linear system with colPivHouseholderQr()
	weightVec_ = mtx.colPivHouseholderQr().solve(bVec_);

	// Calculate weighted RBFs for outInterpolate values
	for (unsigned int i = 0; i < targetCount_; ++i)
	{
		Eigen::VectorXd vec = targetVecs_[i];
		double diff = (poseVec_ - targetVecs_[i]).norm();
		double rbf = RBF(diff, height_, sigmas_[i], ftype_);
		rbf *= weightVec_[i];
		rbfValues_[i] = rbf;
	}

	// Solve for just RGB out
	double outR{ 0.0 };
	double outG{ 0.0 };
	double outB{ 0.0 };
	double poseR = poseVec_[12];
	double poseG = poseVec_[13];
	double poseB = poseVec_[14];

	for (unsigned int i = 0; i < targetCount_; ++i)
	{
		double inR = targetVecs_[i][12];
		double inG = targetVecs_[i][13];
		double inB = targetVecs_[i][14];
		
		double rbf = rbfValues_[i];
		
		outR += inR * rbf;
		outG += inG * rbf;
		outB += inB * rbf;

	}

	//double colNormalizer = 1/(outR + outG + outB);
	//outR *= colNormalizer; outG *= colNormalizer; outB *= colNormalizer;

	// Solve for XYZ translate out
	double outTrsX{ 0.0 };
	double outTrsY{ 0.0 };
	double outTrsZ{ 0.0 };

	for (unsigned int i = 0; i < targetCount_; ++i)
	{
		outTrsX += rbfValues_[i] * targetVecs_[i][0];
		outTrsY += rbfValues_[i] * targetVecs_[i][1];
		outTrsZ += rbfValues_[i] * targetVecs_[i][2];
	}

	// Get output handles // NB: getting inputValue() here sends it tits up in a ditch
	MDataHandle hOutTrsX = data.outputValue(aOutputTranslateX);
	MDataHandle hOutTrsY = data.outputValue(aOutputTranslateY);
	MDataHandle hOutTrsZ = data.outputValue(aOutputTranslateZ);
	MDataHandle hOutColor= data.outputValue(aOutputColor);
	MArrayDataHandle hArrayOutInterpolate = data.outputArrayValue(aOutputInterpolate);
	MArrayDataBuilder hArrayBuilder = hArrayOutInterpolate.builder();
	MArrayDataHandle hArrayOutSigma = data.outputArrayValue(aOutputSigma);
	MArrayDataBuilder hArraySigBuilder = hArrayOutSigma.builder();
	
	for (unsigned int i = 0; i < targetCount_; ++i)
	{
		hArrayOutInterpolate.jumpToElement(i);
		MDataHandle hOutInterpolate = hArrayBuilder.addElement(i);
		hOutInterpolate.setDouble(rbfValues_[i]);

		hArrayOutSigma.jumpToElement(i);
		MDataHandle hOutSigma = hArraySigBuilder.addElement(i);
		hOutSigma.setDouble(sigmas_[i]);
	}

	hOutTrsX.setDouble(outTrsX);
	hOutTrsY.setDouble(outTrsY);
	hOutTrsZ.setDouble(outTrsZ);
	hOutColor.set3Double(outR, outG, outB);

	// Set the attrs on the datablock clean
	data.setClean(aOutputTranslateX);
	data.setClean(aOutputTranslateY);
	data.setClean(aOutputTranslateZ);
	data.setClean(aOutputRotateX);
	data.setClean(aOutputRotateY);
	data.setClean(aOutputRotateZ);
	data.setClean(aOutputScaleX);
	data.setClean(aOutputScaleY);
	data.setClean(aOutputScaleZ);
	data.setClean(aOutputColor);
	data.setClean(aOutputTransparency);
	data.setClean(aOutputInterpolate);
	data.setClean(aOutputSigma);

	auto t1 = Clock::now(); //////////////////////////// BENCHMARK /////////////////////////////

	long long duration = duration_cast<microseconds>(t1 - t0).count();
	msTotal_ += duration;

	if (clockCounter_ >= 100)
	{
		cout << "compute took " << msTotal_ / double(100) << " microseconds" << endl;
		clockCounter_ = 0;
		msTotal_ = 0;
	}
	clockCounter_++;
	
	
	return MS::kSuccess;
}

double JSRadPose::RBF(double r, double height, double sigma, short ftype)
{
	double out;
	if (ftype == 0)
	{
		// Gaussian
		out = height * exp(-(r*r / 2*sigma*sigma));
	}
	else if (ftype == 1)
	{
		// Multiquadratic
		out = height * pow((r*r + sigma*sigma), 0.5);
	}
	else if (ftype == 2)
	{
		// Inverse multiquadratic
		out = height * pow((r*r + sigma*sigma), -0.5);
	}
	else if (ftype == 3)
	{
		// Linear
		out =   r / sigma;
	}
	else
	{
		return MS::kInvalidParameter;
	}
	return out;
}