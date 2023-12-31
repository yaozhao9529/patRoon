% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/features-sirius.R
\name{findFeaturesSIRIUS}
\alias{findFeaturesSIRIUS}
\title{Find features using SIRIUS}
\usage{
findFeaturesSIRIUS(analysisInfo, verbose = TRUE)
}
\arguments{
\item{analysisInfo}{A \code{data.frame} with \link[=analysis-information]{Analysis information}.}

\item{verbose}{If set to \code{FALSE} then no text output is shown.}
}
\value{
An object of a class which is derived from \code{\link{features}}.
}
\description{
Uses \href{https://bio.informatik.uni-jena.de/software/sirius/}{SIRIUS} to find features.
}
\details{
This function uses SIRIUS to automatically find features. This function is called when calling \code{findFeatures} with
  \code{algorithm="sirius"}.

The features are collected by running the \command{lcms-align} \command{SIRIUS} command for every analysis.

  The MS files should be in the \file{mzML} or \file{mzXML} format. Furthermore, this algorithms requires the
  presence of (data-dependent) MS/MS data.

The input MS data files need to be centroided. The \code{\link{convertMSFiles}} function can be used to
  centroid data.
}
\section{Parallelization}{
 \code{findFeaturesSIRIUS} uses multiprocessing to parallelize
  computations. Please see the parallelization section in the handbook for
  more details and \link[=patRoon-package]{patRoon options} for configuration
  options.

 Note that for caching purposes, the analyses files must always exist on the local host
  computer, even if it is not participating in computations.
}

\references{
\insertRef{Dhrkop2019}{patRoon}
}
\seealso{
\code{\link{findFeatures}} for more details and other algorithms.
}
