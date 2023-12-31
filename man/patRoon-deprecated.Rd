% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/deprecated.R
\name{patRoon-deprecated}
\alias{patRoon-deprecated}
\alias{reportMD}
\alias{exportDAFiles}
\alias{plotEIC}
\alias{groups}
\alias{plotSpec}
\alias{formulaTable}
\alias{compoundTable}
\title{Deprecated and renamed functions.}
\usage{
reportMD(...)

exportDAFiles(
  anaInfo,
  format = "mzML",
  exportLine = TRUE,
  outPath = anaInfo$path,
  overWrite = FALSE
)

plotEIC(obj, ...)

groups(object, ...)

plotSpec(obj, ...)

formulaTable(...)

compoundTable(...)
}
\arguments{
\item{\dots}{Passed to successor function.}

\item{format}{The output format of exported files. Should be either
\code{"mzXML"}, \code{"mzML"} or \code{"mzData"}.}

\item{exportLine}{Export line spectra (\code{TRUE}) or profile spectra
(\code{FALSE}). Usually line spectra are preferred, since profile spectra
use signficantly more disk space and increase required memory during
processing.}

\item{outPath}{Character vector of output paths for exported analyses. Will
be recycled if necessary.}

\item{overWrite}{If \code{TRUE} existing files will be overwritten.}
}
\description{
Please do not use these functions anymore since they may be removed in the
future.
}
\details{
\code{reportMD} performs HTML reporting, please use
  \code{\link{reportHTML}} instead.

\code{exportDAFiles} will export a set of analyses either in
  \file{.mzXML} or \file{.mzML} formats.

Please use \code{\link{plotChroms}} instead.

Please use \code{\link{groupTable}} instead.

Please use \code{\link{plotSpectrum}} instead.

Please use \code{\link{annotations}} instead.

Please use \code{\link{annotations}} instead.
}
\keyword{internal}
