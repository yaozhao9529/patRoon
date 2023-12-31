% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/generics.R, R/feature_groups-screening.R,
%   R/feature_groups-screening-set.R
\docType{class}
\name{screenInfo}
\alias{screenInfo}
\alias{annotateSuspects}
\alias{featureGroupsScreening-class}
\alias{featureGroupsScreening}
\alias{screenInfo,featureGroupsScreening-method}
\alias{show,featureGroupsScreening-method}
\alias{[,featureGroupsScreening,ANY,ANY,missing-method}
\alias{delete,featureGroupsScreening-method}
\alias{as.data.table,featureGroupsScreening-method}
\alias{annotateSuspects,featureGroupsScreening-method}
\alias{filter,featureGroupsScreening-method}
\alias{featureGroupsScreeningSet-class}
\alias{featureGroupsScreeningSet}
\alias{screenInfo,featureGroupsScreeningSet-method}
\alias{show,featureGroupsScreeningSet-method}
\alias{[,featureGroupsScreeningSet,ANY,ANY,missing-method}
\alias{delete,featureGroupsScreeningSet-method}
\alias{as.data.table,featureGroupsScreeningSet-method}
\alias{annotateSuspects,featureGroupsScreeningSet-method}
\alias{filter,featureGroupsScreeningSet-method}
\alias{featureGroupsSetScreeningUnset-class}
\alias{featureGroupsSetScreeningUnset}
\alias{unset,featureGroupsScreeningSet-method}
\title{Class for suspect screened feature groups.}
\usage{
screenInfo(obj)

annotateSuspects(
  fGroups,
  MSPeakLists = NULL,
  formulas = NULL,
  compounds = NULL,
  ...
)

\S4method{screenInfo}{featureGroupsScreening}(obj)

\S4method{show}{featureGroupsScreening}(object)

\S4method{[}{featureGroupsScreening,ANY,ANY,missing}(x, i, j, ..., rGroups, suspects = NULL, drop = TRUE)

\S4method{delete}{featureGroupsScreening}(obj, i = NULL, j = NULL, ...)

\S4method{as.data.table}{featureGroupsScreening}(x, ..., collapseSuspects = ",", onlyHits = FALSE)

\S4method{annotateSuspects}{featureGroupsScreening}(
  fGroups,
  MSPeakLists,
  formulas,
  compounds,
  absMzDev = 0.005,
  specSimParams = getDefSpecSimParams(removePrecursor = TRUE),
  checkFragments = c("mz", "formula", "compound"),
  formulasNormalizeScores = "max",
  compoundsNormalizeScores = "max",
  IDFile = system.file("misc", "IDLevelRules.yml", package = "patRoon"),
  logPath = file.path("log", "ident")
)

\S4method{filter}{featureGroupsScreening}(
  obj,
  ...,
  onlyHits = NULL,
  selectHitsBy = NULL,
  selectBestFGroups = FALSE,
  maxLevel = NULL,
  maxFormRank = NULL,
  maxCompRank = NULL,
  minAnnSimForm = NULL,
  minAnnSimComp = NULL,
  minAnnSimBoth = NULL,
  absMinFragMatches = NULL,
  relMinFragMatches = NULL,
  minRF = NULL,
  maxLC50 = NULL,
  negate = FALSE
)

\S4method{screenInfo}{featureGroupsScreeningSet}(obj)

\S4method{show}{featureGroupsScreeningSet}(object)

\S4method{[}{featureGroupsScreeningSet,ANY,ANY,missing}(x, i, j, ..., rGroups, suspects = NULL, sets = NULL, drop = TRUE)

\S4method{delete}{featureGroupsScreeningSet}(obj, i = NULL, j = NULL, ...)

\S4method{as.data.table}{featureGroupsScreeningSet}(x, ..., collapseSuspects = ",", onlyHits = FALSE)

\S4method{annotateSuspects}{featureGroupsScreeningSet}(
  fGroups,
  MSPeakLists,
  formulas,
  compounds,
  absMzDev = 0.005,
  specSimParams = getDefSpecSimParams(removePrecursor = TRUE),
  checkFragments = c("mz", "formula", "compound"),
  formulasNormalizeScores = "max",
  compoundsNormalizeScores = "max",
  IDFile = system.file("misc", "IDLevelRules.yml", package = "patRoon"),
  logPath = file.path("log", "ident")
)

\S4method{filter}{featureGroupsScreeningSet}(
  obj,
  ...,
  onlyHits = NULL,
  selectHitsBy = NULL,
  selectBestFGroups = FALSE,
  maxLevel = NULL,
  maxFormRank = NULL,
  maxCompRank = NULL,
  minAnnSimForm = NULL,
  minAnnSimComp = NULL,
  minAnnSimBoth = NULL,
  absMinFragMatches = NULL,
  relMinFragMatches = NULL,
  minRF = NULL,
  maxLC50 = NULL,
  negate = FALSE
)

\S4method{unset}{featureGroupsScreeningSet}(obj, set)
}
\arguments{
\item{obj, object, x, fGroups}{The \code{featureGroupsScreening} object.}

\item{MSPeakLists, formulas, compounds}{Annotation data (\code{\link{MSPeakLists}}, \code{\link{formulas}} and
\code{\link{compounds}}) obtained for this \code{featureGroupsScreening} object. All arguments can be \code{NULL}
to exclude it from the annotation.}

\item{\dots}{Further arguments passed to the base method.}

\item{i, j, rGroups}{Used for subsetting data analyses, feature groups and
replicate groups, see \code{\link{featureGroups}}.}

\item{suspects}{An optional \code{character} vector with suspect names. If
specified, only \code{featureGroups} will be kept that are assigned to
these suspects.}

\item{drop}{Ignored.}

\item{collapseSuspects}{If a \code{character} then any suspects that were matched to the same feature group are
collapsed to a single row and suspect names are separated by the value of \code{collapseSuspects}. If \code{NULL}
then no collapsing occurs, and each suspect match is reported on a single row. See the \verb{Suspect collapsing}
section below for additional details.}

\item{onlyHits}{For \code{as.data.table}: if \code{TRUE} then only feature groups with suspect hits are reported.

  For \code{filter} \itemize{

  \item if \code{negate=FALSE} and \code{onlyHits=TRUE} then all feature groups without suspect hits will be removed.
  Otherwise nothing will be done.

  \item if \code{negate=TRUE} then \code{onlyHits=TRUE} will select feature groups without suspect hits,
  \code{onlyHits=FALSE} will only retain feature groups with suspect matches and this filter is ignored if
  \code{onlyHits=NULL}.

  }}

\item{absMzDev}{Maximum absolute \emph{m/z} deviation.}

\item{specSimParams}{A named \code{list} with parameters that influence the calculation of MS spectra similarities.
See the \link[=specSimParams]{spectral similarity parameters} documentation for more details.}

\item{checkFragments}{Which type(s) of MS/MS fragments from workflow data should be checked to evaluate the number of
suspect fragment matches (\emph{i.e.} from the \code{fragments_mz}/\code{fragments_formula} columns in the suspect
list). Valid values are: \code{"mz"}, \code{"formula"}, \code{"compounds"}. The former uses \emph{m/z} values in
the specified \code{MSPeakLists} object, whereas the others use the formulae that were annotated to MS/MS peaks in
the given \code{formulas} or \code{compounds} objects. Multiple values are possible: in this case the maximum
number of fragment matches will be reported.}

\item{compoundsNormalizeScores, formulasNormalizeScores}{A \code{character} that specifies how normalization of
annotation scorings occurs. Either

\code{"max"} (normalize to max value) or \code{"minmax"} (perform min-max
normalization). Note that normalization of negative scores (e.g. output by
\command{SIRIUS}) is always performed as min-max. Furthermore, currently
normalization for \code{compounds} takes the original min/max scoring
values into account when candidates were generated. Thus, for
\code{compounds} scoring, normalization is not affected when candidate
results were removed after they were generated (\emph{e.g.} by use of
\code{filter}).}

\item{IDFile}{A file path to a YAML file with rules used for estimation of identification levels. See the
\verb{Suspect annotation} section for more details. If not specified then a default rules file will be used.}

\item{logPath}{A directory path to store logging information. If \code{NULL} then logging is disabled.}

\item{selectHitsBy}{Should be \code{"intensity"} or \code{"level"}. For cases where the same suspect is matched to
multiple feature groups, only the suspect to the feature group with highest mean intensity
(\code{selectHitsBy="intensity"}) or best identification level (\code{selectHitsBy="level"}) is kept. In case of
ties only the first hit is kept. Set to \code{NULL} to ignore this filter. If \code{negate=TRUE} then only those
hits with lowest mean intensity/poorest identification level are kept.}

\item{selectBestFGroups}{If \code{TRUE} then for any cases where a single feature group is matched to several
suspects only the suspect assigned to the feature group with best identification score is kept. In case of ties
only the first is kept.}

\item{maxLevel, maxFormRank, maxCompRank, minAnnSimForm, minAnnSimComp, minAnnSimBoth}{Filter suspects by maximum
identification level (\emph{e.g.} \code{"3a"}), formula/compound rank or with minimum formula/compound/combined
annotation similarity. Set to \code{NULL} to ignore.}

\item{absMinFragMatches, relMinFragMatches}{Only retain suspects with this minimum number MS/MS matches with the
fragments specified in the suspect list (\emph{i.e.} \code{fragments_mz}/\code{fragments_formula}).
\code{relMinFragMatches} sets the minimum that is relative (\samp{0-1}) to the maximum number of MS/MS fragments
specified in the \code{fragments_*} columns of the suspect list. Set to \code{NULL} to ignore.}

\item{minRF}{Filter suspect hits by the given minimum predicted response factor (as calculated by
\code{\link[=predictRespFactors]{predictRespFactors}}). Set to \code{NULL} to ignore.}

\item{maxLC50}{Filter suspect hits by the given maximum toxicity (LC50) (as calculated by
\code{\link[=predictTox]{predictTox}}). Set to \code{NULL} to ignore.}

\item{negate}{If set to \code{TRUE} then filtering operations are performed in opposite manner.}

\item{sets}{\setsWF A \code{character} with name(s) of the sets to keep (or remove if \code{negate=TRUE}).}

\item{set}{\setsWF The name of the set.}
}
\value{
\code{annotateSuspects} returns a \code{featureGroupsScreening} object, which is a
  \code{\link{featureGroups}} object amended with annotation data.

\code{filter} returns a filtered \code{featureGroupsScreening} object.
}
\description{
This class derives from \code{\link{featureGroups}} and adds suspect screening information.
}
\section{Methods (by generic)}{
\itemize{
\item \code{screenInfo(featureGroupsScreening)}: Returns a table with screening information
(see \code{screenInfo} slot).

\item \code{show(featureGroupsScreening)}: Shows summary information for this object.

\item \code{x[i}: Subset on analyses, feature groups and/or
suspects.

\item \code{as.data.table(featureGroupsScreening)}: Obtain a summary table (a \code{\link{data.table}}) with retention, \emph{m/z},
intensity and optionally other feature data. Furthermore, the output table will be merged with information from
\code{screenInfo}, such as suspect names and other properties and annotation data.

\item \code{annotateSuspects(featureGroupsScreening)}: Incorporates annotation data obtained during the workflow to annotate suspects
with matched known MS/MS fragments, formula/candidate ranks and automatic estimation of identification levels. See
the \verb{Suspect annotation} section for more details. The estimation of identification levels for each suspect is
logged in the \code{log/ident} directory.

\item \code{filter(featureGroupsScreening)}: Performs rule based filtering. This method builds on the comprehensive filter
functionality from the base \code{\link{filter,featureGroups-method}}. It adds several filters to select
\emph{e.g.} the best ranked suspects or those with a minimum estimated identification level. \strong{NOTE}: most
filters \emph{only} affect suspect hits, not feature groups. Set \code{onlyHits=TRUE} to subsequently remove any
feature groups that lost any suspect matches due to other filter steps.

}}
\section{Slots}{

\describe{
\item{\code{screenInfo}}{A (\code{\link{data.table}}) with results from suspect screening. This table will be amended with
annotation data when \code{annotateSuspects} is run.}

\item{\code{MS2QuantMeta}}{Metadata from \pkg{MS2Quant} filled in by \code{predictRespFactors}.}
}}

\note{
\code{filter} removes suspect hits with \code{NA} values when any of the filters related to minimum or maximum
  values are applied (unless \code{negate=TRUE}).
}
\section{Suspect annotation}{
 The \code{annotateSuspects} method is used to annotate suspects after
  \code{\link{screenSuspects}} was used to collect suspect screening results and other workflow steps such as formula
  and compound annotation steps have been completed. The annotation results, which can be acquired with the
  \code{as.data.table} and \code{screenInfo} methods, amends the current screening data with the following columns:

  \itemize{

  \item \code{formRank},\code{compRank} The rank of the suspect within the formula/compound annotation results.

  \item \code{annSimForm},\code{annSimComp},\code{annSimBoth} A similarity measure between measured and annotated
  MS/MS peaks from annotation of formulae, compounds or both. The similarity is calculated as the spectral similarity
  between a peaklist with (a) all MS/MS peaks and (b) only annotated peaks. Thus, a value of one means that all MS/MS
  peaks were annotated. If both formula and compound annotations are available then \code{annSimBoth} is calculated
  after combining all the annotated peaks, otherwise \code{annSimBoth} equals the available value for
  \code{annSimForm} or \code{annSimComp}. The similarity calculation can be configured with the \code{specSimParams}
  argument to \code{annotateSuspects}. Note for annotation with \code{generateCompoundsLibrary} results: the method
  and default parameters for \code{annSimComp} calculation slightly differs to those from the spectral similarity
  calculated with compound annotation (\code{libMatch} score), hence small differences in results are typically
  observed.

  \item \code{maxFrags} The maximum number of MS/MS fragments that can be matched for this suspect (based on the
  \code{fragments_*} columns from the suspect list).

  \item \code{maxFragMatches},\code{maxFragMatchesRel} The absolute and relative amount of experimental MS/MS peaks
  that were matched from the fragments specified in the suspect list. The value for \code{maxFragMatchesRel} is
  relative to the value for \code{maxFrags}. The calculation of this column is influenced by the
  \code{checkFragments} argument to \code{annotateSuspects}.

  \item \code{estIDLevel} Provides an \emph{estimation} of the identification level, roughly following that of
  \insertCite{Schymanski2014}{patRoon}. However, please note that this value is only an estimation, and manual
  interpretation is still necessary to assign final identification levels. The estimation is done through a set of
  rules, see the \verb{Identification level rules} section below.

  }

  Note that only columns are present if sufficient data is available for their calculation.
}

\section{Identification level rules}{
 The estimation of identification levels is configured through a YAML file which
  specifies the rules for each level. The default file is shown below.

 \preformatted{1:
    suspectFragments: 3
    retention: 12
2a:
    or:
        - individualMoNAScore:
            min: 0.9
            higherThanNext: .inf
        - libMatch:
            min: 0.9
            higherThanNext: .inf
    rank:
        max: 1
        type: compound
3a:
    or:
        - individualMoNAScore: 0.4
        - libMatch: 0.4
3b:
    suspectFragments: 3
3c:
    annMSMSSim:
        type: compound
        min: 0.7
4a:
    annMSMSSim:
        type: formula
        min: 0.7
    isoScore:
        min: 0.5
        higherThanNext: 0.2
    rank:
        max: 1
        type: formula
4b:
    isoScore:
        min: 0.9
        higherThanNext: 0.2
    rank:
        max: 1
        type: formula
5:
    all: yes
}

 Most of the file should be self-explanatory. Some notes:

  \itemize{

  \item Each rule is either a field of \code{suspectFragments} (minimum number of MS/MS fragments matched from
  suspect list), \code{retention} (maximum retention deviation from suspect list), \code{rank} (the maximum
  annotation rank from formula or compound annotations), \code{all} (this level is always matched) or any of the
  scorings available from the formula or compound annotations.

  \item In case any of the rules could be applied to either formula or compound annotations, the annotation type must
  be specified with the \code{type} field (\code{formula} or \code{compound}).

  \item Identification levels should start with a number and may optionally be followed by a alphabetic character.
  The lowest levels are checked first.

  \item If \code{relative=yes} then the relative scoring will be used for testing.

  \item For \code{suspectFragments}: if the number of fragments from the suspect list (\code{maxFrags} column) is
  less then the minimum rule value, the minimum is adjusted to the number of available fragments.

  \item The \code{or} and \code{and} keywords can be used to combine multiple conditions.
  }

  A template rules file can be generated with the \code{\link{genIDLevelRulesFile}} function, and this file can
  subsequently passed to \code{annotateSuspects}. The file format is highly flexible and (sub)levels can be added or
  removed if desired. Note that the default file is currently only suitable when annotation is performed with GenForm
  and MetFrag, for other algorithms it is crucial to modify the rules.
}

\section{S4 class hierarchy}{
 \itemize{   \item{\code{\link{featureGroups}}}   \itemize{     \item{\strong{\code{\link{featureGroupsScreening}}}}     \itemize{       \item{\code{\link{featureGroupsSetScreeningUnset}}}     }   } }
}

\section{{Suspect collapsing}}{
 The \code{as.data.table} method fir \code{featureGroupsScreening} supports an
  additional format where each suspect hit is reported on a separate row (enabled by setting
  \code{collapseSuspects=NULL}). In this format the suspect
  properties from the \code{screenInfo} method are merged with each suspect row. Alternatively, if \emph{suspect
  collapsing} is enabled (the default) then the regular \code{as.data.table} format is used, and amended with the
  names of all suspects matched to a feature group (separated by the value of the \code{collapseSuspects} argument).

  Suspect collapsing also influences how calculated feature concentrations/toxicities are reported (\emph{i.e.}
  obtained with \code{\link{calculateConcs}}/\code{\link{calculateTox}}). If these values were directly predicted for
  suspects, \emph{i.e.} by using \code{\link{predictRespFactors}}/\code{\link{predictTox}} on the feature groups
  object, \emph{and} suspects are \emph{not} collapsed, then the calculated concentration/toxicity reported for each
  suspect row is not aggregated and specific for that suspect (unless not available). Hence, this allows you to
  obtain specific concentration/toxicity values for each suspect/feature group pair.
}

\section{Sets workflows}{
 \setsWFClass{featureGroupsScreeningSet}{featureGroupsScreening}

  \setsWFNewMethodsSO{featureGroupsScreeningUnset}{Only the screening results present in the specified set are kept.}

  \setsWFChangedMethods{

  \item \code{annotateSuspects} Suspect annotation is performed per set. Thus, formula/compound ranks, estimated
  identification levels etc are calculated for each set. Subsequently, these results are merged in the final
  \code{screenInfo}. In addition, an overall \code{formRank} and \code{compRank} column is created based on the
  rankings of the suspect candidate in the set consensus data. Furthermore, an overall \code{estIDLevel} is generated
  that is based on the 'best' estimated identification level among the sets data (\emph{i.e.} the lowest). In case
  there is a tie between sub-levels (\emph{e.g.} \samp{3a} and \samp{3b}), then the sub-level is stripped
  (\emph{e.g.} \samp{3}).

  \item \code{filter} All filters related to estimated identification levels and formula/compound rankings  are
  applied to the overall set data (see above). All others are applied to set specific data: in this case candidates
  are only removed if none of the set data confirms to the filter.

  }

  This class derives also from \code{\link{featureGroupsSet}}. Please see its documentation for more relevant details
  with sets workflows.

  Note that the \code{formRank} and \code{compRank} columns are \emph{not} updated when the data is subset.
}

\references{
\insertAllCited{} \cr \cr \insertRef{Stein1994}{patRoon}
}
\seealso{
\code{\link{featureGroups}}
}
\author{
Rick Helmus <\email{r.helmus@uva.nl}>, Emma Schymanski <\email{emma.schymanski@uni.lu}> (contributions to
  identification level rules), Bas van de Velde (contributions to spectral similarity calculation).
}
