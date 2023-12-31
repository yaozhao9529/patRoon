% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/features-safd.R
\name{findFeaturesSAFD}
\alias{findFeaturesSAFD}
\title{Find features using SAFD}
\usage{
findFeaturesSAFD(
  analysisInfo,
  profPath = NULL,
  mzRange = c(0, 400),
  maxNumbIter = 1000,
  maxTPeakW = 300,
  resolution = 30000,
  minMSW = 0.02,
  RThreshold = 0.75,
  minInt = 2000,
  sigIncThreshold = 5,
  S2N = 2,
  minPeakWS = 3,
  verbose = TRUE
)
}
\arguments{
\item{analysisInfo}{A \code{data.frame} with \link[=analysis-information]{Analysis information}.}

\item{profPath}{A \code{character} vector with paths to the profile MS data for each analysis (will be re-cycled if
necessary). See the \verb{Using SAFD} section for more details.}

\item{mzRange}{The \emph{m/z} window to be imported (passed to the \code{import_files_MS1} function).}

\item{maxNumbIter, maxTPeakW, resolution, minMSW, RThreshold, minInt, sigIncThreshold, S2N, minPeakWS}{Parameters directly
passed to the \code{safd_s3D} function.}

\item{verbose}{If set to \code{FALSE} then no text output is shown.}
}
\value{
An object of a class which is derived from \code{\link{features}}.
}
\description{
Uses \href{https://bitbucket.org/SSamanipour/safd.jl/src/master/}{SAFD} to obtain features. This functionality is
still experimental. Please see the details below.
}
\details{
This function uses SAFD to automatically find features. This function is called when calling \code{findFeatures} with
  \code{algorithm="safd"}.

The support for SAFD is still experimental, and its interface might change in the future.

  In order to use SAFD, please make sure that its \code{julia} packages are installed and you have verified that
  everything works, \emph{e.g.} by running the test data.

  This algorithm supports profile and centroided MS data. If the use of profile data is desired, centroided data
  must still be available for other functionality of \code{patRoon}. The centroided data is specified through the
  'regular' \link[=analysis-information]{analysis info} mechanism. The location to any profile data is specified
  through the \code{profPath} argument (\code{NULL} for no profile data). The base file names (\emph{i.e.} the file
  name without path and extension) of both centroid and profile data must be the same. Furthermore, the format of the
  profile data must be \file{mzXML}.
}
\section{Parallelization}{
 \code{findFeaturesSAFD} uses multiprocessing to parallelize
  computations. Please see the parallelization section in the handbook for
  more details and \link[=patRoon-package]{patRoon options} for configuration
  options.

 Note that for caching purposes, the analyses files must always exist on the local host
  computer, even if it is not participating in computations.
}

\references{
\insertRef{Samanipour2019}{patRoon}
}
\seealso{
\code{\link{findFeatures}} for more details and other algorithms.
}
