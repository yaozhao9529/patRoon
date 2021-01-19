// Generated by using Rcpp::compileAttributes() -> do not edit by hand
// Generator token: 10BE3573-1514-4C36-9D1C-5A225CD40393

#include <Rcpp.h>

using namespace Rcpp;

// parseAdductConsXMLFile
Rcpp::List parseAdductConsXMLFile(Rcpp::CharacterVector file);
RcppExport SEXP _patRoon_parseAdductConsXMLFile(SEXP fileSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< Rcpp::CharacterVector >::type file(fileSEXP);
    rcpp_result_gen = Rcpp::wrap(parseAdductConsXMLFile(file));
    return rcpp_result_gen;
END_RCPP
}
// parseFeatureXMLFile
Rcpp::DataFrame parseFeatureXMLFile(Rcpp::CharacterVector file);
RcppExport SEXP _patRoon_parseFeatureXMLFile(SEXP fileSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< Rcpp::CharacterVector >::type file(fileSEXP);
    rcpp_result_gen = Rcpp::wrap(parseFeatureXMLFile(file));
    return rcpp_result_gen;
END_RCPP
}
// parseFeatConsXMLFile
Rcpp::List parseFeatConsXMLFile(Rcpp::CharacterVector file, Rcpp::IntegerVector anaCount);
RcppExport SEXP _patRoon_parseFeatConsXMLFile(SEXP fileSEXP, SEXP anaCountSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< Rcpp::CharacterVector >::type file(fileSEXP);
    Rcpp::traits::input_parameter< Rcpp::IntegerVector >::type anaCount(anaCountSEXP);
    rcpp_result_gen = Rcpp::wrap(parseFeatConsXMLFile(file, anaCount));
    return rcpp_result_gen;
END_RCPP
}
// writeFeatureXML
void writeFeatureXML(Rcpp::DataFrame featList, Rcpp::CharacterVector out, Rcpp::LogicalVector hulls);
RcppExport SEXP _patRoon_writeFeatureXML(SEXP featListSEXP, SEXP outSEXP, SEXP hullsSEXP) {
BEGIN_RCPP
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< Rcpp::DataFrame >::type featList(featListSEXP);
    Rcpp::traits::input_parameter< Rcpp::CharacterVector >::type out(outSEXP);
    Rcpp::traits::input_parameter< Rcpp::LogicalVector >::type hulls(hullsSEXP);
    writeFeatureXML(featList, out, hulls);
    return R_NilValue;
END_RCPP
}
// loadEICIntensities
Rcpp::NumericVector loadEICIntensities(Rcpp::List spectra, Rcpp::DataFrame featList, Rcpp::NumericVector rtWindow);
RcppExport SEXP _patRoon_loadEICIntensities(SEXP spectraSEXP, SEXP featListSEXP, SEXP rtWindowSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< Rcpp::List >::type spectra(spectraSEXP);
    Rcpp::traits::input_parameter< Rcpp::DataFrame >::type featList(featListSEXP);
    Rcpp::traits::input_parameter< Rcpp::NumericVector >::type rtWindow(rtWindowSEXP);
    rcpp_result_gen = Rcpp::wrap(loadEICIntensities(spectra, featList, rtWindow));
    return rcpp_result_gen;
END_RCPP
}
// loadEICs
Rcpp::List loadEICs(Rcpp::List spectra, Rcpp::NumericVector rtMins, Rcpp::NumericVector rtMaxs, Rcpp::NumericVector mzMins, Rcpp::NumericVector mzMaxs);
RcppExport SEXP _patRoon_loadEICs(SEXP spectraSEXP, SEXP rtMinsSEXP, SEXP rtMaxsSEXP, SEXP mzMinsSEXP, SEXP mzMaxsSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< Rcpp::List >::type spectra(spectraSEXP);
    Rcpp::traits::input_parameter< Rcpp::NumericVector >::type rtMins(rtMinsSEXP);
    Rcpp::traits::input_parameter< Rcpp::NumericVector >::type rtMaxs(rtMaxsSEXP);
    Rcpp::traits::input_parameter< Rcpp::NumericVector >::type mzMins(mzMinsSEXP);
    Rcpp::traits::input_parameter< Rcpp::NumericVector >::type mzMaxs(mzMaxsSEXP);
    rcpp_result_gen = Rcpp::wrap(loadEICs(spectra, rtMins, rtMaxs, mzMins, mzMaxs));
    return rcpp_result_gen;
END_RCPP
}
// makeSAFDInput
Rcpp::List makeSAFDInput(Rcpp::List spectra, Rcpp::NumericVector mzRange);
RcppExport SEXP _patRoon_makeSAFDInput(SEXP spectraSEXP, SEXP mzRangeSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< Rcpp::List >::type spectra(spectraSEXP);
    Rcpp::traits::input_parameter< Rcpp::NumericVector >::type mzRange(mzRangeSEXP);
    rcpp_result_gen = Rcpp::wrap(makeSAFDInput(spectra, mzRange));
    return rcpp_result_gen;
END_RCPP
}

static const R_CallMethodDef CallEntries[] = {
    {"_patRoon_parseAdductConsXMLFile", (DL_FUNC) &_patRoon_parseAdductConsXMLFile, 1},
    {"_patRoon_parseFeatureXMLFile", (DL_FUNC) &_patRoon_parseFeatureXMLFile, 1},
    {"_patRoon_parseFeatConsXMLFile", (DL_FUNC) &_patRoon_parseFeatConsXMLFile, 2},
    {"_patRoon_writeFeatureXML", (DL_FUNC) &_patRoon_writeFeatureXML, 3},
    {"_patRoon_loadEICIntensities", (DL_FUNC) &_patRoon_loadEICIntensities, 3},
    {"_patRoon_loadEICs", (DL_FUNC) &_patRoon_loadEICs, 5},
    {"_patRoon_makeSAFDInput", (DL_FUNC) &_patRoon_makeSAFDInput, 2},
    {NULL, NULL, 0}
};

RcppExport void R_init_patRoon(DllInfo *dll) {
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}
