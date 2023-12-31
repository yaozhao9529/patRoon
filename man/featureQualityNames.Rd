% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/utils-exported.R
\name{featureQualityNames}
\alias{featureQualityNames}
\title{Returns chromatographic peak quality and score names for features and/or feature groups.}
\usage{
featureQualityNames(feat = TRUE, group = TRUE, scores = FALSE, totScore = TRUE)
}
\arguments{
\item{feat}{If \code{TRUE} then names specific to features are returned.}

\item{group}{If \code{TRUE} then names specific to groups are returned.}

\item{scores}{If \code{TRUE} the score names are returned, otherwise the quality names.}

\item{totScore}{If \code{TRUE} (and \code{scores=TRUE}) then the name of the total score is included.}
}
\description{
Returns chromatographic peak quality and score names for features and/or feature groups.
}
