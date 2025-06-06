//===-- ProtocolTypes.cpp -------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "Protocol/ProtocolTypes.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/JSON.h"
#include <optional>

using namespace llvm;

namespace lldb_dap::protocol {

bool fromJSON(const json::Value &Params, PresentationHint &PH, json::Path P) {
  auto rawHint = Params.getAsString();
  if (!rawHint) {
    P.report("expected a string");
    return false;
  }
  std::optional<PresentationHint> hint =
      StringSwitch<std::optional<PresentationHint>>(*rawHint)
          .Case("normal", ePresentationHintNormal)
          .Case("emphasize", ePresentationHintEmphasize)
          .Case("deemphasize", ePresentationHintDeemphasize)
          .Default(std::nullopt);
  if (!hint) {
    P.report("unexpected value");
    return false;
  }
  PH = *hint;
  return true;
}

bool fromJSON(const json::Value &Params, Source &S, json::Path P) {
  json::ObjectMapper O(Params, P);
  return O && O.map("name", S.name) && O.map("path", S.path) &&
         O.map("presentationHint", S.presentationHint) &&
         O.map("sourceReference", S.sourceReference);
}

json::Value toJSON(const ExceptionBreakpointsFilter &EBF) {
  json::Object result{{"filter", EBF.filter}, {"label", EBF.label}};

  if (EBF.description)
    result.insert({"description", *EBF.description});
  if (EBF.defaultState)
    result.insert({"default", *EBF.defaultState});
  if (EBF.supportsCondition)
    result.insert({"supportsCondition", *EBF.supportsCondition});
  if (EBF.conditionDescription)
    result.insert({"conditionDescription", *EBF.conditionDescription});

  return result;
}

json::Value toJSON(const ColumnType &T) {
  switch (T) {
  case eColumnTypeString:
    return "string";
  case eColumnTypeNumber:
    return "number";
  case eColumnTypeBoolean:
    return "boolean";
  case eColumnTypeTimestamp:
    return "unixTimestampUTC";
  }
  llvm_unreachable("unhandled column type.");
}

json::Value toJSON(const ColumnDescriptor &CD) {
  json::Object result{{"attributeName", CD.attributeName}, {"label", CD.label}};

  if (CD.format)
    result.insert({"format", *CD.format});
  if (CD.type)
    result.insert({"type", *CD.type});
  if (CD.width)
    result.insert({"width", *CD.width});

  return result;
}

json::Value toJSON(const ChecksumAlgorithm &CA) {
  switch (CA) {
  case eChecksumAlgorithmMD5:
    return "MD5";
  case eChecksumAlgorithmSHA1:
    return "SHA1";
  case eChecksumAlgorithmSHA256:
    return "SHA256";
  case eChecksumAlgorithmTimestamp:
    return "timestamp";
  }
  llvm_unreachable("unhandled checksum algorithm.");
}

json::Value toJSON(const BreakpointModeApplicability &BMA) {
  switch (BMA) {
  case eBreakpointModeApplicabilitySource:
    return "source";
  case eBreakpointModeApplicabilityException:
    return "exception";
  case eBreakpointModeApplicabilityData:
    return "data";
  case eBreakpointModeApplicabilityInstruction:
    return "instruction";
  }
  llvm_unreachable("unhandled breakpoint mode applicability.");
}

json::Value toJSON(const BreakpointMode &BM) {
  json::Object result{
      {"mode", BM.mode},
      {"label", BM.label},
      {"appliesTo", BM.appliesTo},
  };

  if (BM.description)
    result.insert({"description", *BM.description});

  return result;
}

static llvm::StringLiteral ToString(AdapterFeature feature) {
  switch (feature) {
  case eAdapterFeatureANSIStyling:
    return "supportsANSIStyling";
  case eAdapterFeatureBreakpointLocationsRequest:
    return "supportsBreakpointLocationsRequest";
  case eAdapterFeatureCancelRequest:
    return "supportsCancelRequest";
  case eAdapterFeatureClipboardContext:
    return "supportsClipboardContext";
  case eAdapterFeatureCompletionsRequest:
    return "supportsCompletionsRequest";
  case eAdapterFeatureConditionalBreakpoints:
    return "supportsConditionalBreakpoints";
  case eAdapterFeatureConfigurationDoneRequest:
    return "supportsConfigurationDoneRequest";
  case eAdapterFeatureDataBreakpointBytes:
    return "supportsDataBreakpointBytes";
  case eAdapterFeatureDataBreakpoints:
    return "supportsDataBreakpoints";
  case eAdapterFeatureDelayedStackTraceLoading:
    return "supportsDelayedStackTraceLoading";
  case eAdapterFeatureDisassembleRequest:
    return "supportsDisassembleRequest";
  case eAdapterFeatureEvaluateForHovers:
    return "supportsEvaluateForHovers";
  case eAdapterFeatureExceptionFilterOptions:
    return "supportsExceptionFilterOptions";
  case eAdapterFeatureExceptionInfoRequest:
    return "supportsExceptionInfoRequest";
  case eAdapterFeatureExceptionOptions:
    return "supportsExceptionOptions";
  case eAdapterFeatureFunctionBreakpoints:
    return "supportsFunctionBreakpoints";
  case eAdapterFeatureGotoTargetsRequest:
    return "supportsGotoTargetsRequest";
  case eAdapterFeatureHitConditionalBreakpoints:
    return "supportsHitConditionalBreakpoints";
  case eAdapterFeatureInstructionBreakpoints:
    return "supportsInstructionBreakpoints";
  case eAdapterFeatureLoadedSourcesRequest:
    return "supportsLoadedSourcesRequest";
  case eAdapterFeatureLogPoints:
    return "supportsLogPoints";
  case eAdapterFeatureModulesRequest:
    return "supportsModulesRequest";
  case eAdapterFeatureReadMemoryRequest:
    return "supportsReadMemoryRequest";
  case eAdapterFeatureRestartFrame:
    return "supportsRestartFrame";
  case eAdapterFeatureRestartRequest:
    return "supportsRestartRequest";
  case eAdapterFeatureSetExpression:
    return "supportsSetExpression";
  case eAdapterFeatureSetVariable:
    return "supportsSetVariable";
  case eAdapterFeatureSingleThreadExecutionRequests:
    return "supportsSingleThreadExecutionRequests";
  case eAdapterFeatureStepBack:
    return "supportsStepBack";
  case eAdapterFeatureStepInTargetsRequest:
    return "supportsStepInTargetsRequest";
  case eAdapterFeatureSteppingGranularity:
    return "supportsSteppingGranularity";
  case eAdapterFeatureTerminateRequest:
    return "supportsTerminateRequest";
  case eAdapterFeatureTerminateThreadsRequest:
    return "supportsTerminateThreadsRequest";
  case eAdapterFeatureSuspendDebuggee:
    return "supportSuspendDebuggee";
  case eAdapterFeatureValueFormattingOptions:
    return "supportsValueFormattingOptions";
  case eAdapterFeatureWriteMemoryRequest:
    return "supportsWriteMemoryRequest";
  case eAdapterFeatureTerminateDebuggee:
    return "supportTerminateDebuggee";
  }
  llvm_unreachable("unhandled adapter feature.");
}

json::Value toJSON(const Capabilities &C) {
  json::Object result;

  for (const auto &feature : C.supportedFeatures)
    result.insert({ToString(feature), true});

  if (C.exceptionBreakpointFilters && !C.exceptionBreakpointFilters->empty())
    result.insert(
        {"exceptionBreakpointFilters", *C.exceptionBreakpointFilters});
  if (C.completionTriggerCharacters && !C.completionTriggerCharacters->empty())
    result.insert(
        {"completionTriggerCharacters", *C.completionTriggerCharacters});
  if (C.additionalModuleColumns && !C.additionalModuleColumns->empty())
    result.insert({"additionalModuleColumns", *C.additionalModuleColumns});
  if (C.supportedChecksumAlgorithms && !C.supportedChecksumAlgorithms->empty())
    result.insert(
        {"supportedChecksumAlgorithms", *C.supportedChecksumAlgorithms});
  if (C.breakpointModes && !C.breakpointModes->empty())
    result.insert({"breakpointModes", *C.breakpointModes});

  // lldb-dap extensions
  if (C.lldbExtVersion && !C.lldbExtVersion->empty())
    result.insert({"$__lldb_version", *C.lldbExtVersion});

  return result;
}

bool fromJSON(const llvm::json::Value &Params, SteppingGranularity &SG,
              llvm::json::Path P) {
  auto raw_granularity = Params.getAsString();
  if (!raw_granularity) {
    P.report("expected a string");
    return false;
  }
  std::optional<SteppingGranularity> granularity =
      StringSwitch<std::optional<SteppingGranularity>>(*raw_granularity)
          .Case("statement", eSteppingGranularityStatement)
          .Case("line", eSteppingGranularityLine)
          .Case("instruction", eSteppingGranularityInstruction)
          .Default(std::nullopt);
  if (!granularity) {
    P.report("unexpected value");
    return false;
  }
  SG = *granularity;
  return true;
}

bool fromJSON(const llvm::json::Value &Params, ValueFormat &VF,
              llvm::json::Path P) {
  json::ObjectMapper O(Params, P);
  return O && O.mapOptional("hex", VF.hex);
}

json::Value toJSON(const BreakpointLocation &B) {
  json::Object result;

  result.insert({"line", B.line});
  if (B.column)
    result.insert({"column", *B.column});
  if (B.endLine)
    result.insert({"endLine", *B.endLine});
  if (B.endColumn)
    result.insert({"endColumn", *B.endColumn});

  return result;
}

} // namespace lldb_dap::protocol
