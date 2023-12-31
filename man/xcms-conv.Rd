% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/generics.R, R/utils-xcms.R
\name{getXCMSSet}
\alias{getXCMSSet}
\alias{getXCMSnExp}
\alias{getXCMSSet,features-method}
\alias{getXCMSSet,featuresXCMS-method}
\alias{getXCMSSet,featureGroups-method}
\alias{getXCMSSet,featureGroupsXCMS-method}
\alias{getXCMSSet,featuresSet-method}
\alias{getXCMSSet,featureGroupsSet-method}
\alias{getXCMSnExp,features-method}
\alias{getXCMSnExp,featuresXCMS3-method}
\alias{getXCMSnExp,featureGroups-method}
\alias{getXCMSnExp,featureGroupsXCMS3-method}
\alias{getXCMSnExp,featuresSet-method}
\alias{getXCMSnExp,featureGroupsSet-method}
\title{Conversion to XCMS objects}
\usage{
getXCMSSet(obj, verbose = TRUE, ...)

getXCMSnExp(obj, verbose = TRUE, ...)

\S4method{getXCMSSet}{features}(obj, verbose, loadRawData)

\S4method{getXCMSSet}{featuresXCMS}(obj, verbose = TRUE, ...)

\S4method{getXCMSSet}{featureGroups}(obj, verbose, loadRawData)

\S4method{getXCMSSet}{featureGroupsXCMS}(obj, verbose, loadRawData)

\S4method{getXCMSSet}{featuresSet}(obj, ..., set)

\S4method{getXCMSSet}{featureGroupsSet}(obj, ..., set)

\S4method{getXCMSnExp}{features}(obj, verbose, loadRawData)

\S4method{getXCMSnExp}{featuresXCMS3}(obj, verbose = TRUE, ...)

\S4method{getXCMSnExp}{featureGroups}(obj, verbose, loadRawData)

\S4method{getXCMSnExp}{featureGroupsXCMS3}(obj, verbose, loadRawData)

\S4method{getXCMSnExp}{featuresSet}(obj, ..., set)

\S4method{getXCMSnExp}{featureGroupsSet}(obj, ..., set)
}
\arguments{
\item{obj}{The object that should be converted.}

\item{verbose}{If \code{FALSE} then no text output is shown.}

\item{\dots}{\setsWF Further arguments passed to non-sets method.

  Otherwise ignored.}

\item{loadRawData}{Set to \code{TRUE} if analyses are available as \code{mzXML} or \code{mzML} files. Otherwise MS
data is not loaded, and some dummy data (\emph{e.g.} file paths) is used in the returned object.}

\item{set}{\setsWF The name of the set to be exported.}
}
\description{
Converts a \code{\link{features}} or \code{\link{featureGroups}} object to an \code{\link{xcmsSet}} or
\code{\link{XCMSnExp}} object.
}
\section{Sets workflows}{
 In a \link[=sets-workflow]{sets workflow}, \code{\link{unset}} is used to convert the
  feature (group) data before the object is exported.
}

